.PHONY: run clean compile

compile shell: main.o input.o exec.o
	@gcc -o shell main.o input.o exec.o

run: shell
	@./shell

clean:
	@rm -f *.o shell

main.o: main.c input.h
	@gcc -c main.c

input.o: input.c
	@gcc -c input.c

exec.o: exec.c
	@gcc -c exec.c
