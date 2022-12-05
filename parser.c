#include "parser.h"

void loadFunctionDefs(){
    DEBUG printf("\x1B[36mLoading function definitions\033[0m\n");

    /*------------------------------DEFINICE VESTAVĚNÝCH FUNKCÍ------------------------------*/
    char* identifier_reads = (char*)malloc((5 +1)* sizeof(char)); identifier_reads = "reads";
    Symtable_InsertSymbol(&globalFunctionsTable, identifier_reads, nullableString);
    int* args_reads = (int*)malloc(0 * sizeof(int)); Symtable_SetFunctionArgs(globalFunctionsTable, identifier_reads, 0, args_reads);

    char* identifier_readi = (char*)malloc((5 +1)* sizeof(char)); identifier_readi = "readi";
    Symtable_InsertSymbol(&globalFunctionsTable, identifier_readi, nullableInt);
    int* args_readi = (int*)malloc(0 * sizeof(int)); Symtable_SetFunctionArgs(globalFunctionsTable, identifier_readi, 0, args_readi);

    char* identifier_readf = (char*)malloc((5 +1)* sizeof(char)); identifier_readf = "readf";
    Symtable_InsertSymbol(&globalFunctionsTable, identifier_readf, nullableFloat);
    int* args_readf = (int*)malloc(0 * sizeof(int)); Symtable_SetFunctionArgs(globalFunctionsTable, identifier_readf, 0, args_readf);

    char* identifier_write = (char*)malloc((5 +1)* sizeof(char)); identifier_write = "write"; //o až ∞ argumentů!
    Symtable_InsertSymbol(&globalFunctionsTable, identifier_write, typeVoid);
    int* args_write = (int*)malloc(0 * sizeof(int)); Symtable_SetFunctionArgs(globalFunctionsTable, identifier_write, 0, args_write);

    char* identifier_floatval = (char*)malloc((8 +1)* sizeof(char)); identifier_floatval = "floatval";
    Symtable_InsertSymbol(&globalFunctionsTable, identifier_floatval, typeFloat);
    int* args_floatval = (int*)malloc(1 * sizeof(int)); args_floatval[0] = typeAny; Symtable_SetFunctionArgs(globalFunctionsTable, identifier_floatval, 1, args_floatval);

    char* identifier_intval = (char*)malloc((6 +1)* sizeof(char)); identifier_intval = "intval";
    Symtable_InsertSymbol(&globalFunctionsTable, identifier_intval, typeInt);
    int* args_intval = (int*)malloc(1 * sizeof(int)); args_intval[0] = typeAny; Symtable_SetFunctionArgs(globalFunctionsTable, identifier_intval, 1, args_intval);

    char* identifier_strval = (char*)malloc((6 +1)* sizeof(char)); identifier_strval = "strval";
    Symtable_InsertSymbol(&globalFunctionsTable, identifier_strval, typeString);
    int* args_strval = (int*)malloc(1 * sizeof(int)); args_strval[0] = typeAny; Symtable_SetFunctionArgs(globalFunctionsTable, identifier_strval, 1, args_strval);

    char* identifier_strlen = (char*)malloc((6 +1)* sizeof(char)); identifier_strlen = "strlen";
    Symtable_InsertSymbol(&globalFunctionsTable, identifier_strlen, typeInt);
    int* args_strlen = (int*)malloc(1 * sizeof(int)); args_strlen[0] = typeString; Symtable_SetFunctionArgs(globalFunctionsTable, identifier_strlen, 1, args_strlen);

    char* identifier_substring = (char*)malloc((9 +1)* sizeof(char)); identifier_substring = "substring";
    Symtable_InsertSymbol(&globalFunctionsTable, identifier_substring, typeString);
    int* args_substring = (int*)malloc(3 * sizeof(int)); args_substring[0] = typeString; args_substring[1] = typeInt; args_substring[2] = typeInt; Symtable_SetFunctionArgs(globalFunctionsTable, identifier_substring, 3, args_substring);

    char* identifier_ord = (char*)malloc((3 +1)* sizeof(char)); identifier_ord = "ord";
    Symtable_InsertSymbol(&globalFunctionsTable, identifier_ord, typeInt);
    int* args_ord = (int*)malloc(1 * sizeof(int)); args_ord[0] = typeString; Symtable_SetFunctionArgs(globalFunctionsTable, identifier_ord, 1, args_ord);

    char* identifier_chr = (char*)malloc((3 +1)* sizeof(char)); identifier_chr = "chr";
    Symtable_InsertSymbol(&globalFunctionsTable, identifier_chr, typeString);
    int* args_chr = (int*)malloc(1 * sizeof(int)); args_chr[0] = typeInt; Symtable_SetFunctionArgs(globalFunctionsTable, identifier_chr, 1, args_chr);

    /*---------------------------------------------------------------------------------------*/

    DLTokenL_FetchNext(tokenList);
    while (DLTokenL_GetLast(tokenList)->tokenType != end)
    {
        if (is_functionDefinitionHeader())
            loadFunctionDefinition();
        DLTokenL_FetchNext(tokenList);
    }

    DLTokenL_UnFetchAll(tokenList);
    DEBUG printf("\x1B[35mFunction definitions loaded\033[0m\n\n");
}


