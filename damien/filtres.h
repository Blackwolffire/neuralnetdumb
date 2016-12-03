# include "filtres.c"

void to_grey(GdkPixbuf *pb);
void Thresholding(GdkPixbuf *pb);
struct matrice *GdkToMat(GdkPixbuf *pb);
void MatToGdk(struct matrice *matr, GdkPixbuf *pb);
GdkPixbuf *lowgrade(GdkPixbuf *pb);
GdkPixbuf *upgrade(GdkPixbuf *pb);
GdkPixbuf *resize(GdkPixbuf *pb);

