//
// Created by xfuisl00 on 22.10.2022.
//

#include <stdio.h>
#include "parser.c"
#include "dynamic_string.c"
#include "basicScannerFunctions.c"
#include "codegen.c"
#include "codegen.h"

int main(){
    //return runAnalysis();
    printf("test\n");
    //str convert test tbd
    char* test_string = "str \t ing\\lomeno";
    char* mod_string = string_converter(test_string);
    //printf("\\0\n");

    printf("%s\n", mod_string);
    // out_code out;
    // //codegen_init(&out);
    // codegen_print_prolog();
    // //cg_prog_init(&out);
    // cg_ins_last(&out, "DEFVAR GF@var\n");
    // cg_ins_last(&out, "PUSHS int@42\n");
    // cg_ins_last(&out, "PUSHS int@10\n");
    // cg_ins_last(&out, "SUBS\n");
    // cg_ins_last(&out, "POPS GF@var\n");
    // cg_ins_last(&out, "WRITE GF@var\n");
    // codegen_print_prog(&out);
    return 0;
}