
#	include <gtk-3.0/gtk/gtk.h>
#	include "sauvegarde.c"

struct data {
	GtkWidget *window;
	GdkPixbuf *pixbuf;
};

void loadImage (GtkButton *button, GtkWidget *window);
void SaveImage (GtkButton *button, GtkWidget *window);
void SetChange ();

