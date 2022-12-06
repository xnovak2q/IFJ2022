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



/// @brief funkce na alokaci instrukce
/// @return ukazatel na instrukci
inst_t* create_new_inst(){
    inst_t *inst = /*(inst_t)*/malloc(sizeof(struct instruction));
    if (!inst)
    {
         exit(ERR_INTERN);
    }
    inst->prev_inst = NULL;
    inst->next_inst = NULL;
    
    initialize_string(&inst->inst_content);
    return inst;
}

void cg_ins_last(out_code *out, char *instruction){
    inst_t *new = create_new_inst();

    add_str_to_string(&new->inst_content, instruction);

    //linking
    if (!out->first_inst)
    {
        out->first_inst = new;
    }
    
    new->prev_inst = out->last_inst;
    new->next_inst = NULL;
    
    if(out->last_inst != NULL)
        out->last_inst->next_inst = new;
    else
        out->first_inst = new;
    out->last_inst = new;
}


// void cg_prog_init(out_code *out){
//     out->first_inst = NULL;
//     out->last_inst = NULL;
//     out->while_lvl = 0;
// }


void codegen_print_prog(out_code *out){
    inst_t *tmp = out->first_inst;
    if(!out->first_inst)
        return;
    while (tmp)
    {
        printf("%s", tmp->inst_content.string);
        tmp = tmp->next_inst;
    }
}
//dispose

void codegen_print_prolog (){
    printf(".IFJcode22\n");
}

void codegen_init(out_code *out){
    codegen_print_prolog();
    //cg_prog_init(out);

}


/*variables*/
static int is_while = 0;


char* string_converter(char* string2conv){
    if(string2conv == NULL)
        return NULL;

    if(string2conv[0] == '\0'){ //empty string
        return "";
    }

    dynamic_string* string = malloc(sizeof(dynamic_string));
    initialize_string(string);
    while (*string2conv != '\0'){
        if(*string2conv <= 32){
            add_char_to_string(string, '\\');
            add_char_to_string(string, '0');
            add_char_to_string(string, ((*string2conv / 10) + 48) );
            add_char_to_string(string, ((*string2conv % 10) + 48) );
        }
        else if (*string2conv == 35)
            add_str_to_string(string,"\\035");
        else if(*string2conv == 92)
            add_str_to_string(string, "\\092");
        else add_char_to_string(string, *string2conv);

        string2conv++;
    }

    return string->string;

}


/*===============PUSH STACK ======================*/

void codegen_push_string(char* string){
    if(!is_while){
        printf("PUSHS string@%s",string_converter(string));
    }
}

void codegen_push_int(int n){
    if(!is_while){
        printf("PUSHS int@%d\n", n);
    }
}


void codegen_push_float(float f){
    if(!is_while){
        printf("PUSHS float@%a\n", f);
    }
}

void codegen_push_nil(){
    if(!is_while){
        printf("PUSHS nil@nil\n");
    }
}

//void codegen_defvar(char* variable){}

//void codegen_var_assign()


/*==============IF=====================TODO while*/

void codegen_if_begin(int unif){
    //stack TODO unique label stackTOP
    if(!is_while){
        printf("POPS GF@exp\n");
        printf("JUMPIFNEQ if$%d$else GF@exp bool@true\n", unif);
    }
}

void codegen_if_else(int unif){
    if(!is_while){
        printf("JUMP if%d$end\n", unif);
        printf("LABEL if$%d$else\n", unif);
    }
}

void codegen_if_end(int unif){
    if(!is_while){
        printf("LABEL if$%d$end\n", unif);
    }
}


/*=======================WHILE================TODO

void codegen_while_begin(){//TODO
    is_while = 1;
    out_code->while_lvl++;//legit?
    printf();
}

void codegen_while_cond(){

}

void codegen_while_end(){

    printf("JUMP");
}
*/

/*=======BUILT IN FUNCS===========

void codegen_write(){

}
*/