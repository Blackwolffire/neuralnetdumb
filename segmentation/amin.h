#ifndef AMIN_H
#define AMIN_H
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include "../neuralnet/list.h"

struct coord
{
  int xmin;
  int xmax;
  int ymin;
  int ymax;
};


void init_sdl(void);
SDL_Surface *screen();
List *Seg_char(SDL_Surface *img);

#endif
