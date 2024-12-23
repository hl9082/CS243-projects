/**
 * @file KStream.h
 * @brief header file for the KStream ADT.
 * @author Huy Le (hl9082)
 */

#ifndef _KSTREAM_H_ ///<If KStream.h is not defined.
#define _KSTREAM_H_ ///<define kstream

#include<stdint.h> /// for integer types.

#include<stddef.h> /// for size_t

///interface for KStream.
typedef struct kstreamstruct *KStream;

/**
 * @brief to construct a KStream instance, initialize it using a key, and return the KStream instance to the client.
 * @param key an array of unisgned bytes.
 * @exception return NULL if key is NULL or unsuccessful allocation happens.
 * @return the KStream data structure.
 */
KStream ks_create(uint8_t * key);

/**
 * @brief to translate data between input (plaintext) and output (ciphertext) forms.
 * @param kstream the kstream object.
 * @param in_bytes the input byte array.
 * @param num the number of bytes to translate.
 * @exception return NULL if num is out of bounds, in_bytes is NULL, kstream is NULL, or allocation is improper.
 * @return an array of translated bytes.
 */
uint8_t * translate(KStream kstream,uint8_t * in_bytes,size_t num);

/**
 * @brief delete the kstream object.
 * @param kstream the kstream object.
 */
void ks_destroy(KStream kstream);

#endif ///end of include guard
