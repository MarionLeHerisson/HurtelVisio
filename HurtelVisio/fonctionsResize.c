#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "prototypes.h"

void actionResize(Params* params, int w, int h) {
    if (params == NULL) return;

    if (w == 99) params->w = 0;
    else {
        if (params->w + w > 0) params->w += w; // Limite à 1 px max
    }

    if (h == 99) params->h = 0;
    else {
        if (params->h + h > 0) params->h += h; // Limite à 1 px max
    }

}
/* marche pas
void resizeMix(Uint8* pt, Uint8* p, int i, int x, int y, double ratioX, double ratioY, long w) {
    int v = 0;//p[j*4];
    double vd = 0, fac;
    int j;

    //ratioX = 1. / ratioX;

    // ratioX = 1.5
    // On prend 1 * le pixel + 0.5 * le pixel X suivant

    while (ratioX > 0) {

        fac = (ratioX < 1) ? ratioX : 1.;

        j = XYtoID(x, y, w);

        vd += p[j*4];// * fac;

        ratioX--;
        x++;
    }

    v = (int)vd;
    //v = (int) ( vd / ratioX );


    pt[i*4] = v;
    pt[i*4+1] = v;//p[j*4+1];
    pt[i*4+2] = v;//p[j*4+2];
    pt[i*4+3] = p[j*4+3];
}*/

void resizePixels(Image* image, Params* params) {
    if (image == NULL || image->pixels == NULL) return;
    int i, j, x, y;
    double rx, ry;
    //long wh4 = image->width * image->height * 4;
    long whr, w, h;
    double ratioX, ratioY;

    if (params->w <= 0) params->w = image->width;
    if (params->h <= 0) params->h = image->height;

    if (params->w == image->width && params->h == image->height) return;

    w = params->w;
    h = params->h;
    whr = w * h;

    Uint8* p = image->pixels;
    Uint8* pt = NULL; // Tableau de pixels temporaire

    pt = malloc(sizeof(Uint8) * whr*4); // Pixels tmp qui contiendra l'image redimensionnée
    if (pt == NULL) return;

    ratioX = image->width / (double)w;
    ratioY = image->height / (double)h;

    for (i = 0 ; i < whr ; i++) {
        IDtoXY(i, w, &x, &y);

        rx = x * ratioX;
        ry = y * ratioY;

        j = XYtoID((int)rx, (int)ry, image->width);

        pt[i*4] = p[j*4];
        pt[i*4+1] = p[j*4+1];
        pt[i*4+2] = p[j*4+2];
        pt[i*4+3] = p[j*4+3];

    }

    copyPixels(pt, &(image->pixels), w, h, &(image->width), &(image->height)); // Copie pixelsTmp vers pixelsM
}
