CC=gcc
 
CPPFLAGS= `pkg-config --cflags --libs gtk+-3.0`
CFLAGS= -Wall -Wextra -Werror -std=c99 -lm -g #-O3
LDFLAGS=
LDLIBS=  `pkg-config --libs gtk+-3.0`
 
SRC=  main.c training.c ../neuralnet/list.c ../neuralnet/neural_net.c ../damien/filtres.c ../segmentation/amin.c
OBJ= ${SRC:.c=.o}
 
all: main
 
main:
	${CC} ${SRC} -o main ${CFLAGS} ${CPPFLAGS}
 
clean:
					rm -f ${OBJ}
					rm -f main
