#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "prototypes.h"

void actionRot(Params* params, Image* imageO, Image* imageF, int dir) {
    rotRogne(params, imageO->width, imageO->height, 1); // Rotation du rognage
    rot(dir, imageO);
    rot(dir, imageF);
}

void rotRogne(Params* params, int w, int h, int dir) {
    int tmp;
    if (dir == 1) {
        tmp = params->rogne.x1;
        params->rogne.x1 = h - params->rogne.y1;
        params->rogne.y1 = tmp;

        tmp = params->rogne.x2;
        params->rogne.x2 = h - params->rogne.y2;
        params->rogne.y2 = tmp;
    }
    else if (dir == -1) {
        tmp = params->rogne.y1;
        params->rogne.y1 = w - params->rogne.x1;
        params->rogne.x1 = tmp;

        tmp = params->rogne.y2;
        params->rogne.y2 = w - params->rogne.x2;
        params->rogne.x2 = tmp;
    }

    tmp = params->w;
    params->w = params->h;
    params->h = tmp;
}

void rot(int dir, Image* image) {
    if (!image->pixels) return; // Aucune image
    long i, j, k, width, height;

    Uint8* pixelsM = image->pixels;
    Uint8* pixelsTmp = NULL; // Tableau de pixels temporaire

    width = image->width;
    height = image->height;

    copyPixels(pixelsM, &pixelsTmp, width, height, &width, &height); // Copie pixelsM vers pixelsTmp
    if (pixelsTmp == NULL) return;

    if(dir == 1) {   // rotation horaire
        for(i=0; i<height; i++){
            for(j=0; j<width; j++){
                for(k = 0; k < 4; k++) {
                   pixelsM[((j+1)*height-i-1)*4+k] = pixelsTmp[(i*width+j)*4+k];
                }
            }
        }
    }
    else if(dir == -1) { // rotat° anti-horaire
        for(i=0; i<width; i++){
            for(j=0; j<height; j++){
                for(k = 0; k < 4; k++) {
                   pixelsM[(i*height+j)*4+k] = pixelsTmp[((j+1)*width-i-1)*4+k];
                }
            }
        }
    }

    image->width = height; // Intervertit la largeur et hauteur
    image->height = width;
    if (pixelsTmp) free(pixelsTmp);
}
