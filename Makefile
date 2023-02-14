all: compile
	./exec

compile: main.c hashmap.c utils.c code.c
	gcc -o exec main.c hashmap.c utils.c code.c