void runAnalysis(){
    globalFunctionsTable = Symtable_Create();
    globalVariablesTable = Symtable_Create();
    localVariablesTable = Symtable_Create();
    currVariablesTable = globalVariablesTable;

    tokenList = DLTokenL_Create();
    programDepth = 0;
    
    /*----KONTROLA PROLOGU----*/
    if (flag_prolog || !flag_debug){
        DLTokenL_FetchNext(tokenList);
        if (DLTokenL_GetLast(tokenList)->tokenType != prolog)
        {
            DLTokenL_Dispose(tokenList); exit(2);
        }
        DLTokenL_FetchNext(tokenList);
        if (DLTokenL_GetLast(tokenList)->tokenType != declare)
        {
            DLTokenL_Dispose(tokenList); exit(2);
        } 
        DLTokenL_FetchNext(tokenList);
        if (DLTokenL_GetLast(tokenList)->tokenType != semicolumn)
        {
            DLTokenL_Dispose(tokenList); exit(2);
        }
    }
    /*-------------------------*/

    loadFunctionDefs();

    /*----PŘESKOČIT PROLOG----*/
    if (flag_prolog || !flag_debug){
        DLTokenL_FetchNext(tokenList);
        DLTokenL_FetchNext(tokenList);
        DLTokenL_FetchNext(tokenList);
    }
    /*------------------------*/

    DEBUG printf("\x1B[36mAt top level\033[0m\n");
    DLTokenL_FetchNext(tokenList);

    while (DLTokenL_GetLast(tokenList)->tokenType != end)
    {
        if (is_functionDefinitionHeader())
            functionDefinition();
        else statement();
        DEBUG printf("\n\x1B[36mAt top level\033[0m\n");
        DLTokenL_FetchNext(tokenList);
    }

    DEBUG printf("\n\x1B[1m\x1B[30mglobalFunctionsTable\033[0m\n");
    DEBUG Symtable_PrintInorder(globalFunctionsTable);
    DEBUG printf("\n\x1B[1m\x1B[30mglobalVariablesTable\033[0m\n");
    DEBUG Symtable_PrintInorder(globalVariablesTable);

    Symtable_Dispose(&globalFunctionsTable);
    Symtable_Dispose(&globalVariablesTable);
    Symtable_Dispose(&localVariablesTable);
    DLTokenL_Dispose(tokenList);

    DEBUG printf("\n\x1B[1m\x1B[32mSuccess ✔\033[0m\n\n");
}

//LL Grammar

