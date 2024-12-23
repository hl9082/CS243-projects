/**
 * emp.c - employee record processing routines
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

#include "emp.h"
#include "parse.h"

/*
** Global list of employees
*/

Employee *employees;
int num_employees;

/*
** Private functions
*/

/*
** Return values for add_employee()
*/

typedef enum {
    ADD_OK, ADD_ALLOC, ADD_DUP
} Status;

/**
 * add_employee() - add a new employee to the database
 *
 * @param  line  input line number (for error reporting)
 * @param  id    employee ID (unique)
 * @param  last  last name
 * @param  first first name
 * @param  hire  date of hire (yyyymmdd)
 * @param  pay   pay grade
 *
 * @return ADD_OK on success; ADD_ALLOC or ADD_DUP on failure
 */

static Status add_employee( int line, int id, char *last, char *first,
    int hire, uint8_t pay ) {
    Employee *emp, *prev, *curr;

    // see if this employee already exists
        
    emp = find_emp( id );
    if( emp != NULL ) {
        fprintf( stderr, "add_emp(): line #%d, duplicate ID '%d'", line, id );
        fputs( ", skipping new record\n", stderr );
        return( ADD_DUP );
    }

    // somebody new; allocate a new employee node
 
    emp = new_emp( id, last, first, hire, pay );
    if( emp == NULL ) {
        // no room for this one?
        fprintf( stderr, "add_emp(): can't create employee #%d\n", id );
        return( ADD_ALLOC );
    }

    //
    // insert this employee into the employee list, which
    // is ordered by ID
    //

    prev = NULL;
    curr = employees;

    while( curr && curr->id < id ) {
        prev = curr;
        curr = curr->next;
    }

    if( curr && curr->id == id ) {  // can't happen!
        fprintf( stderr, "add_emp(): duplicate id %d AFTER check!\n", id );
        exit( EXIT_FAILURE );
    }

    // at this point:
    //  curr    prev    situation   
    //  =====   =====   =======================
    //  NULL    NULL    list was empty
    //  NULL    !NULL   add at end of list
    //  !NULL   NULL    add at front of list
    //  !NULL   !NULL   add in middle of list

    if( curr == NULL ) {

        if( prev == NULL ) {   // empty list
            employees = emp;
        } else {               // add at the end of the list
            prev->next = emp;
        }

    } else {

        if( prev == NULL ) { // add at the front of the list
            emp->next = employees;
            employees = emp;
        } else {             // add in the middle of the list
            emp->next = curr;
            prev->next = emp;
        }
    }

    return( ADD_OK );

}

/*
** Public functions
*/

/**
 * new_emp() - create a new employee
 */

Employee *new_emp( int id, char *last, char *first, int hire, uint8_t grade ) {
    Employee *new_e;//fixed: change variable's name so that it's not a keyword.

    // allocate the node
    new_e = (Employee *) calloc( 9, sizeof(Employee) );//fixed: increase the size of new_e

    // if this succeeded, fill in the fields
    if( new_e != NULL ) {

        // we assume that the pointers we're given can
        // just be copied into the new record
        //
        new_e->id = id;
        new_e->last = last;
        new_e->first = first;
        new_e->hire = hire;
        new_e->grade = grade;

        // we used calloc(), so 'next' is already NULL
    }

    return( new_e );
}

/**
 * free_emp() - deallocate an employee node
 */

void free_emp( Employee *emp ) {

    // sanity check - be sure we were given a non-NULL pointer

    if( emp != NULL ) {
        free( emp->first );
        free(emp->last);//fixed: free first and last names.
        free(emp); //fixed: free employee eventually.
    }

}

/**
 * find_emp() - locate the entry for a specific employee id
 */

Employee *find_emp( int id ) {
    Employee *emp;

    emp = employees;
    while( emp && emp->id != id ) {
        emp = emp->next;
    }

    return( emp );
}

/**
 * load_employees() - load the employee database
 */

int load_employees( FILE *file ) {
    char *buf;
    size_t len;
    int nlines, nemps;

    nlines = nemps = 0;

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
        //     id,lastname,firstname,hiredate,paygrade
        //
        // fields are separate by ',' characters; id, hiredate, and
        // paygrade are integers, names are one or more characters
        //
        // parse the line into five fields
        //

        char *fields[5];

        int num = parse( buf, fields, 5 );
        if( num != 5 ) {
                fprintf( stderr, "load_emp(): bad field count (%d), line %d",
                    num, nlines );
                fputs( "; ignoring line\n", stderr );
                continue;
        }

        // convert the fields as needed

        int id, hiredate, paygrade;

        // field 1:  id
        id = (int) strtol( fields[0], NULL, 10 );
        free( fields[0] );

        // field 4:  hire date
        hiredate = (int) strtol( fields[3], NULL, 10 );
        free( fields[3] );

        // field 5:  pay grade
        paygrade = (int) strtol( fields[4], NULL, 10 );
        free( fields[4] );

        // add this employee to the database

        Status result;
        result = add_employee( nlines, id, fields[1], fields[2], hiredate, paygrade );

        if( result == ADD_OK ) {
            ++nemps;
        }

    }

    // release the getline() buffer

    if( buf != NULL ) {
        free( buf );
    }

    num_employees = nemps;

    return( num_employees );
}

/**
 * delete_all_employees() - delete all employee entries
 */

void delete_all_employees( void ) {
    Employee *emp = employees;
    Employee * next;//fixed : add the dummy pointer to prevent memory leak
    while( emp ) {
        next=emp->next;
        free_emp( emp );
        emp = next;
    }
    
      employees=NULL;//fixed: nullify employees afterward.
}
