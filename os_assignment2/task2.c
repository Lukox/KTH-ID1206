#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX 3 

sem_t mutex, rw_mutex;
int buffer = 0;
int read_count = 0;

void *reader(void *vargp) {
    do {
        // Lock to update read_count
        sem_wait(&mutex); 
        read_count++;
        if (read_count == 1) {
            printf("The first reader acquires the lock.\n");
            // First reader locks the rw_mutex
            sem_wait(&rw_mutex); 
        }
        // Unlock read_count lock
        sem_post(&mutex); 

        // Reading 
        printf("Reader (%d) reads the value %d\n", *(int *)vargp, buffer);

        // Lock read_count to update
        sem_wait(&mutex); 
        read_count--;
        if (read_count == 0) {
            printf("The last reader releases the read lock.\n");
            // Last reader releases the rw_mutex
            sem_post(&rw_mutex); 
        }
        // Release the lock on read_count
        sem_post(&mutex); 

         // Sleep for a bit
        sleep(rand() % 3);

    } while(buffer < MAX);
}

void *writer(void *vargp) {
    do {
        // Lock rw_mutex
        sem_wait(&rw_mutex); 

        // Writing
        buffer++;  
        printf("The writer acquires the lock.\n");
        printf("The writer (%d) writes the value %d\n", *(int *)vargp, buffer);

        // Release the lock for writing
        sem_post(&rw_mutex); 
        printf("The writer releases the lock.\n");

        // Sleep for a bit
        sleep(rand() % 2); 
    } while(buffer < MAX);
}

int main() {
    // Semaphores Initialize
    sem_init(&mutex, 0, 1);
    sem_init(&rw_mutex, 0, 1);

    // Create threads for writer and readers
    pthread_t tid[3];
    pthread_create(&tid[0], NULL, writer, (void *)&tid[0]);
    pthread_create(&tid[1], NULL, reader, (void *)&tid[1]);
    pthread_create(&tid[2], NULL, reader, (void *)&tid[2]);

    // Wait for threads to finish
    for (int i = 0; i < 3; i++) {
        pthread_join(tid[i], NULL);
    }

    // Semaphores Destroy
    sem_destroy(&mutex);
    sem_destroy(&rw_mutex);

    return 0;
}