# include <err.h>
# include <stdio.h>
#include <unistd.h>
# include <stdlib.h>
#include <fcntl.h>
#include <gdk/gdk.h>
#include "amin.h"

/*
struct matrice{
  int height;
  int width;
  int *mat;
};



struct coord
{
  int xmin;
  int xmax;
  int ymin;
  int ymax;
};
*/


void SegLine(struct matrice* matrix, List* list){
 int w=matrix->width;
 int h=matrix->height;
 struct coord *c=malloc(sizeof(struct coord));
 c->xmin=-1;
 c->xmax=-1;
 c->ymin=-1;
 c->ymax=-1;
 int online=0;
 for(int i=0;i<h;i++)
 {
  for(int j=0;j<w;j++)
  {
   if(*(matrix->mat+(i*w+j))==0)
   {
    online=1;
    if(c->xmin<0)
    {
     c->xmin=j;
     c->xmax=j;
     c->ymin=i;
     c->ymax=i;
    }
    else
    {
    if(c->xmin>j)
     c->xmin=j;
    if(c->xmax<j)
     c->xmax=j;
    if(c->ymin>i)
     c->ymin=i;
    if(c->ymax<i)
     c->ymax=i;
   }
   }    
 }
 if(c->xmin>=0&&online==0)
 {
   if(c->xmin<w-1)
   c->xmin-=1;
   if(c->xmin>0)
   c->xmax+=1;
   insertList(list, c, 0); 
  c=malloc(sizeof(struct coord));
  c->xmin=-1;
  c->xmax=-1;
  c->ymin=-1;
  c->ymax=-1;
 }
 online=0;
}
}
void SegLine_to_char(struct matrice* matrix, List* list, List* newlist){
 printList(list);
 int w=matrix->width;
 struct coord *c=malloc(sizeof(struct coord));
 struct coord *c2;
 int b=0;
 int l;
 c->xmin=-1;
 c->xmax=-1;
 c->ymin=-1;
 c->ymax=-1;
 int onchar=0;
 for(size_t n=list->len;n>0;n--)
 {
  printf("lol\n");
  c2=getDataList(list,n-1);
  c->ymin=c2->ymin;
	c->ymax=c2->ymax;
 for(int i=c2->xmin;i<=c2->xmax;i++)
 {
  for(int j=c2->ymin;j<=c2->ymax;j++)
  {
   if(*(matrix->mat+(j*w+i))==0)
   {
    onchar=1;
    if(c->xmin<0)
    {
     c->xmin=i;
     c->xmax=i;
    }
    else
    {
    if(c->xmin>i)
     c->xmin=i;
    if(c->xmax<i)
     c->xmax=i;
   }
   }
}
 if(c->xmin>=0&&onchar==0)
 {
   printf("lol\n");
	 while (c->ymax>c->ymin&&b==0)
	 {
		 l=c->xmin;
    while(l<=c->xmax&&b==0)
		{
			if(*(matrix->mat+(c->ymax*w+l))==0)
				b=2;
			l++;
		}
		if(!b)
		{
			c->ymax--;
		}
	 }
	 b=0;
   while (c->ymin<c->ymax&&b==0)
	 {
		 l=c->xmin;
    while(l<=c->xmax&&b==0)
		{
			if(*(matrix->mat+(c->ymin*w+l))==0)
				b=2;
			l++;
		}
		if(!b)
		{
			c->ymin++;
		}
	 }
	 b=0;
	insertList(newlist,c,0);
  c=malloc(sizeof(struct coord));
  printf("lol");
  c->xmin=-1;
  c->xmax=-1;
  c->ymin=c2->ymin;
	c->ymax=c2->ymax;
}
 onchar=0;
}
}
}
void Seg_char(struct matrice *matrix, List *list2)
{
 List* list=createList();
 SegLine(matrix, list);
 SegLine_to_char(matrix, list,list2);
 destroyList(list, free);
}

/*
int main()
{
   if (argc<2)
	errx(1 , "No image.");
   init_sdl();
   SDL_Surface* img = load_image(argv[1]);
   display_image(img);
   for (int x = 0; x < img->w; x++)
   {
	for(int y = 0; y <img->h; y++)
        {
		Uint32 pxl = getpixel(img, x, y);
		Uint8 r, g, b;
		SDL_GetRGB(pxl , img->format, &r, &g, &b);
		float gray = 0.3 * r + 0.59 * g + 0.11 * b;
                if(gray>127)
                 gray=255;
                else
                 gray=0;
		pxl = SDL_MapRGB(img->format, gray, gray, gray);
		putpixel(img, x, y, pxl);
 	}
   }
   display_image(img);
   List* list = Seg_char(img);
   display_image(img);
   return 0;
}*/
