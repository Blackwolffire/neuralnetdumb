/*	training.c
 *	sdouga_a
 */

void training(NeuralNet *net,List *letters,flint *outputs,flint eta,
							size_t nbTrain,size_t len,char *filename)
{
	flint *inputs;
	struct matrice *mat;

	assert(net && letters && outputs && filename);

	inputs = malloc(sizeof(flint) * 16 * list->len);

	for(size_t j = 0; j < nbTrain; ++j){
		mat = getDataList(list, j);
		printf("%d\n", mat->width * mat->height);
		for(size_t k = 0; k < 256; ++k)
			inputs[j * 256 + k].fl = mat[k];
	}
	trainingNeural(net, inputs, outputs, nbTrain, eta, len, filename);

	free(inputs);
}

