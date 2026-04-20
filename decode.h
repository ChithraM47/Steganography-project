#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "types.h" // Contains user-defined types

/* 
 * Structure to store information required for
 * decoding secret file from a source Image
 * Info about output and intermediate data is
 * also stored
 */

typedef struct _DecodeInfo
{
    char magic_string[20];
    /* Secret File Info */
    char *secret_fname;           
    FILE *fptr_secret;           
    int size_ext_file;           
    long size_secret_file;       
    /* Stego Image Info */
    char *stego_image_fname;     
    FILE *fptr_stego_image;      
} DecodeInfo;
/* Function prototypes */
 // Decode operation
Status do_decoding(DecodeInfo *decInfo);           

// Open file for decoding
Status open_file(DecodeInfo *decInfo);              

// Skip BMP header
Status skip_header(FILE *fptr);                      

// Decode the magic string
Status decode_magic_string(DecodeInfo *decInfo);    

// Decode the length of the data
int decode_size(DecodeInfo *decInfo);

 //decode the string
Status decode_string(int len,char string[],DecodeInfo *decInfo);      

// Decode the secret file extension
Status decode_secret_file_extn(DecodeInfo *decInfo); 

// Decode the secret file data
Status decode_secret_file_data(DecodeInfo *decInfo);

Status add_secrate_data_to_file(char str[],DecodeInfo *decInfo);

#endif

