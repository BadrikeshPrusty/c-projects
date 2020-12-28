#ifndef VIEW_H
#define VIEW_H

#include "types.h"

//Global Variables
char frame_tag_size;
char frame[5];
FILE *mp3;

//Function prototypes for view and help menu

//Operation Type
OperationType check_operation_type(char *argv[]);

//Read and Validate Arguments to check whether it's a .mp3 file or not
Status read_and_validate_args(char *argv[]);

//Open the .mp3 file
Status open_files(char *argv[]);

//Show help menu
void help_menu();

//View mp3 file details
Status view_details(FILE *mp3);

//Store the contents to buffer for printing, move the file pointer to next frame tag
void file_seek(FILE *mp3, char **frame_tag);

//To print the contents of the specific frame tag
void print(char *frame_tag);

//Check if the character for printing is readable or not
Status readable_chk(char ch);

#endif
