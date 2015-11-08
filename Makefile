CC=gcc
CC_FLAGS=-Wall -pedantic -O2

all:
$(CC) $(CC_FLAGS) main.c -o ansi
