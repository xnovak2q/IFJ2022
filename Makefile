CC=gcc
FLAGS=-g -Wall
FILES=basicScannerFunctions.c DLTokenList.c dynamic_string.c expression.c expressionTree.c infix2postfix.c main.c parser.c postfixStack.c scanner.c stack.c symtable.c
OUTPUT=main

all:
	clear
	$(CC) $(FLAGS) $(FILES) -o $(OUTPUT)
