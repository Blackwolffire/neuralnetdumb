# include <gtk-3.0/gtk/gtk.h>
# include <stdlib.h>
# include <stdio.h>
# include <dirent.h>
# include <string.h>
# include <gdk/gdk.h>
# include <math.h>
# include "Darea.h"
# include "sauvegarde.h"
# include "filtres.h"

//****************************FONCTION POUR CALLBACK BUTTON********************
void popup(GtkWidget *widget)
{
  GdkRGBA color;
  int res;
  GtkColorChooserDialog *dialog;
  double r, g, b, a;

  color.red = 1;
  color.green = 1;
  color.blue = 1;
  color.alpha = 1;

  dialog = GTK_COLOR_CHOOSER_DIALOG
    (gtk_color_chooser_dialog_new("color chooser", NULL));

  gtk_color_chooser_set_use_alpha(GTK_COLOR_CHOOSER(dialog), TRUE);

  gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(dialog), &color);

  res = gtk_dialog_run(GTK_DIALOG(dialog));

  if (res == GTK_RESPONSE_OK) {
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(dialog), &color);
    r = color.red;
    g = color.green;
    b = color.blue;
    a = color.alpha;
    set_color(r, g, b, a);
  }

  gtk_widget_destroy(GTK_WIDGET(dialog));
  (void) widget;
}

void actualiser (GtkWidget *widget)
{
  cairo_t *cr;
  cr = cairo_create (surface);
  cairo_set_source_rgba(cr, 1, 1, 1, tout.a);
  double x,y,radius,a,b;
  x = 0;
  y = 0;
  a = 0;
  b = 0;
  radius = (a*a)+(b*b);
  cairo_arc(cr,x-6,y-6,sqrt(radius)-6,0,0);
  cairo_stroke_preserve (cr);
  cairo_surface_write_to_png(surface,".png.tmp");
  cairo_destroy (cr);

  gtk_widget_queue_draw_area (widget, 0, 0, 2000, 2000);
}

void newfile(GtkButton *button, GtkWidget *window)
{
  if (PopupNew(window) == 1) {
    clear_surface();
    SetNewFile();
    actualiser(window);
  }
  (void) button;
}

void togrey(GtkWidget *widget)
{
	char nom[30];
	char nom2[30];
	strcpy(nom, ".png.tmp");
	strcpy(nom2, ".png.tmp");
	char nb[5];
	char nb2[5];
	sprintf(nb, "%d", tout.imp-1);
	sprintf(nb2, "%d", tout.imp);
	strcat(nom, nb);
	strcat(nom2, nb2);
	//printf("FILTRE : %s\n",nom);
	//printf("FILTRE : %s\n",nom2);

	GError **error = NULL;
  GdkPixbuf *Im = gdk_pixbuf_new_from_file(nom, error);
  to_grey(Im);
  gdk_pixbuf_save(Im, nom2, "png", error, NULL);
  SetSurface(nom2);
  SetChange();
	tout.imp++;
	(void) widget;
}

void lowgrad(GtkWidget *widget)
{
	char nom[30];
	char nom2[30];
	strcpy(nom, ".png.tmp");
	strcpy(nom2, ".png.tmp");
	char nb[5];
	char nb2[5];
	sprintf(nb, "%d", tout.imp-1);
	sprintf(nb2, "%d", tout.imp);
	strcat(nom, nb);
	strcat(nom2, nb2);
	//printf("FILTRE : %s\n",nom);
	//printf("FILTRE : %s\n",nom2);

	GError **error = NULL;
  GdkPixbuf *Im = gdk_pixbuf_new_from_file(nom, error);
  Im = lowgrade(Im);
  gdk_pixbuf_save(Im, nom2, "png", error, NULL);
  SetSurface(nom2);
  SetChange();
	tout.imp++;
	(void) widget;
}



void upgrad(GtkWidget *widget)
{
	char nom[30];
	char nom2[30];
	strcpy(nom, ".png.tmp");
	strcpy(nom2, ".png.tmp");
	char nb[5];
	char nb2[5];
	sprintf(nb, "%d", tout.imp-1);
	sprintf(nb2, "%d", tout.imp);
	strcat(nom, nb);
	strcat(nom2, nb2);
	//printf("FILTRE : %s\n",nom);
	//printf("FILTRE : %s\n",nom2);

	GError **error = NULL;
  GdkPixbuf *Im = gdk_pixbuf_new_from_file(nom, error);
  Im = upgrade(Im);
  gdk_pixbuf_save(Im, nom2, "png", error, NULL);
  SetSurface(nom2);
  SetChange();
	tout.imp++;
	(void) widget;
}



void resiz(GtkWidget *widget)
{
	char nom[30];
	char nom2[30];
	strcpy(nom, ".png.tmp");
	strcpy(nom2, ".png.tmp");
	char nb[5];
	char nb2[5];
	sprintf(nb, "%d", tout.imp-1);
	sprintf(nb2, "%d", tout.imp);
	strcat(nom, nb);
	strcat(nom2, nb2);
	//printf("FILTRE : %s\n",nom);
	//printf("FILTRE : %s\n",nom2);

	GError **error = NULL;
  GdkPixbuf *Im = gdk_pixbuf_new_from_file(nom, error);
  Im = resize(Im);
  gdk_pixbuf_save(Im, nom2, "png", error, NULL);
  SetSurface(nom2);
  SetChange();
	tout.imp++;
	(void) widget;
}


