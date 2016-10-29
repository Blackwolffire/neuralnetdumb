/* list.c
 * sdouga_a
 */

#include "list.h"

void initList(List list)
{
	list.len = 0, list.first = NULL;
}

void destroyList(List list)
{
	Element *elem = list.first, *toRm;

	while(elem != NULL){
		toRm = elem, elem = elem->next;
		free(toRm->data);
		free(toRm);
	}
	list.first = NULL, list.len = 0;
}

void insertList(List list, void *data, size_t i)
{
	Element *elem = list.first, *newElem = NULL;

	if(i >= list.len)
		errx(EXIT_FAILURE, "insertList: %s  %u\n", __FILE__, __LINE__);

	newElem = malloc(sizeof(Element));
	newElem->data = data;

	if(!i)
		mewElem->next = list.first, list.first = newElem;
	else{
		for(size_t j = 1; j < i; ++j)
			elem = elem->next;
		newElem->next = elem->next, elem->next = newElem;
	}
	++list.len;
}

