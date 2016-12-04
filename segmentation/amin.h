#ifndef AMIN_H
#define AMIN_H
# include "../neuralnet/list.h"
#include <err.h>
//#include "../damien/filtres.h"
//#include "amin.c"
struct coord
{
  int xmin;
  int xmax;
  int ymin;
  int ymax;
};

struct matrice{
  int height;
  int width;
  int *mat;
};

void Seg_char(struct matrice *matrix, List *list2);

#endif
