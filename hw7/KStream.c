/**
 * @file KStream.c
 * @brief implement functions from KStream.h.
 * @author Huy Le (hl9082)
 */

#include<stdio.h> ///for perror

#include<stdlib.h> /// standard library

#include<string.h> /// for string manipulation and memcpy

#include<stdint.h> ///for unsigned integers

#include<stddef.h> /// for size_t


static const int MAX_LEN = 256;///internal array size

static const int keylen=8;// the number of bytes in the binary key.

static const int discard=1024;///number of bytes to discard.
/**
 * The KStream ADT implementation.
 */
struct kstreamstruct{
    uint8_t * key;// an array of unsigned bytes using the binary key.
    uint8_t * S;// an array of 256 unsigned bytes.
    int i;// first of two indices into the array.
    int j;// second of two indices into the array.
};

#include "KStream.h" ///<for the needed functions.

/**
 * @brief generate the next byte from the keystream.
 * @param kstream the kstream data structure.
 * @return the next byte.
 */
static uint8_t next_byte(KStream kstream){
   
   if(kstream==NULL || kstream->S==NULL){
    perror("No KStream for finding next byte.\n");
    return 0;
   }    

   kstream->i=(kstream->i+1)%MAX_LEN;
   
    kstream->j=(kstream->j+kstream->S[kstream->i])%(MAX_LEN);
    
    uint8_t tmp=kstream->S[kstream->i];
    kstream->S[kstream->i]=kstream->S[kstream->j];
    kstream->S[kstream->j]=tmp;
    // Ensure that we do not access beyond the MAX_LEN array
    size_t index = (kstream->S[kstream->i] + kstream->S[kstream->j]) % (MAX_LEN);
    
    uint8_t B=kstream->S[index];
    return B;
}

/**
 * @brief to construct a KStream instance, initialize it using a key, and return the KStream instance to the client.
 * @param key an array of unisgned bytes.
 * @exception return NULL if key is NULL or unsuccessful allocation happens.
 * @return the KStream data structure.
 */
KStream ks_create(uint8_t * key){
    if (key == NULL) {
        perror("Invalid key.\n");
        return NULL;
    }
    KStream new=(KStream)malloc(sizeof(struct kstreamstruct));
    if(new==NULL){
        perror("Improper KStream allocation.\n");
        return NULL;
    }
    
    // Copy the key to avoid issues with external memory management
    new->key = (uint8_t*)malloc(keylen);

    if (keylen > MAX_LEN) {
        perror("Key length exceeds buffer size.\n");
        return NULL;
    }

    if(new->key==NULL){
        perror("Improper allocation.\n");
        free(new);
        new=NULL;
        return NULL;
    }

    memcpy(new->key, key, keylen);

    

    new->S=(uint8_t*)malloc(MAX_LEN);

    if(new->S==NULL){
        perror("Improper allocation.\n");
        free(new->key);
        new->key=NULL;
        free(new);
        new=NULL;
        return NULL;
    }
   
    for(int i=0;i<MAX_LEN;i++){
        new->S[i]=i;
    }    
    new->j=0;
    for(new->i=0;new->i<MAX_LEN;new->i++){
        new->j=(new->j+new->S[new->i]+new->key[new->i % keylen])%(MAX_LEN);
        uint8_t tmp=new->S[new->i];
        new->S[new->i]=new->S[new->j];
        new->S[new->j]=tmp;
    }
    
    
    
    for(int i=0;i<discard;i++){
        (void)next_byte(new);   
    }
    
    return new;
}



/**
 * @brief to translate data between input (plaintext) and output (ciphertext) forms.
 * @param kstream the kstream object.
 * @param in_bytes the input byte array.
 * @param num the number of bytes to translate.
 * @exception return NULL if num is out of bounds, in_bytes is NULL, kstream is NULL, or allocation is improper.
 * @return an array of translated bytes.
 */
uint8_t * translate(KStream kstream,uint8_t * in_bytes,size_t num){



    if(num==0 ){
        perror("Nothing to translate.\n");
        
        return NULL;
    }

    if(kstream==NULL || in_bytes==NULL ||kstream->S==NULL){
        perror("Invalid data to translate.\n");
        return NULL;
    }

    uint8_t*out_bytes=(uint8_t*)malloc(num);
    if(out_bytes==NULL){
        perror("Error translating data.\n");
        return NULL;
    }
    for(size_t i=0;i<num;i++){
        
        out_bytes[i]=in_bytes[i] ^ next_byte(kstream);
        
    }
    return out_bytes;
}

/**
 * @brief delete the kstream object.
 * @param kstream the kstream object.
 */
void ks_destroy(KStream kstream){
    if(kstream){
        if(kstream->key){
            free(kstream->key);
            kstream->key=NULL;
        }
        if(kstream->S){
            free(kstream->S);
            kstream->S=NULL;
        }
        free(kstream);
        kstream=NULL;
    }
}


