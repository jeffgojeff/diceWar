#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


//global variables

pthread_cond_t cond1= PTHREAD_COND_INITIALIZER;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int done = 1;



// functions


void *thread1(void *vargp){

    pthread_mutex_lock(&lock);
    if(done==1){
        done =2;
        printf("thread1 waiting\n");
        pthread_cond_wait(&cond1, &lock);
    }
    else{
        printf("thread1 signaling\n");
        pthread_cond_signal(&cond1);
    }

    pthread_mutex_unlock(&lock);

    printf("thread1 returning\n");
    
    
    return NULL;
}


void *thread2(void *vargp){

    pthread_mutex_lock(&lock);
    if(done==1){
        done=2;
        printf("thread2 waiting\n");
        pthread_cond_wait(&cond1, &lock);
    }
    else{
        printf("thread2 signaling\n");
        pthread_cond_signal(&cond1);
    }

    pthread_mutex_unlock(&lock);

    printf("thread2 returning\n");
    
    return NULL;
}





int main(){

    pthread_t thread_id;
    printf("start\n");

    pthread_create(&thread_id, NULL, thread1, NULL);
    pthread_create(&thread_id, NULL, thread2, NULL);
    
    pthread_join(thread_id, NULL);


    printf("end\n");

    return 0;
}