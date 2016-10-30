#include <stdio.h>
#include <assert.h>

#include "neural_net.h"

int main()
{
  NeuralNet *lol = NULL;
  flint bias[4] = {{0}, {0}, {3}, {0}};

  lol = createNeural(2, 1, 0, PERCEPTRON, bias);
  bias[0].i = -2;
  boundNeuron(lol, bias[0], 0, 0, 0, 1);
  boundNeuron(lol, bias[0], 1, 0, 0, 1);

  destroyNeural(lol);

  return 0;
}
