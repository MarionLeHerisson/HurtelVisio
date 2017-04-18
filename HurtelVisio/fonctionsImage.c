#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "prototypes.h"

int XYtoID(int x, int y, int width) {
    return ((y * width + x) * 4);
}

void IDtoXY(int id, int width, int* x, int* y) {
    int id4 = id / 4;
    *x = id4 % width;
    *y = id4 / width;
}

void permut(Uint8* pixels, int a, int b) {
    Uint8 tmp = pixels[a];
    pixels[a] = pixels[b];
    pixels[b] = tmp;
}

double lim(int a) {
    return a > 255 ? 255 : a < 0 ? 0 : a;
}

int limcont(int a) {
    return a >= 200 ? 200 : a <= 10 ? 10 : a;
}

int limlum(int a) {
    return a >= 250 ? 250 : a <= -240 ? -240 : a;
}

Uint8 mixer(Uint8 a, Uint8 b, Uint8 c) {
    Uint8 ret;
    double fac = c / 255.;
    ret = (1 - fac) * b + fac * a;
    return ret;
}

void copyPixels(Uint8* pixelsO, Uint8** pixelsM, long widthO, long heightO, long *widthM, long *heightM) {
    // Copie un tableau de pixels vers un autre d'une autre dimension
    if (pixelsO == NULL) return;
    long wh4, i;

    if (*pixelsM) { // Vide le tableau de destination
        free(*pixelsM);
        *pixelsM = NULL;
    }

    // Copie la largeur et hauteur
    *widthM = widthO;
    *heightM = heightO;
    wh4 = widthO * heightO * 4; // 4 pour R+V+B+A

    // Créé et copie les données
    *pixelsM = malloc(sizeof(Uint8) * wh4);

    for (i = 0 ; i < wh4 ; i++) {
        (*pixelsM)[i] = pixelsO[i];
    }
}


