#include "neural_net.h"

int main()
{
	NeuralNet lol;

	initNeural(lol, 3, 2, 2);

	destroyNeural(lol);

	return 0;
}

