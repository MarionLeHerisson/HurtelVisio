#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "prototypes.h"

//-------------- Écriture --------------

void saveTGA_RLE(FILE* fic, Uint8* pixels, int* i, int* m) {

    if (*m == 0) { // S'il n'y a plus de pixels non encodés à écrire
        Uint8 byte, b, v, r, a, b2, v2, r2, a2;
        int n = 0;

        // Compte le nb de pixels identiques qui se suivent

        // Tant que les deux prochains pixels sont égaux
        while (1) {
            b = pixels[(*i)]; // Premier pixel
            v = pixels[(*i)+1];
            r = pixels[(*i)+2];
            a = pixels[(*i)+3];

            b2 = pixels[(*i)+4]; // Second pixel
            v2 = pixels[(*i)+5];
            r2 = pixels[(*i)+6];
            a2 = pixels[(*i)+7];

            if (b == b2 && v == v2 && r == r2 && a == a2) {
                (*i) += 4; // On passe le pixel sans l'écrire
                n++; // Nb de pixels égaux

                if (n == 127) break; // 127 max (7 bits)
            }
            else break;
        }

        if (n == 0) { // Si aucun pixel égaux
            // Compte le nb de pixels différents qui se suivent
            int j = 0;
            while (1) {
                b = pixels[(*i)+j]; // Premier pixel
                v = pixels[(*i)+j+1];
                r = pixels[(*i)+j+2];
                a = pixels[(*i)+j+3];

                b2 = pixels[(*i)+j+4]; // Second pixel
                v2 = pixels[(*i)+j+5];
                r2 = pixels[(*i)+j+6];
                a2 = pixels[(*i)+j+7];

                if (b != b2 || v != v2 || r != r2 || a != a2) {
                    j += 4; // On passe le pixel sans l'écrire
                    (*m)++; // Nb de pixels différents

                    if (*m == 127) break; // 127 max (7 bits)
                }
                else break;
            }
        }

        if (n) byte = n | 128; // Met le dernier bit à 1;
        else byte = (*m);

        fwrite(&byte, 1, 1, fic);
    }
    else {
        (*m)--;
    }
}

void saveTGA_BVRA32(FILE* fic, Uint8* pixels, int imax, int rle) {
    if (!rle) fwrite(pixels, 1, imax, fic);
    else {
        int i, m = 0;
        for (i = 0 ; i < imax ; ) {
            saveTGA_RLE(fic, pixels, &i, &m);
            fwrite(&pixels[i++], 1, 1, fic); // B
            fwrite(&pixels[i++], 1, 1, fic); // V
            fwrite(&pixels[i++], 1, 1, fic); // R
            fwrite(&pixels[i++], 1, 1, fic); // A
        }
    }
}

void saveTGA_BVR24(FILE* fic, Uint8* pixels, int imax, int rle) {
    int i, m = 0;
    for (i = 0 ; i < imax ; ) {
        if (rle) saveTGA_RLE(fic, pixels, &i, &m);
        fwrite(&pixels[i++], 1, 1, fic); // B
        fwrite(&pixels[i++], 1, 1, fic); // V
        fwrite(&pixels[i++], 1, 1, fic); // R
        i++; // A
    }
}

void saveTGA_BVR16(FILE* fic, Uint8* pixels, int imax, int rle) {
    int i, m = 0;
    Uint16 word;
    double fac = 255./31.;
    for (i = 0 ; i < imax ; ) {
        if (rle) saveTGA_RLE(fic, pixels, &i, &m);
        word = (Uint8) (pixels[i++] / fac);                     // 0000 0000 0001 1111
        word = word | (((Uint8) (pixels[i++] / fac)) << 5) ;    // 0000 0011 1110 0000
        word = word | (((Uint8) (pixels[i++] / fac)) << 10) ;   // 0111 1100 0000 0000
        i++; // A
        fwrite(&word, 2, 1, fic); // BVR chacun sur 5 bits
    }
}

