/* training.h
 * sdouga_a
 */

#ifndef TRAINING_H
#define TRAINING_H

#include "../neuralnet/neural_net.h"
#include "../damien/filtres.h"

void training(NeuralNet *net,List *letters,flint *outputs,flint eta,
							size_t nbTrain,size_t len,char *filename);

#endif

