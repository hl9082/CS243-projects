/**
 * log.c - logfile record processing routines
 *
 * @author A. N. Onymous
 * @author Huy Le
 */

// need the getline() prototype from stdio
#define    _XOPEN_SOURCE    700

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#include "log.h"
#include "emp.h"
#include "parse.h"

/*
** Global list of logfile entries
*/

LogEntry *logfile;
int num_logentries;

/*
** Private functions
*/

/*
** Return values for add_logentry()
*/

typedef enum { ADD_OK, ADD_ALLOC, ADD_DUP } Status;



/**
 * add_logentry() - add a new logentry to the database
 *
 * @param  line   current input line number
 * @param  id     relevant employee id
 * @param  date   date of event
 * @param  time   time of event
 * @param  loc    location of event
 * @param  code   event code
 *
 * @return 1 on success, otherwise 0
 */

static Status add_logentry( int line, int id, int date, int time,
                            int loc, uint8_t code ) {
    LogEntry *entry, *prev, *curr;

    // locate the entry for this ID

    entry = find_logentry( id );

    // if there wasn't one, allocate one and insert it

    if( entry == NULL ) {
 
        entry = new_logentry( id );
        if( entry == NULL ) {
            // no room for this one?
            fprintf( stderr, "add_logentry(): can't create for line %d\n",
                     line );
            return( ADD_ALLOC );
        }

        //
        // insert this logentry into the logentry list, which
        // is ordered by ID
        //

        prev = NULL;
        curr = logfile;

        while( curr && curr->id < id ) {
            prev = curr;
            curr = curr->next;
        }

        if( curr && curr->id == id ) {  // can't happen!
            fprintf( stderr, "add_logentry(): duplicate id %d AFTER check!\n",
                     id );
            exit( EXIT_FAILURE );
        }

        // at this point:
        //  curr    prev    situation   
        //  =====   =====   =======================
        //  NULL    NULL    list is empty
        //  NULL    !NULL   add at end of list
        //  !NULL   NULL    add at front of list
        //  !NULL   !NULL   add in middle of list

        if( curr == NULL ) {

            if( prev == NULL ) {  // first node!
                logfile = entry;
            } else {              // add at the end of the list
                prev->next = entry;
            }

        } else {

            if( prev == NULL ) { // add at the front of the list
                entry->next = logfile;
                logfile = entry;
            } else {             // add in the middle of the list
                entry->next = curr;
                prev->next = entry;
            }
        }

    }

    // we've located (or created) the logentry node for this ID
    //
    // next, add a record to its list of events

    EventRecord *event = new_event( date, time, loc, code );
    if( event == NULL ) {
        fprintf( stderr, "add_logentry(): can't create event node, line %d\n",
                 line );
        return( ADD_ALLOC );
    }

    // event records should be maintained in order by time
    //
    // we search for the first entry in the list which has
    // a date > this event's date, or the dates are equal
    // and the time is > this event's time

    EventRecord *erprev, *ercurr;

    erprev = NULL;
    ercurr = entry->first;

    while( ercurr != NULL && // fix: added a NULL check for ercurr
       (ercurr->date < event->date || 
       (ercurr->date == event->date && ercurr->time <= event->time)) ) {//fix: change by adding the condition to check if it's null or not.
//fix : compare the date as well
//fix: put the date before time and make sure dates are in correct order
//fix: we fix the time comparison as well.
//fix: we fix by using the or comparison
        erprev = ercurr;
        ercurr = ercurr->next;
    }

    // see above for the insertion logic

    if( ercurr == NULL ) {
        if( erprev == NULL ) {  // first node!
            entry->first = event;
        } else {              // add at the end of the list
            erprev->next = event;
        }
    } else {
        if( erprev == NULL ) { // add at the front of the list
            event->next = entry->first;
            entry->first = event;
        } else {             // add in the middle of the list
            event->next = ercurr;
            erprev->next = event;
        }
    }

    return( ADD_OK );

}

/**
 * print_date(date,fp) - print a date in yyyy/mm/dd form to a stream
 *
 * @param date the date to be printed
 * @param fp   the output stream to be used
 */

static void print_date( int date, FILE *fp ) {
    fprintf( fp, "%4d/%02d/%02d", date / 10000, (date / 100) % 100,
             date % 100 );
}

/*
** Public functions
*/

/**
 * new_event() - create a new event record
 */

EventRecord *new_event( int date, int time, int loc, uint8_t code ) {
    EventRecord *new;

    // allocate the node
    new = (EventRecord *) calloc( 1, sizeof(EventRecord) );

    // if this succeeded, fill in the fields
    if( new != NULL ) {

        // we assume that the pointers we're given can
        // just be copied into the new record
        //
        new->date = date;
        new->time = time;
        new->loc = loc;
        new->code = code;

        // we used calloc(), so 'next' is already NULL
    }

    return( new );
}

