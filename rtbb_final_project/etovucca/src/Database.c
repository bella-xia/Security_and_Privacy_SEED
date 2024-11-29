/* Database implementations file */
#include "Database.h"

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#define INTEGER_UP_TO_CHANGE 1
#define MAX_SQL_LENGTH 4096

enum _store_type
{
   ELECTION_type,  // 0: There require an id return
   OFFICE_type,    // 1
   CANTIDATE_type, // 2
   VOTER_type,     // 3
   ZIP_type,       // 4: These do not
   VOTE_type,      // 5
   STATUS_type     // 6
};

_id_t storeStatementHelper(sqlite3 *db, int num_args, const int *args,
                           char **arbitrary_data, int is_prepared, const char *sql_format, enum _store_type store_type)
{
   _id_t id = 0;

   if (is_prepared == 1)
   {
      sqlite3_stmt *stmt;
      sqlite3_prepare_v2(db, sql_format, -1, &stmt, NULL);

      for (int i = 0; i < num_args; i++)
      {
         char *data = arbitrary_data[i];
         if (args[i] == 1)
         {
            // label 1 meaning it's integer
            int data_int = atoi(data);
            sqlite3_bind_int(stmt, i + 1, data_int);
         }
         else
         {
            // label 0 meaning it's string
            sqlite3_bind_text(stmt, i + 1, data, (int)strnlen(data, MAX_NAME_LEN),
                              SQLITE_STATIC);
         }
      }
      sqlite3_step(stmt);

      // all store type less or equal to Voter would need to insert a row and have the id returned
      if (store_type <= VOTER_type)
      {
         int rc = sqlite3_finalize(stmt);
         if (rc == SQLITE_OK)
         {
            id = (_id_t)sqlite3_last_insert_rowid(db);
         }
      }
      else
      {
         sqlite3_finalize(stmt);
      }
   }
   else
   {
      char sql[MAX_SQL_LENGTH];
      snprintf(sql, MAX_SQL_LENGTH, sql_format,
               (num_args > 0) ? ((args[0] == 1) ? (void *)(long)atoi(arbitrary_data[0]) : arbitrary_data[0]) : NULL,
               (num_args > 1) ? ((args[1] == 1) ? (void *)(long)atoi(arbitrary_data[1]) : arbitrary_data[1]) : NULL,
               (num_args > 2) ? ((args[2] == 1) ? (void *)(long)atoi(arbitrary_data[2]) : arbitrary_data[2]) : NULL,
               (num_args > 3) ? ((args[3] == 1) ? (void *)(long)atoi(arbitrary_data[3]) : arbitrary_data[3]) : NULL,
               (num_args > 4) ? ((args[4] == 1) ? (void *)(long)atoi(arbitrary_data[4]) : arbitrary_data[4]) : NULL,
               (num_args > 5) ? ((args[5] == 1) ? (void *)(long)atoi(arbitrary_data[5]) : arbitrary_data[5]) : NULL);
      char *errMsg = 0;
      int rc;
      rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
      if (rc == SQLITE_OK)
      {
         if (store_type <= VOTER_type)
         {
            id = (_id_t)sqlite3_last_insert_rowid(db);
         }
      }
   }
   return id;
}

_id_t storeElection(sqlite3 *db, Date deadline)
{
   char *arbitrary_data[4];
   const int args[4] = {1, 1, 1, 1};
   const char *sql_format = (INTEGER_UP_TO_CHANGE == 1)
                                ? "INSERT INTO Election(deadline_day,deadline_mon,deadline_year,status) VALUES (?, ?, ?, ?)"
                                : "INSERT INTO Election(deadline_day,deadline_mon,deadline_year,status) VALUES (%d, %d, %d, %d)";
   char deadline_day[128];
   char deadline_month[128];
   char deadline_year[128];
   char deadline_binary[128];
   snprintf(deadline_day, sizeof(deadline_day), "%d", deadline.day);
   snprintf(deadline_month, sizeof(deadline_month), "%d", deadline.month);
   snprintf(deadline_year, sizeof(deadline_year), "%d", deadline.year);
   snprintf(deadline_binary, sizeof(deadline_binary), "%d", INACTIVE);
   arbitrary_data[0] = deadline_day;
   arbitrary_data[1] = deadline_month;
   arbitrary_data[2] = deadline_year;
   arbitrary_data[3] = deadline_binary;

   _id_t id = storeStatementHelper(db, 4, args, arbitrary_data, INTEGER_UP_TO_CHANGE, sql_format, ELECTION_type);
   return id;
}

