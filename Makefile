CC=gcc

CFLAGS=-Wall
LFLAGS=-lm -lGL -lglut #-lnetpbm

EXEC=connexes
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

$(EXEC) : $(OBJS)
	$(CC) $^ -o $@ $(LFLAGS)

%.o : %.c
	$(CC) -c $^ -o $@ $(CFLAGS)

archive :
	tar -zcvf Projet_Isabelle_Muller.tar.gz *
	
clean :
	/bin/rm $(EXEC) *.o
	rm -fr Documentation.bak
	
doc :
	mkdir -p Documentation
	doxygen -g Documentation *.h $(SRCS)*
	doxygen Documentation
	mv Documentation ./Documentation.bak/
	mv html ./Documentation.bak/
	mv latex ./Documentation.bak/

gdb :
	gdb -q ./connexes
	
geany :
	# geany *.h *.c &
	geany Polygon.* main.c
	
run :
	./connexes 800 600
	
valgrind :
	valgrind --tool=memcheck --leak-check=full --track-origins=yes --show-reachable=yes -v ./connexes 800 600
