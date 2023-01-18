all: main.o tokenizer.o
	gcc -o commandInterpreter main.o tokenizer.o

main.o: main.c
	gcc -c main.c

tokenizer.o: tokenizer.c
	gcc -c tokenizer.c

clean:
	rm -f *.o

run:
	./commandInterpreter test