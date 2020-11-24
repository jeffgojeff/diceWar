#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


//global variables

pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int winner = 0;

struct player{
    int sum;
};

struct player playersArray[4];



// functions

//helper funcs

int rando(int max){
    return(rand() % max) +1;
}

void randomize(){
    srand( (unsigned)time(NULL));
}

int diceRoll(){
    int dice1, dice2, roll = 0;
    //randomize(); //this cant go here
    dice1 = rando(6);
    dice2 = rando(6);
    roll = dice1 + dice2;

    return roll;
}


//threads


void *player(void *addr){

    struct player *playerStruct;
    playerStruct = (struct player *) addr;
    playerStruct->sum = diceRoll();

    
    while(winner == 0){
        pthread_cond_wait(&cond1, &mutex);
        playerStruct->sum = diceRoll();
    }
    //playerStruct->sum = diceRoll();
    printf("sum: %i\n", playerStruct->sum);
    
    return NULL;
}


void *dealer(void *arg){

    struct player *playerA, *playerB, *playerC, *playerD;
    playerA = (struct player *) arg;
    playerB = (struct player *) arg+1;
    playerC = (struct player *) arg+2;
    playerD = (struct player *) arg+3;

    printf("dealer\n");
    printf("test: %i\n", playerA->sum);
    printf("test2: %i\n", playerB->sum);


    while(winner == 0){

        pthread_cond_wait(&cond2, &mutex);
        if(playerA->sum == playerC->sum){
            winner=1;
            //return 1;
        }
        if(playerB->sum == playerD->sum){
            winner=2;
            //return 2;
        }
        else{
            //return 0;
        }

    }

    
    return NULL;
}

void *thread3(void *vargp){

    return NULL;

}







int main(){

    pthread_t dealerThread, playersThread[4];
    void *status; //no idea what this is
    randomize(); //need for new numbers each time is run

    for(int i=0; i<4; i++){
        pthread_create(&playersThread[i], NULL, player, (void *) &playersArray[i]); //last arg is for function
        pthread_join(playersThread[i], &status);
    }

    pthread_create(&dealerThread, NULL, dealer, &playersArray);
    pthread_join(dealerThread, &status);



    return 0;
}



/*

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


//main starts here

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
*/