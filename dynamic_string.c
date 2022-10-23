/* IFJ 2022 */

#include "dynamic_string.h"

void initialize_string(dynamic_string* string)
{
    printf("initialize_string\n");
    string->string = (char*)malloc(DYNSTR_LEN);
    if (!string->string)
    {
        exit(99);
    }
    printf("malloced\n");
    string->allocated_length = DYNSTR_LEN;
    printf("dynstr len\n");
    string->string_length = 0;
    printf("strlen\n");
    string->string[string->string_length] = '\0';
    printf("going home\n");
}

void free_string(dynamic_string* string)
{
    if (string->string)
    {
        free(string->string);
        string->allocated_length = 0;
        string->string_length = 0;
    }
}

void add_char_to_string(dynamic_string* string, char character)
{
    if ((string->string_length + 1) >= (string->allocated_length))
    {
        int new_size = string->string_length + 1;
        string->string = (char*)realloc(string->string, new_size);
        if (!string->string)
        {
            exit(99);
        }
        string->allocated_length = new_size;
    }
    string->string[string->string_length] = character;
    string->string_length++;
    string->string[string->string_length] = '\0';
}

void add_str_to_string(dynamic_string* string, char* new_string)
{
    int size_of_new_string = strlen(new_string);
    if ((string->string_length + size_of_new_string + 1) >= (string->allocated_length))
    {
        int new_size = string->string_length + size_of_new_string + 1;
        string->string = (char*)realloc(string->string, new_size);
        if (!string->string)
        {
            exit(99);
        }
        string->allocated_length = new_size;
    }
    string->string_length = string->string_length + size_of_new_string;
    strcat(string->string, new_string);
    string->string[string->string_length] = '\0';
}