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
int getGamePoint (enum RPS_CHOICE uchoice_i, enum RPS_CHOICE mchoice_i)
{
    return lookUp[uchoice_i][mchoice_i];
}

int main(int argc, char** argv){
    char uname[10];
    char choice_c;
    int uchoice_i,mchoice_i;//user choice and machine choice
    int ret, gamePoint;
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
    // uname can be max 10 char
    ret = scanf("%10s",uname);
    if(ret != 1)
    {
        printf( "Error in reading srting\n");
        goto scan;
    }
    char insert_sql[256];
    snprintf(insert_sql, sizeof(insert_sql), "INSERT OR IGNORE INTO Users (Name, Score) VALUES ('%s', 0);", uname);
    ret = DBExec(insert_sql, db);
    if(ret != 0)
    {
        printf("error inserting User in  db, exiting..\n");
        return -1;
    }
    printf("welcome to Rock Paper Sciccor %s\n",uname);
    int user_score = 0;
    ret = FetchScore(uname, db, &user_score);
    if(ret != 0)
    {
        printf("error fetching score, exiting..\n");
        return -1;
    }
    printf("Your current score is %d\n",user_score);

    /*
    PLAY GAME
    */
    printf("possible options are \n\
            1/'s' for  Stone\n\
            2/'p' for  paper\n\
            3/'c' for  scissor\n");
    scan_choice:
    printf("Choose your option:");
    scanf(" %1c",&choice_c);
    while (getchar() != '\n');
    uchoice_i = atoi(&choice_c);
    if(uchoice_i == 1 || uchoice_i == 2 || uchoice_i == 3)
        goto no_char_conv;
    switch(choice_c)
    {
        case('s'):
            uchoice_i = stone;
            break;
        case('p'):
            uchoice_i = paper;
            break;
        case('c'):
            uchoice_i = scissor;
            break;
        default:
            printf("please read intruction carefully\n");
            goto scan_choice;
    }

    no_char_conv:
    printf("you entered option %s\n", choices[uchoice_i]);
    mchoice_i = genIntRandomInRange(3,1);
    printf("computer's option %d %s\n",mchoice_i, choices[mchoice_i]);
    /*
    UPDATE SCORE
    */
    gamePoint = getGamePoint(uchoice_i,mchoice_i);
    printf("You got  %d point\n", gamePoint);
    user_score = user_score + gamePoint;
    ret = UpdateScore( uname, db, user_score);
    if(ret != 0)
    {
        printf("error inserting User in  db, exiting..\n");
        return -1;
    }
    goto scan_choice;
    sqlite3_close(db);
    return 0;
}