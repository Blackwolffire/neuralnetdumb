CC=gcc
 
CPPFLAGS=
CFLAGS= -Wall -Wextra -Werror -std=c99 -g #-O3
LDFLAGS=
LDLIBS= -lm `pck-config --libs sdl` -lSDL_image
 
SRC=  main.c
OBJ= ${SRC:.c=.o}
 
all: main
 
main: ${OBJ}
 
clean:
					rm -f *~ *.o
					rm -f main
# END
