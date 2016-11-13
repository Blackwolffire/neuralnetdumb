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
			 size_t width, NeuronType type)
{
	size_t outputsIndex = 0;
  NeuralNet *net = malloc(sizeof(NeuralNet));
  flint value;

  if(input >= output && input >= width)
    net->w = input;
  else if(output >= input && output >= width)
    net->w = output;
	else
		net->w = width;

	net->type = type;
  net->inputs = input, net->outputs = output, net->hiddenLayers = hiddenLayers;
  net->h = hiddenLayers + 2;
	net->size = net->w * net->h;
  net->network = malloc(sizeof(Neuron) * net->size);


	outputsIndex = net->w * (net->h - 1);
  srand(time(NULL)); // maybe to replace
  for(size_t i = 0; i < net->size; ++i){

		net->network[i].sizeSynIn = net->network[i].sizeSynOut = 0;

    if(i < input){  																					// INPUT
			net->network[i].type = INPUT;
			net->network[i].inputs = NULL;
			net->network[i].outputs = malloc(sizeof(Synapse*) * net->w);

		}else if(i < outputsIndex + output && i >= outputsIndex){ // OUTPUT
			net->network[i].type = type;
			net->network[i].outputs = NULL;
			net->network[i].inputs = malloc(sizeof(Synapse*) * net->w);

		}else if(i >= net->w && i < outputsIndex)									// HIDDEN NEURON
			net->network[i].type = type;
			if(i / net->w == 1)
				net->network[i].inputs = malloc(sizeof(Synapse*) * input);
			else
				net->network[i].inputs = malloc(sizeof(Synapse*) * net->w);
			if(i >= outputsIndex - net->w)
				net->network[i].outputs = malloc(sizeof(Synapse*) * output);
			else
				net->network[i].outputs = malloc(sizeof(Synapse*) * net->w);

		else{																											// USELESS NEURON
			net->network[i].type = NONE;
    	net->network[i].inputs = NULL;
    	net->network[i].outputs = NULL;
		}
		
    switch(net->network[i].type){
      case PERCEPTRON:
				net->network[i].bias.i = rand() % MAX_RAND;
				if(i / net->w == 1)
	  			for(size_t j = 0; j < net->inputs; ++j){
	    			value.i = rand() % MAX_RAND;
	    			boundNeuron(net, value, j, 0, i % net->w, i / net->w);
	  			}
				if(i / net->w > 1)
	  			for(size_t j = 0; j < net->w; ++j){
	    			value.i = rand() % MAX_RAND;
	    			boundNeuron(net, value, j, i / net->w - 1, i % net->w, i / net->w);
	  			}
				break;
      case SIGMOID:
				net->network[i].bias.fl = MAX_SIGMOID / (rand() % MAX_RAND + 1);
				if(i / net->w == 1)
	  			for(size_t j = 0; j < net->inputs; ++j){
	    			value.fl = MAX_SIGMOID / (rand() % MAX_RAND + 1);
	    			boundNeuron(net, value, j, 0, i % net->w, i / net->w);
	 		 		}
				else if(i / net->w > 1)
	  			for(size_t j = 0; j < net->w; ++j){
	    			value.fl = MAX_SIGMOID / (rand() % MAX_RAND + 1);
	    			boundNeuron(net, value, j, i / net->w - 1, i % net->w, i / net->w);
	  			}
				break;
      /*case INPUT:  uselessness
				break;*/
      default:
				break;
    }
  }

  return net;
}

void destroyNeural(NeuralNet *net)
{
  for(size_t i = 0; i < net->size; ++i)
    switch(net->network[i].type){
      case PERCEPTRON:
			case SIGMOID:
				if(i / net->w == 1)
					for(size_t j = 0; j < net->inputs; ++j)
						free(net->network[i].intputs[j]);
				else
						for(size_t j = 0; j < net->w; ++j)
							free(net->network[i].inputs[j]);
				free(net->network[i].inputs);
				if(net->network[i].outputs)
					free(net->network[i].outputs);
				break;
      case INPUT:
				free(net->network[i].outputs);
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
	if(net->type == PERCEPTRON)
  	printf(" %zu -> %d -> %zu\n", yin * net->w + xin, weight.i, y * net->w + x);
	else
  	printf(" %zu -> %f -> %zu\n", yin * net->w + xin, weight.fl, y * net->w + x);
#endif
  net->network[y * net->w + x].inputs[net->network[y * net->w + x].sizeSynIn++];
  net->network[y * net->w + x].outputs[net->network[y * net->w + x].sizeSynOut++];
}

void setInputNeural(NeuralNet *net, flint *inputs)
{
  for(size_t i = 0; i < net->inputs; ++i){
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

  if(neuron->type == PERCEPTRON){
    neuron->z.i = 0;
    for(size_t i = 0; i < neuron->sizeSynIn; ++i)
      neuron->z.i += neuron->inputs[i]->input->output.i * neuron->inputs[i]->weight.i;

    neuron->output.i = (neuron->z.i + neuron->bias.i) > 0;
  }else{
    neuron->z.fl = 0.;
    for(size_t i = 0; i < neuron->inputSynapse->len; ++i)
      neuron->z.fl += neuron->inputs[i]->input->output.fl * neuron->inputs[i]->weight.fl;

    neuron->z.fl += neuron->bias.fl;
    neuron->output.fl = 1. / (1. + exp(-1. * neuron->z.fl));
  }
}

void startNeural(NeuralNet *net)
{
  for(size_t i = net->w; i < net->size; ++i)
    proceedNeuron(net->network + i);
}

void trainingNeural(NeuralNet *net, flint *inputs, flint *outputs,
		    size_t nbTry, flint eta)
{
  for(size_t i = 0; i < nbTry; ++i){
    improveNeural(net, inputs + i * net->inputs, outputs + i * net->outputs,
eta);
	}
}

void improveNeural(NeuralNet *net, flint *inputs, flint *outputs, flint eta)
{
  flint *a = malloc(sizeof(flint) * net->outputs), sum;

	setInputNeural(net, inputs);
  startNeural(net);
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
				net->w + j].output.i * (1 - net->network[i * net->w + j].output.i)
				 * sum.i;
				net->network[i * net->w + j].bias.i += eta.i *
				 net->network[i * net->w + j].dJ.i;
      }else{
				sum.fl = 0.;
				for(size_t k = 0; k < 
	  				net->network[i * net->w + j].outputSynapse->len; ++k){
	  			Synapse *syn = getDataList(net->network[i *
					 net->w + j].outputSynapse, k);
	  			sum.fl += syn->output->dJ.fl * syn->weight.fl;
	  			syn->weight.fl += -eta.fl * syn->input->output.fl *
					 syn->output->dJ.fl;
				}
				net->network[i * net->w + j].dJ.fl = net->network[i * 
				 net->w + j].output.fl * (1 - net->network[i * net->w + j].output.fl) * sum.fl;
				net->network[i * net->w + j].bias.fl += -eta.fl * net->network[i
				 * net->w + j].dJ.fl;
      }
		}

  free(a);
}

void printNeuralOutput(NeuralNet *net)
{
  printf("Outputs:\n");
  for(size_t i = 0; i < net->outputs; ++i)
    printf("%d - %f\n", net->network[net->w * (net->h - 1) + i].output.i,
						net->network[net->w * (net->h - 1) + i].output.fl);
}

