#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

static int THREADCOUNT = 6;
static int LOTOLCOUNT = 49;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t bcond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t bmutex = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;

int *shuffle(int *o, int length, int seed){
    int i, j, x;
    for(j, x, i=length;i;){
        j = rand_r(&seed) % i;
        x = o[--i];
        o[i] = o[j];
        o[j] = x;
    }
    return o;
}

int waitbarrier(void) {
    int berror = 0;
    int error;
    if (error = pthread_mutex_lock(&bmutex))
        return error;
    while ((counter < THREADCOUNT) && !berror)
        berror = pthread_cond_wait(&bcond, &bmutex);
    if (!berror)
        berror = pthread_cond_broadcast(&bcond);
    error = pthread_mutex_unlock(&bmutex);
    if (berror)
        return berror;
    return error;
}

void * child(void *arg){
    int *input = (int *)arg;
    while(1){
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        pthread_mutex_unlock(&mutex);
        shuffle(input, LOTOLCOUNT, time(NULL) + pthread_self() + counter++);
        pthread_mutex_lock(&mutex);
        pthread_cond_broadcast(&bcond);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main(){
    int i, j, balls[THREADCOUNT], arr[THREADCOUNT][LOTOLCOUNT];
    pthread_t t[THREADCOUNT];
    char op = 'l';
    for(i=0;i<THREADCOUNT;i++)
        for(j=0;j<LOTOLCOUNT;j++)
            arr[i][j] = j;

    for(i=0;i<THREADCOUNT;i++){
        pthread_create(&t[i], NULL, child, arr[i]);
    }
    while(op != 'q'){
        op = getchar();
        getchar();
        if(op == 'l'){
            counter = 0;
            pthread_cond_broadcast(&cond);
            waitbarrier();
            for(i=0;i<THREADCOUNT;i++){
                balls[i] = arr[i][i];
            }
            for(i=0;i<THREADCOUNT;i++){
                for(j=0;j<THREADCOUNT;j++){
                    balls[i] = arr[i][balls[i]];
                }
            }
            printf("the balls are: ");
            for(i=0;i<THREADCOUNT;i++) printf("%3d", balls[i]+1);
            printf("\n");
        }
    }
    for(i=0;i<THREADCOUNT;i++){
        pthread_cancel(t[i]);
    }
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&bcond);
    pthread_mutex_destroy(&bmutex);
    return 0;
}

