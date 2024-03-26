#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define TOTAL_CYLINDERS 5000
#define REQUESTS_COUNT 1000


// COMPARES TWO INTIGERS
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// CLONE REQUEST ARRAY
int* cloneRequests(int source[]) {
    int *destination = malloc(sizeof(int) * REQUESTS_COUNT);
    for(int i = 0; i < REQUESTS_COUNT; i++){
        destination[i] = source[i];
    }
    return destination;
}

// RETURNS THE ULTIMATE PLACEMENT OF THE HEAD BEFORE THE INITIAL POSITION
int maxHead(int requests[], int head) {
    for (int i = REQUESTS_COUNT - 1; i > 0; i--) {
        if (requests[i] <= head) {
            return requests[i];
        } 
    }
}


int fcfs(int requests[], int initialPosition) {
    int sum = 0;

    for (int i = 0; i < REQUESTS_COUNT; i++) {
        sum += abs(initialPosition - requests[i]);
        initialPosition = requests[i];
    }

    return sum;
}

int sstf(int requests[], int initialPosition) {
    int sum = 0;
    int pos = initialPosition;
    for (int req = REQUESTS_COUNT; req > 0; --req) {
        int min = INT_MAX;
        int index = -1;

        for (int i = 0; i < req; ++i) {
            int distance = abs(pos - requests[i]);
            if (distance < min) {
                min = distance;
                index = i;
            }
        }

        sum += min;
        pos = requests[index];

        for (int i = index; i < req - 1; ++i) {
            requests[i] = requests[i + 1];
        }
    }

    return sum;
}


int scan(int requests[], int initialPosition) {
    
    qsort(requests, REQUESTS_COUNT, sizeof(int), compare);
    int min = requests[0];
    int max = requests[REQUESTS_COUNT-1];

    if (min < initialPosition) {
        return (2 * TOTAL_CYLINDERS - 2 - min - initialPosition);
    } else {
        return (max - initialPosition);
    }
}

int c_scan(int requests[], int initialPosition) {
    
    qsort(requests, REQUESTS_COUNT, sizeof(int), compare);
    
    int min = requests[0];
    int max = requests[REQUESTS_COUNT-1];
    int maxBeforeHead = maxHead(requests, initialPosition);


    if (min < initialPosition) {
        return (2* TOTAL_CYLINDERS - 2 - initialPosition + maxBeforeHead);
    } else {
        return (max - initialPosition);
    }
    
}

int look(int requests[], int initialPosition) {

    qsort(requests, REQUESTS_COUNT, sizeof(int), compare);

    int min = requests[0];
    int max = requests[REQUESTS_COUNT-1];

    if (min < initialPosition) {
        return max < initialPosition ? (initialPosition - min) : (max - initialPosition + max - min);
    } else {
        return (max - initialPosition);
    }
}

int c_look(int requests[], int initialPosition) {

    qsort(requests, REQUESTS_COUNT, sizeof(int), compare);
    
    int min = requests[0];
    int max = requests[REQUESTS_COUNT-1];
    int maxBeforeHead = maxHead(requests, initialPosition);

    if (min < initialPosition){
        if (max < initialPosition){
            return (initialPosition - min) + (max - min);
        } else {
            return (max - initialPosition) + (max - min) + (maxBeforeHead - min);
        }
    } else {
        return (max - initialPosition);
    }
}


int main(int argc, char *argv[]) {
    int initialPosition = atoi(argv[1]);
    
    if (initialPosition >= TOTAL_CYLINDERS || initialPosition < 0) {
        printf("INVALID HEAD \n");
        return -1;
    }

    srand(time(NULL));
    int requests[REQUESTS_COUNT];
    int usedIntegers[TOTAL_CYLINDERS];

    for (int i = 0; i < TOTAL_CYLINDERS; i++) {
       usedIntegers[i] = 0;
    }  

    int i = 0;
    while (i < REQUESTS_COUNT) {
        int random = rand() % TOTAL_CYLINDERS;
        if (usedIntegers[random] == 0) {
            requests[i] = random;
            usedIntegers[random] = 1;
            i++;
        }
    }

    printf("FCFS Total head movement: %d\n", fcfs(requests, initialPosition));
    printf("SSTF Total head movement: %d\n", sstf(cloneRequests(requests), initialPosition));
    printf("SCAN Total head movement: %d\n", scan(cloneRequests(requests), initialPosition));
    printf("C-SCAN Total head movement: %d\n", c_scan(cloneRequests(requests), initialPosition));
    printf("LOOK Total head movement: %d\n", look(cloneRequests(requests), initialPosition));
    printf("C-LOOK Total head movement: %d\n", c_look(cloneRequests(requests), initialPosition));

    return 0;
}