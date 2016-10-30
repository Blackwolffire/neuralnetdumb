/* neural_net.c
 * sdouga_a
 */

#include <stdio.h>
#include <assert.h>

#include "neural_net.h"

NeuralNet* createNeural(size_t input, size_t output, size_t hiddenLayers,
			 NeuronType *type, flint *bias)
{
  NeuralNet *net = malloc(sizeof(NeuralNet));

  if(input >= output)
    net->w = input;
  else
    net->w = output;

  net->inputs = input, net->outputs = output, net->hiddenLayers = hiddenLayers;
  net->h = hiddenLayers + 2;
  net->network = malloc(sizeof(Neuron) * net->w * net->h);

  for(size_t i = 0; i < net->w * net->h; ++i){
    net->network[i].type = type[i];
    net->network[i].inputSynapse = NULL;
    if(type[i] != NONE){
    #ifdef DEBUG
      printf(" %zu->%d , %d\t", i, bias[i].i, type[i]);
      if(i % net->w == 0)
	printf("\n");
    #endif
      net->network[i].bias = bias[i];
      net->network[i].inputSynapse = createList();
    }
  }

  return net;
}

void destroyNeural(NeuralNet *net)
{
  for(size_t i = 0; i < net->w * net->h; ++i)
    if(net->network[i].type != NONE)
      destroyList(net->network[i].inputSynapse);
  free(net->network);
  free(net);
}

void boundNeuron(NeuralNet *net, flint weight, size_t  xin, size_t  yin,
		 size_t  x, size_t y)
{
  Synapse *syn = NULL;

  if(!net || xin >= net->w || yin >= net->h || y <= yin)
    errx(EXIT_FAILURE, "boundNeuron:  %s  %u\n", __FILE__, __LINE__);

  syn = malloc(sizeof(Synapse));
  syn->input = net->network + yin * net->w + xin;
  syn->weight = weight;
#ifdef DEBUG
  printf(" %zu -> %d -> %zu\n", yin * net->w + xin, weight.i, y * net->w + x);
#endif
  insertList(net->network[y * net->w + x].inputSynapse, syn, 0);
}

void setInputNeural(NeuralNet *net, flint *inputs)
{
  for(size_t i = 0; i < net->inputs; ++i)
    net->network[i].output = flint[i];
}

flint getOutputNeural(NeuralNet *net, size_t i)
{
  if(i > net->outputs)
    errx(EXIT_FAILURE, "getOutputNeural:  %s  %d\n", __FILE__, __LINE__);
  return net->network[net->w * (net->h - 1) + i].output;
}

void proceedNeuron(Neuron *neuron)
{
  
}

void startNeural(NeuralNet *net)
{
  for(size_t i = 1; i < net->h; ++i)
    for(size_t j = 0; j < net->w; ++j){
      
    }
}

