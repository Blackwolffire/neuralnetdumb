/* list.c
 * sdouga_a
 */

#include "list.h"

List* createList()
{
  List *list = malloc(sizeof(List));
  list->len = 0, list->first = NULL;
  return list;
}

void destroyList(List *list)
{
  Element *elem = list->first, *toRm;

  if(!list)
    errx(EXIT_FAILURE, "destroyList: %s  %u\n", __FILE__, __LINE__);

  while(elem != NULL){
    toRm = elem, elem = elem->next;
    free(toRm->data);
    free(toRm);
  }
  free(list);
}

void insertList(List *list, void *data, size_t i)
{
  Element *elem = NULL, *newElem = NULL;

  if(!list)
    errx(EXIT_FAILURE, "initList: %s  %u\n", __FILE__, __LINE__);
  elem = list->first;

  if(i > list->len)
    errx(EXIT_FAILURE, "insertList: %s  %u\n", __FILE__, __LINE__);

  newElem = malloc(sizeof(Element));
  newElem->data = data;

  if(!i)
    newElem->next = list->first, list->first = newElem;
  else{
    for(size_t j = 1; j < i; ++j)
      elem = elem->next;
    newElem->next = elem->next, elem->next = newElem;
  }
  ++list->len;
}

