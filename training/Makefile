CC=gcc
 
CPPFLAGS=
CFLAGS= -Wall -Wextra -Werror -std=c99 -lm -g #-O3
LDFLAGS=
LDLIBS=  `pkg-config --libs gtk+-3.0`
 
SRC=  main.c training.c ../neuralnet/list.c ../neuralnet/neural_net.c ../damien/filtres.c ../damien/sauvegarde.c ../segmentation/amin.c
OBJ= ${SRC:.c=.o}
 
all: main
 
main: ${OBJ}
 
clean:
					rm -f *~ *.o
					rm -f main
# END
