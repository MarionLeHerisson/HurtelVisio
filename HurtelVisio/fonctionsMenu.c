#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "prototypes.h"
#include <string.h>

int keyboard(SDL_Event event, int idImg, int idCtrl) {
    switch (event.key.keysym.sym) {
        case SDLK_ESCAPE    : return 0;     // Quitter
        case SDLK_LEFT      : return 24;    // Précédent
        case SDLK_RIGHT     : return 25;    // Suivant
        case SDLK_SPACE     : return 3;     // Reset
        case SDLK_r         : return 16;    // Rogner
        case SDLK_i         : return 21;    // Inversion des couleurs
        case SDLK_g         : return 22;    // Niveau de gris
        case SDLK_f         : return 23;    // Fusion
    }
    return -1;
}

int buttons(SDL_Event event, int idImg, int idCtrl) {
    if (event.button.windowID == idCtrl) { // Contrôles
        if (event.button.button == SDL_BUTTON_LEFT) {
            int x = event.button.x;
            int y = event.button.y;
            //printf("\n%d %d   ", x, y);
            if (x > 23 && x < 112 && y > 24 && y <51) return 1; // Open
            else if (x > 139 && x < 264 && y > 24 && y < 51) return 2; // Save
            else if (x > 24 && x < 75 && y > 60 && y < 78) return 3; // Reset
            else if (x > 180 && x < 204 && y > 90 && y < 114) return 4; // Luminosite -
            else if (x > 214 && x < 222 && y > 102 && y < 112) return 5;   // Reset lum
            else if (x > 225 && x < 246 && y > 90 && y < 114) return 6; // Luminosite +
            else if (x > 180 && x < 204 && y > 124 && y < 148) return 7; // Contrast -
            else if (x > 214 && x < 222 && y > 136 && y < 146) return 8;  // Reset cont
            else if (x > 225 && x < 246 && y > 124 && y < 148) return 9;  // Contrast +
            else if (x > 180 && x < 204 && y > 190 && y < 213) return 10;   // Width -
            else if (x > 214 && x < 222 && y > 202 && y < 212) return 11;   // Reset width
            else if (x > 225 && x < 246 && y > 190 && y < 213) return 12;   // Width +
            else if (x > 180 && x < 204 && y > 225 && y < 245) return 13;   // Height -
            else if (x > 214 && x < 222 && y > 236 && y < 246) return 14;   // Reset height
            else if (x > 225 && x < 246 && y > 225 && y < 245) return 15;   // Height +
            else if (x > 180 && x < 201 && y > 257 && y < 277) return 16;   // Rogner
            else if (x > 170 && x < 201 && y > 314 && y < 343) return 17; // Rot H
            else if (x > 223 && x < 253 && y > 314 && y < 343) return 18; // Rot AH
            else if (x > 174 && x < 202 && y > 349 && y < 373) return 19; // Mirror V
            else if (x > 223 && x < 246 && y > 350 && y < 375) return 20; // Mirror H
            else if (x > 34 && x < 258 && y > 411 && y < 438) return 21;    // Invert colors
            else if (x > 34 && x < 258 && y > 450 && y < 478) return 22;   // Grey
            else if (x > 34 && x < 258 && y > 491 && y < 511) return 23;   // Fusion
            else if (x > 62 && x < 114 && y > 545 && y < 605) return 24;    // Prev
            else if (x > 178 && x < 227 && y > 545 && y < 605) return 25;   // Next
        }
    }
    else if (event.button.windowID == idImg) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            return 100; // Image rogner XY
        }
    }
    return -1; // Aucune action
}

int buttonsUP(SDL_Event event, int idImg, int idCtrl) {
    if (event.button.windowID == idImg) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            return 101; // Image rogner Largeur hauteur
        }
    }
    return -1; // Aucune action
}

int mousemove(SDL_Event event, int idImg, int idCtrl) {
    if (event.button.windowID == idImg) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            return 101; // Image rogner Largeur hauteur
        }
    }
    return -1; // Aucune action
}

void rogneDefine(Params* params, int x, int y, int val) {
    if (val == 0) { // Désactive
        params->rogne.etat = 0;
        params->rogne.x1 = x;
        params->rogne.y1 = y;
        params->rogne.x2 = x;
        params->rogne.y2 = y;
    }
    else if (val == 1) { // Initialise ou désactive (permute)
        params->rogne.etat = !params->rogne.etat;
    }

    if (params->rogne.etat == 0) return; // Non initialisé

    if (val == 2) { // Définit X et Y
        params->rogne.x1 = x;
        params->rogne.y1 = y;
        params->rogne.x2 = x;
        params->rogne.y2 = y;
    }
    else if (val == 3) { // Définit X2 Y2
        params->rogne.x2 = x;
        params->rogne.y2 = y;
    }
}

int evenements(int idImg, int idCtrl, int idCM, int* x, int* y) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        *x = event.button.x;
        *y = event.button.y;

        switch (event.type) {

            case SDL_WINDOWEVENT:
                switch(event.window.event) {
                    case SDL_WINDOWEVENT_CLOSE:
                        return 0;
                }
                break;

            case SDL_QUIT:
                return 0;

            case SDL_KEYDOWN: {
                return keyboard(event, idImg, idCtrl);
            }

            case SDL_MOUSEBUTTONDOWN:
                return buttons(event, idImg, idCtrl);

            case SDL_MOUSEBUTTONUP:
                return buttonsUP(event, idImg, idCtrl);

            case SDL_MOUSEMOTION:
                return mousemove(event, idImg, idCtrl);

        }
    }
    return -1; // Aucune action
}

