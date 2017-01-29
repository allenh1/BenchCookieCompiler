all: compiler

integer.o: integer.c integer.h
	gcc -c integer.c integer.h -o integer.o

float.o: float.c float.h
	gcc -c float.c float.h -o float.o

identifier.o: identifier.c identifier.h
	gcc -c identifier.c identifier.h

tokenizer.o: tokens.c tokens.h integer.o float.o identifier.o
	gcc -c tokens.c integer.o float.o identifier.o -o tokenizer.o

compiler: tokenizer.o
	gcc main.c tokenizer.o -o tokenizer

clean:
	rm -f *~ *.o tokenizer
