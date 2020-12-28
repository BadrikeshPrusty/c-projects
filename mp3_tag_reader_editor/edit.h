#ifndef EDIT_H
#define EDIT_H

#include "types.h"

//Global Variables
char frame[5];

//Function Prototypes

//check for input flag validity
OperationType check_edit_flag(char *argv[]);

//check for metadata tag and validate
Status read_and_validate_edit_tags(char *argv[]);

//edit the file details
void edit_details(FILE *mp3, char *argv[]);

//copy same contents as original
Status copy_same_as_org(FILE *mp3, FILE *mp3_update);

//Update the frame tag passed through command line
void update_frame_tag(char *argv[], FILE *mp3, FILE *mp3_update);

//copy remaining mp3 data
void copy_remaining_bytes(FILE *mp3, FILE *mp3_update);

#endif
