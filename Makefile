CC = gcc
all: 
	gcc -Wall rtesNoTimestamps.c -o rtesNoTimestamps
	gcc -Wall rtesTimestamps.c -o rtesTimestamps