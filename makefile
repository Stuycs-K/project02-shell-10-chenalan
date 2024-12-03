.PHONY: run clean compile

compile shell: main.o input.o exec.o cd.o
	@gcc -o shell main.o input.o exec.o cd.o

run: shell
	@./shell

clean:
	@rm -f *.o shell

main.o: main.c input.h
	@gcc -c main.c

input.o: input.c
	@gcc -c input.c

exec.o: exec.c cd.h
	@gcc -c exec.c

cd.o: cd.c
	@gcc -c cd.c
