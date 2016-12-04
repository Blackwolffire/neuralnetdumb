/* main.c
 * sdouga_a
 */

#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

//#include "../segmentation/amin.h"

int main(int argc, char	**argv)
{
	size_t w = (256 + 76) / 2;
	NeuralNet *net = NULL;

	if(argv[1][0] == 'y')
		net = loadNeural("net.sav");
	else
		net = createNeural(256, 76, 1, &w, SIGMOID);

	for(size_t i = 3; i < argc; ++i){
		///////////////////
		training(net, list, outputs, (flint)0.01, list->len, strtoul(argv[2]));
	}

	return EXIT_SUCCESS;
}

