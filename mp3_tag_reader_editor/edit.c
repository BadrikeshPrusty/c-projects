#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "edit.h"

OperationType check_edit_flag(char *argv[])             //check if the flag passed is valid or not
{
    char oper[][3] = {"-t", "-T", "-a", "-A", "-y", "-c", "-g"};    //array of all the valid flags

    if ( argv[3] == NULL )          //if flag not passed
    {
        printf("ERROR: Edit flag not passed\n");
        return False;
    }
    
    char i;

    for ( i = 0; i < 7; i++ )
    {
        if ( strcmp(argv[3], (oper[0] + i * 3)) == 0 )      //compare the passed flag with all the flags in the array
            return True;                    //if found return True
    }

    if ( i == 7 )                   //if none of the flags matched with the passed flag
        printf("ERROR: Invalid flag passed\n");

    return False;
}

Status read_and_validate_edit_tags(char *argv[])        //validation of the tag
{
    if ( argv[4] == NULL )          //if metadata tag is not passed
    {
        printf("ERROR: Tag metadata not found. Please enter the tag metadata to update the frame tag\n");
        return False;
    }

    else if ( argv[5] == NULL )     //if 4th index tag is passed by not 5th index tag
        return True;

    else                            //if more than 1 tag is passed
        printf("ERROR: More than 5 arguments detected. If you have space in metadata put that within double quoute. Ex.: %s\n", "\"Into Eternity\"");
    
    return False;
}

void edit_details(FILE *mp3, char *argv[])
{
    int end;
    char version;

    fseek(mp3, 3, SEEK_SET);

    version = fgetc(mp3);       //ID3 version check

    if ( version != 3 )         //if version is not ID3 v2.3 go to this block
    {
        printf("ERROR: Incompatible version detected. Please use only ID3 v2.3 songs for editing\n");
        exit(1);
    }

    rewind(mp3);                //move the file pointer to 0th position

    FILE *mp3_update;
    char header_info[10];       //to store 10 bytes of header information

    mp3_update = fopen("new.mp3", "w");     //open new file in write mode

    fread(header_info, 1, 10, mp3);
    fwrite(header_info, 1, 10, mp3_update);     //copy header information

    while ( 1 )
    {
        fgets(frame, 5, mp3);                   //get the current frame
        fwrite(frame, 1, 4, mp3_update);        //copy in new file

        if ( strcmp(frame, "TIT2") == 0 && strcmp(argv[3], "-t") == 0 )     //TITLE Update
        {
            update_frame_tag(argv, mp3, mp3_update);    //if title and first frame tag flag matches go to this block to update the tag
            printf("INFO: Title Modification Done\n");
            printf("---------------------------------------------------------------------------------------\n");
            break;                              //after updating break from the loop
        }

        else if ( strcmp(frame, "TPE1") == 0 && strcmp(argv[3], "-a") == 0 )    //ARTIST Update
        {
            update_frame_tag(argv, mp3, mp3_update);
            printf("INFO: Artist Modification Done\n");
            printf("---------------------------------------------------------------------------------------\n");
            break;
        }

        else if ( strcmp(frame, "TALB") == 0 && strcmp(argv[3], "-A") == 0 )    //ALBUM Update
        {
            update_frame_tag(argv, mp3, mp3_update);
            printf("INFO: Album Modification Done\n");
            printf("---------------------------------------------------------------------------------------\n");
            break;
        }

        else if ( strcmp(frame, "TYER") == 0 && strcmp(argv[3], "-y") == 0 )    //YEAR Update
        {
            update_frame_tag(argv, mp3, mp3_update);
            printf("INFO: Year Modification Done\n");
            printf("---------------------------------------------------------------------------------------\n");
            break;
        }

        else if ( strcmp(frame, "COMM") == 0 && strcmp(argv[3], "-c") == 0 )    //COMMENT Update
        {
            update_frame_tag(argv, mp3, mp3_update);
            printf("INFO: Comment Modification Done\n");
            printf("---------------------------------------------------------------------------------------\n");
            break;
        }

        else if ( strcmp(frame, "TCON") == 0 && strcmp(argv[3], "-g") == 0)     //Genre Update
        {
            update_frame_tag(argv, mp3, mp3_update);
            printf("INFO: Genre Modification Done\n");
            printf("---------------------------------------------------------------------------------------\n");
            break;
        }

        else
        {
            end = copy_same_as_org(mp3, mp3_update);          //If none of the condition satisfies copy same as original
            
            if ( end ==  False )                              //check if copy same_as orginal fails
            {
                printf("ERROR: Frame Not found\n");
                printf("Edit Failure\n");
                printf("---------------------------------------------------------------------------------------\n");
                break;
            }
        }
    }

    copy_remaining_bytes(mp3, mp3_update);              //after breaking from the loop copy the remaining mp3 file data to new file
    
    fclose(mp3_update);
    fclose(mp3);                                        //close both the files
}