/**
 * free_event() - deallocate an event node
 */

void free_event( EventRecord *event ) {

    // sanity check - be sure we were given a non-NULL pointer

    if( event != NULL ) {
        free( event );
    }

}

/**
 * new_logentry() - create a new logentry
 */

LogEntry *new_logentry( int id ) {
    LogEntry *new;

    // allocate the node
    new = (LogEntry *) calloc( 1, sizeof(LogEntry) );

    // if this succeeded, fill in the fields
    if( new != NULL ) {

        new->id = id;

        // we used calloc(), so all pointer fields are NULL
    }

    return( new );
}

/**
 * free_logentry() - deallocate a logentry node
 */

void free_logentry( LogEntry *logentry ) {

    // sanity check - be sure we were given a non-NULL pointer

    if( logentry != NULL ) {

        // deal with the event list
        if( logentry->first != NULL ) {
            EventRecord *curr, *next;
            curr = logentry->first;
            while( curr ) {
                next = curr->next;
                free( curr );
                curr = next;
            }
        }

        free( logentry );
    }

}

/**
 * find_logentry() - locate the entry for a specific employee id
 */

LogEntry *find_logentry( int id ) {
    LogEntry *logentry;

    logentry = logfile;
    while( logentry && logentry->id != id ) {
        logentry = logentry->next;
    }

    return( logentry );
}

/**
 * load_logfile() - load the logfile database
 */

int load_logfile( FILE *file ) {
    char *buf;
    size_t len;
    int nlines, nlogs;

    nlines = nlogs = 0;

    // iterate through the records in the file

    buf = NULL;
    len = 0;

    while( getline(&buf,&len,file) >= 0 ) {

        ++nlines;

        // ignore comment lines and blank/empty lines

        if( buf[0] == '#' || buf[0] == '\n' || buf[0] == '\0' ) {
            continue;
        }

        //
        // these lines have the following format:
        //
        //     id,date,time,location,code
        //
        // fields are separate by ',' characters
        //
        // parse the line into five fields
        //

        char *fields[5];

        int num = parse( buf, fields, 5 );
        if( num != 5 ) {
                fprintf( stderr, "load_logfile(): line %d has %d fields",
                    nlines, num );
                fputs( "; ignoring line\n", stderr );
                continue;
        }

        // add this entry to the database

        Status result;
        result = add_logentry( nlines,
            (int) strtol(fields[0],NULL,10),
            (int) strtol(fields[1],NULL,10),
            (int) strtol(fields[2],NULL,10),
            (int) strtol(fields[3],NULL,10),
            (uint8_t) strtol(fields[4],NULL,10) );

        for( int i = 0; i < num; ++i ) {
            free( fields[i] );
        }

        if( result == ADD_OK ) {
            ++nlogs;
        }

    }

    // release the getline() buffer

    if( buf != NULL ) {
        free( buf );
    }

    num_logentries = nlogs;

    return( num_logentries );
}

/**
 * generate_report() - produce the activity report
 */

void generate_report( void ) {
    LogEntry *entry;
    EventRecord *event;
    Employee *emp;

    puts( "Security log analysis" );
    printf( "Total number of employees:  %d\n", num_employees );
    printf( "Number of log entries:  %d\n", num_logentries );

    // iterate through all employees who have security log entries

    for( entry = logfile; entry; entry = entry->next ) {

        printf( "\nReport for employee #%d", entry->id );

        // locate the employee's database entry

        emp = find_emp( entry->id );
        if( emp == NULL ) {

            printf( " (NOT IN SYSTEM)\n" );

        } else {

            printf( " (%s, %s) hired ", emp->last, emp->first );
            print_date( emp->hire, stdout );
            putchar('\n');
            //fixed: change the format of the actual output to match the required output.

            // now, iterate through the event list for this employee

            for( event = entry->first; event; event = event->next ) {

                putchar( '\t' );
                print_date( event->date, stdout );

                printf( " %02d:%02d ", event->time / 100,
                        event->time % 100 );

                switch( event->code ) {
                case LOG_ENTRY:
                    printf( "Entered room %d\n", event->loc );
                    break;
                case LOG_EXIT:
                    printf( "Exited room %d\n", event->loc );
                    break;
                case LOG_BADCODE:
                    printf( "***bad security code***\n" );
                    break;
                default:
                    printf( "***bad event code***\n" );
                    break;
                }
            }
        }
    }
}

/**
 * delete_all_logentries() - delete all logfile entries
 */

void delete_all_logentries( void ) {
    LogEntry *tmp, *entry = logfile;
    //fixed: added the dummy node to free the entry
    while( entry ) {
        tmp = entry->next;
        free_logentry( entry );
        entry = tmp;
    }
}
