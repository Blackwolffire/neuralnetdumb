/* neural_net.c
 * sdouga_a
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "neural_net.h"

NeuralNet* createNeural(size_t input, size_t output, size_t hiddenLayers,
			 NeuronType *type)
{
  NeuralNet *net = malloc(sizeof(NeuralNet));
  flint value;

  if(input >= output)
    net->w = input;
  else
    net->w = output;

  net->inputs = input, net->outputs = output, net->hiddenLayers = hiddenLayers;
  net->h = hiddenLayers + 2;
  net->network = malloc(sizeof(Neuron) * net->w * net->h);

  for(size_t i = 0; i < net->w * net->h; ++i){
    srand(time(NULL));
    net->network[i].type = type[i];
    net->network[i].inputSynapse = NULL;
    net->network[i].outputSynapse = NULL;
    #ifdef DEBUG
      printf(" %zu->%d\n", i, type[i]);
      if((i + 1) % net->w == 0)
	printf("\n");
    #endif
    switch(type[i]){
      case PERCEPTRON:
	net->network[i].bias.i = rand() % MAX_RAND;
	net->network[i].inputSynapse = createList();
	net->network[i].outputSynapse = createList();
	if(i / net->w == 1)
	  for(size_t j = 0; j < net->inputs; ++j){
	    value.i = rand() % MAX_RAND;
	    boundNeuron(net, value, j, i / net->w - 1, i % net->w, i / net->w);
	  }
	if(i / net->w > 1)
	  for(size_t j = 0; j < net->w; ++j){
	    value.i = rand() % MAX_RAND;
	    boundNeuron(net, value, j, i / net->w - 1, i % net->w, i / net->w);
	  }
	break;
      case SIGMOID:
	net->network[i].bias.fl = rand() / (MAX_RAND * 1.);
	while(net->network[i].bias.fl > 1. || net->network[i].bias.fl < -1.)
	  net->network[i].bias.fl /= 10.;
	net->network[i].inputSynapse = createList();
	net->network[i].outputSynapse = createList();
	if(i / net->w == 1)
	  for(size_t j = 0; j < net->inputs; ++j){
	    value.fl = rand() % MAX_RAND / 10.;
	    while(value.fl > 1. || value.fl < -1.)
	      value.fl /= 10.;
	    boundNeuron(net, value, j, i / net->w - 1, i % net->w, i / net->w);
	  }
	if(i / net->w > 1)
	  for(size_t j = 0; j < net->w; ++j){
	    value.fl = rand() % MAX_RAND / 10.;
	    while(value.fl > 1. || value.fl < -1.)
	      value.fl /= 10.;
	    boundNeuron(net, value, j, i / net->w - 1, i % net->w, i / net->w);
	  }
	break;
      case INPUT:
	net->network[i].outputSynapse = createList();
	break;
      default:
	break;
    }
  }

  return net;
}

void destroyNeural(NeuralNet *net)
{
  for(size_t i = 0; i < net->w * net->h; ++i)
    switch(net->network[i].type){
      case PERCEPTRON:
	destroyList(net->network[i].inputSynapse, free);
	destroyList(net->network[i].outputSynapse, NULL);
	break;
      case SIGMOID:
	destroyList(net->network[i].inputSynapse, free);
	destroyList(net->network[i].outputSynapse, NULL);
	break;
      case INPUT:
	destroyList(net->network[i].outputSynapse, NULL);
	break;
      default:
	break;
    }
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
  syn->output = net->network + y * net->w + x;
  syn->weight = weight;
#ifdef DEBUG
  printf(" %zu -> %d -> %zu\n", yin * net->w + xin, weight.i, y * net->w + x);
#endif
  insertList(net->network[y * net->w + x].inputSynapse, syn, 0);
  insertList(net->network[yin * net->w + xin].outputSynapse, syn, 0);
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
      printf(" %p syn %d  %d\n", syn, syn->weight.i,
      syn->input->output.i);
    #endif*/
      neuron->z.i += syn->input->output.i * syn->weight.i;
    }
    neuron->output.i = neuron->z.i + neuron->bias.i > 0;
  }else{
    neuron->z.fl = 0.;
    for(size_t i = 0; i < neuron->inputSynapse->len; ++i){
      syn = getDataList(neuron->inputSynapse, i);
    /*#ifdef DEBUG
      printf("syn %f  %f\n", syn->weight.fl,
      syn->input->output.fl);
    #endif*/
      neuron->z.fl += syn->input->output.fl * syn->weight.fl;
    }
    neuron->z.fl += neuron->bias.fl;
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

void trainingNeural(NeuralNet *net, flint *inputs, flint *outputs,
		    size_t nbTry, flint eta)
{
  for(size_t i = 0; i < nbTry; ++i)
    improveNeural(net, inputs + i * net->inputs, outputs + i * net->outputs,
eta);
}

void improveNeural(NeuralNet *net, flint *inputs, flint *outputs, flint eta)
{
  flint *a = malloc(sizeof(flint) * net->outputs), sum;

  setInputNeural(net, inputs);
  startNeural(net);
  printNeuralOutput(net);
  for(size_t i = 0; i < net->outputs; ++i){
    a[i] = getOutputNeural(net, i);
    if(net->network[net->w * (net->h - 1) + i].type == PERCEPTRON)
      net->network[net->w * (net->h - 1) + i].dJ.i = 
	  a[i].i * (1 - a[i].i) * (a[i].i - outputs[i].i);
    else
      net->network[net->w * (net->h - 1) + i].dJ.fl =
	  a[i].fl * (1. - a[i].fl) * (a[i].fl - outputs[i].fl);
  }

  for(size_t i = net->hiddenLayers; i > 0; --i)
    for(size_t j = 0; j < net->w; ++j){
      if(net->network[i * net->w + j].type == PERCEPTRON){
	sum.i = 0;
	for(size_t k = 0; k < 
	    net->network[i * net->w + j].outputSynapse->len; ++k){
	  Synapse *syn = getDataList(
net->network[i * net->w + j].outputSynapse, k);
	  sum.i += syn->output->dJ.i * syn->weight.i;
	}
	net->network[i * net->w + j].dJ.i = net->network[i *
net->w + j].output.i * (1 - net->network[i * net->w + j].output.i) * sum.i;
	net->network[i * net->w + j].bias.i += eta.i * net->network[i * net->w + j].dJ.i;
      }else{
	sum.fl = 0.;
	for(size_t k = 0; k < 
	  net->network[i * net->w + j].outputSynapse->len; ++k){
	  Synapse *syn = getDataList(net->network[i * net->w + j].outputSynapse, k);
	  sum.fl += syn->output->dJ.fl * syn->weight.fl;
	  syn->weight.fl += -eta.fl * syn->input->output.fl * syn->output->dJ.fl;
	}
	net->network[i * net->w + j].dJ.fl = net->network[i * 
net->w + j].output.fl * (1 - net->network[i * net->w + j].output.fl) * sum.fl;
	net->network[i * net->w + j].bias.fl += -eta.fl * net->network[i * net->w + j].dJ.fl;
      }
    }

  free(a);
}

void printNeuralOutput(NeuralNet *net)
{
  printf("Outputs:\n");
  for(size_t i = 0; i < net->outputs; ++i)
    printf("%d - %f\n", net->network[net->w * (net->h - 1) + i].output.i, net->network[net->w * (net->h - 1) + i].output.fl);
}

