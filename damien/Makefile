# Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags --libs gtk+-3.0`
CFLAGS= -Wall -Wextra -Werror -std=c99 -pedantic -lm -g #-O3
LDFLAGS=
LDLIBS= `pkg-config  --libs gtk+-3.0`

SRC= Interface.c filtres.c ../neuralnet/list.c ../segmentation/amin.c
OBJ= ${SRC:.c=.o}

all: Interface

Interface:
	${CC} ${SRC} -o Amganida ${CFLAGS} ${CPPFLAGS}
	
clean:
	rm -f *~ *.o
	rm -f Amganida .png.tmp*

# END
