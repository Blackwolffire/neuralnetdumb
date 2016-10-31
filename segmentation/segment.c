#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>
#include "quartet.h"
#include "pixel_operations.h"

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

void init_sdl(void) {
  // Init only the video part
  if( SDL_Init(SDL_INIT_VIDEO)==-1 ) {
    // it fails, die with an error message
    errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
  }
  // We don't really need a function for that ...
}


int isblack(SDL_Surface* img,int x,int y){
  Uint32 rgb = getpixel(img,x,y);
  Uint8 r,g,b;
  SDL_GetRGB(rgb,img->format,&r,&g,&b);
  if(b)
    return 0;
  else
    return 1;
}

void detectblock(SDL_Surface* img){
  int x,y,e,tline = 1,y1=1,x1=1,y2=1,x2=1,x3=1,y3=1;
  int a=1;
  int b = 1; 
  int c=1;
  int d = 1;
  e = 1;
  int endline=1;
  int f = 1;
  for(y = 0; y < img -> h;y++){ 
    for(x = 0; x < img -> w && e; x++){//for first line
      if(isblack(img,x,y)==0 && d){
        x1=x;
        x2 = x;
        y1=y;
        for(;y<img -> h && b  ;y++){
          b = 0;
          c = 1;
          for(x=0;x<img -> w && c ;x++){ 
            if(isblack(img,x,y)){
              if(x2<x)
                x2 =x;
              b = 1;
              c = 0;  
            }
          }
        }
        tline = y - y1;
        y2 = y;
        d = 0;
        a = 1;
      }
    }
    //until next line
    for(;y<img->h ;y++){
      for(;x<img->w && a;x++){
        if( y-y2 < tline && isblack(img,x,y) ){
           a = 0;
           y2 = 0;
        }
        if(y-y2 > 2*tline){
           e = 1;
           b = 1;
           d = 1;
           //struct to save bloc to do
           for(x3 = x1;x3<x2;x3++){
             Uint32 pxl1 = getpixel(img,x3,y1);
             Uint8 r, g, b;
             SDL_GetRGB(pxl1,img->format, &r, &g,&b);
             g = 0;
             putpixel(img,x3,y1,pxl1);
             Uint32 pxl2 = getpixel(img,x3,y2);
             Uint8 r2, g2, b2;
             SDL_GetRGB(pxl2,img->format, &r2, &g2,&b2);
             g2 = 0;
             putpixel(img,x3,y2,pxl2);
           }
           for(y3 = y1;y3<y2;y3++){
             Uint32 pxl1 = getpixel(img,x1,y3);
             Uint8 r, g, b;
             SDL_GetRGB(pxl1,img->format, &r, &g,&b);
             g = 0;
             putpixel(img,x1,y3,pxl1);
             Uint32 pxl2 = getpixel(img,x3,y2);
             Uint8 r2, g2, b2;
             SDL_GetRGB(pxl2,img->format, &r2, &g2,&b2);
             g2 = 0;
             putpixel(img,x2,y3,pxl2);

           }           
           a = 0;
        }
      }
   }
   //for  line
   for(;y<img->h && f;y++){
     for(;x<img->w && f;x++){
       if(y-y2<tline && endline){
         //struct to save bloc
         for(x3 = x1;x3<x2;x3++){
             Uint32 pxl1 = getpixel(img,x3,y1);
             Uint8 r, g, b;
             SDL_GetRGB(pxl1,img->format, &r, &g,&b);
             g = 0;
             putpixel(img,x3,y1,pxl1);
             Uint32 pxl2 = getpixel(img,x3,y2);
             Uint8 r2, g2, b2;
             SDL_GetRGB(pxl2,img->format, &r2, &g2,&b2);
             g2 = 0;
             putpixel(img,x3,y2,pxl2);
           }
           for(y3 = y1;y3<y2;y3++){
             Uint32 pxl1 = getpixel(img,x1,y3);
             Uint8 r, g, b;
             SDL_GetRGB(pxl1,img->format, &r, &g,&b);
             g = 0;
             putpixel(img,x1,y3,pxl1);
             Uint32 pxl2 = getpixel(img,x3,y2);
             Uint8 r2, g2, b2;
             SDL_GetRGB(pxl2,img->format, &r2, &g2,&b2);
             g2 = 0;
             putpixel(img,x2,y3,pxl2);

         f = 0;
         e = 1;
         b = 1;
         d = 1;
       }
       endline = 1;
       if(isblack(img,x,y)){
         endline = 0;
         if (x > x2)
           x2 = x;
       }
     }
    }
  }
}

int main(int argc,char *argv[]){
  if( arg < 2 )
    errx(1, "no image";)
  init_sdl();
  SDL_Surface* img =load_image(argv[1]);
  display_img(img);
  detectblock(img);
  display_img(img);
  SDL_FreeSurface;
  return 0;
}
