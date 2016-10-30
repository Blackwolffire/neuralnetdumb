## ON ESSAIE DE FAIRE UN MAKEFILE

CC=gcc
 
CPPFLAGS= `pkg-config --cflags sdl`
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image
 
SRC=  pixel_operations.c segment_char.c
OBJ= ${SRC:.c=.o}
 
all: segment_char
 
main: ${OBJ}
 
clean:
					rm -f *~ *.o
					rm -f main
# END
