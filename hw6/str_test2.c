///
/// Description:  test driver for the String module
///
/// This driver tests "strings" made up of random byte values.
/// It creates ten String objects containing (pseudo-)random
/// byte values, reports their contents, and destroys them.
///
/// @author Warren R. Carithers
///

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "String.h"

#define BUF_LEN 256

///
/// Our "string" buffer
///
unsigned char buffer[256];

// number of test Strings to build
int num = 10;

///
/// Get the length of a C-style string
///
/// @param str  The string
/// @return The length of the string
///
size_t numch( const char *str ) {
    size_t count;

    for( count = 0; str[count] != '\0'; ++count )
        ;

    return( count );
}

///
/// Print a character in a readable fashion
///
void prtch( unsigned int ch ) {

    putchar( '\'' );
    if( ch < ' ' || ch > '~' ) {
        // "non printable"
        printf( "\\%o", ch & 0xff );
    } else {
        putchar( ch );
    }
    putchar( '\'' );
}

///
/// Dump out a String
///
void str_dump( String str ) {

    putchar( '\n' );

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
        for( size_t i = 0; i < len; ++i ) {
            putchar( ' ' );
            prtch( str_char_at(str,i) );
        }
        putchar( '\n' );
    }

    // retrieve it as a C string
    char *chars = str_to_c( str );
    assert(chars);

    len = numch( chars );
    printf( "C length: %lu\n", len );
    for( size_t i = 0; i < len; ++i ) {
        putchar( ' ' );
        prtch( chars[i] );
    }

    putchar( '\n' );
    free( chars );
}

///
/// main routine - initiate all the tests
///
int main( void ) {
    String strs[num];

    // we use a fixed seed for repeatability
    srand( 41 );

    // fill the buffer
    for( int i = 0; i < BUF_LEN; ++i ) {
        buffer[i] = rand() % 128;
    }

    // create a bunch of Strings
    for( int i = 0; i < num; ++i ) {

        // random length, 0..255
        int len = rand() % BUF_LEN;

        // random start position, but must make sure there
        // are enough bytes in the buffer
        int pos = rand() % (BUF_LEN - len);

        while( (pos + len) > BUF_LEN ) {
            pos = rand() % (BUF_LEN - len);
        }

        // create it
        strs[i] = str_create( len, len, buffer + pos );
    }

    // dump them all
    for( int i = 0; i < num; ++i ) {
        str_dump( strs[i] );
    }

    // get rid of them
    for( int i = 0; i < num; ++i ) {
        str_destroy( strs[i] );
    }

    return 0;
}
