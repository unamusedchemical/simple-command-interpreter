all: main.o tokenizer.o
	gcc -o commandInterpreter main.o tokenizer.o

main.o: main.c
	gcc -c main.c

tokenizer.o: tokenizer.c
	gcc -c tokenizer.c

clean:
	rm -f *.o

run:
# test is the name of the file that contains the commands
	./commandInterpreter test 