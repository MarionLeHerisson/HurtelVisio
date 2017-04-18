#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "prototypes.h"

void actionsFus(Image* imageM, Uint8* pixelsTmp, Image* imageF, int action, SDL_Renderer* renderer, SDL_Texture* textureEntoure, Image* imageCM, char* urlf, Params* param){
    //loading(imageF, imageCM, urlf, 1);
    if(!imageM->pixels) return;// printf("\nPas d'image M");
    if(!imageF->pixels) return;// printf("\nPas d'image F");

    switch (action){
        case 1 : superposition(imageM, imageF, pixelsTmp);
            param->fus.mode = 1;
            break;
        case 2 : fusalpha(imageM, imageF,  pixelsTmp);
            param->fus.mode = 2;
            break;
        case 3 : addition(imageM, imageF, pixelsTmp);
            param->fus.mode = 3;
            break;
        case 4 : soustraction(imageM, imageF, pixelsTmp);
            param->fus.mode = 4;
            break;
        case 5 : eclaircir(imageM, imageF, pixelsTmp);
            param->fus.mode = 5;
            break;
        case 6 : obscurcir(imageM, imageF, pixelsTmp);
            param->fus.mode = 6;
            break;
        case 7 : produit(imageM, imageF, pixelsTmp);
            param->fus.mode = 7;
            break;
        case 8 : lumiere(imageM, imageF, pixelsTmp);
            param->fus.mode = 8;
            break;
        case 9 : densite(imageM, imageF, pixelsTmp);
            param->fus.mode = 9;
            break;
    }
    if(param->fus.mode == 1) traceTexture(renderer, textureEntoure, 2, 7, 193, 41);
    if(param->fus.mode == 2) traceTexture(renderer, textureEntoure, 204, 7, 83, 41);
    if(param->fus.mode == 3) traceTexture(renderer, textureEntoure, 12, 56, 114, 41);
    if(param->fus.mode == 4) traceTexture(renderer, textureEntoure, 130, 56, 162, 41);
    if(param->fus.mode == 5) traceTexture(renderer, textureEntoure, 10, 101, 103, 41);
    if(param->fus.mode == 6) traceTexture(renderer, textureEntoure, 150, 101, 152, 41);
    if(param->fus.mode == 7) traceTexture(renderer, textureEntoure, 10, 152, 103, 41);
    if(param->fus.mode == 8) traceTexture(renderer, textureEntoure, 165, 152, 127, 41);
    if(param->fus.mode == 9) traceTexture(renderer, textureEntoure, 10, 198, 220, 41);
}

void superposition(Image * imageM, Image * imageF, Uint8* pt){
    if (!imageF->pixels) return;
    if (!imageM->pixels) return; // No picture
    Uint8 * p = imageM->pixels;
    Uint8 * f = imageF->pixels;
    int i, j;
    long hh = imageM->height, ww = (imageM->width) * 4.0, h = imageF->height, w = (imageF->width) * 4.0;

    long smallh, smallw;
    if(ww > w) smallw = w;
    else smallw = ww;
    if(hh > h) smallh = h;
    else smallh = hh;

    for(i = 0; i < smallh; i++){
        for(j = 0; j < smallw; j+=4){
            p[i*ww+j] = (pt[i*ww+j]+f[i*w+j])/2;
            p[i*ww+j+1] = (pt[i*ww+j+1]+f[i*w+j+1])/2;
            p[i*ww+j+2] = (pt[i*ww+j+2]+f[i*w+j+2])/2;
            p[i*ww+j+3] = pt[i*ww+j+3];
        }
    }
}

void fusalpha(struct Image * imageM, struct Image * imageF,  Uint8* pt){
    if (!imageF->pixels) return;
    if (!imageM->pixels) return; // No picture
    Uint8 * p = imageM->pixels;
    Uint8 * f = imageF->pixels;
    int i, j;
    long hh = imageM->height, ww = (imageM->width) * 4.0, h = imageF->height, w = (imageF->width) * 4.0;

    long smallh, smallw;
    if(ww > w) smallw = w;
    else smallw = ww;
    if(hh > h) smallh = h;
    else smallh = hh;

    for(i = 0; i < smallh; i++){
        for(j = 0; j < smallw; j+=4){
            p[i*ww+j] = mixer(f[i*w+j], pt[i*ww+j], f[i*w+j+3]);
            p[i*ww+j+1] = mixer(f[i*w+j+1], pt[i*ww+j+1], f[i*w+j+3]);
            p[i*ww+j+2] = mixer(f[i*w+j+2], pt[i*ww+j+2], f[i*w+j+3]);
            p[i*ww+j+3] = pt[i*ww+j+3];
        }
    }
}

