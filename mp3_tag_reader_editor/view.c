#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "view.h"

OperationType check_operation_type(char *argv[])        //function definition to check flag used in index 1 of CLA
{
    if ( argv[1] == NULL )      //if no argument passed as argv[1] go to this block
        return False;

    if ( strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0 )     //flag for help menu
        return help;

    else if ( strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--view") == 0 )    //flag for view menu
        return view;
    
    else if ( strcmp(argv[1], "-e") == 0 || strcmp(argv[1], "--edit") == 0 )    //flag for edit menu
        return edit;

    else                        //if other flags are used go to this block
        return unsupported;
}

Status read_and_validate_args(char *argv[])             //function to check if the file passed is a .mp3 file or not
{
    if ( argv[2] == NULL )          //if no file passed
        printf("ERROR: No mp3 file passed. Please pass the .mp3 file\n");

    else
    {
        if ( strstr(argv[2], ".") == NULL )             //check if file has extension or not
            printf("ERROR: File extension not detected. Please use the file extension after the file name\n");

        else if ( strcmp(strstr(argv[2], "."), ".mp3") == 0 )       //check if file format is a .mp3 file or not
            return True;

        else
            printf("ERROR: Input file is not a .mp3 extension file\n");
    }

    return False;
}

Status open_files(char *argv[])         //open files
{
    mp3 = fopen(argv[2], "r");          //open file in read mode

    if ( mp3 == NULL )
        printf("ERROR: Entered file not found. Please enter file name that is present in the directory\n");

    else
        return True;

    return False;
}

//Help Menu
void help_menu()
{
    printf("Help menu for MP3 Tag Reader and Editor:\n");
    printf("---------------------------------------------------------------------------\n");
    printf("For viewing the tags\t: ./mp3_tag_reader -v <.mp3 file>\n\n");
    printf("For editing the tags\t: ./mp3_tag_reader -e <.mp3 file> <modifier> <tag>\n");
    printf("---------------------------------------------------------------------------\n\n");
    printf("Modifier\tFuntion\n");
    printf("-------------------------------------\n");
    printf("   -t\t\tModifies Title tag\n");
    printf("   -a\t\tModifies Artist tag\n");
    printf("   -A\t\tModifies Album tag\n");
    printf("   -y\t\tModifies Year tag\n");
    printf("   -c\t\tModifies Comment tag\n");
    printf("   -g\t\tModifies Genre tag\n");
}

Status view_details(FILE *mp3)
{
    char *frame_tag = malloc(4 * sizeof(char));     //allocating 4 bytes of memory for frame tag

    if ( strcmp(fgets(frame_tag, 4, mp3), "ID3") == 0 ) //check if the file is ID3 file or not
    {
        char version = fgetc(mp3);                  //get the version
        
        if ( version == 3 )
        {
            printf("MP3 Tag reader and editor:\n");
            printf("---------------------------------------------------------------------------------------\n");
            printf("ID3 version\t: 2.%hhd\n", version);     //print ID3 version

            fseek(mp3, 10, SEEK_SET);                       //move to 10th byte as first 10 bytes is header

            while ( 1 )         //inf loop
            {
                fgets(frame, 5, mp3);                       //get the frame from the file

                if ( strcmp(frame, "TIT2") == 0 )           //if the frame is TIT2 for Title
                {
                    file_seek(mp3, &frame_tag);             //file seek function to add tag metadata to the buffer
                    printf("Title\t\t: ");
                    print(frame_tag);                       //print the frame tag
                }

                else if ( strcmp(frame, "TPE1") == 0 )      //for ARTIST
                {
                    file_seek(mp3, &frame_tag);
                    printf("Artist\t\t: ");
                    print(frame_tag);
                }

                else if ( strcmp(frame, "TALB") == 0 )      //ALBUM NAME
                {
                    file_seek(mp3, &frame_tag);
                    printf("Album\t\t: ");
                    print(frame_tag);
                }

                else if ( strcmp(frame, "TYER") == 0 )      //YEAR
                {
                    file_seek(mp3, &frame_tag);
                    printf("Year\t\t: ");
                    print(frame_tag);
                }

                else if ( strcmp(frame, "TCON") == 0 )      //GENRE
                {
                    file_seek(mp3, &frame_tag);
                    printf("Genre\t\t: ");
                    print(frame_tag);
                }

                else if ( strcmp(frame, "COMM") == 0 )      //COMMENTS
                {
                    file_seek(mp3, &frame_tag);
                    printf("Comments\t: ");
                    print(frame_tag);
                }

                else
                    file_seek(mp3, &frame_tag);             //if none of the frame matches move the file pointer to next location
            }
            printf("---------------------------------------------------------------------------------------\n");
        }

        else                //Invalid ID3 version
        {
            printf("ERROR: Incompatible ID3 version. Please use mp3 file of ID3 v2.3 only\n");
            printf("View Failure\n");
            return False;
        }
    }

    else   
    {
        printf("ERROR: File is not a ID3 version file\n");
        return False;
    }
    free(frame_tag);        //remove or free the allocated memory

    return True;
}


void file_seek(FILE *mp3, char **frame_tag)     //move the file pointer and store the frame tags in buffer
{
    fseek(mp3, 3, SEEK_CUR);
    fread(&frame_tag_size, 1, 1, mp3);          //get the tag size

    if ( frame_tag_size < 0 )
    {
        printf("---------------------------------------------------------------------------------------\n");
        printf("Album Art Extraction Success\n");
        fclose(mp3);                            //close mp3 file
        exit(1);                                //stop program execution
    }

    fseek(mp3, 3, SEEK_CUR);                    //skip 3 null bytes

    *frame_tag = realloc(*frame_tag, frame_tag_size);   //allocate as per the size of frame_tag metadata
    fgets(*frame_tag, frame_tag_size, mp3);     //store the tag data to a buffer to print
}

void print(char *frame_tag)                     //print the metadata
{
    char i;
    if ( strcmp(frame, "COMM") == 0 )           //if frame is comments or COMM. eng. must be skipped to print the comment from .mp3 file
        i = 3;                                  //3 for not print first 4 characters
    
    else
        i = 0;                                  //i start from 0

    frame_tag += i;                             //increment the address of the frame

    for ( ; i < frame_tag_size; i++ )           //iterate as per the frame tag size and print the character one by one
    {
        if ( readable_chk(*frame_tag) == True ) //check the character for printing is readable or not
            fputc(*frame_tag, stdout);          //print the buffer content
        frame_tag++;                            //increment tag address so that it points to next location
    }
    printf("\n");
}

Status readable_chk(char ch)                    //to check if the passed character is readable or not
{
    if ( (ch >= 0 && ch <= 32) || ch == 127 || ch < 0 )     //these characters are unreadable in the ascii table
    {
        if ( isblank(ch) == True )              //to check for space, tab character
            return True;                        //return True for readable and False if not readable
        return False;
    }
    return True;
}
