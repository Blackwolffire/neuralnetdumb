/* main.c
 * sdouga_a
 */

#include <stdlib.h>
#include <gtk-3.0/gtk/gtk.h>
#include <gdk/gdk.h>

//#include "../segmentation/amin.h"
#include "../neuralnet/neural_net.h"
#include "filtres.h"

int main(int argc, char	**argv)
{
	size_t w = (256 + 76) / 2;
	flint outputs[76 * 76];
	NeuralNet *net = NULL;

	GdkPixbub *img = NULL;

	for(size_t i = 0; i < 76*76; ++i)
		outputs[i].fl = (i%76 == i/76);

	if(argv[1][0] == 'y')
		net = loadNeural("net.sav");
	else
		net = createNeural(256, 76, 1, &w, SIGMOID);

	for(size_t i = 3; i < argc; ++i){
		img = gdk_new_from_file(argv[i]);
		training(net,list,outputs,(flint)0.01,list->len,strtoul(argv[2]),
						 "net.sav");
	}

	return EXIT_SUCCESS;
}

