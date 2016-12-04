/* main.c
 * sdouga_a
 */

#include <stdlib.h>
#include <gtk-3.0/gtk/gtk.h>
#include <gdk/gdk.h>
//#include <gdk-pixbuf/gdk-pixbuf.h>

//#include "../segmentation/amin.h"
#include "training.h"

int main(int argc, char	**argv)
{
	size_t w = (256 + 76) / 2;
	flint outputs[76 * 76];
	List *list =NULL;
	NeuralNet *net = NULL;

	GtkImage *img = NULL;//gtk_image_new();

	for(size_t i = 0; i < 76*76; ++i)
		outputs[i].fl = (i%76 == i/76);

	if(argv[1][0] == 'y')
		net = loadNeural("net.sav");
	else
		net = createNeural(256, 76, 1, &w, SIGMOID);

	for(int i = 3; i < argc; ++i){
		gtk_image_set_from_file(img, argv[i]);
		list = Amin(gtk_image_get_pixbuf(img));
		training(net,list,outputs,(flint)0.01,list->len,strtoul(argv[2], NULL, 10),
						 "net.sav");
	}

	return EXIT_SUCCESS;
}

