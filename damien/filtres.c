
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
#include <assert.h>
# include <err.h>
//# include <gdk/gdk.h>
# include "filtres.h"

/*
struct matrice{
  int height;
  int width;
  int *mat;
};
*/

void to_grey(GdkPixbuf *pb)
{
  int width, height, rowstride, n_channels;
  int i, j;
  guchar *pixels, *p, grey_p;
  

  n_channels = gdk_pixbuf_get_n_channels (pb);

  g_assert (gdk_pixbuf_get_colorspace (pb) == GDK_COLORSPACE_RGB);
  g_assert (gdk_pixbuf_get_bits_per_sample (pb) == 8);

  width = gdk_pixbuf_get_width (pb);
  height = gdk_pixbuf_get_height (pb);

  rowstride = gdk_pixbuf_get_rowstride (pb);
  pixels = gdk_pixbuf_get_pixels (pb);

  for(i = 0; i < height; i++)
  {
    for(j = 0; j < width; j++)
    {
      p = pixels + i * rowstride + j * n_channels;
      grey_p = 0.3*p[0] + 0.59*p[1] + 0.11*p[2];
      p[0] = grey_p;
      p[1] = grey_p;
      p[2] = grey_p; 
    }
  }
}

void Thresholding(GdkPixbuf *pb)
{
  int width, height, rowstride, n_channels;
  int i, j;
  guchar *pixels, *p;
  
  n_channels = gdk_pixbuf_get_n_channels (pb);
  
  g_assert (gdk_pixbuf_get_colorspace (pb) == GDK_COLORSPACE_RGB);
  g_assert (gdk_pixbuf_get_bits_per_sample (pb) == 8);

  width = gdk_pixbuf_get_width (pb);
  height = gdk_pixbuf_get_height (pb);

  rowstride = gdk_pixbuf_get_rowstride (pb);
  pixels = gdk_pixbuf_get_pixels (pb);

  for (i = 0; i < height; i++)
  {
    for (j = 0; j < width; j++)
    {
      p = pixels + i * rowstride + j * n_channels;
      if ((p[0] + p[1] + p[2]) > 390)
      {
        p[0]= 255;
        p[1] = 255;
        p[2] = 255;
      }
      else
      {
        p[0] = 0;
        p[1] = 0;
        p[2] = 0;
      }
    }
  }
}



 

void GdkToMat(struct matrice *matr, GdkPixbuf *pb)
{
  int width, height, rowstride, n_channels;
  int i, j;
  guchar *pixels, *p;

  n_channels = gdk_pixbuf_get_n_channels (pb);

  g_assert (gdk_pixbuf_get_colorspace (pb) == GDK_COLORSPACE_RGB);
  g_assert (gdk_pixbuf_get_bits_per_sample (pb) == 8);

  width = gdk_pixbuf_get_width (pb);
  height = gdk_pixbuf_get_height (pb);

  rowstride = gdk_pixbuf_get_rowstride (pb);
  pixels = gdk_pixbuf_get_pixels (pb);
  
  matr->width = width;
  matr->height = height;

printf("%d %d\n", width, height);

  for (i = 0; i < height; i++)
  {
    for (j = 0; j < width; j++)
    {
      p = pixels + i * rowstride + j * n_channels;
      if (p[0] == 255)
        matr->mat[i * width + j] = 1;
      else
        matr->mat[i * width + j] = 0;
    }
  }
}



void MatToGdk(struct matrice *matr,GdkPixbuf *pb)
{
  int width, height, rowstride, n_channels;
  int i, j;
  guchar *pixels, *p;

  n_channels = gdk_pixbuf_get_n_channels (pb);

  g_assert (gdk_pixbuf_get_colorspace (pb) == GDK_COLORSPACE_RGB);
  g_assert (gdk_pixbuf_get_bits_per_sample (pb) == 8);

  width = gdk_pixbuf_get_width (pb);
  height = gdk_pixbuf_get_height (pb);

  rowstride = gdk_pixbuf_get_rowstride (pb);
  pixels = gdk_pixbuf_get_pixels (pb);

  for (i = 0; i < height; i++)
  {
    for (j = 0; j < width; j++)
    {
      p = pixels + i * rowstride + j * n_channels;
      if (matr->mat[i * width + j] == 1)
      {
        p[0]= 255;
        p[1] = 255;
        p[2] = 255;
      }
      else
      {
        p[0] = 0;
        p[1] = 0;
        p[2] = 0;
      }  
    }
  }
}



struct matrice *transfo(unsigned int x1, unsigned int x2, unsigned int y1,
												unsigned int y2, GdkPixbuf *pb)
{
  int width,  rowstride, n_channels;
  unsigned int i, j;
  guchar *pixels, *p;
  
  struct matrice *matr = malloc(sizeof(struct matrice));

  width = gdk_pixbuf_get_width (pb);
  int height = gdk_pixbuf_get_height (pb);
	matr->mat = malloc(sizeof(int) * width * height);

  n_channels = gdk_pixbuf_get_n_channels (pb);
  
  g_assert (gdk_pixbuf_get_colorspace (pb) == GDK_COLORSPACE_RGB);
  g_assert (gdk_pixbuf_get_bits_per_sample (pb) == 8);
  
  //height = gdk_pixbuf_get_height (pb);
  
  matr->height = x2 - x1;
  matr->width = y2 - y1;

  rowstride = gdk_pixbuf_get_rowstride (pb);
  pixels = gdk_pixbuf_get_pixels (pb);
  
  for (i = y1; i < y2; i++)
  { 
    for (j = x1; j < x2; j++)
    { 
      p = pixels + i * rowstride + j * n_channels;
      matr->mat[(i - y1) * width + (j - x1)] = p[0];
    }
  }
  return(matr);
}


