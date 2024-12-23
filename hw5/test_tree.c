// file: test_tree.c
// description: white box tester for bst.c
// author: Warren R. Carithers
// created: 10/2024
// based on similar code created by bksteele
//
// invoked as:
//     ./test_tree                # run all tests
//     ./test_tree -h             # print a usage message
//     ./test_tree N1 N2 N3 ...   # run tests N1, N2, N3, etc.

#define  _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>

#include "bst.h"

// For testing purposes, instead of reading words from stdin, we'll
// use a static array of 26 words.

// Some words to insert into the tree; we'll use the International
// Radiotelephony Spelling Alphabet (a.k.a. the NATO phonetic alphabet).
// See https://en.wikipedia.org/wiki/NATO_phonetic_alphabet

const char *words[] = {
	"alfa",   "bravo",    "charlie", "delta",   "echo",    "foxtrot",
	"golf",   "hotel",    "india",   "juliett", "kilo",    "lima",
	"mike",   "november", "oscar",   "papa",    "quebec",  "romeo",
	"sierra", "tango",    "uniform", "victor",  "whiskey", "xray", 
	"yankee", "zulu"
};

/// let the compiler calculate the array length for us
const int num_words = sizeof(words) / sizeof(const char *);

//
// We will run a series of tests on BSTs using the words in our
// word list. Each test will build the tree in a specific order.
//

// test1 builds the tree in ascending alphabetical order, which should
// give us a right-subtree-only structure.

static void test1( void ) {
	bstnode_t *tree = NULL;	// our tree

	puts( "\nRunning test #1." );

	// begin by inserting all 26 words in order from line 1
	for( int i = 0; i < num_words; ++i ) {
		bst_insert( &tree, words[i], 1 );
	}

	// blow up if nothing was inserted into the tree
	assert( tree );

	// next, we'll add a few entries again at random

	int current = 2;   // pretend "line number"

	for( int i = 0; i < 5; ++i ) {
		// select a word
		long num = random() % num_words;
		// select an increment for the line number
		long inc = random() % 3;
		current += (int) inc;
		bst_insert( &tree, words[num], current );
	}

	// dump the tree
	bst_traverse( tree );

	// and clean it out
	bst_cleanup( tree );
}

// test2 builds the tree in descending alphabetical order, which should
// give us a left-subtree-only structure.

static void test2( void ) {
	bstnode_t *tree = NULL;	// our tree

	puts( "\nRunning test #2." );

	// begin by inserting all 26 words in order from line 1
	for( int i = num_words - 1; i >= 0; --i ) {
		bst_insert( &tree, words[i], 1 );
	}

	// blow up if nothing was inserted into the tree
	assert( tree );

	// next, we'll add a few entries again at random

	int current = 2;   // pretend "line number"

	for( int i = 0; i < 5; ++i ) {
		// select a word
		long num = random() % num_words;
		// select an increment for the line number
		long inc = random() % 3;
		current += (int) inc;
		bst_insert( &tree, words[num], current );
	}

	// dump the tree
	bst_traverse( tree );

	// and clean it out
	bst_cleanup( tree );
}

// test3 builds the tree with random insertion order

static void test3( void ) {
	bstnode_t *tree = NULL;	// our tree

	puts( "\nRunning test #3." );

	// begin by inserting 100 words from separate lines
	int current = 1;   // pretend "line number"
	for( int i = 0; i < 100; ++i ) {
		long num = random() % num_words;
		bst_insert( &tree, words[num], current );
		++current;
	}

	// blow up if nothing was inserted into the tree
	assert( tree );

	// next, we'll add another 100 entries again at random

	for( int i = 0; i < 100; ++i ) {
		// select a word
		long num = random() % num_words;
		// select an increment for the line number
		long inc = random() % 3;
		current += (int) inc;
		bst_insert( &tree, words[num], current );
	}

	// dump the tree
	bst_traverse( tree );

	// and clean it out
	bst_cleanup( tree );
}

/// an array of function pointers to simplify running all tests
void (*alltests[])(void) = {
	test1, test2, test3
};

/// let the compiler calculate the array length
const int num_tests = sizeof(alltests) / sizeof(void(*)(void));

/// print a usage message

void usage( const char *name ) {
	fprintf( stderr, "usage: %s [ -h ] [ N [ N ... ] ]\n", name );
	fputs( "where each 'N' is a test number to be run\n", stderr );
	fprintf( stderr, "valid test numbers are in the range 1 .. %d\n",
				num_tests );
}

/// main tests operations on BSTs
/// @param argc   the number of things on the command line
/// @param argv   the command-line argument strings
/// @return 0 for success or EXIT_FAILURE for failure

int main( int argc, char *argv[] ) {

	// we'll use a random number generator for some tests
	srandom( 41U );

	// if the first argument is "-h", print a usage message and exit
	if( argc > 1 && strcmp(argv[1],"-h") == 0 ) {
		usage( argv[0] );
		return 0;
	}

	// if invoked with no arguments, run all the tests; otherwise,
	// run only the specified tests

	if( argc == 1 ) {

		for( int i = 0; i < num_tests; ++i ) {
			// run test #i
			alltests[i]();
		}

	} else {

		for( int i = 1; i < argc; ++i ) {
			char *tmp = NULL;

			int num = (int) strtol( argv[i], &tmp, 10 );

			if( num < 1 || num > num_tests ) {
				// bad test number
				printf( "\n'%s' is not a valid test number.", argv[i] );
				printf( " Valid numbers are 1 through %d.\n", num_tests );
			} else {
				alltests[num-1]();
			}

		}

	}

	return 0;

}