void saveTGA(FILE* fic, Image* imageM, Image* imageCM) {
    Uint8 byte, colormapDepth;
    Uint16 word, colormapStart, colormapSize;
    Uint32 dword;
    int colormap = 0, wh;
    char signature[18] = "TRUEVISION-XFILE.";

    if (imageM->bpp <= 8) colormap = 1;

    byte = 0;
    fwrite(&byte, 1, 1, fic); // 00

    fwrite(&colormap, 1, 1, fic); // Palette (bool) si 8 bits par pixels

    if (!imageM->rle) byte = 1;
    else byte = 9;
    if (!colormap) byte += 1;
    fwrite(&byte, 1, 1, fic); // Type d'image

    colormapStart = 0;
    colormapSize = 0;
    colormapDepth = 0;
    fwrite(&colormapStart, 2, 1, fic); // Début Palette
    fwrite(&colormapSize, 2, 1, fic); // Taille Palette
    fwrite(&colormapDepth, 1, 1, fic); // Profondeur Palette

    word = 0;
    fwrite(&word, 2, 1, fic); // Pos X
    fwrite(&word, 2, 1, fic); // Pos Y

    word = (Uint16)imageM->width;
    fwrite(&word, 2, 1, fic); // Largeur
    word = (Uint16)imageM->height;
    fwrite(&word, 2, 1, fic); // Hauteur
    wh = imageM->width * imageM->height;

    byte = (Uint8)imageM->bpp;
    fwrite(&byte, 1, 1, fic); // Profondeur : nb de bit par pixels

    byte = 0;
    fwrite(&byte, 1, 1, fic); // Attributs*/

    if (colormap) {

    }

    mirror('h', imageM);
    switch (imageM->bpp) {
        //case (8) : saveTGA_CM8(fic, pixels, wh * 4, pixelsCM, rle); break;
        case (16) : saveTGA_BVR16(fic, imageM->pixels, wh * 4, imageM->rle); break;
        case (24) : saveTGA_BVR24(fic, imageM->pixels, wh * 4, imageM->rle); break;
        case (32) : saveTGA_BVRA32(fic, imageM->pixels, wh * 4, imageM->rle); break;
        default : break;
    }

    // Pied

    dword = 0;
    fwrite(&dword, 4, 1, fic); // Taille de l'extension
    fwrite(&dword, 4, 1, fic); // Taille de lu dico de dev (?)
    fwrite(signature, 1, 18, fic);


}

//-------------- Lecture --------------

void readTGA_RLE(FILE* fic, Uint8* pixels, int* i, int* n, int* m) {
    if (*n) { // Pixels encodés à copier n fois
        Uint8 b = pixels[(*i)-4];
        Uint8 v = pixels[(*i)-3]; // Dernier pixel lu
        Uint8 r = pixels[(*i)-2];
        Uint8 a = pixels[(*i)-1];
        while (*n > 0) {
            pixels[(*i)++] = b;
            pixels[(*i)++] = v;
            pixels[(*i)++] = r;
            pixels[(*i)++] = a;
            (*n)--;
        }
        *m = 0;
        *n = 0;
    }

    if (*m == 0) { // S'il n'y a plus de pixels non encodés à lire
        Uint8 byte;
        fread(&byte, 1, 1, fic);
        int cpt = (int)(byte & 0x7F); // Nb de pixels à lire (encodés ou non) (7 premiers bits)

        if ((byte >> 7) == 0) { // Pixels non encodés ? (dernier bit)
            *m = cpt; // m pixels non encodés à lire directement en plus du prochain
            *n = 0;
        }
        else { // Pixels encodés
            *m = 0;
            *n = cpt; // Prochain pixel à copier n fois
        }
    }
    else {
        (*m)--;
    }
}

void readTGA_BVRA32(FILE* fic, Uint8* pixels, int imax, int rle) {
    if (!rle) fread(pixels, 1, imax, fic);
    else {
        int i, n = 0, m = 0;
        for (i = 0 ; i < imax ; ) {
            readTGA_RLE(fic, pixels, &i, &n, &m);
            fread(&pixels[i++], 1, 1, fic); // B
            fread(&pixels[i++], 1, 1, fic); // V
            fread(&pixels[i++], 1, 1, fic); // R
            fread(&pixels[i++], 1, 1, fic); // A
        }
    }
}

void readTGA_BVR24(FILE* fic, Uint8* pixels, int imax, int rle) {
    int i, n = 0, m = 0;
    for (i = 0 ; i < imax ; ) {
        if (rle) readTGA_RLE(fic, pixels, &i, &n, &m);
        fread(&pixels[i++], 1, 1, fic); // B
        fread(&pixels[i++], 1, 1, fic); // V
        fread(&pixels[i++], 1, 1, fic); // R
        pixels[i++] = 255; // Alpha
    }
}

void readTGA_BVR16(FILE* fic, Uint8* pixels, int imax, int rle) {
    int i, n = 0, m = 0;
    Uint16 word;
    double fac = 255./31.;
    for (i = 0 ; i < imax ; ) {
        if (rle) readTGA_RLE(fic, pixels, &i, &n, &m);
        fread(&word, 2, 1, fic); // BVR chacun sur 5 bits
        pixels[i++] = (Uint8)((word & 0x001F) * fac); // 0000 0000 0001 1111 = 001F
        pixels[i++] = (Uint8)((word >> 5 & 0x001F) * fac); // 0000 0011 1110 0000
        pixels[i++] = (Uint8)((word >> 10) * fac); // 0111 1100 0000 0000
        pixels[i++] = 255; // Alpha
    }
}