_id_t storeOffice(sqlite3 *db, _id_t election, char *name)
{
   char *arbitrary_data[2];
   const int args[2] = {0, 1};
   const char *sql_format = (INTEGER_UP_TO_CHANGE == 1)
                                ? "INSERT INTO Office(name, election) VALUES (?, ?)"
                                : "INSERT INTO Office(name, election) VALUES ('%s', %d)";
   char election_str[128];
   snprintf(election_str, sizeof(election_str), "%d", election);
   arbitrary_data[0] = name;
   arbitrary_data[1] = election_str;

   _id_t id = storeStatementHelper(db, 2, args, arbitrary_data, INTEGER_UP_TO_CHANGE, sql_format, OFFICE_type);
   return id;
}

// _id_t storeCandidate(sqlite3 *db, _id_t office, char *name)
_id_t storeCandidate(sqlite3 *db, _id_t office, char *name)
{

    char *arbitrary_data[4];
    const int args[4] = {0, 1, 1, 0};
    const char *sql_format = (INTEGER_UP_TO_CHANGE == 1)
                                 ? "INSERT INTO Candidate(name,votes,office,bio) VALUES (?, ?, ?, ?)"
                                 : "INSERT INTO Candidate(name,votes,office,bio) VALUES ('%s', %d, %d, '%s')";

   char binary_str[128];
   char office_str[128];
   char bio_str[128] =  "This candidate has not created their bio yet...";
   snprintf(binary_str, sizeof(binary_str), "%d", 0);
   snprintf(office_str, sizeof(office_str), "%d", office);
   arbitrary_data[0] = name;
   arbitrary_data[1] = binary_str;
   arbitrary_data[2] = office_str;
   arbitrary_data[3] = bio_str;

   _id_t id = storeStatementHelper(db, 4, args, arbitrary_data, INTEGER_UP_TO_CHANGE, sql_format, CANTIDATE_type);
   return id;
}

void addZip(sqlite3 *db, _id_t office, int zip)
{
   char *arbitrary_data[2];
   const int args[2] = {1, 1};
   const char *sql_format = (INTEGER_UP_TO_CHANGE == 1)
                                ? "INSERT INTO AllowedZip(zip,office) VALUES (?, ?)"
                                : "INSERT INTO AllowedZip(zip,office) VALUES (%d, %d)";
   char zip_str[128];
   char office_str[128];
   snprintf(zip_str, sizeof(zip_str), "%d", zip);
   snprintf(office_str, sizeof(office_str), "%d", office);
   arbitrary_data[0] = zip_str;
   arbitrary_data[1] = office_str;

   storeStatementHelper(db, 2, args, arbitrary_data, INTEGER_UP_TO_CHANGE, sql_format, ZIP_type);
}

void getCandidate(sqlite3 *db, _id_t candidate_id, Candidate *dest)
{
   sqlite3_stmt *stmt;
   const char *sql = "SELECT name\
                      FROM Candidate WHERE id=?";
   sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
   sqlite3_bind_int(stmt, 1, candidate_id);
   sqlite3_step(stmt);
   strncpy(dest->name, (char *)sqlite3_column_text(stmt, 0), MAX_NAME_LEN - 1);
   // strncpy(dest->password, (char *)sqlite3_column_text(stmt, 1),MAX_NAME_LEN-1);
   (dest->name)[MAX_NAME_LEN - 1] = '\0';
   //(dest->password)[MAX_NAME_LEN-1] = '\0';

   sqlite3_finalize(stmt);
}

