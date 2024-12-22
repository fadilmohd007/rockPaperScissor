#include "sqlite_conn.h"
#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>

static int fetchCallback(void *score, int argc, char **argv, char **col_name) {
    *(int*)score = atoi(argv[0]);
    return 0; // Return 0 to continue processing
}

int CreateDB(char* dbName, sqlite3 **db)
{
    char *err_msg = 0;
    int rc;

    // Open the database or create it if it doesn't exist
    rc = sqlite3_open(dbName, db);  // "local.db" is the database file name

    if (rc) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(*db));
        return -1;
    } else {
        printf("Opened database successfully\n");
        return 0;
    }
}

int DBExec(char* dbQeury, sqlite3 *db)
{
    char *err_msg = 0;
    int rc;
    rc = sqlite3_exec(db, dbQeury, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return -1;
    } else {
        printf("Table created successfully or already exists\n");
        return 0;
    }
}

int FetchScore(char* uName, sqlite3 *db, int* score)
{
    char *err_msg = 0;
    int rc;
    char select_sql[200];
    snprintf(select_sql, sizeof(select_sql), "SELECT Score FROM Users WHERE Name = '%s';", uName);
    rc = sqlite3_exec(db, select_sql, fetchCallback, (void *) score, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    } else {
        printf("Query executed successfully\n");
        return 0;
    }
}

int UpdateScore( char *name, sqlite3 *db, int new_score) {
    const char *update_sql = "UPDATE Users SET Score = ? WHERE Name = ?;"; // SQL query with placeholders
    sqlite3_stmt *stmt;
    
    // Prepare the SQL statement
    int rc = sqlite3_prepare_v2(db, update_sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return rc;  // Return error code
    }

    // Bind the new score and user name to the prepared statement
    rc = sqlite3_bind_int(stmt, 1, new_score);  // Bind the new score
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to bind score: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return rc;
    }
    
    rc = sqlite3_bind_text(stmt, 2, name, -1, SQLITE_STATIC);  // Bind the user name
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to bind name: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return rc;
    }

    // Execute the SQL statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return rc;
    }

    // Finalize the prepared statement
    sqlite3_finalize(stmt);

    // Return success
    printf("Score updated successfully.\n");
    return SQLITE_OK;
}