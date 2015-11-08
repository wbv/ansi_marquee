CC=gcc
CC_FLAGS=-Wall -pedantic -O2 --std=c11

all:
	$(CC) $(CC_FLAGS) main.c -o ansi
