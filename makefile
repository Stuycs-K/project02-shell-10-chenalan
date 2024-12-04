.PHONY: run clean compile

compile shell: main.o parse.o exec.o cd.o redirection.o shell.o command.o
	@gcc -o shell main.o parse.o exec.o cd.o redirection.o shell.o command.o

run: shell
	@./shell

clean:
	@rm -f *.o shell

main.o: main.c
	@gcc -c main.c

parse.o: parse.c
	@gcc -c parse.c

exec.o: exec.c cd.h
	@gcc -c exec.c

cd.o: cd.c
	@gcc -c cd.c

redirection.o: redirection.c
	@gcc -c redirection.c

shell.o: shell.c
	@gcc -c shell.c

command.o: command.c
	@gcc -c command.c
