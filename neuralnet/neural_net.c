/* neural_net.c
 * sdouga_a
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

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
    #ifdef DEBUG
      printf(" %zu->%d , %d\t", i, bias[i].i, type[i]);
      if((i + 1) % net->w == 0)
				printf("\n");
    #endif
    if(type[i] != NONE){
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
	printf("set inputs: \n");
  for(size_t i = 0; i < net->inputs; ++i){
		printf(" %d - %f\n", inputs[i].i, inputs[i].fl);
    net->network[i].output = inputs[i];
	}
}

flint getOutputNeural(NeuralNet *net, size_t i)
{
  if(i > net->outputs)
    errx(EXIT_FAILURE, "getOutputNeural:  %s  %d\n", __FILE__, __LINE__);
  return net->network[net->w * (net->h - 1) + i].output;
}

void proceedNeuron(Neuron *neuron)
{
	Synapse *syn = NULL;

	if(neuron->type == NONE)
		return;
	//printList(neuron->inputSynapse);
	if(neuron->type == PERCEPTRON){
		neuron->z.i = 0;
		for(size_t i = 0; i < neuron->inputSynapse->len; ++i){
			syn = getDataList(neuron->inputSynapse, i);
		/*#ifdef DEBUG
			printf(" %p syn %d  %d\n", syn, syn->weight.i, syn->input->output.i);
		#endif*/
			neuron->z.i += syn->input->output.i * syn->weight.i;
		}
		neuron->output.i = neuron->z.i + neuron->bias.i > 0;
	}else{
		neuron->z.fl = 0.;
		for(size_t i = 0; i < neuron->inputSynapse->len; ++i){
			syn = getDataList(neuron->inputSynapse, i);
		/*#ifdef DEBUG
			printf("syn %f  %f\n", syn->weight.fl, syn->input->output.fl);
		#endif*/
			neuron->z.fl += syn->input->output.fl * syn->weight.fl;
		}
		neuron->output.fl = 1. / (1. + exp(-1. * neuron->z.fl));
	}
}

void startNeural(NeuralNet *net)
{
  for(size_t i = 1; i < net->h; ++i)
    for(size_t j = 0; j < net->w; ++j){
      proceedNeuron(net->network + i * net->w + j);
    }
}

