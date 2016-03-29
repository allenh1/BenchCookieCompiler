cc = gcc -g
CC = g++ --std=c++14 -g

LEX=lex
YACC=yacc -g

all: bcc

lex.yy.o: syntax.l
	$(LEX) syntax.l
	$(CC) -c lex.yy.c

y.tab.o: syntax.y
	$(YACC) -d syntax.y
	$(CC) -c y.tab.c

command.o: command.cc arm_compiler.cpp x86_64_compiler.cpp
	$(CC) -c command.cc

bcc: y.tab.o lex.yy.o command.o
	$(CC) -o bcc lex.yy.o y.tab.o command.o -lfl

install:
	cp ./bcc /usr/bin/bcc

clean:
	rm -f lex.yy.c y.tab.c y.tab.h bcc *.o *~ y.output
