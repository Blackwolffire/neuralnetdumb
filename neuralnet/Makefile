CC=gcc
 
CPPFLAGS=
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= -lm
 
SRC=  main.c neural_net.c list.c
OBJ= ${SRC:.c=.o}
 
all: main
 
main: ${OBJ}
 
clean:
					rm -f *~ *.o
					rm -f main
