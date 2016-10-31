/* neural_net.h
 * sdouga_a
 */

#ifndef NEURAL_NET_H
#define NEURAL_NET_H

#define DEBUG

#include "list.h"



#define MAX_RAND 1000

typedef enum NeuronType NeuronType;
enum NeuronType{NONE = 0, INPUT = 1, PERCEPTRON = 2, SIGMOID = 3};

typedef union flint flint;
union flint
{
  int i;
  double fl;
};


/// Neuron

typedef struct Neuron Neuron;
struct Neuron
{
  NeuronType type;
  List *inputSynapse;
  flint bias;
  flint z;
  flint output;
  List *outputSynapse;
  flint dJ;
};


/// Synapse

typedef struct Synapse Synapse;
struct Synapse
{
  flint weight;
  Neuron *input;
  Neuron *output;
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


/// Training Data

typedef struct TrainingData TrainingData;
struct TrainingData
{
  flint input;
  flint output;
};


NeuralNet* createNeural(size_t input, size_t output, size_t hiddenLayers,
			NeuronType *type);
void destroyNeural(NeuralNet *net);
void boundNeuron(NeuralNet *net, flint weight, size_t  xin, size_t  yin,
		 size_t  x, size_t y);
void setInputNeural(NeuralNet *net, flint *inputs);
flint getOutputNeural(NeuralNet *net, size_t i);
void proceedNeuron(Neuron *neuron);
void startNeural(NeuralNet *net);
void trainingNeural(NeuralNet *net, flint *inputs, flint *outputs, size_t nbTry, flint eta);
void improveNeural(NeuralNet *net, flint *inputs, flint *outputs, flint eta);
void printNeuralOutput(NeuralNet *net);

#endif

