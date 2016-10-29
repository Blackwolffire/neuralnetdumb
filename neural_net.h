/* neural_net.h
 * sdouga_a
 */

#ifndef NEURAL_NET_H
#define NEURAL_NET_H

#include "list.h"

typedef enum NeuronType NeuronType;
enum NeuronType{NONE, PERCEPTRON, SIGMOID};

typedef union flint flint;
union flint
{
	int i;
	float fl;
};


/// Synapse

typedef struct Synaspe Synaspe;
struct Synapse
{
	flint weight;
	
};


/// Neuron

typedef struct Neuron Neuron;
struct Neuron
{
	NeuronType type;
	List inputSynapse;
	flint bias;
	flint z;
	flint output;
	//List outputSynapse;
};


/// Neural Network

typedef struct NeuralNet NeuralNet;
struct NeuralNet
{
	Neuron *network;
	size_t inputs;
	size_t hiddenLayers;
	size_t outputs;
	size_t w;
	size_t h;
};

void initNeural(NeuralNet net, size_t input, size_t output, size_t hiddenLayers);
void destroyNeural(NeuralNet net);

#endif

