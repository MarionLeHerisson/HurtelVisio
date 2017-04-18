#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "prototypes.h"

void actionMirror(Params* params, Image* imageO, Image* imageF, char dir) {
    mirrorRogne(params, imageO->width, imageO->height, dir);
    mirror(dir, imageO);
    mirror(dir, imageF);
}

void mirrorRogne(Params* params, int w, int h, char dir) {
    if (params == NULL) return;
    if (dir == 'v') {
        params->rogne.x1 = w - params->rogne.x1;
        params->rogne.x2 = w - params->rogne.x2;
    }
    else if (dir == 'h') {
        params->rogne.y1 = h - params->rogne.y1;
        params->rogne.y2 = h - params->rogne.y2;
    }
}

void mirror(char dir, Image* image) {
    if (!image->pixels) return; // Aucune image

    Uint8* pixelsM;
    int i, j, k;
    int a, b, w, h;

    w = image->width;
    h = image->height;
    pixelsM = image->pixels;

    if(dir == 'v'){     // vertical
        for(i = 0; i < h; i++){    // browse line per line
            for(j = 0; j < w/2; j++){  // column to the half
                for(k = 0; k < 4; k++) {
                    a = i * w * 4 + j * 4 + k;
                    b = i * w * 4 + (w - j - 1) * 4+ k;
                    permut(pixelsM, a, b);
                }
            }
        }
    }else if(dir == 'h'){    // horizontal
        for(i = 0; i < h/2; i++){
            for(j = 0; j < w; j++){
                for (k = 0; k < 4; k++){
                    a = i * w * 4 + j * 4 + k;
                    b = (h - i - 1) * w * 4 + j * 4 + k;
                    permut(pixelsM, a, b);
                }
            }
        }
    }
}


