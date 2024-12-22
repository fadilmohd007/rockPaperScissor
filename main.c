#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "rps.h"
#include "random_gen.h"
#include "sqlite_conn.h"

//Determine the win of pair with lookup
/*
     S   P  C - machine
   S 0  -1  1
   P 1  0  -1
   C-1  1   0      
*/

int lookUp[3][3] = 
{
    0,-1,1,
    1,0,1,
    -1,1,0
};
int getGamePoint (enum RPS_CHOICE uChoiceI, enum RPS_CHOICE mChoiceI)
{
    return lookUp[uChoiceI][mChoiceI];
}

int main(int argc, char** argv){
    char uName[10];
    char choiceC;
    int uChoiceI,mChoiceI;//user choice and machine choice
    int ret, gamePoint;
    int userScore = 0;
    sqlite3 *db;
    char* dbName = "local.db";
    ret = CreateDB(dbName,&db);
    if(ret != 0)
    {
        printf("error openign db file, exiting..\n");
        return -1;
    }
    
    char *create_table_sql = "CREATE TABLE IF NOT EXISTS Users ( Name TEXT PRIMARY KEY, Score INTEGER);";
    ret = DBExec(create_table_sql, db);
    if(ret != 0)
    {
        printf("error creating Table in db, exiting..\n");
        return -1;
    }
    scan:
    printf("Enter user name: ");
    // uName can be max 10 char
    ret = scanf("%10s",uName);
    if(ret != 1)
    {
        printf( "Error in reading srting\n");
        goto scan;
    }
    char insert_sql[256];
    snprintf(insert_sql, sizeof(insert_sql), "INSERT OR IGNORE INTO Users (Name, Score) VALUES ('%s', 0);", uName);
    ret = DBExec(insert_sql, db);
    if(ret != 0)
    {
        printf("error inserting User in  db, exiting..\n");
        return -1;
    }
    printf("welcome to Rock Paper Sciccor %s\n",uName);

    ret = FetchScore(uName, db, &userScore);
    if(ret != 0)
    {
        printf("error fetching score, exiting..\n");
        return -1;
    }
    printf("Your current score is %d\n",userScore);

    /*
    PLAY GAME
    */
    printf("possible options are \n\
            1/'s' for  Stone\n\
            2/'p' for  paper\n\
            3/'c' for  scissor\n");
    scan_choice:
    printf("Choose your option:");
    scanf(" %1c",&choiceC);
    while (getchar() != '\n');
    uChoiceI = atoi(&choiceC);
    if(uChoiceI == 1 || uChoiceI == 2 || uChoiceI == 3)
        goto no_char_conv;
    switch(choiceC)
    {
        case('s'):
            uChoiceI = stone;
            break;
        case('p'):
            uChoiceI = paper;
            break;
        case('c'):
            uChoiceI = scissor;
            break;
        default:
            printf("please read intruction carefully\n");
            goto scan_choice;
    }

    no_char_conv:
    printf("you entered option %s\n", choices[uChoiceI]);
    mChoiceI = genIntRandomInRange(3,1);
    printf("computer's option %d %s\n",mChoiceI, choices[mChoiceI]);
    /*
    UPDATE SCORE
    */
    gamePoint = getGamePoint(uChoiceI,mChoiceI);
    printf("You got  %d point\n", gamePoint);
    userScore = userScore + gamePoint;
    ret = UpdateScore( uName, db, userScore);
    if(ret != 0)
    {
        printf("error inserting User in  db, exiting..\n");
        return -1;
    }
    goto scan_choice;
    sqlite3_close(db);
    return 0;
}