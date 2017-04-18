#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "prototypes.h"

void actionLumi(Params* params, int val, int def) {
    if (def) params->lumi = limlum(val);                // Définir
    else params->lumi = limlum(params->lumi + val);     // Ajouter
}

void actionCont(Params* params, int val, int def) {
    if (def) params->cont = limcont(val);               // Définir
    else params->cont = limcont(params->cont + val);    // Ajouter
}

void actionGrey(Params* params) {
    params->grey = !params->grey;
}

void contrast(Image* imageM, int cont, int lumi) {
    if (!imageM->pixels) return; // No picture
    if (cont == 0 && lumi == 0) return; // No change

    int i, wh4;
    Uint8* p = imageM->pixels;

    wh4 = imageM->width * imageM->height * 4; // 4 pour R+G+B+A
    for(i = 0; i < wh4; i+=4){
        p[i] = lim((p[i] - 127) * (cont/100.0) + 127 + lumi);
        p[i+1] = lim((p[i+1] - 127) * (cont/100.0) + 127 + lumi);
        p[i+2] = lim((p[i+2] - 127) * (cont/100.0) + 127 + lumi);
        p[i+3] += 0;
    }
}

void invert(Image * imageO){
    if (!imageO->pixels) return; // No picture
    Uint8* p = imageO->pixels;
    int i, wh4 = imageO->width * imageO->height * 4.0;
    for(i = 0; i < wh4; i+=4){
        p[i] = 255 - p[i];
        p[i+1] = 255 - p[i+1];
        p[i+2] = 255 - p[i+2];
        p[i+3] = p[i+3];
    }
}

void greylevel(Image * imageO, Image * imageM, int grey){
    if (!imageO->pixels) return;
    if (!imageM->pixels) return; // No picture
    if(!grey) return;
    else{
        Uint8* p = imageM->pixels;
        int i, j, maxi, wh4 = imageM->width * imageM->height * 4.0;

        for(i = 0; i < wh4; i+=4){
            maxi = greymax(p[i], p[i+1], p[i+2]);
            for(j = 0; j < 3; j++){
                p[i+j] = maxi;
            }
            p[i+3] = p[i+3];
        }
    }
}

int greymax(int a, int b, int c){
    if(a > b && a > c) return a;
    if(b > a && b > c) return b;
    return c;
}

void alpha(Image* imageM) {
    if (!imageM->pixels) return; // No picture

    int fr = 255, fv = 255, fb = 255;
    long wh4, i;
    Uint8 alp=0;
    Uint8* p = imageM->pixels;

    wh4 = imageM->width * imageM->height * 4;

    for (i = 0 ; i < wh4 ; i++) {
        alp = p[i+3];
        p[i] = mixer(p[i], fr, alp);
        i++;
        p[i] = mixer(p[i], fv, alp);
        i++;
        p[i] = mixer(p[i], fb, alp);
        i++;
    }

}

