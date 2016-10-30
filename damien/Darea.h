#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include "Darea.c"



static void clear_surface (void);
void plus_taille();
void moins_taille(void);
void set_r(double x);
void set_g(double x);
void set_b(double x);
void set_a(double x);
void set_j(int x);
void set_color(double x, double y, double z, double c);
void set_plein(int x);
int get_plein();

static gboolean
configure_event_cb (GtkWidget         *widget,
            GdkEventConfigure *event,
            gpointer           data);

void SetSurface (char *path);

static gboolean draw_cb (GtkWidget *widget, cairo_t   *cr, gpointer   data);

