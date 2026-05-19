#include "globals.h"

// === GAME STATE ===
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

// === PUZZLE & AI ===
int hintPapanIdx = -1;
int simStepCount = 0;
bool hintIsKanan = false;
int hintDisplayTimer = 0;
int sisaHint = 3;

// === CART & BOLTS ===
float cartColors[5][3] = {{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}};
int boltInCart = 0;
char alertMsg[100] = "";
int alertTimer = 0;

// === TEXTURE ===
GLuint woodTextureID = 0;
GLuint metalTextureID = 0;
GLuint cartTextureID = 0;
GLuint caveTextureID = 0;
GLuint floorTextureID = 0;
bool texturesLoaded = false;

// === PUZZLE ARRAY ===
Bilah puzzle[15];

// === SIMULATION STATE ===
bool simSolved[15];
bool simKiriKlik[15];
bool simKananKlik[15];
float simCart[5][3];
int simBoltInCart;

// Pengaturan Kamera
float camAngleX = 0.0f; // Sudut putar atas-bawah
float camAngleY = 0.0f; // Sudut putar kanan-kiri
int lastMouseX = 0;     // Posisi X terakhir mouse
int lastMouseY = 0;     // Posisi Y terakhir mouse
bool isMousePressed = false;
