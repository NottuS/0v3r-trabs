########################## O Makefile principal ##########################"
CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -lpthread
EXEC=otimiza

all: $(EXEC)

otimiza: otimiza.c
	@$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean mrproper
clean:
	@rm -rf *.o *~
mrproper: clean
	@rm -rf $(EXEC)

