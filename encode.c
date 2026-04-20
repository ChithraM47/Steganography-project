#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include <string.h>
/* Function Definitions */
/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image) 
{
    uint width, height;
    fseek(fptr_image, 18, SEEK_SET);
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);
    return width * height * 3;
}
/* Open required files */
Status open_files(EncodeInfo *encInfo) 
{
    printf("INFO: Opening required files\n");
    // Source Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    if (encInfo->fptr_src_image == NULL) {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
        return e_failure;
    }
    printf("INFO: Opened %s\n", encInfo->src_image_fname);

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    if (encInfo->fptr_secret == NULL) {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
        return e_failure;
    }
    printf("INFO: Opened %s\n", encInfo->secret_fname);

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    if (encInfo->fptr_stego_image == NULL) {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
        return e_failure;
    }
    printf("INFO: Opened %s\n", encInfo->stego_image_fname);
    printf("INFO: Done\n");

    return e_success;
}

/* check capacity */
Status check_capacity(EncodeInfo *encInfo)
{
     /*
       54 Bytes for Header
       To Encode Magic string size:32
       Magic string (size of magic string)*8
       Extension string size -32 Bytes//size is caluclated in int so
       Extension string -(size of extension string)*8
       Secret message-32 Bytes
       Secret Message string-(size of secret message string)*8)
     */
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);//getting image size from structure
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);//getting file size
    fseek(encInfo->fptr_secret, 0, SEEK_SET);
    if (encInfo->image_capacity>=54+16+32+32+32+(8*encInfo->size_secret_file))
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}
/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo)
{
    printf("INFO: ## Encoding Procedure Started ##\n");
    rewind(encInfo->fptr_src_image);
    // Copy BMP header
    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image)==e_success)
        printf("INFO: BMP header copied successfully.\n");
     else 
     {
        printf("ERROR: Failed to copy BMP header.\n");
        return e_failure;
     }
    printf("INFO: Encoding Magic String Signature\n");
    if(check_capacity(encInfo)==e_success)
	printf("INFO:Checked Successfully\n");
    else
    {
	printf("ERROR:Failed to Check Capacity\n");
	return e_failure;
    }
    if(encode_magic_string(encInfo->magic_string,encInfo) == e_success)
        printf("INFO: Magic String encoded successfully.\n");
    else 
     {
        printf("ERROR: Failed to encode Magic String.\n");
        return e_failure;
     }
    // Encode secret file extension
    printf("INFO: Encoding secret.txt File Extension\n");
    if(encode_secret_file_extn(encInfo)==e_success) 
    {
        printf("INFO: File extension encoded successfully.\n");
    }
    else
    {
        printf("ERROR: Failed to encode secret file extension.\n");
        return e_failure;
    }
    printf("INFO: Encoding secret.txt File Data\n");
    if(encode_secret_file_data(encInfo)==e_success)
        printf("INFO: Secret file data encoded successfully.\n");
    else 
     {
        printf("ERROR: Failed to encode secret file data.\n");
        return e_failure;
     }
    // Copy the remaining image data
    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
    {
        printf("INFO: Remaining image data copied successfully.\n");
    }
    else
    {
        printf("ERROR: Failed to copy remaining image data.\n");
        return e_failure;
    }
    printf("INFO: ## Encoding Done Successfully ##\n");
    return e_success;
}
/* Copy BMP header */
Status copy_bmp_header(FILE *fptr_src_image,FILE *fptr_dest_image)
{
    char header[54];
    rewind(fptr_src_image);
    fread(header,54,1,fptr_src_image);
    fwrite(header,54,1,fptr_dest_image);
    printf("INFO: Copying Image Header\n");
    printf("INFO: Done\n");
    return e_success;
}
/* Get file size */
uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
}
/*Read and validate encode args */
Status read_and_validate_encode_args(int argc,char *argv[], EncodeInfo *encInfo) 
{
    if (argc>=3&&strcmp(strstr(argv[2],"."),".bmp")==0)
    encInfo->src_image_fname = argv[2];
    else
    {
    printf("Please send an image file with a proper .bmp extension\n");
    return e_failure;
    }
    if(argc>=4&&strcmp(strstr(argv[3],"."),".txt")==0)
    encInfo->secret_fname = argv[3];
    else
    {
    printf("Please send a secret file with a proper .txt extension\n");
    return e_failure;
    }
    if(argc>=5&&argv[4]!=NULL)
    encInfo->stego_image_fname = argv[4];
    else
    {
    printf("Output file name not provided. Using default 'stego.bmp'\n");
    encInfo->stego_image_fname = "stego.bmp"; // Corrected assignment
    }
      return e_success;
}
/* Encode length */
Status encode_size_to_lsb(int len,EncodeInfo *encInfo) 
{
    int i;
    char buffer[32];
    fread(buffer,32,1,encInfo->fptr_src_image);
    for(i=0;i<32;i++) 
    {
        buffer[i]=buffer[i]&(~1);
        if ((1 << i) & len) 
            buffer[i]=buffer[i]|1;
    }
    fwrite(buffer,32,1,encInfo->fptr_stego_image);
    return e_success;
}
/* Encode string */
Status encode_string_to_image(int len,const char *str,EncodeInfo *encInfo)
{
    int i, j, k;
    char data[8*len];
    fread(data,8*len,1,encInfo->fptr_src_image);
    k = 0;
    for(i=0;i<len;i++)
    {
        for (j=0;j<8;j++)
       	{
            data[k]=data[k]&(~1);
            if ((1<<j)&str[i]) 
                data[k]=data[k]|1;
            k++;
        }
    }
    fwrite(data,8*len,1,encInfo->fptr_stego_image);
    return e_success;
}
/* Get secret file extension length */
int secret_file_extn_len(EncodeInfo *encInfo) 
{
    int i,x;
    for (i=0;encInfo->secret_fname[i]!='.';i++);
    for (x=0;encInfo->secret_fname[i+x] != '\0';x++);
    return x;
}
/* Encode magic string */
Status encode_magic_string(const char *magic_string,EncodeInfo *encInfo)
{
    int len=strlen(magic_string);
    encode_size_to_lsb(len,encInfo);
    encode_string_to_image(len,magic_string,encInfo);
    return e_success;
}
/* Encode secret file extension */
Status encode_secret_file_extn(EncodeInfo *encInfo) 
{
    int len=secret_file_extn_len(encInfo);
    encode_size_to_lsb(len,encInfo);
    int i;
    for (i=0;encInfo->secret_fname[i]!='.';i++);
    encode_string_to_image(len,encInfo->secret_fname+i,encInfo);
    return e_success;
}
/* Encode secret file data */
Status encode_secret_file_data(EncodeInfo *encInfo) 
{
    int len=get_file_size(encInfo->fptr_secret);
    encode_size_to_lsb(len,encInfo);
    char str[len];
    rewind(encInfo->fptr_secret);
    for (int i = 0;i<len;i++) 
    {
        char ch=getc(encInfo->fptr_secret);
        str[i]=ch;
    }
    encode_string_to_image(len,str,encInfo);
    return e_success;
}
/* Copy remaining image data */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest) 
{
    char buffer;
    int ret;
    ret = fread(&buffer, 1, 1, fptr_src);
    while (ret != 0) {
        fwrite(&buffer, 1, 1, fptr_dest);
        ret = fread(&buffer, 1, 1, fptr_src);
    }
    printf("INFO: Copying Left Over Data\n");
    printf("INFO: Done\n");
    return e_success;
}
