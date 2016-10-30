#include <stdio.h>
#include <assert.h>

#include "neural_net.h"

int main()
{
  NeuralNet *lol = NULL;
  flint bias[10] = {{0}, {0}, {3}, {3}, {3}, {3}, {3}, {0}};
  NeuronType type[10] = {NONE, NONE, PERCEPTRON, PERCEPTRON, PERCEPTRON, PERCEPTRON, PERCEPTRON, NONE};

	lol = createNeural(2, 1, 2, type, bias);
  bias[0].i = -2, bias[1].i = -4;
  boundNeuron(lol, bias[1], 0, 0, 0, 1);
  boundNeuron(lol, bias[0], 0, 0, 1, 2);
  boundNeuron(lol, bias[0], 1, 0, 0, 2);
  boundNeuron(lol, bias[1], 1, 0, 1, 1);
  boundNeuron(lol, bias[0], 0, 1, 0, 2);
  boundNeuron(lol, bias[0], 1, 1, 1, 2);
  boundNeuron(lol, bias[0], 0, 2, 0, 3);
  boundNeuron(lol, bias[0], 1, 2, 0, 3);

	bias[0].i = 0, bias[1].i = 0;
	setInputNeural(lol, bias);
	startNeural(lol);
	bias[0] = getOutputNeural(lol, 0);
	printf("rip %d \n\n", bias[0].i);

	bias[0].i = 0, bias[1].i = 1;
	setInputNeural(lol, bias);
	startNeural(lol);
	bias[0] = getOutputNeural(lol, 0);
	printf("rip %d \n\n", bias[0].i);

	bias[0].i = 1, bias[1].i = 0;
	setInputNeural(lol, bias);
	startNeural(lol);
	bias[0] = getOutputNeural(lol, 0);
	printf("rip %d \n\n", bias[0].i);

	bias[0].i = 1, bias[1].i = 1;
	setInputNeural(lol, bias);
	startNeural(lol);
	bias[0] = getOutputNeural(lol, 0);
	printf("rip %d \n\n", bias[0].i);


  destroyNeural(lol);

  return 0;
}

