#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <windows.h>
#include "prototypes.h"
#include <string.h>

void urlToExt(char* url, char* ext) {
    // Récup l'extension ou NULL du fichier à partir d'un lien
    char* pos = strrchr(url, '.');
    if (pos != NULL) strcpy(ext, pos+1);
    else ext[0] = '\0';
}

void urlToName(char* url, char* name) {
    // Récup le nom du fichier à partir d'un lien
    char* pos = strrchr(url, '\\');
    if (pos != NULL) strcpy(name, pos+1);
    else strcpy(name, url);
}

void urlToDir(char* url, char* dir) {
    // Récup le réperoire à partir d'un lien
    char* pos = strrchr(url, '\\');
    if (pos != NULL) {
        strncpy(dir, url, pos - url + 1);
        dir[pos - url + 1] = '\0';
    }
    else strcpy(dir, url);
}

void clearCM(Uint8* pixels) {
    if (pixels == NULL) return;
    int i;
    for (i = 0 ; i < 16 * 16 * 4 ; i++) {
         pixels[i] = 0;
    }
}

// Fonctions structure Image

int initImage(Image* image, char* title, int x, int y, int w, int h, Uint32 flags, int render) {

    image->window = NULL;
    image->renderer = NULL;
    image->pixels = NULL;
    image->width = 0;
    image->height = 0;
    image->bpp = 24;
    image->rle = 0;
    image->id = -1;

    if (title != NULL) { // Si titre, créer la fenêtre
        image->window = SDL_CreateWindow(title, x, y, w, h, flags);
        if (image->window == NULL) return 0; // Échec

        image->id = SDL_GetWindowID(image->window); // Récupère l'ID

        if (render) { // Si 1, créer le Renderer
            image->renderer = SDL_CreateRenderer(image->window, -1, SDL_RENDERER_ACCELERATED);
            if (image->renderer == NULL) return 0; // Échec
        }
    }

    return 1; // RAS
}

int initTabImage(Image* image, int q) {
    if (image->pixels == NULL) image->pixels = malloc(q);
    if (image->pixels == NULL) return 0;
    return 1;
}

void clearImage(Image* image) {
    if (image->pixels) free(image->pixels);
    if (image->window) SDL_DestroyWindow(image->window);
    if (image->renderer) SDL_DestroyRenderer(image->renderer);
}

//

void save(char* url) {
    // Boîte de dialogue Enregistrer Sous
    OPENFILENAME ofn;
    int res, extid;
    char name[1024];
    char ext[10];
    char exturl[10];
    urlToName(url, name);
    url[0] = '\0';

    ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
    ofn.lStructSize = sizeof(OPENFILENAMEW);
    ofn.nMaxFileTitle = 1024;
    ofn.nMaxFile = 1024;
    ofn.lpstrFileTitle = name;
    ofn.lpstrFile = url;
    ofn.lpstrFilter = "Truevision Targa (*.TGA)\0*.TGA\0Windows Bitmap (*.BMP)\0*.BMP\0";

    res = GetSaveFileName(&ofn);
    if (!res) {
        url[0] = '\0';
    }
    else {
        // Ajoute l'extension si absente
        extid = ofn.nFilterIndex;
        urlToExt(url, exturl);
        switch (extid) {
            case 1 : strcpy(ext, "tga"); break;
            case 2 : strcpy(ext, "bmp"); break;
        }
        if (strcmpi(exturl, ext) != 0) {
            strcat(url, ".");
            strcat(url, ext);
        }
    }
}

void my_open(char* url) {
    // Boîte de dialogue Ouvrir
    OPENFILENAME ofn;
    int res;
    url[0] = '\0';

    ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
    ofn.lStructSize = sizeof(OPENFILENAMEW);
    ofn.lpstrFile = url;
    ofn.nMaxFile = 1024;
    ofn.lpstrFilter = "Fichier image (*.TGA, *.BMP)\0*.TGA;*.BMP\0Truevision Targa (*.TGA)\0*.TGA\0Windows Bitmap (*.BMP)\0*.BMP\0Tous (*.*)\0*.*\0";

    res = GetOpenFileName(&ofn);
    if (!res) url[0] = '\0';
}

long ficSize(FILE* fic) {
    // Récupère la taille du fichier
    long size;
    fseek(fic, 0, SEEK_END);
    size = ftell(fic);
    fseek(fic, 0, SEEK_SET);
    return size;
}

int checkBytesOK(FILE* fic, long ficsize, long n) {
    // Vérifie si n octets peuvent être lus
    long pos = ftell(fic) + n;
    if (pos <= ficsize) return 1;
    return 0;
}

void renameWindow(Image* image, char* url) {
    if (!image || !image->window) return;
    if (url == NULL) return;
    char name[1024];
    urlToName(url, name); // Récupère le nom
    SDL_SetWindowTitle(image->window, name);
}

