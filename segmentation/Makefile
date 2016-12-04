## ON ESSAIE DE FAIRE UN MAKEFILE

CC=gcc
 
CPPFLAGS= `pkg-config --cflags sdl`
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image
 
SRC=  amin.c pixel_operations.c ../neuralnet/list.c
OBJ= ${SRC:.c=.o}
 
all: 
 
amin: ${OBJ}
 
clean:
					rm -f *~ *.o
					rm -f amin
# END
