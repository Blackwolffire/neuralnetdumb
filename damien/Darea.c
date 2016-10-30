#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

struct {
  double r; 
  double g;
  double b;
  double a; 
  //r = 0;
  //g = 0;
  //b = 0;
  //a = 1;
  double r_fond; 
  double g_fond;
  double b_fond; 
  double a_fond;
  //r_fond = 1;
  //g_fond = 1;
  //b_fond = 1;
  //a_fond = 1;
  int j;
  int plein;
  int imp;
  //j = 16;
  //plein = 0;
  //imp = 0;
  GtkWidget *p_da;
} tout;


static cairo_surface_t *surface = NULL;


void set_r(double x)
{
  tout.r = x;
}
void set_g(double x)
{
  tout.g = x;
}
void set_b(double x)
{
  tout.b = x;
}
void set_a(double x)
{
  tout.a = x;
}
void set_j(int x)
{
  tout.j = x;
}
void set_noir()
{
tout.r = 0;
tout.g = 0;
tout.b = 0;   
}
void set_red()
{
tout.r = 237.0/255.0;
tout.g = 28.0/255.0;
tout.b = 36.0/255.0;   
}
void set_jaune()
{
tout.r = 255.0/255.0;
tout.g = 242.0/255.0;
tout.b = 0.0/255.0;   
}
void set_vert()
{
tout.r = 34.0/255.0;
tout.g = 177.0/255.0;
tout.b = 76.0/255.0;   
}
void set_bleu()
{
tout.r = 63.0/255.0;
tout.g = 72.0/255.0;
tout.b = 204.0/255.0;   
}
void set_color(double x, double y, double z, double c)
{
tout.r = x;
tout.g = y;
tout.b = z;
tout.a = c;   
}
void set_plein(int x)
{
tout.plein = x;   
}
int get_plein()
{
return tout.plein;
}




void SetChange();



void SetSurface (char *path){
  surface = cairo_image_surface_create_from_png(path);
  gtk_widget_queue_draw_area (tout.p_da, 0, 0, 2000, 2000);
}


static void
clear_surface (void)
{
  cairo_t *cr;

  cr = cairo_create (surface);

  cairo_set_source_rgba (cr, 1, 1, 1, 1);
  cairo_paint (cr);
			char nom[30];
      strcpy(nom, ".png.tmp");
      char nb[5];
      sprintf(nb, "%d", tout.imp);
  	  strcat(nom, nb);
  	  cairo_surface_write_to_png(surface,nom);
  	  tout.imp++;

  cairo_destroy (cr);
}


static gboolean
configure_event_cb (GtkWidget         *widget,
            GdkEventConfigure *event,
            gpointer           data)
{
  if (surface)
    cairo_surface_destroy (surface);

  surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
                                       CAIRO_CONTENT_COLOR,
                                       gtk_widget_get_allocated_width (widget),
                                       gtk_widget_get_allocated_height (widget));


  clear_surface ();
  (void)event;
  (void)data;

  return TRUE;
}


static gboolean
draw_cb (GtkWidget *widget,
 cairo_t   *cr,
 gpointer   data)
{
  cairo_set_source_surface (cr, surface, 0, 0);
  cairo_paint (cr);

  (void)widget;
  (void)data;

  return FALSE;
}

