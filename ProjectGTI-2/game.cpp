#include "game.h"
#include "globals.h"
#include "puzzle.h"
#include "imageloader.h"
#include <GL/glut.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

// Load Texture
void loadCartTextures() {
    // Load texture kayu untuk papan
    Image* woodImg = loadBMP("old_wood.bmp");
    if (woodImg) {
        glGenTextures(1, &woodTextureID);
        glBindTexture(GL_TEXTURE_2D, woodTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, woodImg->width, woodImg->height, 0, 
                     GL_RGB, GL_UNSIGNED_BYTE, woodImg->pixels);
        delete woodImg;
        printf("old_wood.bmp loaded\n");
    } else {
        printf("GAGAL load old_wood.bmp - cek file & format 24-bit BMP!\n");
    }

    // Load texture metal untuk rim/list
    Image* metalImg = loadBMP("metal_rim.bmp");
    if (metalImg) {
        glGenTextures(1, &metalTextureID);
        glBindTexture(GL_TEXTURE_2D, metalTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, metalImg->width, metalImg->height, 0, 
                     GL_RGB, GL_UNSIGNED_BYTE, metalImg->pixels);
        delete metalImg;
        printf("metal_rim.bmp loaded\n");
    } else {
        printf("GAGAL load metal_rim.bmp - cek file & format 24-bit BMP!\n");
    }

	// Load texture kayu untuk body kereta
    Image* cartImg = loadBMP("old_wood.bmp");
    if (cartImg) {
        glGenTextures(1, &cartTextureID);
        glBindTexture(GL_TEXTURE_2D, cartTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cartImg->width, cartImg->height, 0, 
                     GL_RGB, GL_UNSIGNED_BYTE, cartImg->pixels);
        delete cartImg;
        printf("old_wood.bmp loaded\n");
    } else {
        printf("GAGAL load old_wood.bmp - cek file!\n");
    }

	// Load texture batu untuk dinding gua
	Image* caveImg = loadBMP("stone_wall.bmp");
	if (caveImg) {
    	glGenTextures(1, &caveTextureID);
    	glBindTexture(GL_TEXTURE_2D, caveTextureID);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, caveImg->width, caveImg->height, 0, GL_RGB, GL_UNSIGNED_BYTE, caveImg->pixels);
    	delete caveImg;
    	printf("stone_wall.bmp loaded\n");
    } else {
        printf("GAGAL load stone_wall.bmp - cek file!\n");
	}
	
	// Load texture pasir untuk lantai gua
    Image* floorImg = loadBMP("sand.bmp");
    if (floorImg) {
        glGenTextures(1, &floorTextureID);
        glBindTexture(GL_TEXTURE_2D, floorTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, floorImg->width, floorImg->height, 0, 
                     GL_RGB, GL_UNSIGNED_BYTE, floorImg->pixels);
        delete floorImg;
        printf("sand.bmp loaded\n");
    } else {
        printf("GAGAL load sand.bmp - cek file!\n");
    }

    texturesLoaded = (woodTextureID != 0 && metalTextureID != 0 && cartTextureID != 0 && caveTextureID != 0 && floorTextureID != 0);

}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); 
    glEnable(GL_DEPTH_TEST); 
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL); 
    glEnable(GL_NORMALIZE);

    // Light 0 - Ambient light
    glEnable(GL_LIGHT0);
    GLfloat ambientLight[] = { 0.15f, 0.15f, 0.15f, 1.0f }; 
    GLfloat diffuseLight0[] = { 0.3f, 0.3f, 0.3f, 1.0f };   
    GLfloat light_pos0[] = { 0.0f, 5.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);

    // Light 1, 2, 3 - Torch lights (Obor)
    glEnable(GL_LIGHT1); 
    glEnable(GL_LIGHT2); 
    glEnable(GL_LIGHT3);
    
    GLfloat torchDiffuse[] = { 0.9f, 0.5f, 0.1f, 1.0f }; 
    GLfloat torchAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f }; 
    
    glLightfv(GL_LIGHT1, GL_DIFFUSE, torchDiffuse); 
    glLightfv(GL_LIGHT1, GL_AMBIENT, torchAmbient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, torchDiffuse); 
    glLightfv(GL_LIGHT2, GL_AMBIENT, torchAmbient);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, torchDiffuse); 
    glLightfv(GL_LIGHT3, GL_AMBIENT, torchAmbient);

    // Atenuasi untuk torch lights (cahaya meredup sesuai jarak)
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.02f);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.02f);
    glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.02f);

    glShadeModel(GL_SMOOTH);
    
    glEnable(GL_TEXTURE_2D);
    
    loadCartTextures();
}

void resetGame() {
    // Set jumlah papan berdasarkan level
    if (currentLevel == 1) {
        totalPapan = 6;  
    } else if (currentLevel == 2) {
        totalPapan = 9;  
    } else if (currentLevel == 3) {
        totalPapan = 15;
    }

    // Reset semua flag game state
    isGameOver = false;
    isGameFinished = false;
    showLevelPrompt = false;
    isTransitioning = false;
    
    // Reset cart dan bolts
    boltInCart = 0;
    for(int i = 0; i < 5; i++) {
        cartColors[i][0] = 0.0f;
        cartColors[i][1] = 0.0f;
        cartColors[i][2] = 0.0f;
    }
    
    // Reset kamera dan animasi
    worldZ = -22.0f; 
    deltaMove = 0;
    wheelRot = 0;
    alertTimer = 0;
    creditY = 700.0f;
    endCamAngle = 0.0f;
    cartDropY = 0.0f; 
    
    // Reset banner
    isBannerActive = true; 
    bannerY = 600.0f;      
    bannerWaitTime = 0;
    
    // Reset sistem hint
    hintPapanIdx = -1;
    hintDisplayTimer = 0;
    simStepCount = 0;
    
    sisaHint = 3;
    // Generate puzzle baru
    acakPuzzle(); 
    printf("Level %d Dimulai! (Total Papan: %d)\n", currentLevel, totalPapan);
}

void timer(int value) {
    // Animasi Banner
    if (isBannerActive) {
        if (bannerY > 300.0f && bannerWaitTime == 0) {
            bannerY -= 5.0f; 
        } else if (bannerY <= 300.0f && bannerWaitTime < 60) {
            bannerWaitTime++; 
        } else if (bannerWaitTime >= 60) {
            bannerY += 5.0f; 
            if (bannerY > 600.0f) isBannerActive = false; 
        }
    }

    // Animasi Transisi Level
    if (isTransitioning) {
        cartDropY -= 0.3f;
        worldZ += 0.5f;
        wheelRot -= 5.0f;
        if (cartDropY < -15.0f) { 
            currentLevel++; 
            resetGame();
        }
    }
    
    // Animasi Game Finish
    if (isGameFinished) {
        worldZ += 0.8f; 
        wheelRot -= 7.0f;  
        endCamAngle += 0.8f; 
        if (creditY > 300.0f) creditY -= 3.0f;
    } 
    else if (deltaMove != 0) {
        worldZ += deltaMove * moveSpeed;
        if (worldZ < -22.0f) worldZ = -22.0f; 
        if (worldZ > 19.0f) worldZ = 19.0f;  
        if (deltaMove == 1) wheelRot -= 5.0f;  
        else if (deltaMove == -1) wheelRot += 5.0f;
    }

    if(wheelRot > 360) wheelRot -= 360;
    if(wheelRot < -360) wheelRot += 360;

    if (alertTimer > 0) alertTimer--;
    
    if (hintDisplayTimer > 0) hintDisplayTimer--;
    
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}