void functionCall(){
    DEBUG printf("\x1B[36min function call\033[0m\n");

    char* functionName = DLTokenL_GetLast(tokenList)->value->string;

    if (!Symtable_ExistsSymbol(globalFunctionsTable, functionName))
        exit(3);    

    DLTokenL_FetchNext(tokenList); //(

    bool isLastArgument;
    bool isFirstArgument = true;
    int tokensToReturn = 1;
    int startBracketCount;

    size_t argumentCount = 0;
    int* argumentTypes = (int*)malloc(0);
    int currArgType;

    while (DLTokenL_FetchNext(tokenList)->tokenType != closeBracket)
    {
        DLTokenL_UnFetchNext(tokenList);

        while (token_is_expressionMember(DLTokenL_FetchNext(tokenList)))
            tokensToReturn++;

        if (DLTokenL_GetLast(tokenList)->tokenType == comma)
            isLastArgument = false;
        else
            isLastArgument = true;


        while (tokensToReturn > 0){
            DLTokenL_UnFetchNext(tokenList);
            tokensToReturn--;
        }  

        if (isFirstArgument == isLastArgument)
            startBracketCount = 0;
        else if (isLastArgument)
            startBracketCount = 1;
        else if(!isLastArgument)
            startBracketCount = -1;

        
        DLTokenL* expressionTokens = consumeExpression(false, startBracketCount);
        if (isFirstArgument && !isLastArgument)
            DLTokenL_DeleteFirst(expressionTokens);
        else if(!isFirstArgument && isLastArgument)
            DLTokenL_DeleteLast(expressionTokens);
        DEBUG DLTokenL_Print(expressionTokens);

        argumentCount++;
        argumentTypes = (int*)realloc(argumentTypes, argumentCount * sizeof(int));

        //TODO binary_tree* expressionTree = precedencka(expressionTokens);
        currArgType = 9; //TODO data_type_z_precedencky
        argumentTypes[argumentCount-1] = currArgType;

        if (strcmp(functionName, "write")){//není write()
            if (Symtable_GetFunctionArgsCount(globalFunctionsTable, functionName) < argumentCount) exit(4);

            if (!strcmp(functionName, "intval")){         //je intval()
                if (currArgType != typeInt && currArgType != typeFloat) exit(4);
            } else if (!strcmp(functionName, "floatval")){//je floatval()
                if (currArgType != typeInt && currArgType != typeFloat) exit(4);
            } else if (!strcmp(functionName, "strval")){  //je strval()
                if (currArgType != typeString && currArgType != nnull) exit(4);
            } else{//je uživatelem definovaná funkce
                if(!types_match(currArgType, Symtable_GetFunctionArgType(globalFunctionsTable, functionName, argumentCount-1))) exit(4);
            }

        } else {//je write()
            if (currArgType != typeInt && currArgType != typeFloat && currArgType != nnull) exit(4);
        }

        //TODO generator_variableDeclaration(variableName, , expressionTree); pro lokalni promennou
        DLTokenL_Dispose(expressionTokens);

        isFirstArgument = false;
        if (isLastArgument){
            DLTokenL_UnFetchNext(tokenList);
            if (DLTokenL_GetLast(tokenList)->tokenType != closeBracket)
                exit(2);
            DLTokenL_UnFetchNext(tokenList);
        }
    }

    if (strcmp(functionName, "write") && //není write()
        Symtable_GetFunctionArgsCount(globalFunctionsTable, functionName) != argumentCount)
        exit(4);
    
    
    //TODO generate function call
    DEBUG printf("\x1B[35mout of function call\033[0m\n");
}

void whileStatement(){
    DEBUG printf("\x1B[36min while statement\033[0m\n");

    if(DLTokenL_FetchNext(tokenList)->tokenType != openBracket) exit(2); // prvni '('
    
    DLTokenL* expressionTokens = consumeExpression(false, 0);
    //TODO precedencka(expressionTokens);
    DEBUG DLTokenL_Print(expressionTokens);
    DLTokenL_Dispose(expressionTokens);

    compoundStatement();
    DEBUG printf("\x1B[35mout of while statement\033[0m\n");
}

