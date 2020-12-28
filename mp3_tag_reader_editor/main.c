/*
Author Name     : Badrikesh Prusty

Organisation    : Emertxe Infomation Technology (P) Ltd.

Date            : 15/07/2020

Description     : MP3 Tag Reader & Editor
                  MP3 tag reader is a software which will read and display MP3 (ID3) tag information from MP3 files. 
                  The software will be desktop based and not web based. All operations will be performed using command line interface.

Input           : First Compile the program: gcc -Wall main.c view.c edit.c -o mp3_tag

Output          : ./mp3_tag -v sample.mp3
                MP3 Tag reader and editor:
                ---------------------------------------------------------------------------------------
                ID3 version	: 2.3
                Title		: Sunny Sunny - Yo Yo Honey Singh - [SongsPk.CC]
                Artist		: Yo Yo Honey Singh - [SongsPk.CC]
                Album		: Yaariyan
                Year		: 2013
                Genre		: Bollywood Music - [SongsPk.CC]
                Comments	: Downloaded From SongsPk.CC
                ---------------------------------------------------------------------------------------
                Album Art Extraction Success

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "view.h"
#include "edit.h"

int main(int argc, char *argv[])                //collecting passed command line arguments as parameters
{
    system("clear");                            //clear console
    int type;

    type = check_operation_type(argv);          //checking the flag entered by user to check for help, edit and view

    if ( type == help )         //HELP
        help_menu();                            //function call to display help menu

    else if ( type == view )    //VIEW
    {
        int state = read_and_validate_args(argv);   //check if 2nd index argument passed is a .mp3 file or not
        
        if ( state == True )
        {
            state = open_files(argv);           //calling open file function to open the input file through CLA

            if ( state == True )
                view_details(mp3);              //calling view details function to view details about the song

            else
                printf("Viewing details failed\n");
        }

        else
            printf("Validation Failure\n");
    }

    else if ( type == edit )    //EDIT
    {
        int state = read_and_validate_args(argv);       //check if passed file a .mp3 file or not

        if ( state == True )
        {
            state = open_files(argv);                  //open the entered file
        
            if ( state == True )
            {
                state = check_edit_flag(argv);          //check if flag entered is present in the help menu
                
                if ( state == True )
                {
                    state = read_and_validate_edit_tags(argv);  //checking if proper tag metadata is passed or not
                    
                    if ( state == True )
                    {
                        edit_details(mp3, argv);        //calling edit function to edit specific tag
                        rename("new.mp3", argv[2]);     //rename function is used to rename the file
                        open_files(argv);               //open updated file
                        view_details(mp3);              //display info if updated file
                    }

                    else
                        printf("Edit Failure\n");
                }

                else
                    printf("Validation Failure\n");
            }

            else
            {
                printf("ERROR: Invalid flag used. Please check the help menu for more info\n");
                printf("For Help Menu: ./mp3_tag -h or ./mp3_tag --help\n");
            }
        }
    }

    else if ( type == unsupported )         //if argument passed flag is un supported go to this block
    {
        printf("ERROR: Unsupported arguments passed\n");
        printf("For Help Menu: ./mp3_tag -h or ./mp3_tag --help\n");
    }

    else
        printf("ERROR: No arguments Passed\n");

    return 0;
}