bool checkZip(sqlite3 *db, _id_t office, int zip)
{
   int count;
   sqlite3_stmt *stmt;
   const char *sql = "SELECT COUNT(*) FROM AllowedZip WHERE\
                      zip=? AND office=?";
   sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
   sqlite3_bind_int(stmt, 1, zip);
   sqlite3_bind_int(stmt, 2, office);
   sqlite3_step(stmt);
   count = sqlite3_column_int(stmt, 0);
   sqlite3_finalize(stmt);
   return count > 0;
}

_id_t storeVoter(sqlite3 *db, char *name, char *county, int zip, Date dob)
{

   char *arbitrary_data[6];
   const int args[6] = {0, 0, 1, 1, 1, 1};
   int integer_up_to_change = strcmp(county, "proxy") ?  INTEGER_UP_TO_CHANGE : 0;
   const char *sql_format = (integer_up_to_change == 1)
                                ? "INSERT INTO Registration(name,county,zip,dob_day,dob_mon,dob_year) VALUES (?, ?, ?, ?, ?, ?)"
                                : "INSERT INTO Registration(name,county,zip,dob_day,dob_mon,dob_year) VALUES ('%s', '%s', %d, %d, %d, %d)";
   char zip_str[128];
   char dob_day[128];
   char dob_month[128];
   char dob_year[128];
   snprintf(zip_str, sizeof(zip_str), "%d", zip);
   snprintf(dob_day, sizeof(dob_day), "%d", dob.day);
   snprintf(dob_month, sizeof(dob_month), "%d", dob.month);
   snprintf(dob_year, sizeof(dob_year), "%d", dob.year);
   arbitrary_data[0] = name;
   arbitrary_data[1] = county;
   arbitrary_data[2] = zip_str;
   arbitrary_data[3] = dob_day;
   arbitrary_data[4] = dob_month;
   arbitrary_data[5] = dob_year;

   _id_t id = storeStatementHelper(db, 6, args, arbitrary_data, integer_up_to_change, sql_format, VOTER_type);
   return id;
}

void storeStatus(sqlite3 *db, _id_t election, Status new_status)
{

   char *arbitrary_data[2];
   const int args[2] = {1, 1};
   const char *sql_format = (INTEGER_UP_TO_CHANGE == 1)
                                ? "UPDATE Election SET status=? WHERE id=?"
                                : "UPDATE Election SET status=%d WHERE id=%d";
   char new_status_str[128];
   char election_str[128];
   snprintf(new_status_str, sizeof(new_status_str), "%d", (int)new_status);
   snprintf(election_str, sizeof(election_str), "%d", election);
   arbitrary_data[0] = new_status_str;
   arbitrary_data[1] = election_str;

   storeStatementHelper(db, 2, args, arbitrary_data, INTEGER_UP_TO_CHANGE, sql_format, STATUS_type);

   if (new_status == PUBLISHED)
   {
      const char *sql_format = "UPDATE Candidate SET votes=(\
                              SELECT COUNT(*) FROM Vote WHERE\
                              Vote.candidate=Candidate.id AND\
                              Vote.office=Candidate.office)";
      storeStatementHelper(db, 0, NULL, NULL, INTEGER_UP_TO_CHANGE, sql_format, STATUS_type);
   }
}

void deleteElection(sqlite3 *db, _id_t election)
{
   sqlite3_stmt *stmt;
   const char *sql = "DELETE FROM Election WHERE id=?";
   sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
   sqlite3_bind_int(stmt, 1, election);
   sqlite3_step(stmt);
   sqlite3_finalize(stmt);
}

