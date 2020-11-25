/**********************************\
 * Jeffrey C. Slocum
 * 
 * Mina Guirguis Project #2
 * Matching Dice Game
 * 
 * 
/**********************************/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include<time.h>

//global variables

pthread_mutex_t mutex;
pthread_cond_t winner_var;

int turn=0;
int players[4];

int numThreads=4;
int threads[4];

int winner, sum = 0;
int loop = 1;

FILE *fp;

//functions

//randomize the random seed
void randomize(){
    srand( (unsigned)time(NULL));
}

//return random number
int rando(int max){
    return(rand() % max) +1;
}

//player thread function

void *playerThread(void *arg) {

   int id =*((int *)arg);
   int dice1,dice2 = 0;

   while(loop == 1){
       pthread_cond_wait(&winner_var, &mutex);
       //check for winner
       if(winner==1){
           turn=(turn+1)%4;
           break;
       }

        //check for who's turn it is
       if(turn==id){
           dice1=rando(6);
           dice2=rando(6);
           sum = dice1 + dice2;
            //print roll info to screen
           printf("PLAYER %c: %i %i\n", id+65, dice1, dice2);
           //write info to file
           fprintf(fp, "Player %c: gets %d and %d with a sum %d\n",id+65,dice1,dice2,sum);
            //store total roll value correct array slot
           players[id]=sum;

            //change whos turn it is next
           if(id==2){
               turn=4;
               //signal other threads
               pthread_cond_signal(&winner_var);
               continue;
           }
           else if(id==3){
               turn=5;
               //signal other threads
               pthread_cond_signal(&winner_var);
               continue;
           }
           turn=(turn+1)%4;
       }
       //signal other threads
       pthread_cond_signal(&winner_var);
   }
  
}

//dealer thread function

void *dealerThread(void *arg) {

   while(loop == 1){
       //wait for other threads to signal
       //before starting turn
       pthread_cond_wait(&winner_var, &mutex);
       if(turn==4){
           if(players[0]==players[2]){
               //print winner to console
               printf("DEALER: The winning team is A and C\n");
               //write log info to file
               fprintf(fp, "DEALER: The winning team is A and C\n");
               winner=1;
               turn=3;
               //signal other threads
               pthread_cond_signal(&winner_var);
               break;
           }
           turn=3;
       
       }
       else if(turn==5){
           if(players[1]==players[3]){
               printf("DEALER: The winning team is B and D\n");
               fprintf(fp, "DEALER: The winning team is B and D\n");
               winner=1;
               turn=0;
               //signal other threads
               pthread_cond_signal(&winner_var);
               break;
           }
           turn=0;
       }
       //signal other threads
       pthread_cond_signal(&winner_var);
   }
}



int main()
{
    //reseed the rand number for different numbers each time run
    randomize();
    //open file to write logs to
    fp = fopen("logOutput.txt", "a");

    //initialize mutex and semaphore
    pthread_cond_init(&winner_var, NULL);
    //delcare array of player threads
    pthread_t *player = (pthread_t *)malloc(sizeof(pthread_t)*numThreads);
    //declare dealer thread
    pthread_t dealer;
    
    //loop through to create player threads
    //store info for threads in array
    //pass the array location as an argument to the thread being created
    for(int i=0;i<numThreads;i++){
        threads[i]=i;
        pthread_create(&player[i],NULL,playerThread,&threads[i]);
    }
    //create dealer thread
    pthread_create(&dealer,NULL,dealerThread,NULL);
    
    //loop through all created threads and join them
    for (int i = 0; i < numThreads; i++){
        pthread_join(player[i], NULL);
    }
    //join dealer thread to all player threads
    pthread_join(dealer,NULL);

return 0;
}