.PHONY: run clean compile

compile shell: main.o input.o exec.o cd.o redirection.o shell.o
	@gcc -o shell main.o input.o exec.o cd.o redirection.o shell.o

run: shell
	@./shell

clean:
	@rm -f *.o shell

main.o: main.c
	@gcc -c main.c

input.o: input.c
	@gcc -c input.c

exec.o: exec.c cd.h
	@gcc -c exec.c

cd.o: cd.c
	@gcc -c cd.c

redirection.o: redirection.c
	@gcc -c redirection.c

shell.o: shell.c
	@gcc -c shell.c
