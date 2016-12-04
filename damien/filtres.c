# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <err.h>
# include <gdk/gdk.h>
# include "../neuralnet/list.h"
# include "../segmentation/amin.h"

struct matrice{
  int height;
  int width;
  int *mat;
};




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




struct matrice *GdkToMat(GdkPixbuf *pb)
{
  int width, height, rowstride, n_channels;
  int i, j;
  struct matrice *matr = malloc(sizeof(struct matrice));
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
  return(matr);
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
  free(matr);
}



struct matrice *transfo(unsigned int x1, unsigned int x2, unsigned int y1, unsigned int y2, GdkPixbuf *pb)
{
  int width,  rowstride, n_channels;
  unsigned int i, j;
  guchar *pixels, *p;
  
  struct matrice *matr = malloc(sizeof(struct matrice));

  n_channels = gdk_pixbuf_get_n_channels (pb);
  
  g_assert (gdk_pixbuf_get_colorspace (pb) == GDK_COLORSPACE_RGB);
  g_assert (gdk_pixbuf_get_bits_per_sample (pb) == 8);
  
  width = gdk_pixbuf_get_width (pb);
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
  free(matr);
  free(coordonnees);
  return(matrices);
}


GdkPixbuf *segmentation(GdkPixbuf *pb)
{
  struct matrice *matr = malloc(sizeof(struct matrice));
  matr = GdkToMat(pb);
  




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

