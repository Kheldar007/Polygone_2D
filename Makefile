

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

clean :
	/bin/rm $(EXEC) *.o
	
geany :
	geany *.h *.c &
	
run :
	./connexes 800 600
