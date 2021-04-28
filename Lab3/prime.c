#include  <pthread.h>
#include  <stdio.h>
#include  <stdlib.h>

#define  NUMBER_OF_THREADS 1

void *primeFinder(void *params);      /*  thread  that  calculates  prime  */
int primeCheck(int num);             /*  helper  function  that  checks  prime*/

int input;

int main(int argc, char * argv[]) {

    input = atoi(argv[1]);

    pthread_t workers[NUMBER_OF_THREADS];

    pthread_create(&workers[0], 0, primeFinder, (void *) &input);

    pthread_join(workers[0], NULL);
}

void *primeFinder(void *params) {
    int *val = (int *) params;
    int n = *val;

    for (int i = 2; i <= n; i++) {
        if (primeCheck(i)) {
            printf("%d ", i);
        }
    }
    printf("\n");
    pthread_exit(0);
}

int primeCheck(int num) {
    int res = 1;
    for (int i = 2; i < num; i++) {
        if (num % i == 0) {
            res = 0;
        }
    }
    return res;
}