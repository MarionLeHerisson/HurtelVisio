#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "prototypes.h"

void rogneMinMax(int* vmin, int* vmax, int v1, int v2) {
    if (v1 < v2) {
        *vmin = v1;
        *vmax = v2;
    }
    else {
        *vmin = v2;
        *vmax = v1;
    }
}

void rognePreview(Image* imageM, Rogne rogne) {
    if (!imageM->pixels) return; // No picture
    if (rogne.x1 == rogne.x2 && rogne.y1 == rogne.y2) return; // Pas de rognage

    long wh4, i;
    int x, y, xmin, ymin, xmax, ymax; // Coordonnées des pixels
    Uint8* p = imageM->pixels;

    wh4 = imageM->width * imageM->height * 4;
    rogneMinMax(&xmin, &xmax, rogne.x1, rogne.x2);
    rogneMinMax(&ymin, &ymax, rogne.y1, rogne.y2);

    for (i = 0 ; i < wh4 ; i++) {
        IDtoXY(i, imageM->width, &x, &y); // Coordonnées du pixel

        if (x < xmin || x > xmax || y < ymin || y > ymax) {
            p[i] = lim(p[i] - 120);
            i++;
            p[i] = lim(p[i] - 120);
            i++;
            p[i] = lim(p[i] - 120);
            i++;
            p[i] = 255;
        }
        else {
            i += 3;
        }
    }
}

void rogneApply(Image* imageM, Rogne rogne) {
    if (!imageM->pixels) return; // No picture
    if (rogne.x1 == rogne.x2 && rogne.y1 == rogne.y2) return; // Pas de rognage

    long wh4, i, j;
    int x, y, xmin, ymin, xmax, ymax, w, h; // Coordonnées des pixels
    Uint8* p = imageM->pixels;
    Uint8* pixelsTmp = NULL; // Tableau de pixels temporaire

    wh4 = imageM->width * imageM->height * 4;
    rogneMinMax(&xmin, &xmax, rogne.x1, rogne.x2); // Met dans xmin le plus petit de x1 et x2, et dans xmax le plus grand.
    rogneMinMax(&ymin, &ymax, rogne.y1, rogne.y2);

    w = xmax - xmin + 1; // Largeur, hauteur de l'image rognée
    h = ymax - ymin + 1;

    pixelsTmp = malloc(sizeof(Uint8) * w*h*4); // Pixels tmp qui contiendra l'image rognée
    if (pixelsTmp == NULL) return;

    for (i = 0, j = 0 ; i < wh4 ; i++) {
        IDtoXY(i, imageM->width, &x, &y); // Coordonnées du pixel

        if (x < xmin || x > xmax || y < ymin || y > ymax) {
            i += 3; // On passe les pixels hors de la zone de rognage
        }
        else {
            pixelsTmp[j++] = p[i++];
            pixelsTmp[j++] = p[i++];
            pixelsTmp[j++] = p[i++];
            pixelsTmp[j++] = p[i];
        }
    }

    copyPixels(pixelsTmp, &(imageM->pixels), w, h, &(imageM->width), &(imageM->height)); // Copie pixelsTmp vers pixelsM

    free(pixelsTmp);
}
