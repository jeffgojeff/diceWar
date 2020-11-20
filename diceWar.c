#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


//global variables

pthread_cond_t cond1= PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2= PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int done = 1;



// functions


void *thread1(void *vargp){

    pthread_mutex_lock(&mutex);
    printf("thread1 after lock\n");
    //pthread_cond_wait(&cond1, &lock);
    //pthread_cond_signal(&cond1);

    while(done<3){
        pthread_cond_wait(&cond1, &mutex);
        printf("while loop\n");
    }

    pthread_mutex_unlock(&mutex);
    //printf("thread1 after unlock\n");
    
    
    return NULL;
}


void *thread2(void *vargp){

    pthread_mutex_lock(&mutex);
    //printf("thread2 after lock\n");
    //pthread_cond_wait(&cond1, &lock);
    //pthread_cond_signal(&cond1);

    done++;
    pthread_cond_signal(&cond1);

    pthread_mutex_unlock(&mutex);
    printf("thread2 after unlock\n");
    
    return NULL;
}

void *thread3(void *vargp){

    printf("thread3 wait before\n");
    pthread_cond_wait(&cond1, &mutex);
    printf("thread3 wait after\n");



}






int main(){

    pthread_t thread_id;
    printf("start\n");

    //printf("create thread 3\n");
    //pthread_create(&thread_id, NULL, thread3, NULL);

    printf("create thread 1\n");
    pthread_create(&thread_id, NULL, thread1, NULL);

    
    printf("create thread 2\n");
    pthread_create(&thread_id, NULL, thread2, NULL);
    
    
    printf("before join\n");
    pthread_join(thread_id, NULL);
    printf("after join\n");


    printf("end\n");

    return 0;
}