void readTGA_CM8(FILE* fic, Uint8* pixels, int imax, Uint8* cm, int rle) {
    Uint8 byte;
    int i, n = 0, m = 0;
    for (i = 0 ; i < imax ; ) {
        if (rle) readTGA_RLE(fic, pixels, &i, &n, &m);
        fread(&byte, 1, 1, fic);
        pixels[i++] = cm[byte * 4 + 0];
        pixels[i++] = cm[byte * 4 + 1];
        pixels[i++] = cm[byte * 4 + 2];
        pixels[i++] = cm[byte * 4 + 3];
    }
}

void readTGA_TypeImage(FILE* fic, Image* imageO, int* colormap) {
    Uint8 byte;

    fread(&byte, 1, 1, fic); // Type d'image
    switch(byte) {
        case (0)  : return;
        case (1)  : *colormap = 1; imageO->rle = 0; break;
        case (2)  : *colormap = 0; imageO->rle = 0; break;
        case (3)  : *colormap = 0; imageO->rle = 0; break;
        case (9)  : *colormap = 1; imageO->rle = 1; break;
        case (10) : *colormap = 0; imageO->rle = 1; break;
        case (11) : *colormap = 0; imageO->rle = 1; break;
        default   : *colormap = 0; imageO->rle = 0; break;
    }
}

void readTGA_Header(FILE* fic, long ficsize, Image* imageO, Uint8* colormapDepth, Uint16* colormapStart, Uint16* colormapSize, int* colormap) {
    Uint8 byte, colormapType;
    Uint16 word;

    // En-tête (18 octets)
    if (!checkBytesOK(fic, ficsize, 18)) return;

    fread(&byte, 1, 1, fic); // 00
    if (byte != 0) return;
    fread(&colormapType, 1, 1, fic); // Palette (bool) (+ valeurs fac. pour dev)

    readTGA_TypeImage(fic, imageO, colormap); // Type d'image

    fread(colormapStart, 2, 1, fic); // Début Palette
    fread(colormapSize, 2, 1, fic); // Taille Palette
    fread(colormapDepth, 1, 1, fic); // Profondeur Palette

    fread(&word, 2, 1, fic); // Pos X
    fread(&word, 2, 1, fic); // Pos Y

    fread(&word, 2, 1, fic); // Largeur
    imageO->width = (long)word;
    fread(&word, 2, 1, fic); // Hauteur
    imageO->height = (long)word;

    fread(&byte, 1, 1, fic); // Profondeur : nb de bit par pixels
    imageO->bpp = (int)byte;

    fread(&byte, 1, 1, fic); // Attributs
}

void loadTGA(FILE* fic, long ficsize, Image* imageO, Image* imageCM) {
    Uint8 colormapDepth;
    Uint16 colormapStart, colormapSize;
    int wh, colormap;

    // En-tête
    readTGA_Header(fic, ficsize, imageO, &colormapDepth, &colormapStart, &colormapSize, &colormap);

    // Palette de couleurs
    // Une palette de 16 peut être stockée sur une palette de 256, d'où le déplacement
    fseek(fic, colormapStart, SEEK_CUR);

    clearCM(imageCM->pixels); // Nettoie la palette
    if (colormap) {
        switch (colormapDepth) {
            case (16) : readTGA_BVR16(fic, imageCM->pixels, ((int)colormapSize) * 4, 0); break;
            case (24) : readTGA_BVR24(fic, imageCM->pixels, ((int)colormapSize) * 4, 0); break;
            case (32) : readTGA_BVRA32(fic, imageCM->pixels, ((int)colormapSize) * 4, 0); break;
            default : break;
        }
    }

    // Création du tableau contenant les pixels de l'image
    wh = (imageO->width) * (imageO->height);
    imageO->pixels = malloc(sizeof(Uint8) * wh * 4);
    if (imageO->pixels == NULL) return;

    switch (imageO->bpp) {
        case (8) : readTGA_CM8(fic, imageO->pixels, wh * 4, imageCM->pixels, imageO->rle); break;
        case (16) : readTGA_BVR16(fic, imageO->pixels, wh * 4, imageO->rle); break;
        case (24) : readTGA_BVR24(fic, imageO->pixels, wh * 4, imageO->rle); break;
        case (32) : readTGA_BVRA32(fic, imageO->pixels, wh * 4, imageO->rle); break;
        default : break;
    }

    mirror('h', imageO);

}