void actionOpen(Image* imageO, Image* imageCM, char* url, Params* params) {
    loading(imageO, imageCM, url, 0);
    resetOptions(params);
}

void actionReset(Image* imageO, Image* imageCM, char* url, Params* params) {
    loading(imageO, imageCM, url, 1);
    resetOptions(params);
}

void actionArgument(Image* imageO, Image* imageM, Image* imageCM, Image* imageF, char* url, char* urlf, int argc, char** argv) {

    if (argc > 1) { // Charge l'image de l'argument
        strcpy(url, argv[1]);
        loading(imageO, imageCM, url, 1);
        copyPixels(imageO->pixels, &(imageM->pixels), imageO->width, imageO->height, &(imageM->width), &(imageM->height));
        display(imageM);
        renameWindow(imageM, url);
    }

    if(argc > 2){
        loading(imageF, imageCM, argv[2], 1);
    }
}

void actionLoop(Image* imageO, Image* imageM, Image* imageCM, Image* imageCtrl, Image* imageF, char* url, char* urlf) {

    int action = 1, x, y;
    Params params;
    resetOptions(&params);

    while (action) {
        action = evenements(imageM->id, imageCtrl->id, imageCM->id, &x, &y);

        // FONCTIONS NON DESTRUCTIVES : Modifient pixelsO
        switch (action) {

            case 1 : // Open
                actionOpen(imageO, imageCM, url, &params);
                break;
            case 2 : // Save
                saving(imageM, imageCM, url, &params);
                break;
            case 3 : // Reset
                actionReset(imageM, imageCM, url, &params);
                break;
            case 4 : // Luminosité -
                actionLumi(&params, -10, 0);
                break;
            case 5 : // Reset lum
                actionLumi(&params, 0, 1);
                break;
            case 6 : // Luminosité +
                actionLumi(&params, 10, 0);
                break;
            case 7 : // Contraste -
                actionCont(&params, -5, 0);
                break;
            case 8 : // Reset cont
                actionCont(&params, 100, 1);
                break;
            case 9 : // Contraste +
                actionCont(&params, 5, 0);
                break;
            case 10 : // Width -
                actionResize(&params, -10, 0);
                break;
            case 11 : // Reset width
                actionResize(&params, 99, 0);
                break;
            case 12 : // Width +
                actionResize(&params, 10, 0);
                break;
            case 13 : // Height -
                actionResize(&params, 0, -10);
                break;
            case 14 : // Reset height
                actionResize(&params, 0, 99);
                break;
            case 15 : // Height +
                actionResize(&params, 0, 10);
                break;
            case 16 : // Rogner
                rogneDefine(&params, 0, 0, 1);
                break;
            case 17 : // Rotation horaire
                actionRot(&params, imageO, imageF, 1);
                break;
            case 18 : // Rotation anti-horaire
                actionRot(&params, imageO, imageF, -1);
                break;
            case 19 : // Miroir vertical
                actionMirror(&params, imageO, imageF, 'v');
                break;
            case 20 : // Miroir horizontal
                actionMirror(&params, imageO, imageF, 'h');
                break;
            case 21 : // Invert colors
                invert(imageO);
                break;
            case 22 : // Grey
                actionGrey(&params);
                break;
            case 23 : // Fusion
                if (params.fus.mode == 0) loading(imageF, imageCM, urlf, 0);
                params.fus.etat = 1;
                break;
            case 24 : // Prev
                actionNextPrev(imageO, imageCM, url, &params, -1);
                break;
            case 25 : // Next
                actionNextPrev(imageO, imageCM, url, &params, 1);
                break;
            case 100 : // Rogner premier clic
                rogneDefine(&params, x, y, 2);
                break;
            case 101 : // Rogner déclic
                rogneDefine(&params, x, y, 3);
                break;
        }

        // FONCTIONS DESTRUCTIVES : Modifient pixelsM
        if (action > 0) { // Action sur l'image : nécessite un recalcul et un réaffichage

            // Si autre action que rognage, rognage désactivé
            if (action != 16 && action != 100 && action != 101) params.rogne.etat = 0;

            // Copie pixelsO vers pixelsM
            copyPixels(imageO->pixels, &(imageM->pixels), imageO->width, imageO->height, &(imageM->width), &(imageM->height));

            // Traitements
            contrast(imageM, params.cont, params.lumi);
            greylevel(imageO, imageM, params.grey);

            // Fusion
            if (displayMenu(3, imageM, &params, imageO, imageF, imageCM, urlf) == 0)
                actionsFus(imageM, imageM->pixels, imageF, params.fus.mode, NULL, NULL, imageCM, urlf, &params);

            // Redimensionne
            resizePixels(imageM, &params);

            // Rogne
            rognePreview(imageM, params.rogne);

            //alpha(imageM); // Mélange l'alpha avec un fond blanc

            renameWindow(imageM, url);
            display(imageM);
            displayCM(imageCM);
        }
    }
}
