/* IFJ 2022 */

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define DYNSTR_LEN 8

typedef struct dynamic_string
{
    char* string;
    int string_length;
    int allocated_length;
} dynamic_string;

//initialization of string
//length set to 1 and on the end of the string the end of string is added
void initialize_string(dynamic_string* string);

//free allocated memory used for dynamic string
void free_string(dynamic_string* string);

//adding a single character to a dynamic string
void add_char_to_string(dynamic_string* string, char character);

//adding whole string to a dynamic string
void add_str_to_string(dynamic_string* string, char* new_string);