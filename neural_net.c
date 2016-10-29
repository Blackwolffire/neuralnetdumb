/* neural_net.c
 * sdouga_a
 */

#include "neural_net.h"

void initNeural(NeuralNet net, size_t input, size_t output, size_t hiddenLayers)
{
	if(input >= output)
		net.h = input;
	else
		net.h = output;

	net.inputs = input, net.outputs = output, net.hiddenLayers = hiddenLayers;
	net.w = hiddenLayers + 2;
	net.network = malloc(sizeof(Neuron) * net.w * net.h);
}

void destroyNeural(NeuralNet net)
{
	free(net.network);
	net.w = net.h = net.inputs = net.outputs = net.hiddenLayers = 0;
}

