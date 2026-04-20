/*
Name : Chithra M
Date : 20-02-2026
Description : Stegnography
*/
#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"
// Function to close the files used in encoding
void close_encode_files(EncodeInfo *encInfo) 
{
    if (encInfo->fptr_src_image) 
	fclose(encInfo->fptr_src_image);
    if (encInfo->fptr_secret)
       	fclose(encInfo->fptr_secret);
    if (encInfo->fptr_stego_image) 
       fclose(encInfo->fptr_stego_image);
}
void close_decode_files(DecodeInfo *decInfo)
{
    if(decInfo->fptr_secret)
    fclose(decInfo->fptr_secret);
    if(decInfo->fptr_stego_image) 
    fclose(decInfo->fptr_stego_image);
}
int main(int argc, char *argv[]) 
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    uint img_size;
    if (argc < 2)  
    {
        printf("Error! Invalid command line argument.\n");
        return 0;
    }
    int operation_type = check_operation_type(argv);
    if (operation_type==e_encode)
    {
        if (argc < 4)
       	{
            printf("Error! Invalid command line argument for encoding.\n");
            return 0;
        }
        encInfo.src_image_fname = argv[2];
        encInfo.secret_fname = argv[3];
        encInfo.stego_image_fname = (argc == 4) ? "stego_img.bmp" : argv[4];
        if (argc == 4) 
	{
            printf("INFO: Output file not mentioned. Creating stego_img.bmp as default.\n");
        }
        // Open required files
        if(open_files(&encInfo)==e_failure)
       	{
            printf("ERROR: Failed to open required files.\n");
            return 1;
        }
        // Validate the encoding arguments
        if (read_and_validate_encode_args(argc,argv,&encInfo)==e_failure)
       	{
            printf("Error! Source file size is below the required limit.\n");
            close_encode_files(&encInfo);
            return 0;
        }
        // Get the magic string from the user
        printf("Enter the Magic String: ");
        scanf("%s", encInfo.magic_string);
        // Perform encoding
        do_encoding(&encInfo);
        // Close the files used for encoding
        close_encode_files(&encInfo);
        printf("Encoding completed successfully.\n");
    }
    else if (operation_type == e_decode) {
        // Decoding operation
        if (argc<3)
       	{
            printf("Error! Invalid command line argument for decoding.\n");
            return 0;
        }
        // Check if the provided file is a .bmp file
        if(strstr(argv[2],".bmp")==NULL) 
	{
            printf("Error! The stego image file should be a .bmp file.\n");
            return 0;
        }
        decInfo.stego_image_fname = argv[2];
        decInfo.secret_fname=(argc >= 4)?argv[3]:"decoded_secret.txt";
        // Get the magic string from the user
        printf("Enter the Magic String: ");
        scanf("%s", decInfo.magic_string);
        // Perform decoding
        do_decoding(&decInfo);
            close_decode_files(&decInfo);
        printf("Decoding completed successfully.\n");
    }
    else 
    {
        printf("Error! Invalid operation. Use '-e' for encode or '-d' for decode.\n");
    }
}
OperationType check_operation_type(char *argv[])
{
    if (strcmp(argv[1],"-e")==0)
        return e_encode;
    else if(strcmp(argv[1],"-d")==0)
        return e_decode;
    else
        return e_unsupported;
}
