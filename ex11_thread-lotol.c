#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

static int THREADCOUNT = 6;
static int LOTOLCOUNT = 49;
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

void * child(void *arg){
    int *input = (int *)arg;
    shuffle(input, 49, getpid()+counter++);
    pthread_exit(NULL);
}


int main(){
    #pragma region Initialization
    int i, j, balls[THREADCOUNT], arr[THREADCOUNT][LOTOLCOUNT];
    pthread_t t[THREADCOUNT];
    for(i=0;i<THREADCOUNT;i++)
        for(j=0;j<LOTOLCOUNT;j++)
            arr[i][j] = j;
    srand(time(NULL));
    #pragma endregion

    #pragma region Do shuffle
    for(i=0;i<THREADCOUNT;i++){
        pthread_create(&t[i], NULL, child, arr[i]);
        pthread_join(t[i], NULL);
    }
    #pragma endregion

    #pragma region Print result
    printf("The ball's initial setting: ");
    for(i=0;i<THREADCOUNT;i++) printf("%3d", i);
    printf("\n\nThe permuations for balls:\n");
    for(i=0;i<LOTOLCOUNT;i++) printf("%3d", i);
    printf("\n");
    for(i=0;i<LOTOLCOUNT;i++) printf("---");
    printf("\n");
    for(i=0;i<THREADCOUNT;i++){
        for(j=0;j<LOTOLCOUNT;j++)
            printf("%3d",arr[i][j]);
        printf("\n");
    }
        #pragma region Get ball
        for(i=0;i<6;i++){
            balls[i] = arr[i][i];
        }

        for(i=0;i<6;i++){
            for(j=0;j<6;j++){
                balls[i] = arr[i][balls[i]];
            }
        }
        #pragma endregion
    for(i=0;i<LOTOLCOUNT;i++) printf("---");
    printf("\nAfter permutations applied, balls are: ");
    for(i=0;i<THREADCOUNT;i++) printf("%3d", balls[i]);
    printf("\nSo the answer is:                      ");
    for(i=0;i<THREADCOUNT;i++) printf("%3d", balls[i]+1);
    printf("\n");
    #pragma endregion
    return 0;
}
