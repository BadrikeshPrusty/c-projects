#ifndef TYPES_H
#define TYPES_H


/* Status will be used in fn. return type */
typedef enum
{
    False,                         //False has value 0 as it is first enum variable default value
    True                           //True has value 1
} Status;


//Check for operation type
typedef enum
{
    help = 2,                           //for help menu
    view,                               //view value is 3
    edit,                               //edit value is 4
    unsupported                         //unsupported value is 5
} OperationType;

#endif