void ifStatement(){
    DEBUG printf("\x1B[36min if statement\033[0m\n");

    if(DLTokenL_FetchNext(tokenList)->tokenType != openBracket) exit(2); // prvni '('
    
    DLTokenL* expressionTokens = consumeExpression(false, 0);
    //TODO precedencka(expressionTokens);
    DEBUG DLTokenL_Print(expressionTokens);
    DLTokenL_Dispose(expressionTokens);

    compoundStatement();

    DLTokenL_FetchNext(tokenList);
    if (is_elseStatement())
        elseStatement();
    
    DEBUG printf("\x1B[35mout of if statement\033[0m\n");
}
void elseStatement(){
    DEBUG printf("\x1B[36min else statement\033[0m\n");
   
    DLTokenL_FetchNext(tokenList);
    compoundStatement();
    DEBUG printf("\x1B[35mout of else statement\033[0m\n");
}

void compoundStatement(){
    if (!is_compoundStatement())
        exit(2);
    DEBUG printf("\x1B[36min compound statement\033[0m\n");

    
    bool returnStatementOccurred = false;

    DLTokenL_FetchNext(tokenList);
    while (DLTokenL_GetLast(tokenList)->tokenType != closeCurly)
    {
        if (inFunctionDefinition && is_returnStatement()){
            returnStatementOccurred = true;

            DEBUG printf("\x1B[36min return statement\033[0m\n");
            DLTokenL_FetchNext(tokenList);
            if (is_functionCall())
            {
                if (Symtable_ExistsSymbol(globalFunctionsTable, DLTokenL_GetLast(tokenList)->value->string))
                {
                    if (types_match(Symtable_GetType(globalFunctionsTable, DLTokenL_GetLast(tokenList)->value->string), currFunctionReturnType)){
                        functionCall();
                        DLTokenL_FetchNext(tokenList);
                    }
                    else exit(4); //nesprávný návratový typ volané funkce
                    
                } else exit(3); //funkce neexistuje
                
            } else{
                DLTokenL* expressionTokens = consumeExpression(true, 0);
                DEBUG DLTokenL_Print(expressionTokens);
                
                if (DLTokenL_GetLength(expressionTokens) == 0 && !type_is_nullable(currFunctionReturnType))
                    exit(6); //return; ve funkci s nenullovantelným typem
                
                else if (DLTokenL_GetLength(expressionTokens) > 0 && currFunctionReturnType == typeVoid)
                    exit(6); //retuen exp; ve funkci typu void

                else{
                    //TODO typ_z_precedencky
                    //TODO types_match(typ_z_precedencky), currFunctionReturnType)
                }

                DLTokenL_Dispose(expressionTokens);
            }

            printf("%s\n", DLTokenL_GetLast(tokenList)->value->string);
            if (DLTokenL_GetLast(tokenList)->tokenType != semicolumn)
                exit(2);

            DEBUG printf("\x1B[35mout of return statement\033[0m\n");
        } else
            statement(); 

        DLTokenL_FetchNext(tokenList);
        if (DLTokenL_GetLast(tokenList)->tokenType == end)
            exit(2);   
    }

    if (inFunctionDefinition && !returnStatementOccurred && currFunctionReturnType != typeVoid)
        exit(6);    

    DEBUG printf("\x1B[35mout of compound statement\033[0m\n");
}