void loading(Image* imageO, Image* imageCM, char* url, int force) {
    // Chargement
    FILE* fic = NULL;
    char ext[10];
    char urlTmp[1024];
    long ficsize;

    strcpy(urlTmp, url);
    if (!force) my_open(url); // Boîte de dialogue
    if (url[0] == '\0') {
        strcpy(url, urlTmp);
        return;
    }

    fic = fopen(url, "rb");
    if (fic == NULL) return;

    urlToExt(url, ext); // Récupère l'extension

    ficsize = ficSize(fic); // Récupère la taille du fichier

    if (imageO->pixels) { free(imageO->pixels); imageO->pixels = NULL; } // Si pixels, vire les pixels

    if (!stricmp(ext, "tga"))
        loadTGA(fic, ficsize, imageO, imageCM);
    else if (!stricmp(ext, "bmp"))
        loadBMP(fic, ficsize, imageO, imageCM);

    fclose(fic);
}

void saving(Image* imageM, Image* imageCM, char* url, Params *params) {
    // Sauvegarde
    if (imageM->pixels == NULL) return;

    FILE* fic = NULL;
    char ext[10];
    char urlTmp[1024];

    strcpy(urlTmp, url);
    save(url);
    if (url[0] == '\0') { strcpy(url, urlTmp); return; }

    urlToExt(url, ext);
    if (ext == NULL) return;

    // Options
    if (!stricmp(ext, "tga")) {
        if (!displayMenu(1, imageM, params, NULL, NULL, NULL, NULL)) { strcpy(url, urlTmp); return; }
    }
    else if (!stricmp(ext, "bmp")) {
        if (!displayMenu(0, imageM, params, NULL, NULL, NULL, NULL)) { strcpy(url, urlTmp); return; }
    }

    // Enregistrement
    fic = fopen(url, "wb");
    if (fic == NULL) return;

    rogneApply(imageM, params->rogne);

    if (!stricmp(ext, "tga"))
        saveTGA(fic, imageM, imageCM);
    else if (!stricmp(ext, "bmp"))
        saveBMP(fic, imageM, imageCM);

    fclose(fic);
}

void resetOptions(Params* params) {
    params->lumi = 0;
    params->cont = 100;
    params->grey = 0;
    params->w = 0;
    params->h = 0;

    Rogne rogne;
    rogne.etat = 0;
    rogne.x1 = 0;
    rogne.y1 = 0;
    rogne.x2 = 0;
    rogne.y2 = 0;

    Fusion fusion;
    fusion.etat = 0;
    fusion.mode = 0;

    params->rogne = rogne;
    params->fus = fusion;
}

void display(Image* image) {
    if (image->pixels == NULL) return;
    int i, wh;
    SDL_Surface *surface;

    SDL_SetWindowSize(image->window, image->width, image->height);
    surface = SDL_GetWindowSurface(image->window);

    if (surface->w > image->width || surface->h > image->height) {
        // Si image plus petite que la fenêtre, remplir avec du noir
        SDL_FillRect(surface, NULL, 0);
        SDL_UpdateWindowSurface(image->window);

        SDL_SetWindowSize(image->window, image->width, image->height);
        surface = SDL_GetWindowSurface(image->window);
    }


    Uint8 *p = (Uint8*)surface->pixels;
    wh = surface->w * surface->h;

    for (i = 0 ; i < wh * 4 ; i++) {
        p[i] = image->pixels[i];
    }

    SDL_UpdateWindowSurface(image->window);
}

void displayCM(Image* imageCM) {
    if (imageCM->pixels == NULL) return;

    int i, imax, j, k, x, y, id, fac = 10;
    Uint8 r, b, v, a;
    SDL_Surface *surface;

    surface = SDL_GetWindowSurface(imageCM->window);
    Uint8 *p = (Uint8*)surface->pixels;
    imax = 256 * 4;

    for (i = 0 ; i < imax ; i++) {
        IDtoXY(i, 16, &x, &y);

        b = imageCM->pixels[i];
        v = imageCM->pixels[++i];
        r = imageCM->pixels[++i];
        a = imageCM->pixels[++i];

        for (j = 0 ; j < fac ; j++) {
            for (k = 0 ; k < fac ; k++) {
                id = XYtoID(x*fac+j, y*fac+k, 160); // Algo de redimensionnement vite fait
                p[id] = b;
                p[++id] = v;
                p[++id] = r;
                p[++id] = a;
            }
        }

    }
    SDL_UpdateWindowSurface(imageCM->window);
}

void displayCtrl(Image* imageCtrl) {
    SDL_Renderer* pRenderer = imageCtrl->renderer;

    if (!pRenderer) return;
    SDL_Surface* pSprite = SDL_LoadBMP("interface.bmp");

    if (pSprite) {
        SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer,pSprite); // Préparation du sprite
        if (pTexture) {

            SDL_RenderCopy(pRenderer,pTexture,NULL,NULL); // Copie du sprite grâce au SDL_Renderer

            SDL_RenderPresent(pRenderer); // Affichage
            SDL_DestroyTexture(pTexture);
        }
        else {
            fprintf(stdout,"Échec de création de la texture (%s)\n",SDL_GetError());
        }

        SDL_FreeSurface(pSprite); // Libération de la ressource occupée par le sprite
    }
    else {
        fprintf(stdout,"Échec de chargement du sprite (%s)\n",SDL_GetError());
    }

    //SDL_DestroyRenderer(pRenderer); // Libération de la mémoire du SDL_Renderer
}