void eclaircir(struct Image * imageM, struct Image * imageF, Uint8 * pt){
    if (!imageF->pixels) return;
    if (!imageM->pixels) return; // No picture
    Uint8 * p = imageM->pixels;
    Uint8 * f = imageF->pixels;
    int i, j;
    long hh = imageM->height, ww = (imageM->width) * 4.0, h = imageF->height, w = (imageF->width) * 4.0;

    long smallh, smallw;
    if(ww > w) smallw = w;
    else smallw = ww;
    if(hh > h) smallh = h;
    else smallh = hh;

    for(i = 0; i < smallh; i++){
        for(j = 0; j < smallw; j+=4){			// inverser "<" pour obtenir l'autre
            if((pt[i*ww+j] + pt[i*ww+j+1] + pt[i*ww+j+2]) < (f[i*w+j] + f[i*w+j+1] + f[i*w+j+2])){
                p[i*ww+j] = f[i*w+j];
                p[i*ww+j+1] = f[i*w+j+1];
                p[i*ww+j+2] = f[i*w+j+2];
            }else{
                p[i*ww+j] = pt[i*ww+j];
                p[i*ww+j+1] = pt[i*ww+j+1];
                p[i*ww+j+2] = pt[i*ww+j+2];
            }
            p[i*ww+j+3] = pt[i*ww+j+3];
        }
    }
}

void obscurcir(struct Image * imageM, struct Image * imageF, Uint8 * pt){
    if (!imageF->pixels) return;
    if (!imageM->pixels) return; // No picture
    Uint8 * p = imageM->pixels;
    Uint8 * f = imageF->pixels;
    int i, j;
    long hh = imageM->height, ww = (imageM->width) * 4.0, h = imageF->height, w = (imageF->width) * 4.0;

    long smallh, smallw;
    if(ww > w) smallw = w;
    else smallw = ww;
    if(hh > h) smallh = h;
    else smallh = hh;

    for(i = 0; i < smallh; i++){
        for(j = 0; j < smallw; j+=4){			// inverser "<" pour obtenir l'autre
            if((pt[i*ww+j] + pt[i*ww+j+1] + pt[i*ww+j+2]) > (f[i*w+j] + f[i*w+j+1] + f[i*w+j+2])){
                p[i*ww+j] = f[i*w+j];
                p[i*ww+j+1] = f[i*w+j+1];
                p[i*ww+j+2] = f[i*w+j+2];
            }else{
                p[i*ww+j] = pt[i*ww+j];
                p[i*ww+j+1] = pt[i*ww+j+1];
                p[i*ww+j+2] = pt[i*ww+j+2];
            }
            p[i*ww+j+3] = p[i*ww+j+3];
        }
    }
}

void addition(struct Image * imageM, struct Image * imageF, Uint8 * pt){
    if (!imageF->pixels) return;
    if (!imageM->pixels) return; // No picture
    Uint8 * p = imageM->pixels;
    Uint8 * f = imageF->pixels;
    int i, j;
    long hh = imageM->height, ww = (imageM->width) * 4.0, h = imageF->height, w = (imageF->width) * 4.0;

    long smallh, smallw;
    if(ww > w) smallw = w;
    else smallw = ww;
    if(hh > h) smallh = h;
    else smallh = hh;

    for(i = 0; i < smallh; i++){
        for(j = 0; j < smallw; j+=4){
            p[i*ww+j] = lim(pt[i*ww+j] + f[i*w+j]);
            p[i*ww+j+1] = lim(pt[i*ww+j+1] + f[i*w+j+1]);
            p[i*ww+j+2] = lim(pt[i*ww+j+2] + f[i*w+j+2]);
            p[i*ww+j+3] = pt[i*ww+j+3];
        }
    }
}

