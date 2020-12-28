#ifndef TYPES_H
#define TYPES_H

/* User defined types */
typedef unsigned int uint;              //changing typedef of unsigned int to uint

/* Status will be used in fn. return type */
typedef enum
{
    e_success,                          //for enum var if not defined here e_success value is 1
    e_failure                           //e_failure value is 2
} Status;

typedef enum
{
    e_encode,                           //e_encode 0
    e_decode,                           //e_decode 1
    e_unsupported                       //e_unsupported 2
} OperationType;

#endif
