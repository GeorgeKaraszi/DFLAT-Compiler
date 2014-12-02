#Author: George Karaszi
#lm = Math library. Fits standard C with asin switch turned on
#lfl = flex library. Fits standard C with asin switch turned on

CC = gcc
CFLAGS = -lm -lfl -ansi
PFLAGS = -pedantic
OBJECTS = valueHandler.c ht_funcmgr.c RunFunction.c bison.tab.c lex.yy.c
REQOBJECTS = lex.yy.c bison.tab.c bison.tab.h

expression: $(REQOBJECTS) codegen.o RunFunction.o
	$(CC) $(PFLAGS) $(OBJECTS) $(CFLAGS) -o expression.out


valueHandler.o: valueHandler.c
	gcc -c $(PFLAGS) valueHandler.c $(CFLAGS) -o valueHandler.o

ht_funcmgr.o: valueHandler.o ht_funcmgr.c
	gcc -c $(PFLAGS) ht_funcmgr.c $(CFLAGS) -o ht_funcmgr.o

codegen.o: ht_funcmgr.o CG.c
	gcc -c $(PFLAGS) CG.c $(CFLAGS) -o codegen.o

RunFunction.o: valueHandler.o ht_funcmgr.o RunFunction.c
	gcc -c $(PFLAGS) RunFunction.c $(CFLAGS) -o RunFunction.o

bison.tab.c bison.tab.h: bison.y
	bison --debug --verbose -d bison.y

lex.yy.c: lex.l bison.tab.h
	flex lex.l

clean:
	rm -f $(REQOBJECTS) *.out *.o *.output