void update_frame_tag(char *argv[], FILE *mp3, FILE *mp3_update)        //function to update the frame tag metadata
{
    char cpy_null[3], frame_tag_size, prev_tag_size;

    fread(cpy_null, 1, 3, mp3);                 //copy 3 null bytes from source file to new file
    fwrite(cpy_null, 1, 3, mp3_update);
    
    if ( strcmp(frame, "COMM") == 0 )           //if the frame tag is a comment
    {
        frame_tag_size = strlen(argv[4]) + 5;   //frame tag size is length of passed tag and + 4 for eng\0 in the comment
        fwrite(&frame_tag_size, 1, 1, mp3_update);  //copy the size to new file
        frame_tag_size -= 4;                    //reduce the frame tag size by 3 for the eng
    }

    else
    {
        frame_tag_size = strlen(argv[4]) + 1;   //for others except comment frame tag size
        fwrite(&frame_tag_size, 1, 1, mp3_update);  //copy the frame tag size to new file
    }
    
    fread(&prev_tag_size, 1, 1, mp3);           //keep track of previous frame tag size
    fread(cpy_null, 1, 3, mp3);                 //copy 3 NULL from source to dest
    fwrite(cpy_null, 1, 3, mp3_update);
    fseek(mp3, (prev_tag_size - 1), SEEK_CUR);  //move the prev file pointer of old file as per the prev tag size from current location
    
    if ( strcmp(frame, "COMM") == 0 )           //if frame is a comment
        fwrite("eng\0", 1, 4, mp3_update);      //add eng\0 tag to the comment before adding the tag
    
    fwrite(argv[4], 1, (frame_tag_size - 1), mp3_update);   //add the tag to the new file
}


Status copy_same_as_org(FILE *mp3, FILE *mp3_update)  //function definition to copy same as original file to a new file
{
    char cpy_null[3], prev_tag_size;
    fread(cpy_null, 1, 3, mp3);                     //copy the NULL bytes to new file
    fwrite(cpy_null, 1, 3, mp3_update);

    fread(&prev_tag_size, 1, 1, mp3);
    fwrite(&prev_tag_size, 1, 1, mp3_update);       //copy the tag size

    if ( prev_tag_size < 0 )                        //check if tag size is negative
        return False;                               //return
    
    prev_tag_size += 2;                             //increment the tag size by 2 to copy prev to NULL bytes also before the actual tag

    char frame_tag[prev_tag_size];                  //create array as per the given size
    fread(frame_tag, 1, prev_tag_size, mp3);
    fwrite(frame_tag, 1, prev_tag_size, mp3_update);    //copy the tag from old file to new file

    return True;
}


void copy_remaining_bytes(FILE *mp3, FILE *mp3_update)  //copy the remaining bytes
{
    char byte;

    while ( fread(&byte, 1, 1, mp3) )
        fwrite(&byte, 1, 1, mp3_update);                //run the loop as per the end of old file and copy that to new file
}
