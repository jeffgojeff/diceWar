#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//global vars
int winner = 0;

int players[4][1];




int rando(int max){
    return(rand() % max) +1;
}

void randomize(){
    srand( (unsigned)time(NULL));
}

int winnerCheck(){
    if(players[0][0] == players[2][0]){
        winner=1;
        return 1;
    }
    if(players[1][0] == players[3][0]){
        winner=2;
        return 2;
    }
    else{
        return 0;
    }
}

void printWinner(){
    if(winner==1){
        printf("Players 0 & 2 are the winners\n");
    }
    if(winner==2){
        printf("Players 1 & 3 are the winners\n");
    }
}

int diceRoll(){
    int dice1, dice2, roll = 0;
    //randomize(); //this cant go here
    dice1 = rando(6);
    dice2 = rando(6);
    roll = dice1 + dice2;

    return roll;
}

void init(){
    for(int i=0; i<4; i++){
        players[i][0] = diceRoll();
    }
}



void dealer(){
    winnerCheck();
    int test = 0;
    int forCount = 0;
    int whileCount = 0;
    while(winner == 0){
        for(int i=0;i<4;i++){
            players[i][0] = diceRoll();
            test = winnerCheck();
            forCount++;
            if(test !=0){
                break;
            }
        }
        printf("\n");
        whileCount++;
        for(int i=0;i<4;i++){
            printf("player%i: %i\n", i, players[i][0]);
        }
    }

    printf("*************************\n");
    printf("\n");
    printf("final rolls\n");
    for(int i=0;i<4;i++){

        printf("player%i: %i\n", i, players[i][0]);
    }
    printf("forCount: %i\n", forCount);
    printf("whileCount: %i\n", whileCount);
    printWinner();


}



int main(){
    randomize();
    init();

    dealer();

    
    



}