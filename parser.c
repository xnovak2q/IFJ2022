//
// Autor: xkonec86
// Syntaktická a sémantická analýza
//

#include "parser.h"

DLTokenL* tokenList;                    // Globální seznam tokenů
Symtable_node* globalVariablesTable;    // Globální tabulka globálních proměnných
Symtable_node* localVariablesTable;     // Globální tabulka lokálních proměnných
Symtable_node* currVariablesTable;      // Momentálně nastavená globální tabulka proměnných
Symtable_node* globalFunctionsTable;    // Globální tabulka globálních definicí funkcí
bool flag_debug = false;
bool flag_prolog = true;

bool inFunctionDefinition = false;
bool returnStatementOccurred = false;
int currFunctionReturnType;
int programDepth;

void loadFunctionDefs(){

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
}


void runAnalysis(){
    globalFunctionsTable = Symtable_Create();
    globalVariablesTable = Symtable_Create();
    localVariablesTable = Symtable_Create();
    currVariablesTable = globalVariablesTable;

    tokenList = DLTokenL_Create();
    programDepth = 0;
    
    /*----KONTROLA PROLOGU----*/
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
    /*-------------------------*/

    loadFunctionDefs();

    /*----PŘESKOČIT PROLOG----*/
    DLTokenL_FetchNext(tokenList);
    DLTokenL_FetchNext(tokenList);
    DLTokenL_FetchNext(tokenList);
    /*------------------------*/
    DLTokenL_FetchNext(tokenList);

    while (DLTokenL_GetLast(tokenList)->tokenType != end)
    {
        if (is_functionDefinitionHeader())
            functionDefinition();
        else statement();
        DLTokenL_FetchNext(tokenList);
    }

    Symtable_Dispose(&globalFunctionsTable);
    Symtable_Dispose(&globalVariablesTable);
    Symtable_Dispose(&localVariablesTable);
    DLTokenL_Dispose(tokenList);
}

/*========================= L L  G R A M A T I K A =========================*/

void functionCall(){

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

        if (DLTokenL_GetLast(tokenList)->tokenType == comma){
            isLastArgument = false;
            
            if (DLTokenL_FetchNext(tokenList)->tokenType == closeBracket) exit(2);
            DLTokenL_UnFetchNext(tokenList);
        }
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

        argumentCount++;
        argumentTypes = (int*)realloc(argumentTypes, argumentCount * sizeof(int));

        treeNode* expressionTree = precedence(expressionTokens);
        currArgType = getExpressionDataType(expressionTree);
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
}

void whileStatement(){

    if(DLTokenL_FetchNext(tokenList)->tokenType != openBracket) exit(2); // prvni '('
    
    DLTokenL* expressionTokens = consumeExpression(false, 0);
    //TODO precedencka(expressionTokens);
    DLTokenL_Dispose(expressionTokens);

    compoundStatement();
}

void ifStatement(){

    if(DLTokenL_FetchNext(tokenList)->tokenType != openBracket) exit(2); // prvni '('
    
    DLTokenL* expressionTokens = consumeExpression(false, 0);
    //TODO precedencka(expressionTokens);
    DLTokenL_Dispose(expressionTokens);

    compoundStatement();

    DLTokenL_FetchNext(tokenList);
    if (is_elseStatement())
        elseStatement();
    else DLTokenL_UnFetchNext(tokenList);
}
void elseStatement(){
   
    DLTokenL_FetchNext(tokenList);
    compoundStatement();
}

void compoundStatement(){
    if (!is_compoundStatement())
        exit(2);

    
    DLTokenL_FetchNext(tokenList);
    while (DLTokenL_GetLast(tokenList)->tokenType != closeCurly)
    {
        if (inFunctionDefinition && is_returnStatement()){
            returnStatementOccurred = true;
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
                
                if (DLTokenL_GetLength(expressionTokens) == 0 && !type_is_nullable(currFunctionReturnType))
                    exit(6); //return; ve funkci s nenullovantelným typem
                
                else if (DLTokenL_GetLength(expressionTokens) > 0 && currFunctionReturnType == typeVoid)
                    exit(6); //return (EXP); ve funkci typu void

                else{
                    treeNode* expressionTree = precedence(expressionTokens);
                    int returnType = getExpressionDataType(expressionTree);

                    if(!types_match(returnType, currFunctionReturnType))
                        exit(4); //neplatný typ výrazu za return
                }

                DLTokenL_Dispose(expressionTokens);
            }

            if (DLTokenL_GetLast(tokenList)->tokenType != semicolumn)
                exit(2);
        } else {
            
            statement();
        }
            

        DLTokenL_FetchNext(tokenList);
        if (DLTokenL_GetLast(tokenList)->tokenType == end)
            exit(2);
    }

    if (inFunctionDefinition && !returnStatementOccurred && currFunctionReturnType != typeVoid)
        exit(6);    
}


