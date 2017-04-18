// En-tête TGA
/*typedef struct {
    uint8_t   head;           // 00
    uint8_t   colormapType;   // Palette (bool) (+ valeurs fac. pour dev)
    uint8_t   type;           // Type d'image
    uint16_t  colormapStart;  // Début Palette (pos relative)
    uint16_t  colormapSize;   // Taille Palette
    uint8_t   colormapDepth;  // Profondeur Palette
    uint16_t  posX;           // Pos X
    uint16_t  posY;           // Pos Y
    uint16_t  width;          // Largeur
    uint16_t  height;         // Hauteur
    uint8_t   bpp;            // Profondeur : nb de bit par pixels
    uint8_t   attr;           // Attributs
} TGAHeader;
*/

typedef struct Image {
    SDL_Window*     window;
    SDL_Renderer*   renderer;
    int             id;
    Uint8*          pixels;
    long            width;
    long            height;
    int             bpp;
    int             rle;
} Image;

typedef struct Rogne {
    int etat;
    long x1;
    long x2;
    long y1;
    long y2;
} Rogne;

typedef struct Fusion {
    int etat;   // de la fenetre menu
    int mode;   // fusion choisie
} Fusion;

typedef struct Params {
    int lumi;
    int cont;
    int grey;
    int w;
    int h;
    Fusion fus;
    Rogne rogne;
} Params;


// F O N C T I O N S
void urlToExt(char* url, char* ext);
void urlToName(char* url, char* name);
void urlToDir(char* url, char* dir);
void clearCM(Uint8* pixels);
// Fonctions structure Image
int initImage(Image* image, char* title, int x, int y, int w, int h, Uint32 flags, int render);
int initTabImage(Image* image, int q);
void clearImage(Image* image);
void save(char* url);
void my_open(char* url);
long ficSize(FILE* fic);
int checkBytesOK(FILE* fic, long ficsize, long n);
void renameWindow(Image* image, char* url);
void loading(Image* imageO, Image* imageCM, char* url, int force);
void saving(Image* imageM, Image* imageCM, char* url, Params *params);
void resetOptions(Params* params);
void display(Image* image);
void displayCM(Image* imageCM);
void displayCtrl(Image* imageCtrl);


// F O N C T I O N S   I M A G E S
int XYtoID(int x, int y, int width);
void IDtoXY(int id, int width, int* x, int* y);
void permut(Uint8* pixels, int a, int b);
double lim(int a);
int limcont(int a);
int limlum(int a);
Uint8 mixer(Uint8 a, Uint8 b, Uint8 c);
void copyPixels(Uint8* pixelsO, Uint8** pixelsM, long widthO, long heightO, long *widthM, long *heightM);


// F O N C T I O N S   R E S I Z E
void actionResize(Params* params, int w, int h);
void resizePixels(Image* image, Params* params);


// F O N C T I O N S   R O G N E
void rogneMinMax(int* vmin, int* vmax, int v1, int v2);
void rognePreview(Image* imageM, Rogne rogne);
void rogneApply(Image* imageM, Rogne rogne);


// F O N C T I O N S   R O T
void actionRot(Params* params, Image* imageO, Image* imageF, int dir);
void rotRogne(Params* params, int w, int h, int dir);
void rot(int dir, Image* image);


// F O N C T I O N S   M I R O I R
void actionMirror(Params* params, Image* imageO, Image* imageF, char dir) ;
void mirrorRogne(Params* params, int w, int h, char dir);
void mirror(char dir, Image* image);


// F O N C T I O N S   F I L T R E S
void actionLumi(Params* params, int val, int def);
void actionCont(Params* params, int val, int def);
void actionGrey(Params* params);
void contrast(Image* imageM, int cont, int lumi);
void invert(Image * imageO);
void greylevel(Image * imageO, Image * imageM, int grey);
int greymax(int a, int b, int c);
void alpha(Image* imageM);


// F O N C T I O N S   N E X T P R E V
void actionNextPrev(Image* imageO, Image* imageCM, char* url, Params* params, int sens);
void parcourir(char* url, int sens);


