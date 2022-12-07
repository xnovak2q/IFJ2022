


#ifndef IFJ22_ERROR_H
#define IFJ22_ERROR_H

#include <stdint.h>

typedef enum error {
    ERR_LEX         = 1,
    ERR_SYNTAX      = 2,
    ERR_SEM_DEF     = 3,
    ERR_SEM_PARAM   = 4,
    ERR_SEM_UNDEF   = 5,
    ERR_SEM_EXP     = 6,
    ERR_SEM_TYPE    = 7,
    ERR_SEM_OTHER   = 8,
    ERR_INTERN      = 99,
} error_t;

extern error_t err;

#endif
