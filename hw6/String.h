///
/// Description:  Interface to the String ADT module.
///
/// @author Warren R. Carithers
///

#ifndef STRING_H_
#define STRING_H_

// need size_t
#include <stddef.h>
// need FILE
#include <stdio.h>

//
// String error codes
//

// generic error return value for functions returning integer results
#define STR_ERROR    (-1)

///
/// The String type (opaque to the application)
///
typedef struct string_s *String;

///
/// Create a String entity.
///
/// Strings have three important characteristics:  capacity, length,
/// and contents.  The capacity of a String is the maximum number of
/// characters it can contain.  The length of the String is the number of
/// characters it currently contains.  The contents of the String are the
/// actual characters it contains.  Initial capacity is determined by the
/// 'capacity' parameter, initial length by the 'length' parameter, and
/// contents by the 'data' parameter.
///
/// If 'capacity' is 0, 'length' and 'data' are ignored, and a zero-length
/// String with a capacity of zero is created.
///
/// If 'capacity' is > 0, a String of capacity MAX(capacity,length) is created.
/// If 'length' is zero or 'data' is NULL, the initial contents will be
/// no characters (i.e., its initial length is zero); otherwise the
/// first 'length' bytes from 'data' are copied in as the initial
/// contents, and the length is 'length'.  (Note: the capacity can be
/// greater than the length, but the reverse is not possible.)
///
/// @param capacity   An initial capacity for the String
/// @param length     An initial length for the String
/// @param data       An array of bytes to be used as the initial contents
///
/// @return The created String, or NULL if some error other than an
///    allocation failure occurs.
///
/// @exception If space for the String cannot be allocated, the program
///    terminates with an appropriate error message.
///
String str_create( size_t capacity, size_t length, const unsigned char *data );

///
/// Tear down and deallocate the supplied String.
///
/// @param str   The String to be deleted
///
/// @pre str must be a valid String
///
void str_destroy( String str );

///
/// Create a C-style string from the supplied String.
///
/// Dynamically allocates space for a copy of the data found in
/// the String parameter plus a trailing NUL byte, and returns
/// the pointer to that space.
///
/// @param str   The String to be duplicated
///
/// @pre str must be a valid String
///
/// @return The created C-style string
///
/// @exception If space for the String cannot be allocated, the program
///    terminates with an appropriate error message.
///
char *str_to_c( const String str );

///
/// Return the length of the supplied String.
///
/// @param str    The String to examine
///
/// @pre str must be a valid String
///
/// @return The number of characters in the String
///
size_t str_length( const String str );

///
/// Reverse an existing string.
///
/// @param src    The String to be reversed
///
/// @pre str must be a valid String
///
/// @return A new String containing the reversed original String
///
/// @exception If space for the String cannot be allocated, the program
///    terminates with an appropriate error message.
///
String str_reverse( const String str );

///
/// Copy one string into another, replacing its contents.
///
/// If the capacity of the destination String is too small, it will
/// be grown to be large enough to hold the copied data.
///
/// @param dst    The String to be replaced
/// @param src    The String to be copied
///
/// @pre str must be a valid String
///
/// @return The number of characters that were copied.
///
size_t str_copy( String dst, const String src );

///
/// Concatenate one string to another.
///
/// Appends the source String to the destination String.  If the
/// capacity of the destination String is too small, it will be
/// grown to contain the additional data.
///
/// @param dst    The String to be modified
/// @param src    The String to be appended
///
/// @pre src must be a valid String
/// @pre dst must be a valid String
///
/// @return The number of characters in the resulting string.
///
size_t str_concat( String dst, const String src );

///
/// Compare two strings, returning an integer indicating the result
/// of the comparison.
///
/// Returns a negative value if str1 < str2, zero if they are equal,
/// and a positive value if str1 > str2.
///
/// @param str1    The first String to be examined
/// @param str2    The second String to be examined
///
/// @pre str1 must be a valid String
/// @pre str2 must be a valid String
///
/// @return A value indicating the relationship between the strings
///
int str_compare( const String str1, const String str2 );

///
/// Return the character at a specific position in the String.
///
/// Indices are enumerated from either end of the string.  Non-negative
/// values are indices from the beginning of the string; negative
/// values are indices from the end of the string.  As an example:
///
///     + index      0   1   2   3   4
///     string       H   e   l   l   o
///     - index     -5  -4  -3  -2  -1
///
/// @param str    The String to be examined
/// @param index  The index of the desired character
///
/// @pre str must be a valid String
///
/// @return The selected character; if that character does not exist
///    (i.e., the index is out of bounds), returns STR_ERROR
///
int str_char_at( const String str, int index );

///
/// Extract a substring from an existing String.
///
/// Creates a new String object which contains a substring from
/// the contents of an existing String object, as specified by
/// starting and ending indices.
///
/// @param str   The String to be examined
/// @param start The starting index
///
/// @param end   The ending index
///
/// @pre str must be a valid String
///
/// @return A new String object containing the substring str[start..end]
///    from the str parameter, or NULL if an error occurs. (This failure
///    does not cause termination of the program.)
///
/// @exception If space cannot be allocated for the new String, the
///    program terminates with an appropriate error message.
///
String str_substring( const String str, int start, int end );

#endif
