## ON ESSAIE DE FAIRE UN MAKEFILE

CC=gcc
 
CPPFLAGS= `pkg-config --cflags gtk+-3.0`
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs gtk+-3.0` 
 
SRC=  amin.c filtres.c ../neuralnet/list.c
OBJ= ${SRC:.c=.o}
 
all: 
 
amin: ${OBJ}
 
clean:
					rm -f *~ *.o
					rm -f amin
# END
