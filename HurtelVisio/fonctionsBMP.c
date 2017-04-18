#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "prototypes.h"

//-------------- Écriture --------------

void saveBMP_BVRA32(FILE* fic, Uint8* pixels, int imax, int w, int rle) {
    int i;
    for (i = 0 ; i < imax ; ) {
        fwrite(&pixels[i++], 1, 1, fic); // B
        fwrite(&pixels[i++], 1, 1, fic); // V
        fwrite(&pixels[i++], 1, 1, fic); // R
        fwrite(&pixels[i++], 1, 1, fic); // A
    }
}

void saveBMP_BVRA24(FILE* fic, Uint8* pixels, int imax, int bt, int w, int rle) {
    int i, j = 0;
    Uint8 zeros[4] = {0,0,0,0};
    for (i = 0 ; i < imax ; ) {
        fwrite(&pixels[i++], 1, 1, fic); // B
        fwrite(&pixels[i++], 1, 1, fic); // V
        fwrite(&pixels[i++], 1, 1, fic); // R
        i++;
        j++;
        if (j == w) { // Une ligne de pixel doit être un multiple de 4 octets
            fwrite(zeros, 1, bt, fic); // Écrire des 0 pour combler
            j = 0;
        }
    }
}

void saveBMP(FILE* fic, Image* imageM, Image* imageCM) {
    // BMP V3
    Uint16 word;
    Uint32 dword, bitmapStart, colormapNbCol, ficsize, wh;
    int colormap = 0, bouchetrou, renverse = 0; // int16
    long sdword; // int32

    // En-tête (14 octets)

    word = 0x4D42;
    fwrite(&word, 2, 1, fic); // BM : 4D42

    wh = imageM->width * imageM->height;
    bouchetrou = (imageM->width * (imageM->bpp/8)) % 4; // Nombre d'octets qui comblent une ligne de pixels
    if (bouchetrou) bouchetrou = 4 - bouchetrou; // 0 1 2 3 -> 0 3 2 1

    ficsize = 14 + 40 + wh*(imageM->bpp/8) + bouchetrou*imageM->height; // Taille du fichier (sans palette)
    fwrite(&ficsize, 4, 1, fic);

    word = 0;
    fwrite(&word, 2, 1, fic); // 0 (Réservé)
    fwrite(&word, 2, 1, fic); // 0 (Réservé)

    bitmapStart = 14 + 40; // Position du début des données des pixels (sans palette)
    fwrite(&bitmapStart, 4, 1, fic);


    // En-tête 2 (40 octets)

    dword = 40; // BMP V3
    fwrite(&dword, 4, 1, fic);

    sdword = (long)imageM->width;
    fwrite(&sdword, 4, 1, fic); // Largeur
    sdword = (long)imageM->height;
    if (renverse) sdword = -sdword;
    fwrite(&sdword, 4, 1, fic); // Hauteur

    word = 1;
    fwrite(&word, 2, 1, fic); // Nombre de "planes" (?) toujours 1

    word = (Uint16)imageM->bpp;
    fwrite(&word, 2, 1, fic); // Profondeur : nb de bit par pixels

    if (imageM->bpp < 16) colormap = 1; // Palette présente

    dword = 0;
    fwrite(&dword, 4, 1, fic); // Type de compression
    fwrite(&dword, 4, 1, fic); // Taille du bitmap en octet, 0 si non compressé

    sdword = 0;
    fwrite(&sdword, 4, 1, fic); // Résolution horizontale (pour impression)
    fwrite(&sdword, 4, 1, fic); // Résolution verticale (pour impression)

    colormapNbCol = 0; // Nb de couleurs de la palette pour bpp < 16.
    fwrite(&colormapNbCol, 4, 1, fic);

    dword = 0;
    fwrite(&dword, 4, 1, fic); // Nb de couleurs les plus importantes dans la palette, rangées au début de celle-ci


    // Palette de couleurs
    if (colormap) {
        // Sauvegarde de la palette à faire
    }

    // Pixels
    if (!renverse) mirror('h', imageM);
    switch (imageM->bpp) {
        case (32) : saveBMP_BVRA32(fic, imageM->pixels, wh * 4, imageM->width, imageM->rle); break;
        case (24) : saveBMP_BVRA24(fic, imageM->pixels, wh * 4, bouchetrou, imageM->width, imageM->rle); break;
        default : break;
    }
}