void functionDefinition(){
    //if (!is_functionDefinitionHeader() || programDepth != 0) exit(2);
    if (!is_functionDefinitionHeader()) exit(2);
    DEBUG printf("\x1B[36min function definition\033[0m\n");
    
    currVariablesTable = localVariablesTable;
    inFunctionDefinition = true;

    if (DLTokenL_FetchNext(tokenList)->tokenType != identificator) exit(2);
    if(DLTokenL_FetchNext(tokenList)->tokenType != openBracket) exit(2); // function Foo->(<-string $x, int $y): bool {}
    
    if (DLTokenL_FetchNext(tokenList)->tokenType == closeBracket)
    {
        //TODO function without parameters
    } else {
        DLTokenL_UnFetchNext(tokenList); // zpet na ->(<-
        while (DLTokenL_GetLast(tokenList)->tokenType != closeBracket)
        {
            if (!token_is_type(DLTokenL_FetchNext(tokenList)))        exit(2);
            int argType = DLTokenL_GetLast(tokenList)->tokenType;
            if (DLTokenL_FetchNext(tokenList)->tokenType != variable) exit(2);
            Symtable_InsertSymbol(&currVariablesTable, DLTokenL_GetLast(tokenList)->value->string, argType);
            Symtable_InsertSymbol(&currVariablesTable, DLTokenL_GetLast(tokenList)->value->string, argType);

            if (DLTokenL_FetchNext(tokenList)->tokenType != comma && DLTokenL_GetLast(tokenList)->tokenType != closeBracket) exit(2);
        }
    }
    
    
    if (DLTokenL_FetchNext(tokenList)->tokenType != colon) exit(2);
    if (!token_is_type(DLTokenL_FetchNext(tokenList))) exit(2);

    currFunctionReturnType = DLTokenL_GetLast(tokenList)->tokenType;

    DLTokenL_FetchNext(tokenList);
    compoundStatement();
    DEBUG printf("\x1B[1m\x1B[30mlocalVariablesTable\033[0m\n");
    DEBUG Symtable_PrintInorder(currVariablesTable);
    currVariablesTable = globalVariablesTable;
    Symtable_Clear(&localVariablesTable);
    inFunctionDefinition = false;
    DEBUG printf("\x1B[35mout of function definition\033[0m\n");
}

void loadFunctionDefinition(){
    if (!is_functionDefinitionHeader()) return;
    DEBUG printf("\x1B[36mloading function definition\033[0m\n");
    
    if (DLTokenL_FetchNext(tokenList)->tokenType != identificator) return;
    char* functionName = DLTokenL_GetLast(tokenList)->value->string;
    if (Symtable_ExistsSymbol(globalFunctionsTable, functionName)) //pokus o redefinici funkce
        exit(3);

    size_t functionArgsCount = 0;
    int* functionArgs = (int*)malloc(0);

    if(DLTokenL_FetchNext(tokenList)->tokenType != openBracket) return; // function Foo->(<-string $x, int $y): bool {}
    
    if (DLTokenL_FetchNext(tokenList)->tokenType == closeBracket)
    {
        //TODO function without parameters
    } else {
        DLTokenL_UnFetchNext(tokenList); // zpet na ->(<-
        while (DLTokenL_GetLast(tokenList)->tokenType != closeBracket)
        {
            if (!token_is_type(DLTokenL_FetchNext(tokenList)))        return;
            int argType = DLTokenL_GetLast(tokenList)->tokenType;
            functionArgsCount++;
            functionArgs = (int*)realloc(functionArgs, functionArgsCount * sizeof(int));
            functionArgs[functionArgsCount-1] = argType;

            if (DLTokenL_FetchNext(tokenList)->tokenType != variable) return;


            if (DLTokenL_FetchNext(tokenList)->tokenType != comma && DLTokenL_GetLast(tokenList)->tokenType != closeBracket) return;
        }
    }
    
    
    if (DLTokenL_FetchNext(tokenList)->tokenType != colon) return;
    if (!token_is_type(DLTokenL_FetchNext(tokenList))) return;

    int functionReturnType = DLTokenL_GetLast(tokenList)->tokenType;
    Symtable_InsertSymbol(&globalFunctionsTable, functionName, functionReturnType);
    Symtable_SetFunctionArgs(globalFunctionsTable, functionName, functionArgsCount, functionArgs);

    /*if ( Symtable_ExistsSymbol(globalFunctionsTable, functionName) ){
        printf("%s: %s\n", functionName, tokenTypos[Symtable_GetType(globalFunctionsTable, functionName)]);
        printf("%zu - ", Symtable_GetFunctionArgsCount(globalFunctionsTable, functionName));

        for (size_t i = 0; i < Symtable_GetFunctionArgsCount(globalFunctionsTable, functionName); i++)
            printf("%s ", tokenTypos[Symtable_GetFunctionArgType(globalFunctionsTable, functionName, i)]);
        
        printf("\n");
    }*/

    DLTokenL_FetchNext(tokenList);
    if (!is_compoundStatement())
        return;
    DLTokenL_FetchNext(tokenList);
    int curlyBracketCount = 1;
    while (curlyBracketCount > 0 && DLTokenL_GetLast(tokenList)->tokenType != end)
    {
        if (DLTokenL_GetLast(tokenList)->tokenType == openCurly)
            curlyBracketCount++;
        else if (DLTokenL_GetLast(tokenList)->tokenType == closeCurly)
            curlyBracketCount--;

        DLTokenL_FetchNext(tokenList);
    }
    if (curlyBracketCount != 0)
        return;
    
    DLTokenL_UnFetchNext(tokenList);
    DEBUG printf("\x1B[35mloaded %s():%s definition\033[0m\n", functionName, tokenTypos[Symtable_GetType(globalFunctionsTable, functionName)]);
}

