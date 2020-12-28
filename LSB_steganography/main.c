/*
Author Name     : Badrikesh Prusty

Date            : 27/06/2020

Description     : To implement LSB Image Steganography
                  Steganography is the art of hiding the fact that communication is taking place, by hiding information in other information. 
                  Many different carrier file formats can be used, but digital images are the most popular because of their frequency on the 
                  internet. Here, we are encoding secret_file.txt test file data in the image. Then we are decoding the encoded data using
                  encoded image file.

Input           : First compile the program: gcc -Wall main.c encode.c decode.c -o lsb_steg (to create an executable file)

Output          : ./lsb_steg -e <source_image_filename.bmp> <txt_file_name.txt> <encoded_image_name.bmp> (Note: last arg encoded.bmp is optional)
                INFO: Validation is Successful
                INFO: Open files success
                INFO: Image Resolution: 1024 x 768
                INFO: Check Capacity Success
                INFO: Copy BMP header success
                INFO: Magic string encode success
                INFO: Extension size encode success
                INFO: Secret file extension encode success
                INFO: Secret file size encode success
                INFO: Secret file data encode success
                INFO: Copy remaining image data success
                Encoding is done successfully

*/

#include <stdio.h>
#include <stdlib.h>         //here to clear the screen
#include "types.h"          //importing user created libraries present in current location
#include "encode.h"
#include "decode.h"


int main(int argc, char *argv[])        //collecting total argument count as argument variables as parameters
{
    EncodeInfo encInfo;     //creating structure variables as encInfo for encoding and decInfo for decoding
    DecodeInfo decInfo;

    system("clear");        //clear the screen

    int type;

    if ( argc < 2 )         //if no arguments passed go to this block
    {
        printf("ERROR: No arguments passed for operation\n");
        printf("Please use either of the two:\n");
        printf("For encoding: ./lsb_steg -e File_name.bmp secret.txt <output.bmp>\n");
        printf("For decoding: ./lsb_steg -d File_name.bmp <secret.txt>\n");
        return -1;          //come out of the program
    }

    type = check_operation_type(argv);      //checking if user wants to encode or decode

    if ( type == e_encode )                 //encoding
    {
        int state = read_and_validate_encode_args(argv, &encInfo);  //checking if argument passed are successfully validated

        if ( state == e_success )
        {
            printf("INFO: Validation is Successful\n");
            state = do_encoding(&encInfo);
            
            if ( state == e_success )       //encode process info check
            {   
                printf("Encoding is done successfully\n");
            }

            else
            {
                printf("ERROR: Encode Failure\n");
                return -1;
            }
            
        }
        else        //print the following message argument verification fails
        {
            printf("ERROR: Validation Failure\n");
            printf("Please follow this pattern for encoding: ./lsb_steg -e File_name.bmp secret.txt <output.bmp>\n");
            return -1;        
        }
    }

    else if ( type == e_decode )            //if -d is entered as first argument
    {
        int state = read_and_validate_decode_args(argv, &decInfo);
        
        if ( state == e_success )           //passed argument validation check
        {
            printf("INFO: Validation is Successful\n");
            state = do_decoding(&decInfo);

            if ( state == e_success )       //checking decode process info
            {
                printf("Decoding is done successfully\n");
            }

            else
            {
                printf("ERROR: Decode Failure\n");
                return -1;
            }
        }
        
        else
        {
            printf("ERROR: Validation Failure\n");
            printf("Please follow this pattern for decoding: ./lsb_steg -d File_name.bmp <secret.txt>\n");
            return -1;        
        }
    }

    else
    {
        printf("ERROR: Unsupported operation detected\n");
        printf("Please use either of the two:\n");
        printf("For encoding: ./lsb_steg -e File_name.bmp secret.txt <output.bmp>\n");
        printf("For decoding: ./lsb_steg -d File_name.bmp <secret.txt>\n");
        return -1;
    }

    return 0;     
}