// F O N C T I O N S   F U S I O N;
void actionsFus(Image* imageM, Uint8* pixelsTmp, Image* imageF, int action, SDL_Renderer* renderer, SDL_Texture* textureEntoure, Image* imageCM, char* urlf, Params* param);
void superposition(Image * imageM, Image * imageF, Uint8* pt);
void fusalpha(struct Image * imageM, struct Image * imageF,  Uint8* pt);
void eclaircir(struct Image * imageM, struct Image * imageF, Uint8 * pt);
void obscurcir(struct Image * imageM, struct Image * imageF, Uint8 * pt);
void addition(struct Image * imageM, struct Image * imageF, Uint8 * pt);
void soustraction(struct Image * imageM, struct Image * imageF, Uint8 * pt);
void produit(struct Image * imageM, struct Image * imageF, Uint8 * pt);
void lumiere(struct Image * imageM, struct Image * imageF, Uint8 * pt);
void densite(struct Image * imageM, struct Image * imageF, Uint8 * pt);


// F O N C T I O N S   M E N U
int buttons(SDL_Event event, int idImg, int idCtrl);
void rogneDefine(Params* params, int x, int y, int etat);
int evenements(int idImg, int idCtrl, int idCM, int* x, int* y);
void actionOpen(Image* imageO, Image* imageCM, char* url, Params* params);
void actionReset(Image* imageO, Image* imageCM, char* url, Params* params);
void actionArgument(Image* imageO, Image* imageM, Image* imageCM, Image* imageF, char* url, char* urlf, int argc, char** argv);
void actionLoop(Image* imageO, Image* imageM, Image* imageCM, Image* imageCtrl, Image* imageF, char* url, char* urlf);


// F O N C T I O N S   T G A
//-------------- Ecriture --------------
void saveTGA_RLE(FILE* fic, Uint8* pixels, int* i, int* m);
void saveTGA_BVRA32(FILE* fic, Uint8* pixels, int imax, int rle);
void saveTGA_BVR24(FILE* fic, Uint8* pixels, int imax, int rle);
void saveTGA_BVR16(FILE* fic, Uint8* pixels, int imax, int rle);
void saveTGA(FILE* fic, Image* imageM, Image* imageCM);
//-------------- Lecture --------------
void readTGA_RLE(FILE* fic, Uint8* pixels, int* i, int* n, int* m);
void readTGA_BVRA32(FILE* fic, Uint8* pixels, int imax, int rle);
void readTGA_BVR24(FILE* fic, Uint8* pixels, int imax, int rle);
void readTGA_BVR16(FILE* fic, Uint8* pixels, int imax, int rle);
void readTGA_CM8(FILE* fic, Uint8* pixels, int imax, Uint8* cm, int rle);
void readTGA_TypeImage(FILE* fic, Image* imageO, int* colormap);
void readTGA_Header(FILE* fic, long ficsize, Image* imageO, Uint8* colormapDepth, Uint16* colormapStart, Uint16* colormapSize, int* colormap);
void loadTGA(FILE* fic, long ficsize, Image* imageO, Image* imageCM);


// F O N C T I O N S   S O U S   M E N U S
int buttonsTGA(SDL_Event event);
int buttonsBMP(SDL_Event event);
int buttonsFus(SDL_Event event);
int evenementsMenu(int id, int ext, Params* params);
void traceTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int w, int h);
void actionsBMP(Image* image, int action, SDL_Renderer* renderer, SDL_Texture* textureEntoure);
void actionsTGA(Image* image, int action, SDL_Renderer* renderer, SDL_Texture* textureEntoure, SDL_Texture* textureEntoureC, SDL_Texture* textureRaye);
int displayMenu(int ext, Image* image, Params* params, Image* imageO, Image* imageF, Image* imageCM, char* urlf);


// F O N C T I O N S   B M P
//-------------- Écriture --------------
void saveBMP_BVRA32(FILE* fic, Uint8* pixels, int imax, int w, int rle);
void saveBMP_BVRA24(FILE* fic, Uint8* pixels, int imax, int bt, int w, int rle);
void saveBMP(FILE* fic, Image* imageM, Image* imageCM);
//-------------- Lecture --------------
void readBMP_BVRA32_CM(FILE* fic, Uint8* pixels, int imax);
void readBMP_BVRA24_CM(FILE* fic, Uint8* pixels, int imax);
void readBMP_BVRA32(FILE* fic, Uint8* pixels, int imax, int w, int rle);
void readBMP_BVRA24(FILE* fic, Uint8* pixels, int imax, int bt, int w, int rle);
void readBMP_CM8(FILE* fic, Uint8* pixels, int imax, Uint8* cm, int bt, int w, int rle);
void loadBMP(FILE* fic, long ficsize, Image* imageO, Image* imageCM);
