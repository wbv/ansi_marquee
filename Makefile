CC=gcc
CC_FLAGS=-Wall -pedantic -O2

all:
	$(CC) $(CC_FLAGS) hd15.c -o hackduke

