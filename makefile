# Name of the main executable:
MAIN = assembler
#compiler we'll be using:
CC = gcc
#flags we'll be using to compile:
CFLAGS = -ansi -Wall -pedantic
#source files:
SRCS = assembler.c firstPass.c secondPass.c symbolTable.c assembly_tables.c inputFileHandle.c memoryWord.c outputFiles.c preAssembler.c
#object files:
OBJS = $(SRCS: .c=.o)
all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(MAIN)

assembler.o: assembler.c assembler.h
	$(CC) $(CFLAGS) -c assembler.c

firstPass.o: firstPass.c firstPass.h
	$(CC) $(CFLAGS) -c firstPass.c

secondPass.o: secondPass.c secondPass.h
	$(CC) $(CFLAGS) -c secondPass.c

symbolTable.o: symbolTable.c symbolTable.h
	$(CC) $(CFLAGS) -c symbolTable.c

assembly_tables.o: assembly_tables.c assembly_tables.h
	$(CC) $(CFLAGS) -c assembly_tables.c

inputFileHandle.o: inputFileHandle.c inputFileHandle.h
	$(CC) $(CFLAGS) -c inputFileHandle.c

memoryWord.o: memoryWord.c memoryWord.h
	$(CC) $(CFLAGS) -c memoryWord.c

outputFiles.o: outputFiles.c outputFiles.h
	$(CC) $(CFLAGS) -c outputFiles.c

preAssembler.o: preAssembler.c preAssembler.h
	$(CC) $(CFLAGS) -c preAssembler.c
clean:
	rm -f *.o $(MAIN)