void getVoter(sqlite3 *db, _id_t voter_id, Registration *dest)
{
   sqlite3_stmt *stmt;
   const char *sql = "SELECT name,county,zip,dob_day,dob_mon,dob_year\
                      FROM Registration WHERE id=?";
   sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
   sqlite3_bind_int(stmt, 1, voter_id);
   sqlite3_step(stmt);
   strncpy(dest->name, (char *)sqlite3_column_text(stmt, 0), MAX_NAME_LEN - 1);
   strncpy(dest->county, (char *)sqlite3_column_text(stmt, 1), MAX_NAME_LEN - 1);
   (dest->name)[MAX_NAME_LEN - 1] = '\0';
   (dest->county)[MAX_NAME_LEN - 1] = '\0';
   dest->zip = sqlite3_column_int(stmt, 2);
   (dest->dob).day = sqlite3_column_int(stmt, 3);
   (dest->dob).month = sqlite3_column_int(stmt, 4);
   (dest->dob).year = sqlite3_column_int(stmt, 5);
   sqlite3_finalize(stmt);
}

void getElection(sqlite3 *db, _id_t election_id, Election *dest)
{
   sqlite3_stmt *stmt;
   const char *sql = "SELECT deadline_day,deadline_mon,deadline_year,status\
                      FROM Election WHERE id=?";
   sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
   sqlite3_bind_int(stmt, 1, election_id);
   sqlite3_step(stmt);
   (dest->deadline).day = sqlite3_column_int(stmt, 0);
   (dest->deadline).month = sqlite3_column_int(stmt, 1);
   (dest->deadline).year = sqlite3_column_int(stmt, 2);
   dest->status = (Status)sqlite3_column_int(stmt, 3);
   sqlite3_finalize(stmt);
}

void storeVote(sqlite3 *db, _id_t voter, _id_t candidate, _id_t office)
{

   char *arbitrary_data[3];
   const int args[3] = {1, 1, 1};
   const char *sql_format = (INTEGER_UP_TO_CHANGE == 1)
                                ? "INSERT INTO Vote(voter,candidate,office) VALUES (?, ?, ?)"
                                : "INSERT INTO Vote(voter,candidate,office) VALUES (%d, %d, %d)";
   char voter_str[128];
   char candidate_str[128];
   char office_str[128];
   snprintf(voter_str, sizeof(voter_str), "%d", voter);
   snprintf(candidate_str, sizeof(candidate_str), "%d", candidate);
   snprintf(office_str, sizeof(office_str), "%d", office);
   arbitrary_data[0] = voter_str;
   arbitrary_data[1] = candidate_str;
   arbitrary_data[2] = office_str;

   storeStatementHelper(db, 3, args, arbitrary_data, INTEGER_UP_TO_CHANGE, sql_format, VOTE_type);
}

int getVote(sqlite3 *db, _id_t voter_id, _id_t office_id)
{
   int count;
   sqlite3_stmt *stmt;
   const char *sql = "SELECT COUNT(*) FROM Vote WHERE\
                      voter=? AND office=?";
   sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
   sqlite3_bind_int(stmt, 1, voter_id);
   sqlite3_bind_int(stmt, 2, office_id);
   sqlite3_step(stmt);
   count = sqlite3_column_int(stmt, 0);
   sqlite3_finalize(stmt);
   return count;
}

void getVoters(sqlite3 *db)
{
   sqlite3_stmt *stmt;
   const char *sql = "SELECT name,county,zip,dob_day,dob_mon,dob_year\
                      FROM Registration";
   sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
   printf("[\n");
   bool is_first = false;
   while (sqlite3_step(stmt) != SQLITE_DONE)
   {
      if (!is_first)
      {
         is_first = true;
      }
      else
      {
         printf(",\n");
      }
      printf("{\"name\": \"%s\", \"county\": \"%s\", \"zip\": \"%d\", ",
             sqlite3_column_text(stmt, 0),
             sqlite3_column_text(stmt, 1),
             sqlite3_column_int(stmt, 2));
      printf("\"dob\": \"%d-%02d-%02d\"}",
             sqlite3_column_int(stmt, 5) + 1900,
             sqlite3_column_int(stmt, 4),
             sqlite3_column_int(stmt, 3));
   }
   printf("\n]\n");
   sqlite3_finalize(stmt);
}

void getElections(sqlite3 *db)
{
   system("./database_helper.py"); /* U+1F914 */
}
