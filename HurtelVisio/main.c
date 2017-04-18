#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "prototypes.h"

int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stdout,"Echec de l'initialisation de la SDL (%s)\n", SDL_GetError());
        return -1;
    }

    Image imageO; // Image d'origine, sans fenêtre
    Image imageM; // Image modifiée, affichée, avec fenêtre
    Image imageF;    // Image à fusionner
    Image imageCtrl; // Fenêtre de contrôle, avec fenêtre et Renderer
    Image imageCM; // Palette de couleur, avec fenêtre

    if (!initImage(&imageO, NULL, 0, 0, 0, 0, 0, 0)) return 0;
    if (!initImage(&imageM, "Hurtel Visio", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 300, 300, SDL_WINDOW_SHOWN, 0)) return 0;
    if (!initImage(&imageCM, "Palette", 50, 600, 160, 160, SDL_WINDOW_SHOWN, 0)) return 0;
    if (!initImage(&imageF, NULL, 0, 0, 0, 0, 0, 0)) return 0;
    if (!initImage(&imageCtrl, "Hurtel Visio", 50, 50, 291, 627, SDL_WINDOW_SHOWN, 1)) return 0;

    if (!initTabImage(&imageCM, 256*4)) return 0; // Palette toujours de 256 couleurs max

    char url[1024]; url[0] = '\0';
    char urlf[1024]; urlf[0] = '\0';

    displayCtrl(&imageCtrl);

    actionArgument(&imageO, &imageM, &imageCM, &imageF, url, urlf, argc, argv);

    actionLoop(&imageO, &imageM, &imageCM, &imageCtrl, &imageF, url, urlf);

    clearImage(&imageO);
    clearImage(&imageM);
    clearImage(&imageF);
    clearImage(&imageCtrl);
    clearImage(&imageCM);
    SDL_Quit();

    return 0;
}
