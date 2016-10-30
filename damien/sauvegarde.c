//loadnsave.c

# include <gtk-3.0/gtk/gtk.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <err.h>


/******************************** Structure ***********************************/
struct SaveData {
	GtkWidget *window, *SaveWindow;
	GtkWidget *inv1, *inv2, *inv3, *inv4;
	GtkWidget *s_compPNG,  *sp_compPNG,  *l_compPNG;
	GtkWidget *s_compJPEG, *sp_compJPEG, *l_compJPEG;
};

/******************************** Variables ***********************************/
GtkWidget *GSaveWindow = NULL;
int loaded = 0, IsHide = 1, changed = 0;
double compression = 9, quality = 100;
char *path;
char *fmt;

/****************************** Outils de base ********************************/
size_t ArrayCmp (char a[], char b[], size_t count) {
	for (size_t i = 0 ; i < count ; i++)
			if (a[i] != b[i])
				return 0;
	return 1;
}

/*Value returned : -1 = Error
										0 = png,
										1 = jpeg,
										2 = bmp,
										3 = gif,
										4 = tiff.
*/
int isCorrectImage(char *path) {
	int fd, fmt = -1;
	char png[]   = {'\x89', 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
	char gif[]   = {0x47, 0x49, 0x46};
	char bmp[]   = {0x42, 0x4D};
	char jpg[]   = {'\xFF', '\xD8'};
	char tiff1[] = {0x4D, 0x4D};
	char tiff2[] = {0x49, 0x49};

	if ((fd = open(path, O_RDONLY)) == -1)
		return -1;
	else {
		char buf[8];
		if (read(fd, buf, 8) == -1) {
			return -2;
			err(1, "Reading Failed");
		}
		if (ArrayCmp(png, buf, 8) == 1)
			fmt = 0;
		else if (ArrayCmp(jpg, buf, 2) == 1)
			fmt = 1;
		else if (ArrayCmp(bmp, buf, 2) == 1)
			fmt = 2;
		else if (ArrayCmp(gif, buf, 3) == 1)
			fmt = 3;
		else if (ArrayCmp(tiff1, buf, 2) == 1 || ArrayCmp(tiff2, buf, 2) == 1)
			fmt = 4;
	}

	close(fd);
	return fmt;
}
//Init char variables
void InitChar() {
	fmt = malloc(256*sizeof(char));
	path = malloc(256*sizeof(char));
}


// Convertie l'image en .png
void ConvertToPng (char *path) {
	GError **error = NULL;
	GdkPixbuf *Im = gdk_pixbuf_new_from_file(path, error);
	tout.imp = 2;

	gdk_pixbuf_save(Im, ".png.tmp1", "png", error, NULL);
}

//Retourne l'extension
char* get_fmt(char *img) {
	char *ext = malloc(6*sizeof(char));
	size_t len;
	for (len = 0 ; img[len] != '\0' ; len++);

	switch (img[len-2]) {
		case 'n':
			ext = ".png";
		case 'i':
			ext = ".gif";
		case 'm':
			ext = ".bmp";
		case 'e':
			ext = ".jpeg";
		case 'f':
			ext = ".tiff";
	}
	return ext;
}

// Value returned : same isCorrectImage
int FormatImage(char *fmt) {
	if (strcmp(fmt, ".png") == 0)
		return 0;
	else if (strcmp(fmt, ".jpeg") == 0)
		return 1;
	else if (strcmp(fmt, ".bmp") == 0)
		return 2;
	else if (strcmp(fmt, ".gif") == 0)
		return 3;
	else if (strcmp(fmt, ".tiff") == 0)
		return 4;
	else
		return -1;
}

//Determine un nouveau fichier
void SetNewFile() {
	loaded = 0;
	tout.imp = 0;
	path = NULL;
	fmt = NULL;
}

// Cache les widgets extras de "save"
void hideWidget (struct SaveData *d) {
	gtk_widget_hide(d->sp_compPNG);
	gtk_widget_hide(d->s_compPNG);
	gtk_widget_hide(d->l_compPNG);
	gtk_widget_hide(d->inv1);
	gtk_widget_hide(d->inv2);
	gtk_widget_hide(d->s_compJPEG);
	gtk_widget_hide(d->sp_compJPEG);
	gtk_widget_hide(d->l_compJPEG);
	gtk_widget_hide(d->inv3);
	gtk_widget_hide(d->inv4);
}

//Renvoi V ou F si les extra pour la save sont affichées
int IsExtraWindowHide() {
	if (IsHide == 0) {
		IsHide = 1;
		return 0;
	} else {
		IsHide = 0;
		return 1;
	}
}

void SetChange() {
	changed = 1;
}

/**************************** Boite de dialogues ******************************/

/* Affiche une boîte de dialogue pour choisir
 * une image dans la base de données
 */
char* dialog_box(GtkWidget *window) {
	char *picturePath = malloc(256*sizeof(char));
  GtkWidget *chooser = 	gtk_file_chooser_dialog_new("Pick a picture",
									 			GTK_WINDOW(window),
												GTK_FILE_CHOOSER_ACTION_OPEN,
												"_Cancel",
												GTK_RESPONSE_CANCEL,
												"_Load",
												GTK_RESPONSE_ACCEPT,
												NULL);

	// Lancement de l'explorateur
  switch (gtk_dialog_run(GTK_DIALOG(chooser))) {
  	//L'utilisateur clique LOAD
    case GTK_RESPONSE_ACCEPT:
			picturePath = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooser));
      break;
    // L'utilisateur clique CANCEL
    default:
			picturePath = NULL;
			break;
   }

	/* Destruction de la boîte de dialogue */
  gtk_widget_destroy(chooser);
	return picturePath;
}

