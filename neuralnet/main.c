#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
//#include <time.h>

#include "neural_net.h"

int main(int argc, char **argv)
{
  NeuralNet *net = NULL;
  flint inputs[8];
  flint outputs[4];
  flint eta;
  NeuronType type = SIGMOID;

  inputs[0].fl = 0., inputs[1].fl = 0.;
	inputs[2].fl = 0., inputs[3].fl = 1.;
  inputs[4].fl = 1., inputs[5].fl = 0.;
	inputs[6].fl = 1., inputs[7].fl = 1.;
  outputs[0].fl = 0.;
	outputs[1].fl = 1.;
  outputs[2].fl = 1.;
	outputs[3].fl = 0.;
  eta.fl = 0.1;
  net = createNeural(2, 1, 5, 3, type);

  size_t len;
  if(argc > 1)
    len = strtoul(argv[1], NULL, 10);
  else
    len = 0;

	printf("Before training: \ninputs: 0. 0.\n");
	setInputNeural(net, inputs);
	startNeural(net);
	printNeuralOutput(net);
	
	printf("inputs: 0. 1.\n");
	setInputNeural(net, inputs + 2);
	startNeural(net);
	printNeuralOutput(net);

	printf("inputs: 1. 0.\n");
	setInputNeural(net, inputs + 4);
	startNeural(net);
	printNeuralOutput(net);

	printf("inputs: 1. 1.\n");
	setInputNeural(net, inputs + 6);
	startNeural(net);
	printNeuralOutput(net);

  for(size_t i = 0; i < len; ++i)
    trainingNeural(net, inputs, outputs, 4, eta);

	printf("\nAfter training: \ninputs: 0. 0.\n");
	setInputNeural(net, inputs);
	startNeural(net);
	printNeuralOutput(net);
	
	printf("inputs: 0. 1.\n");
	setInputNeural(net, inputs + 2);
	startNeural(net);
	printNeuralOutput(net);

	printf("inputs: 1. 0.\n");
	setInputNeural(net, inputs + 4);
	startNeural(net);
	printNeuralOutput(net);

	printf("inputs: 1. 1.\n");
	setInputNeural(net, inputs + 6);
	startNeural(net);
	printNeuralOutput(net);
  
  destroyNeural(net);

  return EXIT_SUCCESS;
}

