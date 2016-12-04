/* neural_net.c
 * sdouga_a
Sorry if the code seems tight,i had to respect the only 80 column rule likeThat
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "neural_net.h"

NeuralNet* createNeural(size_t input,size_t output,size_t hiddenLayers,
												size_t *width,NeuronType type)
{
  size_t outputsIndex = 0;
  NeuralNet *net = malloc(sizeof(NeuralNet));
  flint value;

	assert(width);

  net->w = 0;
  for(size_t i = 0; i < hiddenLayers; ++i)
    if(net->w < width[i])
      net->w = width[i];
  if(input >= output && input >= net->w)
    net->w = input;
  else if(output >= input && output >= net->w)
    net->w = output;

  net->type = type;
  net->inputs = input, net->outputs = output;
 	net->hiddenLayers = hiddenLayers;
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

		}else if(i>=net->w && i<outputsIndex
						 && i%net->w<width[i/net->w-1]){// HIDDEN NEURON

			net->network[i].type = type;
			if(i / net->w == 1)
				net->network[i].inputs = malloc(sizeof(Synapse*) * input);
			else
				net->network[i].inputs = malloc(sizeof(Synapse*)
											 									*width[i/net->w-2]);
			if(i >= outputsIndex - net->w)
				net->network[i].outputs = malloc(sizeof(Synapse*) * output);
			else
				net->network[i].outputs = malloc(sizeof(Synapse*)
																				 *width[i/net->w]);

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
	    			boundNeuron(net,value,j,i/net->w-1,i%net->w,i/net->w);
	  			}
				break;
      case SIGMOID:
				net->network[i].bias.fl = rand();
				net->network[i].bias.fl =
							 	(net->network[i].bias.fl/1000000000.0
								 - (int)(net->network[i].bias.fl/1000000000))*2.-1.;
				if(i / net->w == 1)
	  			for(size_t j = 0; j < net->inputs; ++j){
	    			value.fl = rand();
	    			value.fl=(value.fl/1000000000.0-(int)(value.fl/1000000000))*2.-1.;
	    			boundNeuron(net, value, j, 0, i % net->w, i / net->w);
	  			}
				else if(i / net->w > 1)
	  			for(size_t j = 0; j < width[i /net->w - 2]; ++j){
	    			value.fl = rand();
	    			value.fl=(value.fl/1000000000.0-(int)(value.fl/1000000000))*2.-1.;
	    			boundNeuron(net,value,j,i/net->w-1,i%net->w,i/net->w);
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
	assert(net);

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

void boundNeuron(NeuralNet *net,flint weight,size_t xin,size_t yin,
								 size_t x,size_t y)
{
  Synapse *syn = NULL;

  assert(net && xin < net->w && yin < net->h && y > yin);

  syn = malloc(sizeof(Synapse));
  syn->input = net->network + yin * net->w + xin;
  syn->output = net->network + y * net->w + x;
  syn->weight = weight;
	net->network[y*net->w+x].inputs[net->network[y*net->w+x].sizeSynIn++]=syn;
  net->network[yin*net->w+xin].outputs[net->network[
							 yin*net->w+xin].sizeSynOut++]=syn;
}

void setInputNeural(NeuralNet *net, flint *inputs)
{
	assert(net && inputs);
  for(size_t i = 0; i < net->inputs; ++i)
    net->network[i].output = inputs[i];
}

flint getOutputNeural(NeuralNet *net, size_t i)
{
  assert(net && i <= net->outputs);
  return net->network[net->w * (net->h - 1) + i].output;
}

void proceedNeuron(Neuron *neuron)
{
	flint z;
  if(neuron->type == NONE)
    return;

  if(neuron->type == PERCEPTRON){
    z.i = 0;
    for(size_t i = 0; i < neuron->sizeSynIn; ++i)
      z.i+=neuron->inputs[i]->input->output.i*neuron->inputs[i]->
									 weight.i;

    neuron->output.i = (z.i + neuron->bias.i) > 0;
  }else{
    z.fl = 0.;
    for(size_t i = 0; i < neuron->sizeSynIn; ++i)
      z.fl+=neuron->inputs[i]->input->output.fl
										*neuron->inputs[i]->weight.fl;

    z.fl += neuron->bias.fl;
    neuron->output.fl = 1. / (1. + exp(-z.fl));
  }
}

void startNeural(NeuralNet *net)
{
	assert(net);
  for(size_t i = net->w; i < net->size; ++i)
    proceedNeuron(net->network + i);
}

void trainingNeural(NeuralNet *net,flint *inputs,flint *outputs,
										size_t nbTrain,flint eta,size_t len,char *filename)
{
	unsigned short int percent = 200;
	double cost = 1., bestScore = 0.4;
	flint *a = NULL;

	assert(net && inputs && outputs && filename);

	a = malloc(sizeof(flint) * nbTrain * (net->size - net->w));
	for(size_t j =0; j < len; ++j){
  	for(size_t i = 0; i < nbTrain; ++i){
			size_t indexA = i * (net->size - net->w);
			setInputNeural(net, inputs + i * net->inputs);
			startNeural(net);
			for(size_t k = net->w; k < net->size; ++k)
				a[indexA + k - net->w] = net->network[k].output;
		}
    cost = improveNeural(net, a, outputs, nbTrain, eta);
		if(percent / 10 != j * 10 / len){
				percent = j * 100 / len;
				printf(" %d%%\tcost: %f\n", percent, cost);
		}
		if(cost <= bestScore - 0.0001){
			bestScore = cost;
			printf("SAVE - cost: %f\n", cost);
			saveNeural(net, filename);
		}
	}
	printf(" 100%%\tcost: %f\n", cost);
	free(a);
}

double improveNeural(NeuralNet *net,flint *a,flint *outputs,size_t len,
										 flint eta)
{
	size_t outputIndex = net->size - net->w, indexA = net->size - net->w;
	double cost = 0.;
  flint sum;

	for(size_t l = len - 1; l < len; --l){
	  for(size_t i = 0; i < net->outputs; ++i){
			size_t index = (l + 1) * indexA - net->w + i;
 	   	if(net->network[outputIndex + i].type == PERCEPTRON){
  	    net->network[outputIndex+i].dJ.i=a[index].i*(1-a[index].i)*(a[index].i
																				 -outputs[l*net->outputs+i].i);
				net->network[outputIndex+i].bias.i-=eta.i*net->network[outputIndex+i]
																						.dJ.i;
				cost += pow(fabs(outputs[l * net->outputs + i].i - a[index].i), 2);
			}else{
      	net->network[outputIndex+i].dJ.fl=a[index].fl*(1.-a[index].fl)
																					*(a[index].fl
																					-outputs[l*net->outputs+i].fl);
				net->network[outputIndex+i].bias.fl-=eta.fl*net->network[outputIndex
																																 +i].dJ.fl;
				cost += pow(fabs(outputs[l * net->outputs + i].fl - a[index].fl), 2);
			}
  	}

  	for(size_t i = net->hiddenLayers; i <= net->hiddenLayers; --i)
    	for(size_t j = 0; j < net->w; ++j){
				size_t index = i * net->w + j;

      	if(net->network[index].type != NONE && net->type == PERCEPTRON){
					sum.i = 0;
					for(size_t k = 0; k < net->network[index].sizeSynOut; ++k){
	  				sum.i+=net->network[index].outputs[k]->output->dJ.i*net->network[
									 index].outputs[k]->weight.i;
	  				net->network[index].outputs[k]->weight.i-=eta.i
										*net->network[index].output.i
										*net->network[index].outputs[k]->output->dJ.i;
					}
					if(net->network[index].type == PERCEPTRON){
						net->network[index].dJ.i=a[l*indexA-net->w+index].i*(1
																		-a[l*indexA-net->w+index].i)*sum.i;
						net->network[index].bias.i += -eta.i * net->network[index].dJ.i;
					}

      	}else if(net->network[index].type != NONE && net->type == SIGMOID){
					sum.fl = 0.;
					for(size_t k = 0; k < net->network[index].sizeSynOut; ++k){
	  				sum.fl+=net->network[index].outputs[k]->output->dJ.fl*net->network
										[index].outputs[k]->weight.fl;
	  				net->network[index].outputs[k]->weight.fl-=eta.fl
										*a[l*indexA-net->w+index].fl
										*net->network[index].outputs[k]->output->dJ.fl;
					}
					if(net->network[index].type == SIGMOID){
						net->network[index].dJ.fl=a[l*indexA-net->w+index].fl*(1
																			-a[l*indexA-net->w+index].fl)*sum.fl;
						net->network[index].bias.fl -= eta.fl * net->network[index].dJ.fl;
					}
      	}
			}
	}
	return cost;
}

void printNeuralOutput(NeuralNet *net)
{
	size_t outputIndex;

	assert(net);

	outputIndex = net->size - net->w;

  printf("Outputs:\n");
  for(size_t i = 0; i < net->outputs; ++i)
    printf("%d - %f\n",net->network[outputIndex+i].output.i,
					net->network[outputIndex+i].output.fl);
}

void printNeural(NeuralNet *net)
{
	assert(net);
	for(size_t i = 0; i < net->h; ++i)
		for(size_t j = 0; j < net->w; ++j)
			if(net->type == PERCEPTRON){
				printf("(%zu, %zu) bias = %d \n ", j, i, net->network[i * net->w + j].bias.i);
				for(size_t k = 0; k < net->network[i * net->w + j].sizeSynOut; ++k)
					printf("%d\t", net->network[i * net->w + j].outputs[k]->weight.i);
				printf("\n");
			}else{
				printf("(%zu, %zu) bias = %f \n ", j, i, net->network[i * net->w + j].bias.fl);
				for(size_t k = 0; k < net->network[i * net->w + j].sizeSynOut; ++k)
					printf("%f\t", net->network[i * net->w + j].outputs[k]->weight.fl);
				printf("\n");
			}
}

void saveNeural(NeuralNet *net, char *filename)
{
	FILE *file = NULL;

	assert(filename);

	file = fopen(filename, "w");
	assert(file);

	fprintf(file, "%zu %zu %zu ", net->inputs, net->outputs, net->hiddenLayers);
	for(size_t i = 0; i < net->hiddenLayers; ++i){
		size_t w = 0;
		while(net->network[(i + 1) * net->w + w].type != NONE && w < net->w)
			++w;
		fprintf(file, "%zu ", w);
	}
	fprintf(file, "%d\n", net->type);

	for(size_t i = 0; i < net->inputs; ++i)
		for(size_t j = 0; j < net->network[i].sizeSynOut; ++j)
			fwrite(&net->network[i].outputs[j]->weight, sizeof(flint), 1, file);

	for(size_t i = 1; i < net->h; ++i){
		for(size_t j = 0; j < net->w; ++j){
			size_t index = i * net->w + j;
			if(!net->network[index].type)
				continue;
			fwrite(&net->network[index].bias, sizeof(flint), 1, file);
			for(size_t k = 0; k < net->network[index].sizeSynOut; ++k)
				fwrite(&net->network[index].outputs[k]->weight, sizeof(flint), 1, file);
		}
	}
	fclose(file);
}

NeuralNet *loadNeural(char *filename)
{
	char c = 0;
	size_t ioh[3] = {0}, *w;
	NeuronType type;
	FILE *file = NULL;
	NeuralNet *net = NULL;

	assert(filename);
	file = fopen(filename, "r");
	assert(file);

	for(size_t i = 0; i < 3; ++i)
		while((c = fgetc(file)) != ' ')
			 ioh[i]= ioh[i] * 10 + c - '0';

	w = malloc(sizeof(size_t) * ioh[2]);

	for(size_t i = 0; i < ioh[2]; ++i)
		while((c = fgetc(file)) != ' ')
			 w[i]= w[i] * 10 + c - '0';

	type = fgetc(file) - '0';
	assert((c = fgetc(file)) == '\n');

	net = createNeural(ioh[0], ioh[1], ioh[2], w, type);

	for(size_t i = 0; i < net->inputs; ++i)
		for(size_t j = 0; j < net->network[i].sizeSynOut; ++j)
			fread(&net->network[i].outputs[j]->weight, sizeof(flint), 1, file);

	for(size_t i = 1; i < net->h; ++i)
		for(size_t j = 0; j < net->w; ++j){
			size_t index = i * net->w + j;
			if(!net->network[index].type)
				continue;
			fread(&net->network[index].bias, sizeof(flint), 1, file);
			for(size_t k = 0; k < net->network[index].sizeSynOut; ++k)
				fread(&net->network[index].outputs[k]->weight, sizeof(flint), 1, file);
		}

	fclose(file);
	free(w);

	return net;
}

