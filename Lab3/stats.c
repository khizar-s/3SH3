#include  <pthread.h>
#include  <stdio.h>
#include  <stdlib.h>

#define  NUMBER_OF_THREADS 3

void *averageFinder(void *params);      /*  thread  that  calculates  average  */
void *minimumFinder(void *params);      /*  thread  that  calculates  minimum  */
void *maximumFinder(void *params);      /*  thread  that  calculates  maximum  */

int argCount, avg, min, max;

int main(int argc, char * argv[]) {

    argCount = argc - 1;
    int arguments[argCount];

    for (int i = 0; i < argCount; i++) {
        arguments[i] = atoi(argv[i + 1]);
    }

    pthread_t workers[NUMBER_OF_THREADS];

    pthread_create(&workers[0], 0, averageFinder, (void *) &arguments);
    pthread_create(&workers[1], 0, minimumFinder, (void *) &arguments);
    pthread_create(&workers[2], 0, maximumFinder, (void *) &arguments);

    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        pthread_join(workers[i], NULL);
    }

    printf("The average value is %d \n", avg);
    printf("The minimum value is %d \n", min);
    printf("The maximum value is %d \n", max);
}

void *averageFinder(void *params) {
    int *l = (int *) params;
    int sum = 0;
    for (int i = 0; i < argCount; i++) {
        sum += l[i];
    }
    avg = sum/argCount;
    pthread_exit(0);
}

void *minimumFinder(void *params) {
    int *l = (int *) params;
    min = l[0];
    for (int i = 1; i < argCount; i++) {
        if (l[i] < min) {
            min = l[i];
        }
    }
    pthread_exit(0);
}

void *maximumFinder(void *params) {
    int *l = (int *) params;
    max = l[0];
    for (int i = 1; i < argCount; i++) {
        if (l[i] > max) {
            max = l[i];
        }
    }
    pthread_exit(0);
}