/* Affiche une boîte de dialogue pour choisir
 * l'emplacement où l'image sera sauvegardée
 */
char* dialog_box_save(GtkWidget *window) {
	char *picturePath = malloc(256*sizeof(char));
  GtkWidget *chooser = 	gtk_file_chooser_dialog_new("Save as..",
									 			GTK_WINDOW(window),
												GTK_FILE_CHOOSER_ACTION_SAVE,
												"_Cancel",
												GTK_RESPONSE_CANCEL,
												"_Save",
												GTK_RESPONSE_ACCEPT,
												NULL);

	gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER(chooser),
																		"Untitled document");

	// Lancement de l'explorateur
  switch (gtk_dialog_run(GTK_DIALOG(chooser))) {
  	//L'utilisateur clique LOAD
    case GTK_RESPONSE_ACCEPT:
			picturePath = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooser));
      break;
    // L'utilisateur clique CANCEL
    default:
			picturePath = NULL;
			break;
   }

	//Destruction de la boîte de dialogue
  gtk_widget_destroy(chooser);
	return picturePath;
}

/******************************** Pré-définition ******************************/
void SaveImage (GtkButton *button, GtkWidget *window);
void InitSaveAsWindow(struct SaveData *d);

/******************************** Callbacks ***********************************/
//Reviens à la fenêtre principale
void gtk_main_display_f_save(GtkWidget *button, struct SaveData *d) {
	gtk_widget_destroy(d->SaveWindow);
	(void) button;
}

//Lance le processus de SaveAs avec l'interface
void gtk_save_display(GtkButton *button, GtkWidget *window) {
	struct SaveData *d = malloc(sizeof(struct SaveData));
	d->window = window;
	InitSaveAsWindow(d);
	gtk_widget_show_all(d->SaveWindow);
	hideWidget(d);
 	gtk_window_resize(GTK_WINDOW(d->SaveWindow), 200, 370);
	(void) button;
}

//Lance le processus de Save
void gtk_save(GtkButton *button, char *format) {
	loaded = 1;
	fmt = format;
	free(path);
	path = NULL;
	path = dialog_box_save(gtk_widget_get_toplevel(GTK_WIDGET(button)));
	if (path == NULL)
		return;
	path = strcat(path, fmt);
	SaveImage(button, gtk_widget_get_toplevel(GTK_WIDGET(button)));
}

//Cache ou affiche les options de sauvegarde
void gtk_display_extra(GtkButton *button, struct SaveData *d) {
	if (IsExtraWindowHide()) {
		gtk_button_set_image(GTK_BUTTON(button),
											 gtk_image_new_from_file("./icone/fleche_g.png"));
		gtk_window_resize(GTK_WINDOW(d->SaveWindow), 405, 370);
		gtk_widget_show_now(d->sp_compPNG);
		gtk_widget_show_now(d->s_compPNG);
		gtk_widget_show_now(d->l_compPNG);
		gtk_widget_show_now(d->inv1);
		gtk_widget_show_now(d->inv2);
		gtk_widget_show_now(d->s_compJPEG);
		gtk_widget_show_now(d->sp_compJPEG);
		gtk_widget_show_now(d->l_compJPEG);
		gtk_widget_show_now(d->inv3);
		gtk_widget_show_now(d->inv4);
	}
	else {
		gtk_button_set_image(GTK_BUTTON(button),
											 gtk_image_new_from_file("./icone/fleche_d.png"));
		gtk_widget_hide(d->sp_compPNG);
		gtk_widget_hide(d->s_compPNG);
		gtk_widget_hide(d->l_compPNG);
		gtk_widget_hide(d->inv1);
		gtk_widget_hide(d->inv2);
		gtk_widget_hide(d->s_compJPEG);
		gtk_widget_hide(d->sp_compJPEG);
		gtk_widget_hide(d->l_compJPEG);
		gtk_widget_hide(d->inv3);
		gtk_widget_hide(d->inv4);
		gtk_window_resize(GTK_WINDOW(d->SaveWindow), 200, 370);
	}
	(void)button;
}

