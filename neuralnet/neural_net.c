/* neural_net.c
 * sdouga_a
 */



///////////////////////////   METTRE A JOUR LES GESTIONS D'ERREURS


#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "neural_net.h"

NeuralNet* createNeural(size_t input, size_t output, size_t hiddenLayers,
			 size_t *width, NeuronType type)
{
  size_t outputsIndex = 0;
  NeuralNet *net = malloc(sizeof(NeuralNet));
  flint value;

  net->w = 0;
  for(size_t i = 0; i < hiddenLayers; ++i)
    if(net->w < width[i])
      net->w = width[i];
  if(input >= output && input >= net->w)
    net->w = input;
  else if(output >= input && output >= net->w)
    net->w = output;

  net->type = type;
  net->inputs = input, net->outputs = output, net->hiddenLayers = hiddenLayers;
  net->h = hiddenLayers + 2;
  net->size = net->w * net->h;
  net->network = malloc(sizeof(Neuron) * net->size);


  srand(time(NULL)); // maybe to replace
  outputsIndex = net->size - net->w;
  for(size_t i = 0; i < net->size; ++i){
    net->network[i].sizeSynIn = net->network[i].sizeSynOut = 0;

    if(i < input){  							    // INPUT
			net->network[i].type = INPUT;
			net->network[i].inputs = NULL;
			net->network[i].outputs = malloc(sizeof(Synapse*) * width[0]);

		}else if(i < outputsIndex + output && i >= outputsIndex){ // OUTPUT
			net->network[i].type = type;
			net->network[i].outputs = NULL;
			net->network[i].inputs = malloc(sizeof(Synapse*) * net->w);

		}else if(i >= net->w && i < outputsIndex && i % net->w < width[i / net->w - 1]){ // HIDDEN NEURON
			net->network[i].type = type;
			if(i / net->w == 1)
				net->network[i].inputs = malloc(sizeof(Synapse*) * input);
			else
				net->network[i].inputs = malloc(sizeof(Synapse*) * width[i / net->w - 2]);
			if(i >= outputsIndex - net->w)
				net->network[i].outputs = malloc(sizeof(Synapse*) * output);
			else
				net->network[i].outputs = malloc(sizeof(Synapse*) * width[i / net->w]);

		}else{						  // USELESS NEURON
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
				else if(i / net->w > 1)
	  			for(size_t j = 0; j < net->w; ++j){
	    			value.i = rand() % MAX_RAND;
	    			boundNeuron(net, value, j, i / net->w - 1, i % net->w, i / net->w);
	  			}
				break;
      case SIGMOID:
				net->network[i].bias.fl = rand();
				net->network[i].bias.fl = (net->network[i].bias.fl / 1000000000.0 - (int)(net->network[i].bias.fl / 1000000000)) * 2. - 1.;
				if(i / net->w == 1)
	  			for(size_t j = 0; j < net->inputs; ++j){
	    			value.fl = rand();
	    			value.fl = (value.fl / 1000000000.0 - (int)(value.fl / 1000000000)) * 2. - 1.;
	    			boundNeuron(net, value, j, 0, i % net->w, i / net->w);
	  			}
				else if(i / net->w > 1)
	  			for(size_t j = 0; j < width[i /net->w - 2]; ++j){
	    			value.fl = rand();
	    			value.fl = (value.fl / 1000000000.0 - (int)(value.fl / 1000000000)) * 2. - 1.;
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
				for(size_t j = 0; j < net->network[i].sizeSynIn; ++j)
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
  	printf("( %zu, %zu) -> %d -> ( %zu, %zu)\n", yin, xin, weight.i, y, x);
	else
  	printf("( %zu, %zu) -> %f -> ( %zu, %zu)\n", yin, xin, weight.fl, y, x);
#endif
  net->network[y * net->w + x].inputs[net->network[y * net->w + x].sizeSynIn++] = syn;
  net->network[yin * net->w + xin].outputs[net->network[yin * net->w + xin].sizeSynOut++] = syn;
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
  if(neuron->type == NONE)
    return;

  if(neuron->type == PERCEPTRON){
    neuron->z.i = 0;
    for(size_t i = 0; i < neuron->sizeSynIn; ++i)
      neuron->z.i += neuron->inputs[i]->input->output.i * neuron->inputs[i]->weight.i;

    neuron->output.i = (neuron->z.i - neuron->bias.i) > 0;
  }else{
    neuron->z.fl = 0.;
    for(size_t i = 0; i < neuron->sizeSynIn; ++i)
      neuron->z.fl += neuron->inputs[i]->input->output.fl * neuron->inputs[i]->weight.fl;

    neuron->z.fl += neuron->bias.fl;
    neuron->output.fl = 1. / (1. + exp(-neuron->z.fl));
  }
}

void startNeural(NeuralNet *net)
{
  for(size_t i = net->w; i < net->size; ++i)
    proceedNeuron(net->network + i);
}

void trainingNeural(NeuralNet *net, flint *inputs, flint *outputs,
		    						size_t nbTrain, flint eta, size_t len)
{
	unsigned short int percent = 200;
	flint *a = NULL;

	assert(net && inputs && outputs);

	a = malloc(sizeof(flint) * nbTrain * (net->size - net->w));
	for(size_t j =0; j < len; ++j){
		if(percent / 10 != j * 10 / len){
				percent = j * 100 / len;
				printf(" %d%%\n", percent);
		}
  	for(size_t i = 0; i < nbTrain; ++i){
			size_t indexA = i * (net->size - net->w);
			setInputNeural(net, inputs + i * net->inputs);
			startNeural(net);
			for(size_t k = net->w; k < net->size; ++k)
				a[indexA + k - net->w] = net->network[k].output;
		}
    improveNeural(net, a, outputs, nbTrain, eta);
	}
	free(a);
}

void improveNeural(NeuralNet *net, flint *a, flint *outputs, size_t len, flint eta)
{
	size_t outputIndex = net->size - net->w, indexA = net->size - net->w;
  flint sum;

	for(size_t l = len - 1; l < len; --len){
	  for(size_t i = 0; i < net->outputs; ++i){
 	   	if(net->network[outputIndex + i].type == PERCEPTRON){
  	    net->network[outputIndex + i].dJ.i = a[i].i * (1 - a[i].i) * (a[i].i - outputs[i].i);
				net->network[outputIndex + i].bias.i += -eta.i * net->network[outputIndex + i].dJ.i;
			}else{
      	net->network[outputIndex + i].dJ.fl = a[(l + 1) * indexA - net->w + i].fl * (1. - a[(l+1)* indexA - net->w + i].fl) * (a[(l+1)* indexA - net->w + i].fl - outputs[l * net->outputs + i].fl);
				net->network[outputIndex + i].bias.fl -= eta.fl * net->network[outputIndex + i].dJ.fl;
			}
  	}

  	for(size_t i = net->hiddenLayers; i <= net->hiddenLayers; --i)
    	for(size_t j = 0; j < net->w; ++j){
				size_t index = i * net->w + j;

      	if(net->network[index].type != NONE && net->type == PERCEPTRON){
					sum.i = 0;
					for(size_t k = 0; k < net->network[index].sizeSynOut; ++k){
	  				sum.i += net->network[index].outputs[k]->output->dJ.i * net->network[index].outputs[k]->weight.i;
	  				net->network[index].outputs[k]->weight.i += -eta.i * net->network[index].output.i * net->network[index].outputs[k]->output->dJ.i;
					}
					net->network[index].dJ.i = 
net->network[index].output.i * (1 - net->network[index].output.i) * sum.i;
					net->network[index].bias.i += -eta.i * net->network[index].dJ.i;

      	}else if(net->network[index].type != NONE && net->type == SIGMOID){
					sum.fl = 0.;
					for(size_t k = 0; k < net->network[index].sizeSynOut; ++k){
	  				sum.fl += net->network[index].outputs[k]->output->dJ.fl * net->network[index].outputs[k]->weight.fl;
	  				net->network[index].outputs[k]->weight.fl -= eta.fl * a[l * indexA - net->w + index].fl * net->network[index].outputs[k]->output->dJ.fl;
					}
					if(net->network[index].type == SIGMOID){
						net->network[index].dJ.fl = a[l * indexA - net->w + index].fl * (1 - a[l * indexA - net->w + index].fl) * sum.fl;
						net->network[index].bias.fl -= eta.fl * net->network[index].dJ.fl;
					}
      	}
			}
	}
}

void printNeuralOutput(NeuralNet *net)
{
	size_t outputIndex;

	if(!net)
		errx(EXIT_FAILURE, "printNeuralOutput: %s  %d", __FILE__, __LINE__);

	outputIndex = net->size - net->w;

  printf("Outputs:\n");
  for(size_t i = 0; i < net->outputs; ++i)
    printf("%d - %f\n", net->network[outputIndex + i].output.i, net->network[outputIndex + i].output.fl);
}

