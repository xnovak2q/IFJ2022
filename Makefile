CC=gcc
FLAGS=-g -Wall
FILE=main

all:
	$(CC) $(FLAGS) $(FILE).c -o $(FILE)