//Ajuste le Scale Button PNG à la bonne valeur
void gtk_adjustSB_PNG(GtkSpinButton *sp, struct SaveData *d) {
	double value = gtk_spin_button_get_value(sp);
	gtk_range_set_value(GTK_RANGE(d->s_compPNG), value);
	compression = value;
}
//Ajuste le Spin Button PNG à la bonne valeur
void gtk_adjustSP_PNG(GtkScale *sb, struct SaveData *d) {
	double value = gtk_range_get_value(GTK_RANGE(sb));
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(d->sp_compPNG), value);
	compression = value;
}
//Ajuste le Scale Button JPEG à la bonne valeur
void gtk_adjustSB_JPEG(GtkSpinButton *sp, struct SaveData *d) {
	double value = gtk_spin_button_get_value(sp);
	gtk_range_set_value(GTK_RANGE(d->s_compJPEG), value);
	quality = value;
}
//Ajuste le Spin Button JPEG à la bonne valeur
void gtk_adjustSP_JPEG(GtkScale *sb, struct SaveData *d) {
	double value = gtk_range_get_value(GTK_RANGE(sb));
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(d->sp_compJPEG), value);
	quality = value;
}

/******************************* LOAD N SAVE **********************************/
/* Charge l'image selectionnée par l'utilisateur sur la
 * surface de travail si elle correspond aux normes*/
void loadImage (GtkButton *button, GtkWidget *window) {
	char *lpath = dialog_box(window), *messError;
	if (lpath == NULL) {
		//free(lpath);
		return;
	}
	int error;
	if ((error = isCorrectImage(lpath)) < 0)	{
		if (error == -1)
			messError = "Error loading file : Is not an image";
		else
			messError = "Error loading file : Reading failed";

		GtkWidget *M_notImage = gtk_message_dialog_new(GTK_WINDOW(window),
														GTK_DIALOG_DESTROY_WITH_PARENT,
														GTK_MESSAGE_ERROR,
														GTK_BUTTONS_CLOSE,
														messError);
		gtk_dialog_run(GTK_DIALOG(M_notImage));
		gtk_widget_destroy(M_notImage);
		free(lpath);
		return;
	}
	else {
		//free(path);
		path = lpath;
		loaded = 1;
		//free(fmt);
		fmt = get_fmt(lpath);
		if (fmt == NULL)
			err(1, "Getting extension failed");
		ConvertToPng(lpath);
		SetSurface(".png.tmp1");
	}
	(void)button;
}

/* Sauvegarde l'image recu*/
void SaveImage (GtkButton *button, GtkWidget *window) {
	printf("%d", loaded);
	if (loaded == 0) {
		printf("HEY\n");
		gtk_save_display(button, window);
		printf("HEY\n");
	}
	else {
		GError **error = NULL;
		char nom[30];
		strcpy(nom, ".png.tmp");
		char nb[5];
		sprintf(nb, "%d", tout.imp-1);
		strcat(nom, nb);
		GdkPixbuf *Im = gdk_pixbuf_new_from_file(nom, error);
		if (path != NULL) {
			int frmt = FormatImage(fmt);
			char valPNG[8],valJPEG[8];
			sprintf(valPNG, "%d", (int)compression);
			sprintf(valJPEG, "%d", (int)quality);
			switch (frmt) {
				case 0: //PNG
					gdk_pixbuf_save(Im, path, fmt+1, error,"compression", valPNG, NULL);
					break;
				case 1: //JPEG
					gdk_pixbuf_save(Im, path, fmt+1, error,"quality", valJPEG, NULL);
					break;
				case 2: //BMP
					gdk_pixbuf_save(Im, path, fmt+1, error, NULL);
					break;
				case 3: //GIF
					gdk_pixbuf_save(Im, path, fmt+1, error, NULL);
					break;
				case 4: //TIFF
					gdk_pixbuf_save(Im, path, fmt+1, error, NULL);
					break;
			}
			changed = 0;
		}
	}
	if (GSaveWindow != NULL) {
		gtk_widget_destroy(GSaveWindow);
		GSaveWindow = NULL;
	}

	(void) button;
}