void functionDefinition(){
    if (!is_functionDefinitionHeader()) exit(2);
    
    currVariablesTable = localVariablesTable;
    inFunctionDefinition = true;
    returnStatementOccurred = false;

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
    DLTokenL_FetchNext(tokenList);

    currFunctionReturnType = DLTokenL_GetLast(tokenList)->tokenType;
    if (!token_is_type(DLTokenL_GetLast(tokenList)) || currFunctionReturnType == nnull)
        exit(2);

    DLTokenL_FetchNext(tokenList);
    compoundStatement();
    currVariablesTable = globalVariablesTable;
    Symtable_Clear(&localVariablesTable);
    inFunctionDefinition = false;
}

void loadFunctionDefinition(){
    if (!is_functionDefinitionHeader()) return;
    
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
}

void lineStatement(){

    if (is_functionCall())
        functionCall();

    DLTokenL_FetchNext(tokenList);
    
}

DLTokenL* consumeExpression(bool canBeEmpty, int startingRoundBracketCount){
    DLTokenL_Last(tokenList);

    int roundBracketCount = startingRoundBracketCount;
    int nonBracketCount = 0;

    int expressionGeneralType = invalid;
    int currTermType;

    token* currToken;
    while (token_is_expressionMember(DLTokenL_GetLast(tokenList))){
        currToken  = DLTokenL_GetLast(tokenList);

        if (currToken->tokenType == openBracket)
            roundBracketCount++;
        else if (currToken->tokenType == closeBracket)
            roundBracketCount--;
        else{
            if (currToken->tokenType == variable){
                if(!Symtable_ExistsSymbol(currVariablesTable, currToken->value->string)) exit(5);//nedefinovaná proměnná
                
                currTermType = Symtable_GetType(currVariablesTable, currToken->value->string);

            } else if (expressionTree_token_is_literal(currToken)){
                currTermType = currToken->tokenType;
            }

            if (expressionGeneralType != invalid && expressionGeneralType != expressionTree_generalType(currTermType))
                exit(7);

            expressionGeneralType = expressionTree_generalType(currTermType);

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
    return returnedList;
}

void variableDefinition(){

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
        if (variableType == typeVoid) exit(4);
        

        functionCall();
        DLTokenL_FetchNext(tokenList);
    } else {
        DLTokenL* expressionList = consumeExpression(false, 0);

        treeNode* expressionTree = precedence(expressionList);
        variableType = getExpressionDataType(expressionTree);

        DLTokenL_Dispose(expressionList);
    }

    if (DLTokenL_GetLast(tokenList)->tokenType != semicolumn)
        exit(2);

    Symtable_InsertSymbol(&currVariablesTable, variableName, variableType);
}

void statement(){
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
}

/*================================= P O M O C N É  F U N K C E =================================*/
bool is_ifStatement()              { return DLTokenL_GetLast(tokenList)->tokenType == iff       ;}
bool is_whileStatement()           { return DLTokenL_GetLast(tokenList)->tokenType == wwhile    ;}
bool is_elseStatement()            { return DLTokenL_GetLast(tokenList)->tokenType == eelse     ;}
bool is_functionDefinitionHeader() { return DLTokenL_GetLast(tokenList)->tokenType == ffunction ;}
bool is_compoundStatement()        { return DLTokenL_GetLast(tokenList)->tokenType == openCurly ;}
bool is_returnStatement()          { return DLTokenL_GetLast(tokenList)->tokenType == rreturn   ;}
bool is_variableDefinition(){
    DLTokenL_FetchNext(tokenList);
    bool isVariableDefinition = DLTokenL_GetLast(tokenList)->tokenType == equal && DLTokenL_GetLastElement(tokenList)->previousElement->token->tokenType == variable;
    DLTokenL_UnFetchNext(tokenList);
    return isVariableDefinition;
}
bool is_lineStatement(){
    int fetchedTokensCount = 0;

    int functionCall_bracketCount;
    while (token_is_expressionMember(DLTokenL_GetLast(tokenList)) || is_functionCall())
    {
        if (is_functionCall()){
            functionCall_bracketCount = 1;
            DLTokenL_FetchNext(tokenList); 
            DLTokenL_FetchNext(tokenList); // foo(>arg<
            fetchedTokensCount += 2;

            while ((token_is_expressionMember(DLTokenL_GetLast(tokenList)) || DLTokenL_GetLast(tokenList)->tokenType == comma) && functionCall_bracketCount > 0)
            {
                if (DLTokenL_GetLast(tokenList)->tokenType == openBracket)
                    functionCall_bracketCount++;
                else if (DLTokenL_GetLast(tokenList)->tokenType == closeBracket)
                    functionCall_bracketCount--;
                                
                DLTokenL_FetchNext(tokenList);
                fetchedTokensCount++;
            }

        } else{
            fetchedTokensCount++;
            DLTokenL_FetchNext(tokenList);
        }
    }

    //DLTokenL_FetchNext(tokenList);
    bool isLineStatement = DLTokenL_GetLast(tokenList)->tokenType == semicolumn;

    for (size_t i = 0; i < fetchedTokensCount; i++)
        DLTokenL_UnFetchNext(tokenList);
    
    return isLineStatement;
}
bool is_functionCall(){
    bool isFunctionCall = false;
    if (DLTokenL_GetLast(tokenList)->tokenType == identificator){
        isFunctionCall = DLTokenL_FetchNext(tokenList)->tokenType == openBracket;
        DLTokenL_UnFetchNext(tokenList);
    }
    return isFunctionCall;
}
bool token_is_type(token* token){
    switch (token->tokenType){
        case typeInt:
        case typeString:
        case typeFloat:
        case typeVoid:
        case nullableInt:
        case nullableString:
        case nullableFloat:
        case nullableVoid:
        case nnull:
            return true;    
        default:
            return false;
    }
}
bool type_is_nullable(int type){
    switch (type){
        case typeVoid:
        case nullableInt:
        case nullableString:
        case nullableFloat:
        case nullableVoid:
        case nnull:
            return true;    
        default:
            return false;
    }
}
bool token_is_expressionMember(token* token){
    switch (token->tokenType){
        case variable:
        case sstring:
        case integer:
        case ffloat:
        case exponent:
        case add:
        case sub:
        case mul:
        case ddiv:
        case concat:
        case openBracket:
        case closeBracket:
        case cmpEqual:
        case notEquals:
        case greater:
        case lower:
        case greaterEqual:
        case lowerEqual:
        case nnull:
            return true;
        default:
            return false;
    }
}

size_t typeMatchesKey(int type){
    
    switch (type){
    case typeInt:        return 0;
    case typeString:     return 1;
    case typeFloat:      return 2;
    case typeVoid:       return 3;
    case nullableInt:    return 4;
    case nullableString: return 5;
    case nullableFloat:  return 6;
    case nnull:          return 7;
    case typeBool:       return 8;
    default:             exit(99);
    }
}
int typeMatches[9][9] = {
                //TO     typeInt typeString typeFloat typeVoid nullableInt nullableString nullableFloat nnull typeBool
            /*FROM*/
/*typeInt*/        {         1,        0,       0,        0,         1,          0,            0,         0,      0   },
/*typeString*/     {         0,        1,       0,        0,         0,          1,            0,         0,      0   },
/*typeFloat*/      {         0,        0,       1,        0,         0,          0,            1,         0,      0   },
/*typeVoid*/       {         0,        0,       0,        0,         1,          1,            1,         0,      0   },
/*nullableInt*/    {         0,        0,       0,        0,         1,          0,            0,         0,      0   },
/*nullableString*/ {         0,        0,       0,        0,         0,          1,            0,         0,      0   },
/*nullableFloat*/  {         0,        0,       0,        0,         0,          0,            1,         0,      0   },
/*nnull*/          {         0,        0,       0,        0,         1,          1,            1,         1,      0   },
/*typeBool*/       {         1,        0,       1,        0,         1,          0,            1,         1,      1   }
};
bool types_match(int type_src, int type_dest){
    return (bool)typeMatches[typeMatchesKey(type_src)][typeMatchesKey(type_dest)];
}

int getExpressionDataType(treeNode* root){
    if(root == NULL) return 0;

    if(expressionTree_token_is_literal(root->item->token)){
        int tokenTypeThis = root->item->token->tokenType;
        switch (tokenTypeThis)
        {
            case integer: return typeInt;
            case ffloat: return typeFloat;
            case sstring: return typeString;
            
            default: return typeInt;
        }
        
    
       return root->item->token->tokenType;
    } 
    else if (root->item->token->tokenType == variable){
        return Symtable_GetType(currVariablesTable, root->item->token->value->string);
    }
    else if (expressionTree_token_is_operator(root->item->token)){
        int tokenTypeThis = root->item->token->tokenType;

        int dataTypeRight = getExpressionDataType(root->right);
        int dataTypeLeft = getExpressionDataType(root->left);
    
        if(tokenTypeThis == ddiv){
            if(expressionTree_generalType(dataTypeRight) == numeric && expressionTree_generalType(dataTypeLeft) == numeric){
                return typeFloat;

            } else exit(7);

        } else if(tokenTypeThis == add || tokenTypeThis == sub || tokenTypeThis == mul){
            if(expressionTree_generalType(dataTypeRight) == numeric && expressionTree_generalType(dataTypeLeft) == numeric){
                return dataTypeRight == ffloat || dataTypeLeft == ffloat || dataTypeRight == typeFloat || dataTypeLeft == typeFloat ?
                    typeFloat : typeInt;

            } else exit(7);

        } else if(tokenTypeThis == concat){
            if(expressionTree_generalType(dataTypeRight) == string && expressionTree_generalType(dataTypeLeft) == string){
                return typeString;
                
            } else exit(7);

        }/*  else if(tokenTypeThis == cmpEqual || tokenTypeThis == notEquals || tokenTypeThis == greater || tokenTypeThis == lower || tokenTypeThis == greaterEqual || tokenTypeThis == lowerEqual){
            if(expressionTree_generalType(dataTypeRight) == string && expressionTree_generalType(dataTypeLeft) == string){
                return string;
                
            } else exit(7);
        } */ else return typeBool;
    }
}