#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <windows.h>
#include <sys/types.h>
#include <dirent.h>
#include "prototypes.h"

void actionNextPrev(Image* imageO, Image* imageCM, char* url, Params* params, int sens) {
    parcourir(url, sens);
    loading(imageO, imageCM, url, 1);
    resetOptions(params);
}

void parcourir(char* url, int sens) {
    struct dirent *lecture;
    DIR *rep;
    char dir[1024];
    char name[1024];
    char name_prem[1024]; name_prem[0] = '\0';
    char name_prec[1024]; name_prec[0] = '\0';
    char name_actu[1024]; name_actu[0] = '\0';
    char name_suiv[1024]; name_suiv[0] = '\0';
    char name_dern[1024]; name_dern[0] = '\0';
    char ext[10];
    int trouve = 0, i = 0;

    urlToName(url, name);
    urlToDir(url, dir);

    // Ouvre le dossier
    rep = opendir(dir);
    if (rep == NULL) return;

    // Parcourt les fichiers
    while ((lecture = readdir(rep))) {

        // Cherche les TGA et BMP uniquement
        urlToExt(lecture->d_name, ext);
        if (strcmp(ext, "tga") && strcmp(ext, "bmp")) continue;

        // Premier fichier
        if (!i) strcpy(name_prem, lecture->d_name);

        // Fichier suivant
        if (trouve == 1) {
            strcpy(name_suiv, lecture->d_name);
            trouve++;
        }

        // Fichier actuel trouvé
        if (strcmp(lecture->d_name, name) == 0) {
            strcpy(name_actu, lecture->d_name);
            trouve = 1;
        }

        // Fichier précédent
        if (!trouve) strcpy(name_prec, lecture->d_name);

        // Dernier fichier
        strcpy(name_dern, lecture->d_name);

        i++;
    }

    if (name_prec[0] == '\0') strcpy(name_prec, name_dern);
    if (name_suiv[0] == '\0') strcpy(name_suiv, name_prem);

    if (sens == -1) strcpy(url, strcat(dir, name_prec));
    else strcpy(url, strcat(dir, name_suiv));

    closedir(rep);
}

