#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


//global variables

//pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
//pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;

//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

sem_t mutex;

int winner, turn, sum = 0;
int playersArray[4];

struct player{
    int sum;
    int playerId;
};





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
    playerStruct->playerId = *((int *) addr);

    int dice1, dice2 = 0;

    
    while(1){

        sem_wait(&mutex);
        
        if(winner == -1){
            turn=(turn+1)%4;
            return NULL;
        }
        
        if(turn == playerStruct->playerId){
            dice1 = rando(6);
            dice2 = rando(6);
            printf("player: %i\n", playerStruct->playerId);
            printf("dice1: %i\n", dice1);
            printf("dice2: %i\n", dice2);
            playerStruct->sum = dice1+dice2;
            if(playerStruct->playerId == 2){
                turn = 4;
                sem_post(&mutex);
                continue;
            }
            else if(playerStruct->playerId == 3){
                turn = 5;
                sem_post(&mutex);
                continue;
            }
            turn=(turn+1)%4;
        }
        sem_post(&mutex);

        }




    
    

    printf("sum: %i\n", playerStruct->sum);
    printf("id: %i\n", playerStruct->playerId);

    return NULL;
}


void *dealer(void *arg){

    struct player *playerA, *playerB, *playerC, *playerD;
    playerA = (struct player *) arg;
    playerB = (struct player *) arg+1;
    playerC = (struct player *) arg+2;
    playerD = (struct player *) arg+3;

    //printf("dealer\n");
    //printf("test: %i\n", playerA->sum);
    //printf("test2: %i\n", playerB->sum);

       while(1){
       sem_wait(&mutex);
      
       //dealer turn checking for player A and C
       if(turn==4){      
           if(playerA->sum == playerC->sum){
               printf("DEALER: The winning team is A and C\n");
               winner=-1;
               turn=3;
               sem_post(&mutex);
               return NULL;
           }
           turn=3;
       //checking for player B and D
       }
       else if(turn==5){
           if(playerB->sum == playerD->sum){
               printf("DEALER: The winning team is B and D\n");
               winner=-1;
               turn=0;
               sem_post(&mutex);
               return NULL;
           }
           turn=0;
       }
       sem_post(&mutex);
   }

    
    return NULL;
}







int main(){

    pthread_t dealerThread, playersThread[4];
    void *status; //no idea what this is
    randomize(); //need for new numbers each time is run

    sem_init(&mutex, 0, 1);

    for(int i=0; i<4; i++){
        playersArray[i]=i;
        pthread_create(&playersThread[i], NULL, player, (void *) &playersArray[i]); //last arg is for function
        //pthread_join(playersThread[i], NULL);
    }

    

    pthread_create(&dealerThread, NULL, dealer, &playersArray);
    pthread_join(dealerThread, NULL);




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