List *forAmin (GdkPixbuf *pb, List *coor)
{
  List *matrices = createList();
  struct coord *coordonnees = malloc(sizeof(struct coord));
  struct matrice *matr = malloc(sizeof(struct matrice));

  int width = gdk_pixbuf_get_width (pb);
  int height = gdk_pixbuf_get_height (pb);
	matr->mat = malloc(sizeof(int) * width * height);

  unsigned int i = 0;
  unsigned int x1, x2, y1, y2;
  while (i < coor->len)
  {
    coordonnees = getDataList(coor ,i);
    x1 = coordonnees->xmin;
    x2 = coordonnees->xmax;
    y1 = coordonnees->ymin;
    y2 = coordonnees->ymax;
    matr = transfo(x1, x2, y1, y2, pb);
    insertList(matrices, matr, i);
    i++;
  }
  free(matr->mat);
  free(matr);
  free(coordonnees);
  return(matrices);
}


void PrintCoordFunc(GdkPixbuf *pb, List *coor)
{
  struct coord *coordonnees = malloc(sizeof(struct coord));
  struct matrice *matr = malloc(sizeof(struct matrice));
  int width,  rowstride, n_channels;
  guchar *p, *pixels;

  g_assert (gdk_pixbuf_get_colorspace (pb) == GDK_COLORSPACE_RGB);
  g_assert (gdk_pixbuf_get_bits_per_sample (pb) == 8);

  rowstride = gdk_pixbuf_get_rowstride (pb);
  pixels = gdk_pixbuf_get_pixels (pb); 
  n_channels = gdk_pixbuf_get_n_channels (pb);


  width = gdk_pixbuf_get_width (pb);
  int height = gdk_pixbuf_get_height (pb);
	matr->mat = malloc(sizeof(int) * width * height);
  printf("popopopo\n");
  unsigned int i = 0;
  unsigned int x,y;
  unsigned int x1, x2, y1, y2;
  while (i < coor->len)
  {
    printf("poop\n");
    coordonnees = getDataList(coor ,i);
    x1 = coordonnees->xmin;
    x2 = coordonnees->xmax;
    y1 = coordonnees->ymin;
    y2 = coordonnees->ymax;
    printf("%d %d %d %d \n",x1,x2,y1,y2);
    for (y = y1; y <= y2; y++)
    {
      for (x = x1; x <= x2; x++)
      {
        p = pixels + y * rowstride + x * n_channels;
				if ((x==x1) || (x==x2) || (y == y1) || (y == y2))
        {	
					p[0] = 0;
        	p[1] = 255;
        	p[2] = 0;
				}
      }
    }
    i++;
  }
  free(matr->mat);
  free(matr);
  free(coordonnees);
}



void PrintCoord(GdkPixbuf *pb)
{
  struct matrice *matr = malloc(sizeof(struct matrice));
  List *liste = createList();
  printf("ca\n");
  int width = gdk_pixbuf_get_width (pb);
  int height = gdk_pixbuf_get_height (pb);
	matr->mat = malloc(sizeof(int) * width * height);
  printf("cb\n");
  GdkToMat(matr, pb);
  printf("cc\n");
  Seg_char(matr,liste);
  printList(liste);
  printf("cd\n");

  PrintCoordFunc(pb,liste);
  printf("ce\n");
  free(matr->mat);
  free(matr);
}



void segmentation(GdkPixbuf *pb)
{
  struct matrice *matr = malloc(sizeof(struct matrice));
  List *liste = createList();

  int width = gdk_pixbuf_get_width (pb);
  int height = gdk_pixbuf_get_height (pb);
	matr->mat = malloc(sizeof(int) * width * height);

  GdkToMat(matr, pb);
  Seg_char(matr,liste);
  MatToGdk(matr,pb);
  free(matr->mat);
  free(matr);
}

List *Amin(GdkPixbuf *pb)
{
  struct matrice *matr = malloc(sizeof(struct matrice));
  List *liste = createList();

  int width = gdk_pixbuf_get_width (pb);
  int height = gdk_pixbuf_get_height (pb);
	matr->mat = malloc(sizeof(int) * width * height);
  List *res = createList();


  GdkToMat(matr, pb);
  Seg_char(matr,liste);
  MatToGdk(matr,pb);
  free(matr->mat);
  free(matr);
  res = forAmin(pb,liste);
  return(res);
}




GdkPixbuf *lowgrade(GdkPixbuf *pb)
{
  GdkPixbuf *mini = NULL;
  mini = gdk_pixbuf_scale_simple (pb,
  				  gdk_pixbuf_get_width(pb)/2,
				  gdk_pixbuf_get_height(pb)/2,
				  GDK_INTERP_HYPER);
  return(mini);
}



GdkPixbuf *upgrade(GdkPixbuf *pb)
{
  GdkPixbuf *maxi = NULL;
  maxi = gdk_pixbuf_scale_simple (pb,
                                  gdk_pixbuf_get_width(pb)*2,
				  gdk_pixbuf_get_height(pb)*2,
				  GDK_INTERP_HYPER);
  return(maxi);
}

GdkPixbuf *resize(GdkPixbuf *pb)
{
  GdkPixbuf *resized = NULL;
  resized = gdk_pixbuf_scale_simple (pb,
  				  16,
				  16,
				  GDK_INTERP_HYPER);
  return(resized);
}

