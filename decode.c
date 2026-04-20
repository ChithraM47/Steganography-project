#include <stdio.h>
#include "decode.h"
#include "types.h"
#include "common.h"
#include <string.h>
Status do_decoding(DecodeInfo *decInfo)
{
    printf("INFO: ## Decoding Procedure Started ##\n");

    // Open the stego image file
    if (open_file(decInfo) == e_failure) {
        printf("ERROR: Unable to open stego image file.\n");
        return e_failure;
    }

    // Skip the header in the stego image
    if (skip_header(decInfo->fptr_stego_image) == e_failure) {
        printf("ERROR: Unable to skip header in stego image file.\n");
        return e_failure;
    }

    // Decode the magic string signature
    printf("INFO: Decoding Magic String Signature\n");
    if (decode_magic_string(decInfo) == e_failure) {
        printf("ERROR: Decoding magic string failed.\n");
        return e_failure;
    } else {
        printf("INFO: Done\n");
    }

    // Decode the secret file extension
    printf("INFO: Decoding Output File Extension\n");
    if (decode_secret_file_extn(decInfo) == e_failure) {
        printf("ERROR: Decoding file extension failed.\n");
        return e_failure;
    } else {
        printf("INFO: Done\n");
    }

    // Decode the secret file data (decoded.txt file size)
    printf("INFO: Decoding decoded.txt File Size\n");
    if (decode_secret_file_data(decInfo) == e_failure) {
        printf("ERROR: Decoding file data failed.\n");
        return e_failure;
    } else {
        printf("INFO: Done\n");
    }

    printf("INFO: ## Decoding Done Successfully ##\n");
    return e_success;
}
Status open_file(DecodeInfo *decInfo)
{
    printf("INFO: Opening required files\n");
    decInfo->fptr_stego_image=fopen(decInfo->stego_image_fname, "r");
    if(decInfo->fptr_stego_image==NULL) 
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
        return e_failure;
    }
    printf("INFO: Opened %s\n",decInfo->stego_image_fname);
    return e_success;
}
Status skip_header(FILE *fptr) 
{
    fseek(fptr, 54, SEEK_SET);
    return e_success;
}
Status decode_magic_string(DecodeInfo *decInfo) 
{
    int len=decode_size(decInfo);
    if (len>=10) 
    {
        printf("Magic String not matching!\n");
        return e_failure;
    }
    char str[len+1];
    decode_string(len,str,decInfo);
    if (strcmp(str,decInfo->magic_string)!=0)
    {
        printf("Magic String not matching!\n");
        return e_failure;
    }
    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo) 
{
    int len = decode_size(decInfo);
    char str[len + 1];
    decode_string(len, str, decInfo);
    // Concatenate the decoded file extension to the output filename
    if (strlen(str) > 0) 
    {
        strcat(decInfo->secret_fname, str);
        printf("Concatenated string is %s\n", decInfo->secret_fname);
        decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
        if (decInfo->fptr_secret == NULL)
        {
            perror("fopen");
            fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->secret_fname);
            return e_failure;
        }
    }  
    return e_success;
}
Status decode_secret_file_data(DecodeInfo *decInfo) 
{
    int len = decode_size(decInfo);
    char str[len + 1];
    decode_string(len, str, decInfo);
    printf("INFO: Decoding decoded.txt File Data\n");
    add_secrate_data_to_file(str, decInfo);
    printf("INFO: Done\n");
    return e_success;
}
int decode_size(DecodeInfo *decInfo) 
{
    int i, len = 0;
    char buffer[32];
    fread(buffer, 32, 1, decInfo->fptr_stego_image);
    for (i = 0; i < 32; i++) {
        if (buffer[i] & 1) {
            len = (1 << i) | len;
        }
    }
    return len;
}
Status decode_string(int len, char str[], DecodeInfo *decInfo) {
    int i, j, k = 0;
    char buffer[8 * len];
    fread(buffer,8*len,1,decInfo->fptr_stego_image);
    for (i=0;i<len;i++) {
        str[i]=0;
        for (j=0;j<8;j++) {
            if (buffer[k] & 1) 
	    {
                str[i] = (1 << j) | str[i];
            }
            k++;
        }
    }
    str[i] = '\0';
    return e_success;
}

Status add_secrate_data_to_file(char str[], DecodeInfo *decInfo) 
{
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
    if (decInfo->fptr_secret == NULL) {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->secret_fname);
        return e_failure;
    }
    printf("INFO: Opened %s\n", decInfo->secret_fname);
    fprintf(decInfo->fptr_secret, "%s", str);
    printf("INFO: Done. Opened all required files\n");
    return e_success;
}



