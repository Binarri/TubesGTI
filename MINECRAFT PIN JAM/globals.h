#ifndef GLOBALS_H
#define GLOBALS_H

#include <GL/glut.h>

// Game State
extern float worldZ;
extern int deltaMove;
extern float moveSpeed;
extern float wheelRot;
extern float endCamAngle;
extern int currentLevel;
extern bool showLevelPrompt;
extern bool isTransitioning;
extern float cartDropY;
extern bool isBannerActive;
extern float bannerY;
extern int bannerWaitTime;
extern bool isGameOver;
extern bool isGameFinished;
extern float creditY;
extern int totalPapan;

// Puzzle dan Hint
extern int hintPapanIdx;
extern int simStepCount;
extern bool hintIsKanan;
extern int hintDisplayTimer;
extern int sisaHint;

// Kereta dan Pin
extern float cartColors[5][3];
extern int boltInCart;
extern char alertMsg[100];
extern int alertTimer;

// Texture
extern GLuint woodTextureID;
extern GLuint metalTextureID;
extern GLuint cartTextureID;
extern GLuint caveTextureID;
extern GLuint caveVoidTextureID;
extern GLuint floorTextureID;
extern GLuint flameTextureIDs[4];
extern int currentFrame;
extern GLuint torchStickTextureID;
extern GLuint rockTextureID;
extern bool texturesLoaded;

// Array Puzzle
struct Bilah {
    float x, y, z, rot, w, h;
    float rL, gL, bL;
    float rR, gR, bR;
    bool isSolved, bautKiriKlik, bautKananKlik;
    int id;
};
extern Bilah puzzle[15];
extern float puzzleAlpha[15];

// State Simulasi
extern bool simSolved[15];
extern bool simKiriKlik[15];
extern bool simKananKlik[15];
extern float simCart[5][3];
extern int simBoltInCart;

// Pengaturan Kamera
extern float camAngleX;
extern float camAngleY;
extern int lastMouseX;
extern int lastMouseY;
extern bool isMousePressed;

#endif
