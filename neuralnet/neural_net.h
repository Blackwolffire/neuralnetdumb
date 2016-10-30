/* neural_net.h
 * sdouga_a
 */

#ifndef NEURAL_NET_H
#define NEURAL_NET_H

#define DEBUG

#include "list.h"

typedef enum NeuronType NeuronType;
enum NeuronType{NONE = 0, PERCEPTRON = 1, SIGMOID = 2};

typedef union flint flint;
union flint
{
  int i;
  float fl;
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
  //List outputSynapse;
};


/// Synapse

typedef struct Synapse Synapse;
struct Synapse
{
  flint weight;
  Neuron *input;
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

NeuralNet* createNeural(size_t input, size_t output, size_t hiddenLayers,
			NeuronType *type, flint *bias);
void destroyNeural(NeuralNet *net);
void boundNeuron(NeuralNet *net, flint weight, size_t  xin, size_t  yin,
		 size_t  x, size_t y);
void setInputNeural(NeuralNet *net, flint *inputs);
flint getOutputNeural(NeuralNet *net, size_t i);
void startNeural(NeuralNet *net);

#endif