void togg (GtkWidget *widget, gpointer data)
{
         if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data)))
	{
	   set_plein(1);
	}
	 else
	   set_plein(0);
	(void)widget;
}
void mode(GtkWidget *widget)
{
  if (get_plein() == 0)
set_plein(1);
  else
set_plein(0);
  (void) widget;
}
// **************** Event KeyBoard *****************************************

static gboolean keyboard(GtkWidget *widget, GdkEventKey *event,
								    		 gpointer user_data) {
				switch (event->keyval) {
					case GDK_KEY_z :
						if (event->state == 20 && tout.imp > 1) {
							char nom[30];
							strcpy(nom, ".png.tmp");
							char nb[5];
							sprintf(nb, "%d", tout.imp-2);
							strcat(nom, nb);
							SetSurface(nom);
							tout.imp--;
						}
						break;
				}
	(void) user_data;
	(void) widget;
	return TRUE;
}



// **************** Creation fenetre entiere *****************************************
void fenetre()
{
  // ********************************* Init widget
  GtkWidget *home;
  GtkWidget *grid;
  GtkWidget *new_file;
  GtkWidget *open;
  GtkWidget *save;
  GtkWidget *quit;
  GtkWidget *darea = gtk_drawing_area_new();
	tout.p_da = darea;
  GtkWidget *frame;
	GError *error;

  // ************************** Home
  home = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(home), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(home), 2000,2000);
  gtk_window_set_title (GTK_WINDOW(home),"Amganida");
  gtk_window_set_icon(GTK_WINDOW(home),
									gdk_pixbuf_new_from_file("icone/pinceau.png", &error));
	grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(home),grid);

  //********************* Menu
  new_file = gtk_button_new_with_label("New file");
  open = gtk_button_new_with_label ("Open file");
  save = gtk_button_new_with_label ("Save");
  quit = gtk_button_new_with_label ("Quit");

 
  GtkWidget *palette3 = gtk_tool_palette_new();
  GtkWidget *tool3 = gtk_tool_item_group_new ("Modif Img");
  GtkToolItem *nb = gtk_tool_button_new(NULL,"Nuances de gris");
  GtkToolItem *lowgrade = gtk_tool_button_new(NULL,"Reduire");
  GtkToolItem *upgrade = gtk_tool_button_new(NULL,"Agrandir");
  GtkToolItem *resize = gtk_tool_button_new(NULL,"Taille Definie");

  //***********************Palette

  gtk_tool_item_group_insert (GTK_TOOL_ITEM_GROUP(tool3),nb,1);
  gtk_tool_item_group_insert (GTK_TOOL_ITEM_GROUP(tool3),lowgrade,2);
  gtk_tool_item_group_insert (GTK_TOOL_ITEM_GROUP(tool3),upgrade,3);
  gtk_tool_item_group_insert (GTK_TOOL_ITEM_GROUP(tool3),resize,4);


  gtk_container_add (GTK_CONTAINER(palette3),tool3);


  //*************************** Drawing area
  frame = gtk_frame_new(NULL);
  gtk_frame_set_shadow_type(GTK_FRAME(frame),GTK_SHADOW_IN);
  gtk_widget_set_size_request (darea, 1400, 1400);
  gtk_container_add(GTK_CONTAINER(frame),darea);

  //**************** Grid Attach
  gtk_grid_attach(GTK_GRID(grid), new_file,0,0,1,1);
  gtk_grid_attach(GTK_GRID(grid), open,1,0,1,1);
  gtk_grid_attach(GTK_GRID(grid), save,2,0,1,1);
  gtk_grid_attach(GTK_GRID(grid), quit,10,0,1,1);
  gtk_grid_attach(GTK_GRID(grid), palette3,0,2,1,5);
  gtk_grid_attach(GTK_GRID(grid), frame,1,3,8,5);

  //******************* Signaux de bouton
  g_signal_connect(G_OBJECT(home), "destroy",G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(G_OBJECT(quit), "clicked", G_CALLBACK(PopupQuit), NULL);
  g_signal_connect(G_OBJECT(open), "clicked", G_CALLBACK(loadImage), home);
  g_signal_connect(G_OBJECT(save), "clicked", G_CALLBACK(SaveImage), home);
  g_signal_connect(G_OBJECT(new_file), "clicked", G_CALLBACK(newfile), home);
  g_signal_connect(G_OBJECT(nb), "clicked", G_CALLBACK(togrey),darea);
  g_signal_connect(G_OBJECT(lowgrade), "clicked", G_CALLBACK(lowgrad),darea);
  g_signal_connect(G_OBJECT(upgrade), "clicked", G_CALLBACK(upgrad),darea);
  g_signal_connect(G_OBJECT(resize), "clicked", G_CALLBACK(resiz),darea);


  //*****************************************************************
  g_signal_connect (darea, "draw",
      G_CALLBACK (draw_cb), NULL);
  g_signal_connect (darea,"configure-event",
      G_CALLBACK (configure_event_cb), NULL);
  g_signal_connect(home, "key-release-event",
      G_CALLBACK(keyboard), NULL);
  gtk_widget_set_events (darea, gtk_widget_get_events (darea)
      | GDK_BUTTON_PRESS_MASK
      | GDK_POINTER_MOTION_MASK
      | GDK_BUTTON_RELEASE_MASK);
  //******************************************************************


  //***************  SHOW ALL
  gtk_widget_show_all(home);
}



//* *********** Main **************

int main(int argc, char **argv)
{
  gtk_init(& argc,& argv);
  fenetre();
  gtk_main();
  return 0;
}
