#ifndef FILTRES_H
#define FILTRES_H
# include <gdk/gdk.h>
//# include "filtres.c"
# include "../neuralnet/list.h"
# include "../segmentation/amin.h"
# include "../fixme/nicolas.h"



/*
struct matrice{
  int height;
  int width;
  int *mat;
};
*/

void to_grey(GdkPixbuf *pb);
void Thresholding(GdkPixbuf *pb);
void GdkToMat(struct matrice *matr, GdkPixbuf *pb);
void MatToGdk(struct matrice *matr, GdkPixbuf *pb);
struct matrice *transfo(unsigned int x1, unsigned int x2, unsigned int y1,
												unsigned int y2, GdkPixbuf *pb);
List *forAmin(GdkPixbuf *pb, List *coor);
void PrintCoord(GdkPixbuf *pb);
void segmentation(GdkPixbuf *pb);
void Amin(GdkPixbuf *pb, const char *filename);
void Nicolas(GdkPixbuf *pb);
GdkPixbuf *lowgrade(GdkPixbuf *pb);
GdkPixbuf *upgrade(GdkPixbuf *pb);
GdkPixbuf *resize(GdkPixbuf *pb);
GdkPixbuf *same(GdkPixbuf *pb);

#endif

