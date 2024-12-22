#include <stdio.h>
#include <sqlite3.h>

#ifndef SQLITE_CONN_H
#define SQLITE_CONN_H

int CreateDB(char* dbName, sqlite3 **db);
int DBExec(char* dbQeury, sqlite3 *db);
int FetchScore(char* uName, sqlite3 *db, int* score);
int UpdateScore( char *name, sqlite3 *db, int new_score);


#endif //SQLITE_CONN_H