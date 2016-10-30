# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include <err.h>
# include "quartet.h"
# include "pixel_operations.h"

void wait_for_keypressed(void) {
  SDL_Event             event;
  // Infinite loop, waiting for event
  for (;;) {
    // Take an event
    SDL_PollEvent( &event );
    // Switch on event type
    switch (event.type) {
    // Someone pressed a key -> leave the function
    case SDL_KEYDOWN: return;
    default: break;
    }
  // Loop until we got the expected event
  }
}
struct coord Get_min_max_val(int* matrix, int w, int h, int i, int j)
{
 *(matrix+(i*w+j))=2;
 struct coord c={.xmin=j, .xmax=j, .ymin=i, .ymax=i};
 struct coord c1={.xmin=0,.xmax=0,.ymin=0,.ymax=0};
 if(j>0&&*(matrix+i*w+j-1)==1)
  {
   c1=Get_min_max_val(matrix,w,h,i,j-1);
   if(c1.xmin<c.xmin)
    c.xmin=c1.xmin;
   if(c1.ymin<c.ymin)
    c.ymin=c1.ymin;
   if(c1.ymax>c.ymax)
    c.ymax=c1.ymax;
   if(c1.xmax>c.xmax)
    c.xmax=c1.xmax;
   if(i>0&&*(matrix+(i-1)*w+j-1)==1)
   {
    c1=Get_min_max_val(matrix,w,h,i-1,j-1);
    if(c1.xmin<c.xmin)
     c.xmin=c1.xmin;
    if(c1.ymin<c.ymin)
     c.ymin=c1.ymin;
    if(c1.ymax>c.ymax)
     c.ymax=c1.ymax;
    if(c1.xmax>c.xmax)
     c.xmax=c1.xmax;
   }
   if(i+1<h&&*(matrix+(i+1)*w+j-1)==1)
   {
    c1=Get_min_max_val(matrix,w,h,i+1,j-1);
    if(c1.xmin<c.xmin)
     c.xmin=c1.xmin;
    if(c1.ymin<c.ymin)
     c.ymin=c1.ymin;
    if(c1.ymax>c.ymax)
     c.ymax=c1.ymax;
    if(c1.xmax>c.xmax)
     c.xmax=c1.xmax;
   }
  }
 if(j+1<w&&*(matrix+i*w+j+1)==1)
  {
   c1=Get_min_max_val(matrix,w,h,i,j+1);
   if(c1.xmin<c.xmin)
    c.xmin=c1.xmin;
   if(c1.ymin<c.ymin)
    c.ymin=c1.ymin;
   if(c1.ymax>c.ymax)
    c.ymax=c1.ymax;
   if(c1.xmax>c.xmax)
    c.xmax=c1.xmax;
   if(i>0&&*(matrix+(i-1)*w+j+1)==1)
   {
    c1=Get_min_max_val(matrix,w,h,i-1,j+1);
    if(c1.xmin<c.xmin)
     c.xmin=c1.xmin;
    if(c1.ymin<c.ymin)
     c.ymin=c1.ymin;
    if(c1.ymax>c.ymax)
     c.ymax=c1.ymax;
    if(c1.xmax>c.xmax)
     c.xmax=c1.xmax;
   }
   if(i+1<h&&*(matrix+(i+1)*w+j+1)==1)
   {
    c1=Get_min_max_val(matrix,w,h,i+1,j+1);
    if(c1.xmin<c.xmin)
     c.xmin=c1.xmin;
    if(c1.ymin<c.ymin)
     c.ymin=c1.ymin;
    if(c1.ymax>c.ymax)
     c.ymax=c1.ymax;
    if(c1.xmax>c.xmax)
     c.xmax=c1.xmax;
   }
  }
 if(i+1<h&&*(matrix+(i+1)*w+j)==1)
   {
    c1=Get_min_max_val(matrix,w,h,i+1,j);
    if(c1.xmin<c.xmin)
     c.xmin=c1.xmin;
    if(c1.ymin<c.ymin)
     c.ymin=c1.ymin;
    if(c1.ymax>c.ymax)
     c.ymax=c1.ymax;
    if(c1.xmax>c.xmax)
     c.xmax=c1.xmax;
   }
 if(i>0&&*(matrix+(i-1)*w+j)==1)
   {
    c1=Get_min_max_val(matrix,w,h,i-1,j);
    if(c1.xmin<c.xmin)
     c.xmin=c1.xmin;
    if(c1.ymin<c.ymin)
     c.ymin=c1.ymin;
    if(c1.ymax>c.ymax)
     c.ymax=c1.ymax;
    if(c1.xmax>c.xmax)
     c.xmax=c1.xmax;
   }

