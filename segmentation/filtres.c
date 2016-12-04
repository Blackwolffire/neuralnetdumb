# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <err.h>
# include <gdk/gdk.h>


struct matrice{
  int width;
  int height;
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

