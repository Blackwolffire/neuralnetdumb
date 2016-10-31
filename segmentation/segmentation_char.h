# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include <err.h>
# include "quartet.h"
# include "../neuralnet/list.h"
# include "pixel_operations.h"
# ifndef _SEGMENTATION_CHAR_H_
# define _SEGMENTATION_CHAR_H_
void wait_for_keypressed(void)!
void SegLine(SDL_Surface *img, int* matrix, int w, int h, List* list);
void SegLine_to_char(SDL_Surface *img,  int w,int* matrix, List* list);
void print_Mat(int *matrix,int h, int w);
void init_sdl(void);
SDL_Surface* load_image(char *path);
SDL_Surface* display_image(SDL_Surface *img);
SDL_Surface *Seg_char(SDL_Surface *img);
SDL_Surface *Segmentation(int argc, char *argv[]);