/*********************************** Popups  **********************************/

int PopupNew(GtkWidget *window) {
	if (changed == 1) {
	GtkWidget *M_new = gtk_message_dialog_new(GTK_WINDOW(window),
														GTK_DIALOG_DESTROY_WITH_PARENT,
														GTK_MESSAGE_QUESTION,
														GTK_BUTTONS_YES_NO,
														"You didn't save your work, do you want to save "
														"before oping another project ?");
		gint result = gtk_dialog_run(GTK_DIALOG(M_new));
		switch (result) {
			case GTK_RESPONSE_YES:
				SaveImage(NULL, window);
				changed = 0;
				gtk_widget_destroy(M_new);
				return 0;
			case GTK_RESPONSE_NO:
				changed = 0;
				gtk_widget_destroy(M_new);
				return 1;
		}
		gtk_widget_destroy(M_new);
		return 0;
	}
	else {
		return 1;
	}
}

void PopupQuit(GtkWidget *Button, GtkWidget *window) {
	if (changed == 1) {
	GtkWidget *M_new = gtk_message_dialog_new(GTK_WINDOW(window),
														GTK_DIALOG_DESTROY_WITH_PARENT,
														GTK_MESSAGE_QUESTION,
														GTK_BUTTONS_YES_NO,
														"You didn't save your work, do you want to save "
														"before exiting ?");
		gint result = gtk_dialog_run(GTK_DIALOG(M_new));
		switch (result) {
			case GTK_RESPONSE_YES:
				SaveImage(NULL, window);
				gtk_widget_destroy(M_new);
				return;
			case GTK_RESPONSE_NO:
				gtk_widget_destroy(M_new);
				gtk_main_quit();
		}
		gtk_widget_destroy(M_new);
	}
	else
		gtk_main_quit();
	(void) Button;
}
/**************************** Init Save Window ********************************/
void InitSaveAsWindow(struct SaveData *d) {
	GError **error = NULL;
	InitChar();
	// Declaration des widgets
	GtkWidget *grid;
  GtkWidget	*b_PNG, *b_JPG, *b_BMP, *b_TIFF, *b_GIF, *b_plus;

	/* Creation et initialisation de la fenetre secondaire convert*/
	d->SaveWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	GSaveWindow = d->SaveWindow;
 	gtk_window_set_title(GTK_WINDOW(d->SaveWindow), "Save As...");
 	gtk_container_set_border_width(GTK_CONTAINER(d->SaveWindow), 10);
 	gtk_window_set_default_size(GTK_WINDOW(d->SaveWindow), 200, 335);
	gtk_window_set_position(GTK_WINDOW(d->SaveWindow), GTK_WIN_POS_CENTER);
	gtk_window_set_icon(GTK_WINDOW(d->SaveWindow),
									gdk_pixbuf_new_from_file("icone/SaveAs.png", error));

	/* Création de la table de positionnement */
  grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(d->SaveWindow), grid);
	gtk_grid_set_row_spacing (GTK_GRID (grid), 10);
	gtk_grid_set_column_spacing (GTK_GRID (grid), 10);

	/*Creation des buttons*/
  b_PNG  = gtk_button_new_with_label("PNG");
	gtk_widget_set_size_request (b_PNG, 220, 80);
  b_JPG  = gtk_button_new_with_label("JPG");
	gtk_widget_set_size_request (b_JPG, 220, 80);
  b_BMP  = gtk_button_new_with_label("BMP");
	gtk_widget_set_size_request (b_BMP, 220, 80);
  b_TIFF = gtk_button_new_with_label("TIFF");
	gtk_widget_set_size_request (b_TIFF, 220, 80);
  b_GIF  = gtk_button_new_with_label("GIF");
	gtk_widget_set_size_request (b_GIF, 220, 80);
	b_plus = gtk_button_new();
	gtk_button_set_image(GTK_BUTTON(b_plus),
											 gtk_image_new_from_file("./icone/fleche_d.png"));
	gtk_widget_set_size_request (b_plus, 20, 20);

	/*Création des labels*/
	d->l_compPNG = gtk_label_new("Niveau de compression");
	gtk_widget_set_size_request (d->l_compPNG, 200, 10);
	d->l_compJPEG = gtk_label_new("Niveau de qualité");
	gtk_widget_set_size_request (d->l_compJPEG, 200, 10);

	/*Creation des scale buttons*/
	d->s_compPNG = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 9, 1);
	gtk_range_set_value(GTK_RANGE(d->s_compPNG), compression);
	gtk_widget_set_size_request (d->s_compPNG, 150, 20);
	d->s_compJPEG = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL,0,100,1);
	gtk_range_set_value(GTK_RANGE(d->s_compJPEG), quality);
	gtk_widget_set_size_request (d->s_compJPEG, 150, 20);

	/*Creation des spin buttons*/
	d->sp_compPNG = gtk_spin_button_new_with_range (0, 9, 1);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(d->sp_compPNG), compression);
	gtk_widget_set_size_request (d->sp_compPNG, 1, 1);
	d->sp_compJPEG = gtk_spin_button_new_with_range (0, 100, 1);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(d->sp_compJPEG), quality);
	gtk_widget_set_size_request (d->sp_compJPEG, 1, 1);

	/*Creation des widgets invisibles*/
	d->inv1 = gtk_label_new(" ");
	d->inv2 = gtk_label_new(" ");
	d->inv3 = gtk_label_new(" ");
	d->inv4 = gtk_label_new(" ");

	/*Ajout des éléments a la grid*/
	gtk_grid_attach (GTK_GRID (grid), b_PNG,  	0, 0, 1, 2);
	gtk_grid_attach (GTK_GRID (grid), b_plus, 	1, 0, 1, 1);
	gtk_grid_attach (GTK_GRID (grid), d->inv1, 	0, 2, 3, 1);
	gtk_grid_attach (GTK_GRID (grid), b_BMP,  	0, 3, 1, 2);
	gtk_grid_attach (GTK_GRID (grid), d->inv2, 	0, 5, 1, 1);
	gtk_grid_attach (GTK_GRID (grid), b_JPG,  	0, 6, 1, 2);
	gtk_grid_attach (GTK_GRID (grid), d->inv3, 	0, 8, 1, 1);
	gtk_grid_attach (GTK_GRID (grid), b_GIF,  	0, 9, 1, 2);
	gtk_grid_attach (GTK_GRID (grid), d->inv4, 	0, 11, 1, 1);
	gtk_grid_attach (GTK_GRID (grid), b_TIFF, 	0, 12, 1, 2);

	gtk_grid_attach (GTK_GRID (grid), d->l_compPNG,	 2, 0, 2, 1);
	gtk_grid_attach (GTK_GRID (grid), d->s_compPNG,  2, 1, 1, 1);
	gtk_grid_attach (GTK_GRID (grid), d->sp_compPNG, 3, 1, 1, 1);

	gtk_grid_attach (GTK_GRID (grid), d->l_compJPEG,	2, 6, 2, 1);
	gtk_grid_attach (GTK_GRID (grid), d->s_compJPEG,  2, 7, 1, 1);
	gtk_grid_attach (GTK_GRID (grid), d->sp_compJPEG, 3, 7, 1, 1);

	/* Création des signaux/événements avec leur callback*/
	g_signal_connect(G_OBJECT(d->SaveWindow), "destroy",
 									 G_CALLBACK(gtk_main_display_f_save), d);
  g_signal_connect(G_OBJECT(b_PNG), "clicked",
									 G_CALLBACK(gtk_save), ".png");
  g_signal_connect(G_OBJECT(b_BMP), "clicked",
									 G_CALLBACK(gtk_save), ".bmp");
  g_signal_connect(G_OBJECT(b_JPG), "clicked",
									 G_CALLBACK(gtk_save), ".jpeg");
  g_signal_connect(G_OBJECT(b_GIF), "clicked",
									 G_CALLBACK(gtk_save), ".jpeg");
  g_signal_connect(G_OBJECT(b_TIFF), "clicked",
									 G_CALLBACK(gtk_save), ".tiff");
  g_signal_connect(G_OBJECT(b_plus), "clicked",
									 G_CALLBACK(gtk_display_extra), d);
  g_signal_connect(G_OBJECT(d->sp_compPNG), "value-changed",
									 G_CALLBACK(gtk_adjustSB_PNG), d);
  g_signal_connect(G_OBJECT(d->s_compPNG), "value-changed",
									 G_CALLBACK(gtk_adjustSP_PNG), d);
	g_signal_connect(G_OBJECT(d->sp_compJPEG), "value-changed",
									 G_CALLBACK(gtk_adjustSB_JPEG), d);
  g_signal_connect(G_OBJECT(d->s_compJPEG), "value-changed",
									 G_CALLBACK(gtk_adjustSP_JPEG), d);
}
