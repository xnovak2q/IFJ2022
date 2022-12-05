#include <stdio.h>
#include <ctype.h>
#include "codegen.h"
#include "parser.h"

//static int while_lvl = 0;

//TODO void codegen_push_variable(char* var){}
//TODO char* string_converter(char* str2convert){}
//TODO: predrazeni pred while
/*
*dll
*stack
*if print

*while print

*assignment
*builtin
*operations

*/
void codegen_print_prolog (){
    printf(".IFJcode22");
}

/// @brief funkce na alokaci instrukce
/// @return ukaxatel na instrukci
inst_t* create_new_inst(){
    inst_t inst = (inst_t*)malloc(sizeof(inst_t));
    if (!inst)
        exit(ERR_INTERN);

    inst->prev_inst = NULL;
    inst->next_inst = NULL;
    
    initialize_string(&inst->inst_content);
    return inst;
}

//TODO tbd
char* string_converter(char* string2conv){
    if(string2conv == NULL)
        return NULL;

    if(string2conv[0] == '\0'){ //empty string
        return "";
    }

    dynamic_string* string = malloc(sizeof(dynamic_string));
    initialize_string(string);
    //add_str_to_string(string, string2conv);
    while (*string2conv != '\0'){
        if(*string2conv == ' ')
            add_str_to_string(string, "\\032");
        else if(*string2conv == '\\'){
            //printf("tady1\n");
            add_char_to_string(string, '\\');
            if (!isdigit(*(string2conv+1)))
            {
                //string2conv++;

                switch (*string2conv)
                {
                case '\\':
                    add_str_to_string(string, "092");
                    break;

                case 't':
                    add_str_to_string(string, "009");
                    break;

                default:
                    break;
                }

            }


            // if(!isdigit(*(string2conv+1))){
            //     string2conv++;

            //     switch (*string2conv)
            //     {
            //     case 'n':
            //         add_str_to_string(string, "010");
            //         break;

                

            //     case '\\':
            //         /*printf("tady2");
            //         add_char_to_string(string, '0');
            //         add_char_to_string(string, '9');
            //         add_char_to_string(string, '2');*/
            //         add_str_to_string(string, "092");
            //         break;

            //     default:
            //         break;
            //     }
            // }
        }
        else add_char_to_string(string, *string2conv);

        string2conv++;
    }

    return string->string;

}


/*===============PUSH STACK ======================*/

void codegen_push_string(char* string){
    if(!while_lvl){
        printf("PUSHS string@%s",string_converter(string));
    }
}

void codegen_push_int(int n){
    if(!while_lvl){
        printf("PUSHS int@%d\n", n);
    }
}


void codegen_push_float(float f){
    if(!while_lvl){
        printf("PUSHS float@%a\n", f);
    }
}

void codegen_push_nil(){
    if(!while_lvl){
        printf("PUSHS nil@nil\n");
    }
}

//void codegen_defvar(char* variable){}

//void codegen_var_assign()


/*==============IF=====================TODO while*/
/*
void codegen_if_begin(){
    stack TODO unique label stackTOP
    if(!while_lvl){
        printf("POPS GF@exp\n");
        printf("JUMPIFNEQ if$%d$else GF@exp bool@true\n", unifyingint);
    }
}

void codegen_if_else(){
    if(!while_lvl){
        printf("JUMP if%d$end\n", unifyingint);
        printf("LABEL if$%d$else\n");
    }
}

void codegen_if_end(){
    if(!while_lvl){
        printf("LABEL if$%d$end\n", unifyingint);
    }
}
*/

/*=======================WHILE================

void codegen_while_begin(){//TODO
    while_lvl++;
    printf();
}

void codegen_while_cond(){

}

void codegen_while_end(){

    printf("JUMP");
}
*/
