///
/// Description:  test driver for the String module
///
/// This driver performs more complicated tests using "normal"
/// C-style strings.
///
/// @author Warren R. Carithers
///

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "String.h"

///
/// Some sample strings
///

///
/// Names, sorted
///
char *names[] = {
    "Burgess, Rolando",
    "Shellenberger, Kristin",
    "Tate, Maria",
};

// number of names (in case we need it)
int n_names = sizeof(names) / sizeof(char *);

///
/// Get the length of a C-style string
///
/// @param str  The string
/// @return The length of the string
///
size_t numch( char *str ) {
    size_t count;

    for( count = 0; str[count] != '\0'; ++count )
        ;

    return( count );
}

///
/// Print a character in a readable fashion
///
/// This version doesn't print the apostrophes around the character
///
void prtch( unsigned int ch ) {

    if( ch < ' ' || ch > '~' ) {
        // "non printable"
        printf( "\\%o", ch & 0xff );
    } else {
        putchar( ch );
    }
}

///
/// Print a String
///
void str_print( String str, size_t len ) {
    putchar( '\'' );
    for( size_t i = 0; i < len; ++i ) {
        prtch( str_char_at(str,i) );
    }
    putchar( '\'' );
    putchar( '\n' );
}

///
/// Dump out a String
///
void str_dump( const char *msg, String str ) {

    putchar( '\n' );

    if( msg != NULL ) {
        fputs( msg, stdout );
    }

    // verify that we have a String to examine
    if( str == NULL ) {
        puts( "String is NULL." );
        return;
    }

    // report its official length
    size_t len = str_length( str );
    printf( "String length: %lu\n", len );

    // if there are characters, report them one by one
    if( len > 0 ) {
        str_print( str, len );
    }

    // retrieve it as a C string
    char *chars = str_to_c( str );

    len = numch( chars );
    printf( "C length: %lu\n", len );
    putchar( '\'' );
    for( size_t i = 0; i < len; ++i ) {
        prtch( chars[i] );
    }
    putchar( '\'' );

    putchar( '\n' );
    free( chars );
}

///
/// Substring tester
///
void test_subs( String str, int start, int end ) {
    String tmp;

    tmp = str_substring( str, start, end );
    printf( "Sub[%d,%d]:  ", start, end );
    str_print( tmp, str_length(tmp) );
    str_destroy( tmp );
}

///
/// main routine - initiate all the tests
///
int main( void ) {

    // start by doing some comparisons on names
    int len[n_names], slen[n_names];
    String str[n_names];

    // create the Strings, and record their lengths
    for( int i = 0; i < n_names; ++i ) {
        len[i] = numch( names[i] );
        str[i] = str_create( len[i], len[i], (unsigned char *) names[i] );
        slen[i] = str_length( str[i] );
    }

    // verify their lengths
    puts( "String length checks" );
    for( int i = 0; i < n_names; ++i ) {
        if( len[i] != slen[i] ) {
            printf( "String %d length mismatch: %d, %d\n",
                    i, len[i], slen[i] );
            printf( "  Orig: '%s'\n", names[i] );
            char *buf = str_to_c( str[i] );
            int blen = numch( buf );
            printf( "  New:  '%s' (%d)\n", buf, blen );
            free( buf );
        } else {
            printf( "String %d OK\n", i );
        }
    }

    // compare them to themselves
    int res;
    puts( "\nString comparison tests" );
    for( int i = 0; i < n_names; ++i ) {
        res = str_compare( str[i], str[i] );
        if( res != 0 ) {
            printf( "String %d::%d, result %d\n", i, i, res );
        } else {
            printf( "String %d::%d, OK\n", i, i );
        }
    }

    // do some cross-comparisons
    // assumes str[0] < str[1] < str[2]
    res = str_compare( str[0], str[1] );
    if( res >= 0 ) {
        printf( "String %d::%d, result %d\n", 0, 1, res );
    } else {
        printf( "String %d::%d, OK\n", 0, 1 );
    }

    res = str_compare( str[1], str[2] );
    if( res >= 0 ) {
        printf( "String %d::%d, result %d\n", 1, 2, res );
    } else {
        printf( "String %d::%d, OK\n", 1, 2 );
    }

    res = str_compare( str[2], str[0] );
    if( res <= 0 ) {
        printf( "String %d::%d, result %d\n", 2, 0, res );
    } else {
        printf( "String %d::%d, OK\n", 2, 1 );
    }

    // do a reversal
    puts( "\nString reverse tests" );
    fputs( "Original:  ", stdout );
    str_print( str[0], slen[0] );
    String tmp = str_reverse( str[0] );
    fputs( "Reversed:  ", stdout );
    str_print( tmp, str_length(tmp) );

    // done with this one now
    str_destroy( tmp );

    // create an empty string
    tmp = str_create( 0, 0, NULL );
    res = str_length( tmp );
    if( res != 0 ) {
        printf( "\nEmpty string has length %d\n", res );
    }

    // copy into the empty string - will require reallocation
    res = str_copy( tmp, str[0] );
    if( res != slen[0] ) {
        printf( "\nString copy, length %d into length 0, result %d\n",
                slen[0], res );
    }

    str_destroy( tmp );

    // test concatenation
    // create a String with some extra room
    puts( "\nConcatenation tests" );
    tmp = str_create( 2 * len[0], len[0], (unsigned char *) names[0] );

    res = str_length( tmp );
    if( res != slen[0] ) {
        printf( "Initial length %d, not %d\n", res, slen[0] );
    }
    fputs( "Original: ", stdout );
    str_print( tmp, res );

    // first concatenation shouldn't require growing the string
    res = str_concat( tmp, str[1] );
    if( res != (slen[0] + slen[1]) ) {
        printf( "Concat 1 length %d, not %d\n", res,
                slen[0] + slen[1] );
    }
    fputs( "Concat 1: ", stdout );
    str_print( tmp, res );

    // second concatenation does require growing the string
    res = str_concat( tmp, str[2] );
    if( res != (slen[0] + slen[1] + slen[2]) ) {
        printf( "Concat 2 length %d, not %d\n", res,
                slen[0] + slen[1] + slen[2] );
    }
    fputs( "Concat 2: ", stdout );
    str_print( tmp, res );

    str_destroy( tmp );

    // substring tests
    puts( "\nSubstring tests" );
    fputs( "Original: ", stdout );
    str_print( str[0], slen[0] );
    test_subs( str[0], 5, 10 );
    test_subs( str[0], -10, -5 );
    test_subs( str[0], 3, -2 );

    // get rid of the test strings
    for( int i = 0; i < n_names; ++i ) {
        str_destroy( str[i] );
    }

    return 0;
}
