/**
 * @file String.c
 * @brief this is where we implement all functions from String.h, for the string ADT.
 * @author Huy Le (hl9082)
 */

#include "String.h" /// < to implement functions in this library.
#include<assert.h> ///<for assert() function
#include<stdlib.h> /// < standard library (also for error handling).

static const size_t empty_string_length=0; /// < empty string's length.

/**
 * This is the struct for the string object.
 * 
 */
struct string_s{
    size_t capacity; ///String capacity
    size_t length; /// String's size
    unsigned char *data; /// The value of the string.
};

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
String str_create( size_t capacity, size_t length, const unsigned char *data ){

    String new_string=(String)malloc(sizeof(struct string_s));
    if(new_string==NULL){
        perror("String cannot be allocated!\n");
        return NULL;
    }
    new_string->length = (data == NULL) ? empty_string_length : length;
    new_string->capacity = (capacity < new_string->length) ? new_string->length+1 : capacity;//+1 for null terminator
    new_string->data = malloc(new_string->capacity+1); // +1 for null terminator

    if (new_string->data == NULL) {
        free(new_string); // Free the allocated struct
        new_string=NULL;
        perror("Data allocation failed!");
        return NULL;
    }
    
    if (data != NULL) {
        
        for(size_t i=0;i<new_string->length;i++){
            new_string->data[i]=data[i];
        }
        new_string->data[new_string->length] = '\0'; // Ensure null termination
    }
    else {
        new_string->data[0] = '\0'; // Initialize to empty string
    }

    return new_string;
}

///
/// Tear down and deallocate the supplied String.
///
/// @param str   The String to be deleted
///
/// @pre str must be a valid String
///
void str_destroy( String str ){
    
    if (str == NULL ) {
        return; // Early exit if str is NULL
    }

    str->length=empty_string_length;
    str->capacity=empty_string_length;

    free(str->data);
    str->data=NULL;//prevent dangling pointer.
    
    free(str);
    str=NULL;//prevent dangling pointer
}

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
char *str_to_c( const String str ){
    
    if(str==NULL ){
        perror("Failure to convert string to char*.\n");
        return NULL;
    }
   
    if(str->capacity==empty_string_length || str->length==empty_string_length){
        return NULL;;
    }

    char * res=(char *)malloc(str->length+1);//+1 for null terminated
    if (res == NULL) {
        perror("Memory allocation failed for C string!");
        return (char*)str->data;
    }
    for(size_t i=0;i<str->length;i++){
        res[i]=str->data[i];
    }
    res[str->length]='\0';//null terminator
    return res;
}

///
/// Return the length of the supplied String.
///
/// @param str    The String to examine
///
/// @pre str must be a valid String
///
/// @return The number of characters in the String
///
size_t str_length( const String str ){
    
    if(str==NULL){
       perror("Invalid string.\n");
       return empty_string_length;
    }    
    return str->length;
}

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
String str_reverse( const String str ){
    
    if(str==NULL || str->data==NULL||str->length==empty_string_length){
        return NULL;
    }
    
    String res=malloc(sizeof(struct string_s));

    if (res == NULL) {
        perror("Memory allocation for reverse failed!");
        return NULL;
    }
     
    res->length=str->length;
    res->capacity=str->capacity;
    res->data=malloc(res->length+1);//+1 for null-terminated
    
    for(size_t i=0;str->data[i]!='\0';i++){
        res->data[i]=str->data[str->length-1-i];
    }
    res->data[res->length]='\0';
    return res;
}

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
size_t str_copy( String dst, const String src ){
    
    if(src==NULL){
        return empty_string_length;
    }
    if(dst->capacity<src->capacity){
        dst->capacity=src->capacity;
    }
    
    unsigned char* new_data = realloc(dst->data, dst->capacity+1); // Plus 1 for null terminator
    if (new_data == NULL) {
        perror("Reallocation failed!");
        return empty_string_length; // Or handle accordingly
    }
        dst->data = new_data;
    
    dst->length=(dst->length<src->length)?src->length:dst->length;
    for(size_t i=0;src->data[i]!='\0';i++){
        dst->data[i]=src->data[i];
    }
    dst->data[dst->length]='\0';
    return dst->length;
}

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
size_t str_concat( String dst, const String src ){
    
    if(src==NULL || src->data==NULL || !dst || !dst->data){
        return dst->length;
    }
    
    if (dst->capacity < src->capacity) { 
        dst->capacity = src->capacity;
    }
        size_t old_length=dst->length;
        unsigned char *new_data = realloc(dst->data, dst->length+src->length+1);//+1 for null-terminated
        if (new_data == NULL) {
            perror("Reallocation failed!");
            return dst->length; 
        }
        dst->data = new_data;
        
    

    for (size_t i = 0; src->data[i]!='\0'; i++,old_length++) {
        dst->data[old_length] = src->data[i];
    }
    dst->length = old_length;
    dst->data[dst->length] = '\0';
    return dst->length;

}

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
int str_compare( const String str1, const String str2 ){   
    
    const int equal=0,greater=1,less=-1;
    // Check if both strings are NULL
    if (str1 == NULL && str2 == NULL) {
        return equal; // Both are equal
    }
    
    // If one is NULL and the other is not
    if (str1 == NULL) {
        return less; // NULL is considered less than non-NULL
    }
    if (str2 == NULL) {
        return greater; // Non-NULL is considered greater than NULL
    }

    // Compare character by character
    size_t min_size = (str1->length < str2->length) ? str1->length : str2->length;

    for (size_t i = 0; i < min_size; i++) {
        if (str1->data[i] != str2->data[i]) {
            return (unsigned char)str1->data[i] - (unsigned char)str2->data[i];
        }
    }

    // If all characters matched, compare sizes
    return str1->length - str2->length;

}

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
int str_char_at( const String str, int index ){
    
    if(abs(index)>str->length){
        perror("Index out of bounds!\n");
        return STR_ERROR;
    }
    return (index<0)?str->data[str->length+index]:str->data[index];
}

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
String str_substring( const String str, int start, int end ){
    assert(str!=NULL && str->data!=NULL);
    if(start<0){
        start+=str->length;
    }
    if(end<0){
        end+=str->length;
    }
    
    if(start<0 || end < 0||end>=(int)str->length){
        perror("Index out of bounds!");
        return NULL;
    }
    
    int length=end-start+1;

    String substring=str_create(str->capacity,(size_t)length,&str->data[start]);
    
    return substring;
}