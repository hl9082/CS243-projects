/**
 * log.h - data structures and prototypes for logfile records
 *
 * @author A. N. Onymous
 * @author Huy Le
 */

#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>
#include <stdint.h>

/*
** Logfile entry code values
*/

#define LOG_ENTRY   0
#define LOG_EXIT    1
#define LOG_BADCODE 2

    // number of logfile codes
#define LOG_NCODES  3

/*
** Event record
*/

typedef
    struct record {
        struct record *next;  // link to next entry for this employee
        int date;      // date of this entry
        int time;      // time of this entry
        int loc;       // location of the event
        uint8_t code;  // the event code
    }
        EventRecord;

/*
** Logfile entry data structure
*/

typedef
    struct logentry {
        struct logentry *next;  // next logfile entry in the list
        EventRecord *first;     // first event record for this employee
        int id;                 // employee ID
    }
        LogEntry;

/*
** Global list of log file entires
*/

extern LogEntry *logfile;
extern int num_logentries;

/**
 * new_event() - create a new event record
 *
 * @param  date    date of event
 * @param  time    time of event
 * @param  loc     location
 * @param  code    event code
 *
 * @return newly allocated event record, or NULL on failure
 */

EventRecord *new_event( int date, int time, int loc, uint8_t code );

/**
 * free_event() - deallocate an event node
 *
 * @param  event    pointer to the node to deallocate
 */

void free_event( EventRecord *event );

/**
 * new_logentry() - create a new logentry
 *
 * @param  id      relevant employee id
 *
 * @return newly allocated logentry record, or NULL on failure
 */

LogEntry *new_logentry( int id );

/**
 * free_logentry() - deallocate a logentry node
 *
 * @param  logentry    pointer to the node to deallocate
 */

void free_logentry( LogEntry *logentry );

/**
 * find_logentry() - locate the entry for a specific employee id
 *
 * @param  id  the ID to locate
 *
 * @return a pointer to the LogEntry record, or NULL
 */

LogEntry *find_logentry( int id );

/**
 * load_logfile() - load the logfile database
 *
 * @param  file    FILE * to read entries from
 *
 * @return count of logfile records loaded, or 0 on error
 */

int load_logfile( FILE *file );

/*
** generate_report() - produce the activity report
*/

void generate_report( void );

/*
** delete_all_logentries() - delete all logfile entries
*/

void delete_all_logentries( void );

#endif
