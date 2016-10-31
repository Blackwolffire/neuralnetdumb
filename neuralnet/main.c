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
  NeuronType type[10] = {INPUT, INPUT, SIGMOID, SIGMOID, SIGMOID, SIGMOID,
			  SIGMOID, SIGMOID, SIGMOID, NONE};

  inputs[0].fl = inputs[1].fl = inputs[2].fl = inputs[5].fl = 0.;
  inputs[3].fl = inputs[4].fl = inputs[6].fl = inputs[7].fl = 1.;
  outputs[0].fl = outputs[3].fl = 0.;
  outputs[1].fl = outputs[2].fl = 1.;
  eta.fl = 0.1;
  net = createNeural(2, 1, 3, type);

  size_t len;
  if(argc > 1)
    len = strtoul(argv[1], NULL, 10);
  else
    len = 500;

  for(size_t i = 0; i < len; ++i)
    trainingNeural(net, inputs, outputs, 4, eta);
  
  destroyNeural(net);

  return 0;
}

