#include <stdio.h>
#include <stdlib.h>
#include <err.h>


void detecbloc(struct matrice* matr){
        int x1 = 0,x2 = matr->width,y1 =0 ,y2 =matr->height,newp = 1,x4,x3;
        int oneb = 1,endline =1,tline = 0;
	for(int j = 0;j<matr->height;j++){
		for(int i = 0;i<matr->width;i++){
			//pixel position = j*width + i si pixel = 0 -> noir
			//until next line
			for(;j<matr->height && oneb;j++){
				for(;i<matr->width && oneb;i++){
					if(matr->mat[j*matr->width +i])
						oneb = 0;
				}
			}
			if(newp==0 && j - y2 > tline){
				newp = 1;
				//draw and save bloc from x1-2y1-2 to x2+2y2+2
				for(int x = x1-2;x<x2+2;x++){
					matr->mat[(y1-2)*matr->width +x]=0;
					matr->mat[(y2+2)*matr->width +x]=0;
				}
				for(int y = y1-2;y<y2+2;y++){
					matr->mat[y*matr->width +x1-2]=0;
					matr->mat[y*matr->width +x2+2]=0;
				}
			}
			y1 = j;
			x1 = i;
			if(newp){
				//first line of bloc
				for(;j<matr->height && endline;j++){
					endline = 0;
					for(;i<matr->width;i++){
						if(matr->mat[j*matr->width +i]){
							endline = 1;
							if(i<x1)
								x1 = i;
							if(i>x2)
								x2 = i;
						}
					}
				}
				tline = j - y1;
				newp = 0;
				oneb = 1;
				endline = 0;
				y2=j;
			}
			else{
				//lines in bloc
				y2 = j;
				for(;j<matr->height;j++){
					endline = 0;
					for(;i<matr->width;i++){
						if(matr->mat[j*matr->width +i]){
							endline = 1;
							if(i<x1)
								x3 = i;
							if(i>x2)
								x4 = i;
						}
					}
				}
				if(j-y2 > tline || j-y2 < 3*tline/4){
					//end of bloc
					newp = 1;
					//draw and save bloc 
					//from x1-1y1-1 to x2+1 j+1
					for(int x = x1-2;x<x2+2;x++){
						matr->mat[(y1-2)*matr->width +x]=0;
						matr->mat[(y2+2)*matr->width +x]=0;
					}
					for(int y = y1-2;y<y2+2;y++){
						matr->mat[y*matr->width +x1-2]=0;
						matr->mat[y*matr->width +x2+2]=0;
					}

				}
				oneb = 1;
				y2 = j;
				x1 =x3;
				x2 =x4;
			}
		}   
	}
}

