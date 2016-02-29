#
# CS252 - Shell Project
#
#Use GNU compiler
cc = gcc -g
CC = g++ --std=c++14 -g

LEX=lex
YACC=yacc

all: bcc cat_grep

lex.yy.o: syntax.l
	$(LEX) syntax.l
	$(CC) -c lex.yy.c

y.tab.o: syntax.y
	$(YACC) -d syntax.y
	$(CC) -c y.tab.c

command.o: command.cc
	$(CC) -c command.cc

bcc: y.tab.o lex.yy.o command.o
	$(CC) -o bcc lex.yy.o y.tab.o command.o -lfl

clean:
	rm -f lex.yy.c y.tab.c y.tab.h bcc *.o