 return c;
}
void SegChar(SDL_Surface *img, int* matrix, int w, int h)
{
 for(int i=0;i<h;i++)
 {
  for(int j=0;j<w;j++)
  {
   if(*(matrix+(i*w+j))==1)
   {
    struct coord c=Get_min_max_val(matrix,w,h,i,j);
    if(c.xmin>0)
     c.xmin-=1;
    if(c.ymin>0)
     c.ymin-=1;
    if(c.xmax+1<w)
     c.xmax+=1;
    if(c.ymax+1<h)
     c.ymax+=1;
    for(int k=c.xmin;k<=c.xmax;++k)
    {
     putpixel(img,k,c.ymin,500);
     putpixel(img,k,c.ymax,500);
    }
    for(int k=c.ymin;k<=c.ymax;++k)
    {
     putpixel(img,c.xmin,k,500);
     putpixel(img,c.xmax,k,500);
    }

   }
}
}
}
void print_Mat(int *matrix,int h, int w)
{  
 for(int i=0; i<h; ++i)
  { 
   for(int j=0;j<w;++j)
   {
    printf("%d",*(matrix+(i*w+j)));
   }
   //printf("");
  }
}    
void Seg_char(SDL_Surface *img)
{
 int w=img->w;
 int h=img->h;
 SDL_PixelFormat* format=img->format;
 int *matrix = malloc(sizeof(int)*(h)*(w));
 for (int i = 0;i < h;i++)
 {
  for (int j = 0;j < w;j++)
  {
   Uint32 pxl = getpixel(img, j , i);
   Uint8 r,g,b;
   SDL_GetRGB(pxl,format, &r, &g, &b);
   if (r == 0)
    *(matrix+(i*w+j)) = 1;
   else
    *(matrix+(i*w+j)) = 0;
  }
 }
 //print_Mat(matrix, h, w);
 SegChar(img,matrix,w,h);
}
void init_sdl(void) {
  // Init only the video part
  if( SDL_Init(SDL_INIT_VIDEO)==-1 ) {
    // If it fails, die with an error message
    errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
  }
  // We don't really need a function for that ...
}


SDL_Surface* load_image(char *path) {
  SDL_Surface          *img;
  // Load an image using SDL_image with format detection
  img = IMG_Load(path);
  if (!img)
    // If it fails, die with an error message
    errx(3, "can't load %s: %s", path, IMG_GetError());
  return img;
}


SDL_Surface* display_image(SDL_Surface *img) {
  SDL_Surface          *screen;
  // Set the window to the same size as the image
  screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
  if ( screen == NULL ) {
    // error management
    errx(1, "Couldn't set %dx%d video mode: %s\n",
         img->w, img->h, SDL_GetError());
  }
 
  /* Blit onto the screen surface */
  if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
    warnx("BlitSurface error: %s\n", SDL_GetError());
 
  // Update the screen
  SDL_UpdateRect(screen, 0, 0, img->w, img->h);
 
  // wait for a key
  wait_for_keypressed();
 
  // return the screen for further uses
  return screen;
}

int main(int argc, char *argv[])
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
   Seg_char(img);
   display_image(img);
   SDL_FreeSurface(img);
   return 0;
}
