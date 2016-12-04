#ifndef FILTRES_H
#define FILTRES_H
# include "filtres.c"


void to_grey(GdkPixbuf *pb);
void Thresholding(GdkPixbuf *pb);
struct matrice *GdkToMat(GdkPixbuf *pb);
void MatToGdk(struct matrice *matr, GdkPixbuf *pb);
struct matrice *transfo(unsigned int x1, unsigned int x2, unsigned int y1, unsigned int y2, GdkPixbuf *pb);
List *forAmin(GdkPixbuf *pb, List *coor);
GdkPixbuf *lowgrade(GdkPixbuf *pb);
GdkPixbuf *upgrade(GdkPixbuf *pb);
GdkPixbuf *resize(GdkPixbuf *pb);

#endif

