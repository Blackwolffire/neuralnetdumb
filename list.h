/* list.h
 * sdouga_a
 */

#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <err.h>

typedef struct Element Element;
struct Element
{
	void *data;
	Element *next;
};

typedef struct List List;
struct List
{
	size_t len;
	Element *first;
};

#endif

