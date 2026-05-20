#include "globals.h"

// Game State
float worldZ = -22.0f;
int deltaMove = 0;
float moveSpeed = 0.5f;
float wheelRot = 0;
float endCamAngle = 0.0f;
int currentLevel = 1;
bool showLevelPrompt = false;
bool isTransitioning = false;
float cartDropY = 0.0f;
bool isBannerActive = true;
float bannerY = 600.0f;
int bannerWaitTime = 0;
bool isGameOver = false;
bool isGameFinished = false;
float creditY = 700.0f;
int totalPapan = 6;

// Puzzle dan Hint
int hintPapanIdx = -1;
int simStepCount = 0;
bool hintIsKanan = false;
int hintDisplayTimer = 0;
int sisaHint = 3;

// Kereta dan Pin
float cartColors[5][3] = {{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}};
int boltInCart = 0;
char alertMsg[100] = "";
int alertTimer = 0;

// Texture
GLuint woodTextureID = 0;
GLuint metalTextureID = 0;
GLuint cartTextureID = 0;
GLuint caveTextureID = 0;
GLuint floorTextureID = 0;
bool texturesLoaded = false;

// Array Puzzle
Bilah puzzle[15];

// State Simulasi
bool simSolved[15];
bool simKiriKlik[15];
bool simKananKlik[15];
float simCart[5][3];
int simBoltInCart;

// Pengaturan Kamera
float camAngleX = 0.0f;
float camAngleY = 0.0f;
int lastMouseX = 0;
int lastMouseY = 0;
bool isMousePressed = false;