void lineStatement(){
    DEBUG printf("\x1B[36min line-statement\033[0m\n");

    if (is_functionCall())
        functionCall();

    DLTokenL_FetchNext(tokenList);

    DEBUG printf("\x1B[35mout of line-statement\033[0m\n");
    
}

DLTokenL* consumeExpression(bool canBeEmpty, int startingRoundBracketCount){
    DEBUG printf("\x1B[36min expression\033[0m\n");
    DLTokenL_Last(tokenList);

    int roundBracketCount = startingRoundBracketCount;
    int nonBracketCount = 0;

    token* currToken;
    while (token_is_expressionMember(DLTokenL_GetLast(tokenList))){
        currToken  = DLTokenL_GetLast(tokenList);

        if (currToken->tokenType == openBracket)
            roundBracketCount++;
        else if (currToken->tokenType == closeBracket)
            roundBracketCount--;
        else{
            if (currToken->tokenType == variable && !Symtable_ExistsSymbol(currVariablesTable, currToken->value->string))
                exit(5);    
                    
            nonBracketCount++;
        }
        
        DLTokenL_FetchNext(tokenList);
    }

    if (roundBracketCount != 0)
        exit(2);
    if (nonBracketCount == 0 && !canBeEmpty)
        exit(2);

    DLTokenL* returnedList = DLTokenL_CopyFromActive(tokenList);
    DLTokenL_DeleteLast(returnedList);
    DEBUG printf("\x1B[35mout of expression\033[0m\n");
    return returnedList;
}

void variableDefinition(){
    DEBUG printf("\x1B[36min variable definition\033[0m\n");

    char* variableName = DLTokenL_GetLast(tokenList)->value->string;
    int variableType = -1;
    DLTokenL_FetchNext(tokenList);
    DLTokenL_FetchNext(tokenList);

    if (!is_lineStatement(tokenList))
        exit(2);

    if (is_functionCall()) {
        char* functionName = DLTokenL_GetLast(tokenList)->value->string;
        if (!Symtable_ExistsSymbol(globalFunctionsTable, functionName)) exit(3);
        variableType = Symtable_GetType(globalFunctionsTable, functionName);

        functionCall();
        DLTokenL_FetchNext(tokenList);
    } else {
        DLTokenL* expressionList = consumeExpression(false, 0);
        //TODO binary_tree* expressionTree = precedencka(expressionList);
        //TODO generator_variableDeclaration(variableName, , expressionTree);
        //variableType = getTypeFromPrecedenceTree(expressionTree);
        DEBUG DLTokenL_Print(expressionList);
        DLTokenL_Dispose(expressionList);
    }

    if (DLTokenL_GetLast(tokenList)->tokenType != semicolumn)
        exit(2);

    Symtable_InsertSymbol(&currVariablesTable, variableName, variableType);
    DEBUG printf("\x1B[35mout of variable definition\033[0m\n");
}

void statement(){
    DEBUG printf("\x1B[36min statement\033[0m\n");
    if(is_ifStatement())   
        ifStatement();
    else if (is_whileStatement())
        whileStatement();
    else if (is_compoundStatement())
       compoundStatement();
    else if (is_variableDefinition())
       variableDefinition();
    else if (is_lineStatement())
       lineStatement();
    else
        exit(2);

    DEBUG printf("\x1B[35mout of statement\033[0m\n");
}