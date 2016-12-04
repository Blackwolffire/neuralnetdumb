/* neural_net.h
 * sdouga_a
 */

#ifndef NEURAL_NET_H
#define NEURAL_NET_H

#include "list.h"

#define MAX_RAND 1000

typedef enum NeuronType NeuronType;
typedef union flint flint;
typedef struct Synapse Synapse;
typedef struct Neuron Neuron;
typedef struct NeuralNet NeuralNet;


enum NeuronType{NONE = 0, INPUT = 1, PERCEPTRON = 2, SIGMOID = 3};

union flint // flint => fl(oat)int 					Captain Flint is coming!
{
//	char *c;
  int i;
  double fl;
};


/// Neuron

struct Neuron
{
  NeuronType type;
	Synapse **inputs;
  Synapse **outputs;
	size_t sizeSynIn;
	size_t sizeSynOut;
  flint bias;
  flint output;
  flint dJ;
};


/// Synapse

struct Synapse
{
  flint weight;
  Neuron *input;
  Neuron *output;
};


/// Neural Network

struct NeuralNet
{
  Neuron *network;
  NeuronType type;
  size_t inputs;
  size_t hiddenLayers;
  size_t outputs;
  size_t w;
  size_t h;
	size_t size;
};

/*
/// Training Data

typedef struct TrainingData TrainingData;
struct TrainingData
{
  flint input;
  flint output;
};
*/

NeuralNet* createNeural(size_t input,size_t output,size_t hiddenLayers,
												size_t *width,NeuronType type);
void destroyNeural(NeuralNet *net);
void boundNeuron(NeuralNet *net,flint weight,size_t xin,size_t yin,
								 size_t x,size_t y);
void setInputNeural(NeuralNet *net, flint *inputs);
flint getOutputNeural(NeuralNet *net, size_t i);
void proceedNeuron(Neuron *neuron);
void startNeural(NeuralNet *net);
void trainingNeural(NeuralNet *net,flint *inputs,flint *outputs,
										size_t nbTrain,flint eta,size_t len,char *filename);
double improveNeural(NeuralNet *net,flint *a,flint *outputs,size_t len,
										 flint eta);
void printNeuralOutput(NeuralNet *net);
void printNeural(NeuralNet *net);
void saveNeural(NeuralNet *net, char *filename);
NeuralNet *loadNeural(char *filename);

#endif

