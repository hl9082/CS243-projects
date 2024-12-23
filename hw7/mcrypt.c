
/**
 * @file mcrypt.c
 * @brief where we implement the encrytion and decryption operations from Kstream.c.
 * @author Huy Le (hl9082)
 */

#define _XOPEN_SOURCE ///for isascii

#include "KStream.h" ///<to get necessary functions.

#include<stdio.h> /// for perror

#include<stdlib.h> ///for better error handling

#include<stdbool.h> /// < for boolean operations.

#include<string.h> /// < for string operations.

#include<ctype.h> ///<for isascii function

static const int arg_num=4;///maximum number of arguments.

/**
 * This is where we do the cipher and decipher the binary keys.
 * @param argv number of arguments.
 * @param args list of arguments.
 * @return EXIT_SUCCESS if it's successful. Else, return EXIT_FAILURE.
 */
int main(int argv,char * args[]){
    if(argv != arg_num){
        fprintf(stderr,"usage: mcrypt key-file in-file [ out-file | - ]\n");
        return(EXIT_FAILURE);
    }
  
    FILE * fp_1=fopen(args[1],"rb");

    if (fp_1 == NULL) { // Check if the file opened successfully
        perror("Error opening file.\n");
        return(EXIT_FAILURE);
    }

     // Get file size
    fseek(fp_1, 0, SEEK_END);
    size_t filesize = ftell(fp_1);
    fseek(fp_1, 0, SEEK_SET);  // Reset file pointer to beginning

     if (filesize == 0) {
        perror("Key file is empty.\n");
        fclose(fp_1);
        return(EXIT_FAILURE);
    }

    uint8_t * key=(uint8_t*)malloc(filesize);

    if(key==NULL){
        perror("Key doesn't exist!\n");
        fclose(fp_1);
        return(EXIT_FAILURE);
    }

    size_t bytes_read=fread(key,sizeof(uint8_t),filesize,fp_1);
    fclose(fp_1);

    if (bytes_read == 0) {
        perror("Key file is empty or not properly loaded.\n");
        free(key);
        key=NULL;
        return(EXIT_FAILURE);
    }


    if (bytes_read != filesize) {
        perror("Error reading file.\n");
        free(key);
        key=NULL;
        return(EXIT_FAILURE);
    }

    KStream kstream=ks_create(key);

    if(kstream==NULL){
        return(EXIT_FAILURE);
    }
    
    

    FILE * fp_2=fopen(args[2],"rb");

    if (fp_2 == NULL) { // Check if the file opened successfully
        perror("Error opening file.\n");
        free(key);
        key=NULL;
        ks_destroy(kstream);
        return(EXIT_FAILURE);
    }

     // Get input file size
    fseek(fp_2, 0, SEEK_END);
    size_t filesize_2 = ftell(fp_2);
    fseek(fp_2, 0, SEEK_SET);  // Reset file pointer to beginning

    if(filesize_2==0){
        perror("No input file to read.\n");
        fclose(fp_2);
        free(key);
        key=NULL;
        ks_destroy(kstream);
        exit(EXIT_FAILURE);
    }

    uint8_t * in_bytes=(uint8_t*)malloc(filesize_2);

    if(in_bytes==NULL){
        perror("Improper allocation.\n");
        fclose(fp_2);
        free(key);
        key=NULL;
        ks_destroy(kstream);
        return(EXIT_FAILURE);
    }

    size_t bytes_read_2=fread(in_bytes,sizeof(uint8_t),filesize_2,fp_2);
    fclose(fp_2);

    if (bytes_read_2 == 0 ) {
        perror("Key file is empty or not properly loaded.\n");
        free(key);
        key=NULL;
        free(in_bytes);
        in_bytes=NULL;
        ks_destroy(kstream);
        return(EXIT_FAILURE);
    }    

     if (bytes_read_2 != filesize_2) {
        perror("Error reading input file.\n");
        free(key);
        key=NULL;
        free(in_bytes);
        in_bytes=NULL;
        ks_destroy(kstream);
        return(EXIT_FAILURE);
    }

    uint8_t * out_bytes=translate(kstream,in_bytes,filesize_2);

    if (out_bytes == NULL) {
        free(key);
        key=NULL;
        free(in_bytes);
        in_bytes=NULL;
        ks_destroy(kstream);
        return(EXIT_FAILURE);
    }

    if(strcmp(args[3],"-")==0){
        for(size_t i=0;i<filesize_2;i++){
            if(isascii(out_bytes[i])){// Printable ASCII range
                printf("%c",(char)out_bytes[i]);
            }
            else{
                printf("%02x", out_bytes[i]);
            }
        }
        
        free(key);

        key=NULL;

        free(in_bytes);
        in_bytes=NULL;

        free(out_bytes);
        out_bytes=NULL;


        ks_destroy(kstream);
        return(EXIT_SUCCESS);
    }

    FILE * file=fopen(args[3],"wb");

    if(file==NULL){
        perror("Error opening output file.\n");
        free(key);
        key=NULL;
        free(in_bytes);
        in_bytes=NULL;
        free(out_bytes);
        out_bytes=NULL;
        ks_destroy(kstream);
        return EXIT_FAILURE;
    }

    size_t bytes_written=fwrite(out_bytes,sizeof(uint8_t),filesize_2,file);



    fclose(file);

    free(key);
    key=NULL;

    free(in_bytes);
    in_bytes=NULL;

    free(out_bytes);
    out_bytes=NULL;


    ks_destroy(kstream);


    if(bytes_written==0){
        perror("Nothing is written to the file.\n");
    
        return(EXIT_FAILURE);
    }

    if (bytes_written != filesize_2) {
        perror("Error writing to file.\n");
        return(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}