//-------------- Lecture --------------

void readBMP_BVRA32_CM(FILE* fic, Uint8* pixels, int imax) {
    int i;
    Uint8 byte;
    for (i = 0 ; i < imax ; ) {
        fread(&pixels[i++], 1, 1, fic); // B
        fread(&pixels[i++], 1, 1, fic); // V
        fread(&pixels[i++], 1, 1, fic); // R
        fread(&byte, 1, 1, fic); // Réservé, toujours 0
        pixels[i++] = 255; // A
    }
}

void readBMP_BVRA24_CM(FILE* fic, Uint8* pixels, int imax) {
    int i;
    for (i = 0 ; i < imax ; ) {
        fread(&pixels[i++], 1, 1, fic); // B
        fread(&pixels[i++], 1, 1, fic); // V
        fread(&pixels[i++], 1, 1, fic); // R
        pixels[i++] = 255; // A
    }
}

void readBMP_BVRA32(FILE* fic, Uint8* pixels, int imax, int w, int rle) {
    int i;
    for (i = 0 ; i < imax ; ) {
        fread(&pixels[i++], 1, 1, fic); // B
        fread(&pixels[i++], 1, 1, fic); // V
        fread(&pixels[i++], 1, 1, fic); // R
        fread(&pixels[i++], 1, 1, fic); // A
    }
}

void readBMP_BVRA24(FILE* fic, Uint8* pixels, int imax, int bt, int w, int rle) {
    int i, j = 0;
    for (i = 0 ; i < imax ; ) {
        fread(&pixels[i++], 1, 1, fic); // B
        fread(&pixels[i++], 1, 1, fic); // V
        fread(&pixels[i++], 1, 1, fic); // R
        pixels[i++] = 255; // Alpha
        j++;
        if (j == w) { // Une ligne de pixel doit être un multiple de 4 octets
            fseek(fic, bt, SEEK_CUR);// Si c'est pas le cas, on saute les valeurs bouche-trou
            j = 0;
        }
    }
}

void readBMP_CM8(FILE* fic, Uint8* pixels, int imax, Uint8* cm, int bt, int w, int rle) {
    int i, j = 0;
    Uint8 byte;
    for (i = 0 ; i < imax ; ) {
        fread(&byte, 1, 1, fic);
        pixels[i++] = cm[byte * 4 + 0];
        pixels[i++] = cm[byte * 4 + 1];
        pixels[i++] = cm[byte * 4 + 2];
        pixels[i++] = cm[byte * 4 + 3];
        j++;
        if (j == w) { // Une ligne de pixel doit être un multiple de 4 octets
            fseek(fic, bt, SEEK_CUR);// Si c'est pas le cas, on saute les valeurs bouche-trou
            j = 0;
        }
    }
}

