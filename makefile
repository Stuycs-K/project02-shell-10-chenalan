.PHONY: run clean compile

compile shell: main.o
	@gcc -o shell main.o

run: shell
	@./shell

clean:
	@rm -f *.o shell

main.o: main.c
	@gcc -c main.c
