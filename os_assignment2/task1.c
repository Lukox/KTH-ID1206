#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>

#define MAX_BUFFER 15

int buffer = 0;
pthread_mutex_t lock;

void *f(void *vargp)
{
    int count = 0;

    do {
        pthread_mutex_lock(&lock);

        printf("TID: %d, PID %d, Buffer: %d \n", *(int *)vargp, getpid(), buffer++);
        count++;
        
        pthread_mutex_unlock(&lock);

        // Sleep for a bit
        sleep(rand() % 2);  
    } while (buffer < MAX_BUFFER);

    return (void *)(uintptr_t)count;
}

int main()
{
    pthread_t tid[3];
    int counts[3], total = 0;

    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < 3; i++)
    {
        pthread_create(&tid[i], NULL, f, (void *)&tid[i]);
    }

    for (int i = 0; i < 3; i++)
    {
        void *count;
        pthread_join(tid[i], &count);
        counts[i] = (int)(uintptr_t)count;
        printf("TID %d worked on the buffer %d times \n", (int)tid[i], counts[i]);
        total += counts[i];
    }

    printf("Total buffer accesses: %d \n", total);

    pthread_mutex_destroy(&lock);
    return 0;
}