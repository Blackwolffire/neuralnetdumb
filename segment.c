#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

static inline
Uint8* pixelref(SDL_Surface *surf, unsigned x, unsigned y) {
  int bpp = surf->format->BytesPerPixel;
  return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}
 
Uint32 getpixel(SDL_Surface *surface, unsigned x, unsigned y) {
  Uint8 *p = pixelref(surface, x, y);
  switch(surface->format->BytesPerPixel) {
  case 1:
    return *p;
  case 2:
    return *(Uint16 *)p;
  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
      return p[0] << 16 | p[1] << 8 | p[2];
    else
      return p[0] | p[1] << 8 | p[2] << 16;
  case 4:
    return *(Uint32 *)p;
  }
  return 0;
}
 
void putpixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel) {
  Uint8 *p = pixelref(surface, x, y);
  switch(surface->format->BytesPerPixel) {
  case 1:
    *p = pixel;
    break;
  case 2:
    *(Uint16 *)p = pixel;
    break;
  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
      p[0] = (pixel >> 16) & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = pixel & 0xff;
    } else {
      p[0] = pixel & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = (pixel >> 16) & 0xff;
    }
    break;
  case 4:
    *(Uint32 *)p = pixel;
    break;
  }
}
//besoin de taille caractere ou de un caracter pour tester la fin du bloc sur la
//largeur
//besoin d un teste pour corriger la hauteur d une ligne a chaque ligne et la
//changer avectline
//besoin de continuer a tester apres que l on ai pas fini le bloc
void detectblock(SDL_Surface* img){
  int x,y,e,tline,y1,y2,x2;
  int rgb;
  int a;
  int b = 0; 
  int c;
  int d = 0;
  int e = 0;
  for(y = 0; y < img -> h;y++){ 
    for(x = 0; x < img -> w && e; x++){//traitement premiere ligne du bloc
      if( isblack(img,x,y) && d){//repeter au debut de chaque bloc pour fixer une premiere valeur de tline DEBUT DU BLOC
        x1=x;
        //x2 = x car dernier pixel noir du bloc quand on debute le bloc  x2 est le prenier
        y1=y;
        for(;y<img -> h && b  ;y++){  // b est un booleen true tant que ligne contient au moind un p noir
          b = 1;
          c = 0;
          for(x=0;x<img -> w && c ;x++){ 
            if(isblack(img,x,y)){
              //if(x2<x)
              //  x2 =x;
              b = 0;
              c = 1;  
            }
          }
        }
        tline = y - y1;
        y2 = y;
        d = 1;
        a = 0;
      }                
    }
    //traitement jusqu a debut de ligne suivante
    for(;y<img->h ;y++){
      for(;x<img->w && a;x++){
        if( y-y2 < tline && isblack(img,x,y) ){//reset y2 a la valeur y si tline > y-y2 et un pixel noir sur la ligne est detecte
           a = 1;
           y2 = 0;
        }
        if(y-y2 > 2*tline){
           e = 0;
           b = 0;
           d = 0;
           // endbloc = 0 or true if ligne de y2 contient un pixel noi
           //fin du bloc nous pouvons maintenant
           // notre struct de blocs chaine et reininitialiser la plupart sauf y et en
           // creer un nouveau block sauvegarder l ancien,,,,,,,,,
           
           a = 1;
        }
      }
   }
   //traitement de la ligne
   for(;y<img->h;y++){
     for(;x<img->w;x++){
       if(y-y2<tline && endline){//se repete tant aue y-y2<tline || rencontre une  ligne sanspixel noir pour si  taille de ligne  != tline alors debut d un nouveau bloc
       //a la fin du traitement de la ligne mettre y2 a 0
       }
       if(isblack(img,x,y) && x2<x)
         x2 = x;
     }
  }
 }
}

int isblack(SDL_Surface* img,int x,int y){
  Uint32 rgb = getpixel(img,x,y)
  return (!(rgb & 0x00ffffff));
  // si r=g=b=255 return 0 else return 1
  
}

