#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "prototypes.h"

int buttonsTGA(SDL_Event event) {
    if (event.button.button == SDL_BUTTON_LEFT) {
        int x = event.button.x;
        int y = event.button.y;
        //printf("\n%d %d   ", x, y);
        if (x > 15 && x < 218 && y > 9 && y < 40) return 1; // 16 bits
        else if (x > 15 && x < 218 && y > 48 && y < 85) return 2; // 24 bits
        else if (x > 15 && x < 218 && y > 95 && y < 126) return 3; // 32 bits
        else if (x > 15 && x < 218 && y > 136 && y < 174) return 4; // RLE
        else if (x > 44 && x < 92 && y > 194 && y < 220) return 99; // OK
        else if (x > 95 && x < 181 && y > 192 && y < 220) return 0; // Annuler
    }
    return -1;
}

int buttonsBMP(SDL_Event event) {
    if (event.button.button == SDL_BUTTON_LEFT) {
        int x = event.button.x;
        int y = event.button.y;
        //printf("\n%d %d   ", x, y);
        if (x > 27 && x < 114 && y > 15 && y < 51) return 1; // 24 bits
        else if (x > 26 && x < 115 && y > 53 && y < 82) return 2; // 32 bits
        else if (x > 3 && x < 52 && y > 112 && y < 137) return 99; // OK
        else if (x > 56 && x < 137 && y > 112 && y < 139) return 0; // Annuler
    }
    return -1; // Aucune action
}

int buttonsFus(SDL_Event event) {
    if (event.button.button == SDL_BUTTON_LEFT) {
        int x = event.button.x;
        int y = event.button.y;
        if (x > 22 && x < 175 && y > 16 && y < 37) return 1; // Superposition
        else if (x > 213 && x < 276 && y > 16 && y < 37) return 2; // Alpha
        else if (x > 22 && x < 116 && y > 66 && y < 84) return 3; // Addition
        else if (x > 137 && x < 279 && y > 66 && y < 84) return 4; // Soustraction
        else if (x > 22 && x < 105 && y > 111 && y < 133) return 5; // Eclaircir
        else if (x > 171 && x < 276 && y > 111 && y < 133) return 6; // Obscurcir
        else if (x > 22 && x < 101 && y > 162 && y < 184) return 7; // Produit
        else if (x > 185 && x < 278 && y > 162 && y < 181) return 8; // Lumire
        else if (x > 22 && x < 210 && y > 208 && y < 228) return 9; // DensitŽ
        else if (x > 64 && x < 115 && y > 263 && y < 290) return 99; // OK
        else if (x > 133 && x < 237 && y > 263 && y < 290) return 0; // Annuler
    }
    return -1; // Aucune action
}

int evenementsMenu(int id, int ext, Params* params) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.window.windowID != id) continue; // Limite à une fenêtre

        switch (event.type) {
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_CLOSE) return 0;
                break;

            case SDL_QUIT: return 0;

            case SDL_KEYDOWN: {
                if (event.key.keysym.sym == SDLK_ESCAPE)  return 0;
                break;
            }

            case SDL_MOUSEBUTTONDOWN:
                if (ext == 0) return buttonsBMP(event);
                else if (ext == 1) return buttonsTGA(event);
                else if (params->fus.etat) return buttonsFus(event);
                break;
        }
    }
    return -1; // Aucune action
}

void traceTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int w, int h) {
    SDL_Rect bmpPos;
    bmpPos.x = x;
    bmpPos.y = y;
    bmpPos.w = w;
    bmpPos.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &bmpPos);
}

void actionsBMP(Image* image, int action, SDL_Renderer* renderer, SDL_Texture* textureEntoure) {
    if (action < 0) return;

    switch (action) {
        case 1 :
            image->bpp = 24;
            break;
        case 2 :
            image->bpp = 32;
            break;
        default :
            break;
    }
    if (image->bpp == 24) traceTexture(renderer, textureEntoure, 5, 12, 130, 45);
    if (image->bpp == 32) traceTexture(renderer, textureEntoure, 5, 47, 130, 45);
}