void soustraction(struct Image * imageM, struct Image * imageF, Uint8 * pt){
    if (!imageF->pixels) return;
    if (!imageM->pixels) return; // No picture
    Uint8 * p = imageM->pixels;
    Uint8 * f = imageF->pixels;
    int i, j;
    long hh = imageM->height, ww = (imageM->width) * 4.0, h = imageF->height, w = (imageF->width) * 4.0;

    long smallh, smallw;
    if(ww > w) smallw = w;
    else smallw = ww;
    if(hh > h) smallh = h;
    else smallh = hh;

    for(i = 0; i < smallh; i++){
        for(j = 0; j < smallw; j+=4){
            p[i*ww+j] = lim(255 - lim(255-pt[i*ww+j]) - f[i*w+j]);
            p[i*ww+j+1] = lim(255 - lim(255-pt[i*ww+j+1]) - f[i*w+j+1]);
            p[i*ww+j+2] = lim(255 - lim(255-pt[i*ww+j+2]) - f[i*w+j+2]);
            p[i*ww+j+3] = pt[i*ww+j+3];
        }
    }
}

void produit(struct Image * imageM, struct Image * imageF, Uint8 * pt){
    if (!imageF->pixels) return;
    if (!imageM->pixels) return; // No picture
    Uint8 * p = imageM->pixels;
    Uint8 * f = imageF->pixels;
    int i, j;
    long hh = imageM->height, ww = (imageM->width) * 4.0, h = imageF->height, w = (imageF->width) * 4.0;

    long smallh, smallw;
    if(ww > w) smallw = w;
    else smallw = ww;
    if(hh > h) smallh = h;
    else smallh = hh;

    for(i = 0; i < smallh; i++){
        for(j = 0; j < smallw; j+=4){
            // p[i] - p[i] + 255 - p[i]
            p[i*ww+j] = lim(f[i*w+j] * pt[i*ww+j] / 255);
            p[i*ww+j+1] = lim(f[i*w+j+1] * pt[i*ww+j+1] / 255);
            p[i*ww+j+2] = lim(f[i*w+j+2] * pt[i*ww+j+2] / 255);
            p[i*ww+j+3] = pt[i*ww+j+3];
        }
    }
}

void lumiere(struct Image * imageM, struct Image * imageF, Uint8 * pt){
    if (!imageF->pixels) return;
    if (!imageM->pixels) return; // No picture
    Uint8 * p = imageM->pixels;
    Uint8 * f = imageF->pixels;
    int i, j;
    long hh = imageM->height, ww = (imageM->width) * 4.0, h = imageF->height, w = (imageF->width) * 4.0;

    long smallh, smallw;
    if(ww > w) smallw = w;
    else smallw = ww;
    if(hh > h) smallh = h;
    else smallh = hh;

    for(i = 0; i < smallh; i++){
        for(j = 0; j < smallw; j+=4){
            p[i*ww+j] = 255 - lim((255 - pt[i*ww+j]) * (255 - f[i*w+j]) / 255.);
            p[i*ww+j+1] = 255 - lim((255 - pt[i*ww+j+1]) * (255 - f[i*w+j+1]) / 255.);
            p[i*ww+j+2] = 255 - lim((255 - pt[i*ww+j+2]) * (255 - f[i*w+j+2]) / 255.);
            p[i*ww+j+3] = pt[i*ww+j+3];
        }
    }
}


void densite(struct Image * imageM, struct Image * imageF, Uint8 * pt){
    if (!imageF->pixels) return;
    if (!imageM->pixels) return; // No picture
    Uint8 * p = imageM->pixels;
    Uint8 * f = imageF->pixels;
    int i, j, invpt;
    long hh = imageM->height, ww = (imageM->width) * 4.0, h = imageF->height, w = (imageF->width) * 4.0;

    long smallh, smallw;
    if(ww > w) smallw = w;
    else smallw = ww;
    if(hh > h) smallh = h;
    else smallh = hh;

    for(i = 0; i < smallh; i++){
        for(j = 0; j < smallw; j+=4){
            invpt = lim(255 - pt[i*ww+j]);
            p[i*ww+j] = lim(-invpt + f[i*w+j]);
            invpt = lim(255 - pt[i*ww+j+1]);
            p[i*ww+j+1] = lim(-invpt + f[i*w+j+1]);
            invpt = lim(255 - pt[i*ww+j+2]);
            p[i*ww+j+2] = lim(-invpt + f[i*w+j+2]);
            p[i*ww+j+3] = pt[i*ww+j+3];
        }
    }
}
