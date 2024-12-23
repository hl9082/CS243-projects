///
/// Description:  test driver for the String module
///
/// This driver performs simple tests using "normal" C strings.
/// It creates four String objects and reports on their contents,
/// then destroys them.
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
char *messages[] = {
    "Masquerading as a man with a reason,",
    "My charade is the event of the season;",
    "And if I claim to be a wise man, well,",
    "It surely means that I don't know."
};

// number of test Strings to build
int num = sizeof(messages) / sizeof(char *);

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

    // create a bunch of Strings
    for( int i = 0; i < num; ++i ) {
        size_t len = numch( messages[i] );
        strs[i] = str_create( len, len, (unsigned char *) messages[i] );
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