void actionsTGA(Image* image, int action, SDL_Renderer* renderer, SDL_Texture* textureEntoure, SDL_Texture* textureEntoureC, SDL_Texture* textureRaye) {
    if (action < 0) return;

    switch (action) {
        case 1 :
            image->bpp = 16;
            break;
        case 2 :
            image->bpp = 24;
            break;
        case 3 :
            image->bpp = 32;
            break;
        case 4 :
            image->rle = !(image->rle);
            break;
        default :
            break;
    }
    if (image->bpp == 16) traceTexture(renderer, textureEntoure, 47, 7, 130, 45);
    if (image->bpp == 24) traceTexture(renderer, textureEntoure, 47, 46, 130, 45);
    if (image->bpp == 32) traceTexture(renderer, textureEntoure, 47, 87, 130, 45);
    if (image->rle) traceTexture(renderer, textureEntoureC, 7, 135, 210, 45);
    else traceTexture(renderer, textureRaye, 7, 135, 210, 45);
}

int displayMenu(int ext, Image* image, Params* params, Image* imageO, Image* imageF, Image* imageCM, char* urlf) {
    SDL_Window* window = NULL;
    Uint8* pixelsTmp = NULL; // Tableau de pixels temporaire
    if (ext == 0) window = SDL_CreateWindow("BMP", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 143, 143, SDL_WINDOW_SHOWN);
    else if (ext == 1) window = SDL_CreateWindow("TGA", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 226, 226, SDL_WINDOW_SHOWN);
    else if (params->fus.etat) window = SDL_CreateWindow("FUSIONS", 200, 50, 300, 300, SDL_WINDOW_SHOWN);
    if (window == NULL) return 0;

    int ret = 0, id = SDL_GetWindowID(window), action = 1;

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer) {
        id = SDL_GetWindowID(window);
        SDL_Surface* surface = NULL;
        SDL_Surface* surfaceEntoure = SDL_LoadBMP("interfaceEntoure.bmp");
        SDL_Surface* surfaceEntoureC = SDL_LoadBMP("interfaceEntoureC.bmp");
        SDL_Surface* surfaceRaye = SDL_LoadBMP("interfaceRaye.bmp");

        if (ext == 0) surface = SDL_LoadBMP("interfaceBMP.bmp");
        else if (ext == 1) surface = SDL_LoadBMP("interfaceTGA.bmp");
        else if (params->fus.etat) {
            surface = SDL_LoadBMP("interfaceFusion.bmp");
            copyPixels(image->pixels, &pixelsTmp, image->width, image->height, &(image->width), &(image->height));
            if (pixelsTmp == NULL) return 0;
        }

        if (surface && surfaceEntoure && surfaceEntoureC && surfaceRaye) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Texture* textureEntoure = SDL_CreateTextureFromSurface(renderer, surfaceEntoure);
            SDL_Texture* textureEntoureC = SDL_CreateTextureFromSurface(renderer, surfaceEntoureC);
            SDL_Texture* textureRaye = SDL_CreateTextureFromSurface(renderer, surfaceRaye);

            if (texture && textureEntoure && textureEntoureC && textureRaye) {

                if (params->fus.etat && params->fus.mode) action = params->fus.mode;

                while (action) {

                    if (action == 99) { // OK
                        ret = 1;
                        params->fus.etat = 0;
                        break;
                    }
                    if (action == 0) { // Annuler
                        params->fus.etat = 0;
                        break;
                    }
                    if (action > 0) {
                        SDL_RenderCopy(renderer, texture, NULL, NULL);
                        if (ext == 0) actionsBMP(image, action, renderer, textureEntoure);
                        else if (ext == 1) actionsTGA(image, action, renderer, textureEntoure, textureEntoureC, textureRaye);
                        else if (params->fus.etat) {
                            actionsFus(image, pixelsTmp, imageF, action, renderer, textureEntoure, imageCM, urlf, params);
                            display(image);
                        }

                        SDL_RenderPresent(renderer);

                    }

                    action = evenementsMenu(id, ext, params);

                }
                if (params->fus.etat) {
                    params->fus.etat = 0;
                    if (!action) {
                        params->fus.mode = 0;
                        copyPixels(pixelsTmp, &image->pixels, image->width, image->height, &(image->width), &(image->height));
                    }

                }

            }
            if (texture) SDL_DestroyTexture(texture);
            if (textureEntoure) SDL_DestroyTexture(textureEntoure);
            if (textureEntoureC) SDL_DestroyTexture(textureEntoureC);
            if (textureRaye) SDL_DestroyTexture(textureRaye);
        }
        if (surface) SDL_FreeSurface(surface);
        if (surfaceEntoure) SDL_FreeSurface(surfaceEntoure);
        if (surfaceEntoureC) SDL_FreeSurface(surfaceEntoureC);
        if (surfaceRaye) SDL_FreeSurface(surfaceRaye);
    }

    SDL_DestroyWindow(window);
    return ret;
}

