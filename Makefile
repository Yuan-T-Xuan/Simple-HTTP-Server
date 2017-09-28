CC=gcc
CFLAGS=-I.
DEPS = parse.h y.tab.h logging.h
OBJ = y.tab.o lex.yy.o parse.o logging.o server.o
FLAGS = -g -Wall

default:all

all: server

lex.yy.c: lexer.l
	flex $^

y.tab.c: parser.y
	yacc -d $^

%.o: %.c $(DEPS)
	$(CC) $(FLAGS) -c -o $@ $< $(CFLAGS)

server: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *~ *.o server lex.yy.c y.tab.c y.tab.h
