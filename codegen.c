//
//  Created by xsrnak00
//

#include <stdio.h>
#include <ctype.h>
#include "codegen.h"
#include "parser.h"



//TODO void codegen_push_variable(char* var){}
//TODO: predrazeni pred while
/*
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
    
    new->prev_inst = out->last_inst;
    new->next_inst = NULL;
    
    if(out->last_inst != NULL)
        out->last_inst->next_inst = new;
    else
        out->first_inst = new;
    out->last_inst = new;
}


void cg_prog_init(out_code *program){
    program->first_inst = NULL;
    program->last_inst = NULL;
    program->while_lvl = 0;
}


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
void codegen_prog_dispose(out_code *out){

    while (out->first_inst)
    {   
        inst_t *tmp = out->first_inst;
        out->first_inst = out->first_inst->next_inst;
        free(tmp);
        tmp = NULL;
    }
    out->first_inst = NULL;
    out->last_inst = NULL;
}

void codegen_print_prolog (){
    printf(".IFJcode22\n");
}


/*promenne*/
static int is_while = 0;
static int if_id = 0;
static int while_id = 0;
out_code* out = NULL;

void codegen_init(){
    out = malloc(sizeof(out_code));
    codegen_print_prolog();
    cg_prog_init(out);
    printf("DEFVAR GF@expression\n");

}


char* string_converter(char* string2conv){
    if(string2conv == NULL)
        return NULL;

    if(string2conv[0] == '\0'){ //prazdny retezec
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
void codegen_push_var(char* var_name){
    if(!is_while){
        printf("PUSHS TF@%s\n", var_name);
    }
    else{
        char* buffer = (char*)malloc(strlen(var_name) + 20);
        sprintf(buffer, "PUSHS TF@%s\n", var_name);
        cg_ins_last(out, buffer);
        
    }
}

void codegen_push_string(char* string){
    if(!is_while){
        printf("PUSHS string@%s",string_converter(string));
    }
    else{
        char* buffer = (char*)malloc(strlen(string_converter(string)) +20);
        sprintf(buffer, "PUSHS string@%s\n",string_converter(string));
        cg_ins_last(out, buffer);
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
    if_id++;
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
    
    printf();
}

void codegen_while_cond(){

}

void codegen_while_end(){

    printf("JUMP");
}
*/

/*=======vestavene fce===========*/
//TODO param parsing
void codegen_write(){
    printf("#codegen write function\n");
    printf("JUMP $write$end\n");
    printf("LABEL write\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@par1\n");
    printf("MOVE LF@par1 LF@%%1\n");
    printf("WRITE LF@par1\n");
    printf("POPFRAME");
    printf("RETURN\n");
    printf("LABEL $write$end\n");

}


void codegen_defvar(char* varname){
    if(!is_while){
        printf("DEFVAR TF@%s", varname);
    }
}


void codegen_pops_var(char* varname){
    if (!is_while){
        printf("POPS TF@%s", varname);
    }
}

/*========FUNKCE===========*/

void codegen_func_begin(char* funcname){
    printf("JUMP %s$fun$end\n", funcname);
    printf("LABEL %s", funcname);
    printf("PUSHFRAME\n");
    printf("CREATEFRAME\n");
}

void codegen_func_return(){
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void codegen_func_end(char* funcname){
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL %s$fun$end\n", funcname);
}

void codegen_func_call(char* funcname){
    printf("CALL %s", funcname);
}

/*
void codegen_oper(tokenType operation){
    switch (operation)
    {
    case add:
        printf("ADDS\n");
        break;
    case sub:
        printf("SUBS\n");
        break;
    default:
        break;
    }
}*/

int main(void){
    codegen_init();
    is_while = 1;
    // cg_ins_last(out, "DEFVAR GF@var\n");
    // cg_ins_last(out, "PUSHS int@42\n");
    // cg_ins_last(out, "PUSHS int@10\n");
    // cg_ins_last(out, "SUBS\n");
    // cg_ins_last(out, "POPS GF@var\n");
    // cg_ins_last(out, "WRITE GF@var\n");
    // codegen_print_prog(out);
    // codegen_prog_dispose(out);
    // codegen_print_prog(out);

    // cg_ins_last(out, "POPS GF@var\n");
    codegen_push_string("testing");
    codegen_print_prog(out);
    return 0;
}