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
//besoin de continuer a tester en verifiant qu l on ai pas fini le bloc
void detectblock(SDL_Surface* img){
  int x,y,e,tline,y1,y2;
  int rgb;
  int b = 0; 
  int c = 0;
  int d = 0;
  for(y = 0; y < img -> h;y++){  //seulement a faire pour la premier ligne apres c est easy
    e = 0;
    for(x = 0; x < img -> w && e; x++){
      if( isblack(img,x,y) && d){
        x1=x;
        y1=y;
        for(;y<img -> h && b  ;y++){  // b est un booleen true tant que ligne pas sans noir
          b = 1;
          c = 0;
          for(x=0;x<img -> w && c ;x++){ 
            if(isblack(img,x,y)){
              b = 0;
              c = 1;  
            }
          }
        }
        tline = y1 - y;
        y2 = y
        d = 1;
      }
      
    }
    if(y-y2 == tline && endblock == true)//fin du bloc nous pouvons maintenant
// notre struct de blocs chaine et reininitialiser la plupart sauf y et en
// sauvegardant y1 ypour sa hauteur,,,,,,,,,
  }
}

int isblack(SDL_Surface* img,int x, y){
  Uint32 rgb = getpixel(img,x,y);
  // si r=g=b=255 return 0 else return 1
  
}

