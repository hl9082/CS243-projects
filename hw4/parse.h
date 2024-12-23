/*
** parse.h - parse function for the log file report program
**
** @author A. N. Onymous
** @author Huy Le
*/

#ifndef _PARSE_H_
#define _PARSE_H_

// need size_t declaration
#include <stddef.h>

/**
 * parse() - parse a comma-separated string of up to 'count' fields
 *
 * @param  str     string to be parsed
 * @param  fields  array of pointers into which fields are extracted
 * @param  count   number of elements in fields[]
 *
 * @return intrinsic:  count of fields converted, or -1 on error
 * @return via 'fields':  pointers to dynamically-allocated strings
 *
 * the caller is responsible for freeing the returned strings
 */

int parse( char *str, char *fields[], size_t count );

#endif