void loadBMP(FILE* fic, long ficsize, Image* imageO, Image* imageCM) {
    // BMP V2 et +
    Uint16 word;
    Uint32 dword, bitmapStart, compressType, colormapNbCol;
    int sword; // int16
    long sdword; // int32
    int wh, colormap = 0, renverse = 0, bmpVersion = 2, bouchetrou;

    // En-tête (14 octets)
    if (!checkBytesOK(fic, ficsize, 14)) return;

    fread(&word, 2, 1, fic); // BM : 4D42
    if (word != 0x4D42) return;

    fread(&dword, 4, 1, fic); // Taille du fichier
    if (dword != ficsize) return;

    fread(&word, 2, 1, fic); // 0 (Réservé)
    fread(&word, 2, 1, fic); // 0 (Réservé)

    fread(&bitmapStart, 4, 1, fic); // Position du début des données des pixels

    // En-tête 2 (12 ou 40 octets)
    fread(&dword, 4, 1, fic); // Taille de second en-tête : 12 = BMP v2, 40 = v3, 108 = v4
    if (dword != 12 && dword != 40 && dword != 108) return;
    if (dword == 40) bmpVersion = 3;
    if (dword == 108) bmpVersion = 4;

    if (!checkBytesOK(fic, ficsize, dword)) return;

    if (bmpVersion == 2) { // V2 Largeur hauteur sur 2 octets}
        fread(&sword, 2, 1, fic); // Largeur
        imageO->width = (int)sword;
        fread(&sword, 2, 1, fic); // Hauteur
        imageO->height = (int)sword;
    }
    else { // V3 ou V4 Largeur hauteur sur 4 octets
        fread(&sdword, 4, 1, fic); // Largeur
        imageO->width = (int)sdword;
        fread(&sdword, 4, 1, fic); // Hauteur
        imageO->height = (int)sdword;
    }

    if (imageO->height < 0) { // Si hauteur négative, renverser l'image
        imageO->height = -(imageO->height);
        renverse = 1;
    }

    wh = (imageO->width) * (imageO->height);

    fread(&word, 2, 1, fic); // Nombre de "planes" (?) toujours 1

    fread(&word, 2, 1, fic); // Profondeur : nb de bit par pixels
    imageO->bpp = (int)word;
    if (imageO->bpp < 16) colormap = 1; // Palette présente

    if (bmpVersion >= 3) { // V3 ou V4
        // Type de compression
        // 0 = sans, 1 = rle 8 bits, 2 = rle 4 bits, 3 = bitfields (pour 16 à 32 bpp)
        fread(&compressType, 4, 1, fic);
        imageO->rle = (int)compressType;
        fread(&dword, 4, 1, fic); // Taille du bitmap en octet, 0 si non compressé
        fread(&sdword, 4, 1, fic); // Résolution horizontale (pour impression)
        fread(&sdword, 4, 1, fic); // Résolution verticale (pour impression)
        // Nb de couleurs de la palette pour bpp < 16.
        // Si 0 et bpp < 16, alors = 1 << bpp (taille max possible de la palette en fonction de bpp)
        fread(&colormapNbCol, 4, 1, fic);
        if (colormapNbCol == 0 && imageO->bpp < 16) colormapNbCol = 1 << (imageO->bpp);
        // Nb de couleurs les plus importantes dans la palette, rangées au début de celle-ci
        // Sert pour les affichages au bpp inférieur au bpp de l'image et avoir un rendu moins dégueux
        fread(&dword, 4, 1, fic);
    }
    else if (bmpVersion == 2) { // V2
        colormapNbCol = 1 << (imageO->bpp); // Nb de couleurs de la palette = 2^bpp
    }

    if (bmpVersion == 4) { // V4
        // (non pris en compte)
        fread(&dword, 4, 1, fic); // Masque RVBA
        fread(&dword, 4, 1, fic);
        fread(&dword, 4, 1, fic);
        fread(&dword, 4, 1, fic);
        fread(&dword, 4, 1, fic); // Type de "color space"
        fread(&sdword, 4, 1, fic); // Coordonnées XYZ RVB
        fread(&sdword, 4, 1, fic);
        fread(&sdword, 4, 1, fic);
        fread(&sdword, 4, 1, fic);
        fread(&sdword, 4, 1, fic);
        fread(&sdword, 4, 1, fic);
        fread(&sdword, 4, 1, fic);
        fread(&sdword, 4, 1, fic);
        fread(&sdword, 4, 1, fic);
        fread(&dword, 4, 1, fic); // Gamma RVB
        fread(&dword, 4, 1, fic);
        fread(&dword, 4, 1, fic);
    }

    // Palette de couleurs
    clearCM(imageCM->pixels);
    if (colormap) {
        if (bmpVersion == 2) { // V2
            readBMP_BVRA24_CM(fic, imageCM->pixels, colormapNbCol * 4);
        }
        else { // V3 ou V4
            readBMP_BVRA32_CM(fic, imageCM->pixels, colormapNbCol * 4);
        }
    }

    // Pixels
    bouchetrou = (imageO->width * (imageO->bpp / 8)) % 4;
    if (bouchetrou) bouchetrou = 4 - bouchetrou; // 0 1 2 3 -> 0 3 2 1

    fseek(fic, bitmapStart, SEEK_SET);
    imageO->pixels = malloc(sizeof(Uint8) * wh * 4);
    if (imageO->pixels == NULL) return;

    switch (imageO->bpp) {
        case (32) : readBMP_BVRA32(fic, imageO->pixels, wh * 4, imageO->width, imageO->rle); break;
        case (24) : readBMP_BVRA24(fic, imageO->pixels, wh * 4, bouchetrou, imageO->width, imageO->rle); break;
        case (8) : readBMP_CM8(fic, imageO->pixels, wh * 4, imageCM->pixels, bouchetrou, imageO->width, imageO->rle); break;
        default : break;
    }

    if (!renverse) mirror('h', imageO);

}
