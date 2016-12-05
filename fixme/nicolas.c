#include "nicolas.h"


void detecbloc(struct matrice* matr){
	int x1 = 1,x2 = matr->width -1,y1 =1 ,y2 = matr->height,newp = 1,x4,x3;
	int oneb = 1,endline = 0,tline = 3;
	for(int j =1;j<matr->height;j++){
		printf("0");   
		for(int i = 1;i<matr->width;i++){
			//pixel position = j*width + i si pixel = 0 -> noir
			//until next line
			for(;j<matr->height && oneb;j++){
				for(i=0;i<matr->width && oneb;i++){
					if(!(matr->mat[j*matr->width +i]))
						oneb = 0;				
				}
			}
			if(newp==0 && j - y2 > 2*(tline) && endline){
				printf("%d",1);
				newp = 1;
				//draw and save bloc from x1-2y1-2 to x2+2y2+2
				for(int x = x1;x<x2;x++){
					matr->mat[(y1+1)*matr->width+x]=0;
					matr->mat[(y2-1)*matr->width+x]=0;
				}
				for(int y = y1;y<y2;y++){
					matr->mat[y*matr->width+x1+1]=0;
					matr->mat[y*matr->width+x2-1]=0;
				}
			}
			if(newp){
				//first line of bloc
				printf("%d",2);
				y1=j;
				x1=i;
				endline = 1;
				for(;j<matr->height && endline;j++){
					endline = 0;
					for(i =0 ;i<matr->width;i++){
						if(!(matr->mat[j*matr->width +i])){
							endline = 1;
							if(i<x1)
								x1 = i;
							if(i>x2)
								x2 = i;
						}
					}
				}
				tline = j - y1;
                                printf("%d",tline);
				oneb = 1;
				newp = 0;
				y2=j;
			}
			else{
				//lines in bloc
				printf("%d",3);
				y2 = j;
				endline = 1;
				for(;j<matr->height && endline;j++){
					endline = 0;
					for(i=0;i<matr->width;i++){
						if(!(matr->mat[j*matr->width +i])){
							endline = 1;
							if(i<x1)
								x3 = i;
							if(i>x2)
								x4 = i;
						}
					}
				}
				if(j-y2 > tline){
					printf("%d",4);
					//end of bloc
					newp = 1;
					//draw and save bloc 
					//from x1-1y1-1 to x2+1 j+1
					for(int x = x1;x<x2;x++){
						matr->mat[(y1+1)*matr->width+x]=0;
						matr->mat[(j-1)*matr->width+x]=0;
					}
					for(int y = y1;y<y2;y++){
						matr->mat[y*matr->width+x1+1]=0;
						matr->mat[y*matr->width+x2-1]=0;
					}

				}
				oneb = 1;
				y2 = j;
				x1 =x3;
				x2 =x4;
			}
			j++;
		}   
	}
}
