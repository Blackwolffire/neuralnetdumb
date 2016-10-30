/* neural_net.c
 * sdouga_a
 */

#include <stdio.h>
#include <assert.h>

#include "neural_net.h"

NeuralNet* createNeural(size_t input, size_t output, size_t hiddenLayers,
			 NeuronType type, flint *bias)
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
    net->network[i].type = type;
    net->network[i].inputSynapse = NULL;
  }

  if(type != NONE && bias)
    for(size_t i = 0; i < net->h; ++i){
      for(size_t j = 0; j < net->w; ++j){
      #ifdef DEBUG
	printf(" %zu->%d\t", i * net->w + j, bias[i * net->w + j].i);
      #endif
	net->network[i * net->w + j].bias = bias[i * net->w + j];
	net->network[i * net->w + j].inputSynapse = createList();
      }
    #ifdef DEBUG
      printf("\n");
    #endif
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
  printf("\nlol?%zu  %d\n", y*net->w +x, net->network[y*net->w +x].bias.i);
  insertList(net->network[y * net->w + x].inputSynapse, syn, 0);
}

