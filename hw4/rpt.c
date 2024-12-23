/**
 * rpt - security log report generator
 *
 * @author A. N. Onymous
 * @author Huy Le
 */

#include <stdio.h>

#include "emp.h"
#include "log.h"

/**
 * main routine for the report generator
 *
 * @param  argc  command-line argument count
 * @param  argv  array of strings containing command-line arguments
 *
 * @return termination status for the program
 */

int main( int argc, char *argv[] ) {
    FILE *fp;

    // verify that we were given exactly one command-line argument

    if( argc != 2 ) {
        fprintf( stderr, "usage:  %s employee_file\n", argv[0] );
        return( 1 );
    }

    // open the employee file

    fp = fopen( argv[1], "r" );
    if( fp == NULL ) {
        perror( argv[1] );
        return( 1 );
    }

    // pull in all the employee records; if that
    // succeeds, go on to read all the log entries
    // and then generate the report

    if( load_employees(fp) > 0 ) {
        (void) load_logfile( stdin );
        generate_report();
    }

    // clean up by closing the employee file, then
    // deleting all our dynamic storage

    fclose( fp );
    delete_all_employees();
    delete_all_logentries();

    return( 0 );
}

