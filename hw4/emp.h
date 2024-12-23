/**
 * emp.h - data structures and prototypes for employee records
 *
 * @author A. N. Onymous
 * @autho  Huy Le
 */

#ifndef _EMP_H_
#define _EMP_H_

#include <stdio.h>
#include <stdint.h>

#include "log.h"

/*
** Employee pay grades
*/

#define PAY_HOURLY      1
#define PAY_WEEKLY      2
#define PAY_MONTHLY     3
#define PAY_COMMISSION  4

/*
** Employee data structure
*/

typedef
    struct emp {
        struct emp *next;   // next employee in the list
        char *last;         // employee last name
        char *first;        // employee first name
        int id;             // employee ID (unique)
        int hire;           // date of hire
        uint8_t grade;      // pay grade
        char counts[LOG_NCODES];    // counts of each code type
    }
        Employee;

/*
** Global list of employees
*/

extern Employee *employees;
extern int num_employees;

/*
** Pay grades
*/


/**
 * new_emp() - create a new employee
 *
 * @param  id    unique employee ID
 * @param  last  employee last name
 * @param  first employee first name
 * @param  hire  date of hire (as an integer)
 * @param  grade pay grade code
 *
 * @return newly allocated employee record, or NULL on failure
 */

Employee *new_emp( int id, char *last, char *first, int hire, uint8_t grade );

/**
 * free_emp() - deallocate an employee node
 *
 * @param emp    pointer to the node to deallocate
 */

void free_emp( Employee *emp );

/**
 * find_emp() - locate the entry for a specific employee id
 *
 * @param id  the ID to locate
 *
 * @return a pointer to the Employee record, or NULL
 */

Employee *find_emp( int id );

/**
 * load_employees() - load the employee database
 *
 * @param file    FILE * for the already-open input file
 *
 * @return count of employee records loaded
 */

int load_employees( FILE *file );

/*
** delete_all_employees() - delete all employee entries
*/

void delete_all_employees( void );

#endif
