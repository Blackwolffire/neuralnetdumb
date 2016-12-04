#ifndef AMIN_H
#define AMIN_H
# include "../neuralnet/list.h"
#include <err.h>
#include "filtres.h"
struct coord
{
  int xmin;
  int xmax;
  int ymin;
  int ymax;
};


void Seg_char(struct matrice *matrix, List* list2);

#endif
