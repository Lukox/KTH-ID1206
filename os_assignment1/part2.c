#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>

#define MAX_SIZE 100
#define MAX_NUM_MSG 10
char *my_mq = "/mymq";
char buf[MAX_SIZE];
mqd_t mqd;
struct mq_attr attr;

void parent() {
    // Open an existing message queue
    mqd = mq_open(my_mq, O_WRONLY);

    // Read content from the file
    FILE *file = fopen("part2.txt", "r");

    // Read the content of the file
    fgets(buf, MAX_SIZE, file);

    // Close the file
    fclose(file);

    // Send the message to the queue
    if (mq_send(mqd, buf, MAX_SIZE, 0) == -1) {
        mq_close(mqd);
        exit(1);
    }

    // Close the message queue
    mq_close(mqd);
}

void child() {
    // Create message queue | ‘MQ_MODE’ was undefined
    mqd = mq_open(my_mq, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR, &attr);
    
    // Read the message from the queue
    if (mq_receive(mqd, buf, MAX_SIZE, NULL) == -1) {
        mq_close(mqd);
        exit(1);
    }

    int didHappen = 0;

    // Count the number of words
    int count = 0;
    for (int i = 0; buf[i] != '\0'; i++) {
        if (buf[i] == ' ' || buf[i] == '\n') {
            count++;
        }
        didHappen = 1;
    }

    if (didHappen){ count++; }

    printf("Number of words in the file: %d\n", count);

    // Close the message queue
    mq_close(mqd);

}

int main() {
    // Form the queue attributes
    attr.mq_maxmsg = MAX_NUM_MSG;
    attr.mq_msgsize = MAX_SIZE;

    pid_t pid = fork();

    if (pid == -1) {
        exit(1);
    } else if (pid == 0) {
        child();
    } else {
        parent();
    }

    return 0;
}