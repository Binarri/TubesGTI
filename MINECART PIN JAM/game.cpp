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
void loadTextures() {
	
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

	// Load texture api untuk obor (3 frame animasi)
	char fileName[20];
    bool flameSuccess = true;

    for (int i = 0; i < 3; i++) {
        sprintf(fileName, "api%d.bmp", i + 1); 
        
        Image* flameImg = loadBMP(fileName);
        if (flameImg) {
            glGenTextures(1, &flameTextureIDs[i]);
            glBindTexture(GL_TEXTURE_2D, flameTextureIDs[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, flameImg->width, flameImg->height, 0, 
                         GL_RGB, GL_UNSIGNED_BYTE, flameImg->pixels);
            delete flameImg;
            printf("%s loaded\n", fileName);
        } else {
            printf("GAGAL load %s - cek file!\n", fileName);
            flameSuccess = false;
        }
    }
    
	// Load texture kayu untuk batang obor
	Image* torchStickImg = loadBMP("torch_stick.bmp");
	if (torchStickImg) {
		glGenTextures(1, &torchStickTextureID);
		glBindTexture(GL_TEXTURE_2D, torchStickTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, torchStickImg->width, torchStickImg->height, 0, 
					 GL_RGB, GL_UNSIGNED_BYTE, torchStickImg->pixels);
		delete torchStickImg;
		printf("torch_stick.bmp loaded\n");
	} else {
		printf("GAGAL load torch_stick.bmp - cek file!\n");
	}
	
	// Load texture untuk lorong
	Image* caveVoidImg = loadBMP("lorong.bmp");
	if (caveVoidImg) {
		glGenTextures(1, &caveVoidTextureID);
		glBindTexture(GL_TEXTURE_2D, caveVoidTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, caveVoidImg->width, caveVoidImg->height, 0, 
					 GL_RGB, GL_UNSIGNED_BYTE, caveVoidImg->pixels);
		delete caveVoidImg;
		printf("lorong.bmp loaded\n");
	} else {
		printf("GAGAL load lorong.bmp - cek file!\n");
	}
	
	// Load texture untuk bebatuan
    Image* rockImg = loadBMP("batu.bmp");
    if (rockImg) {
        glGenTextures(1, &rockTextureID);
        glBindTexture(GL_TEXTURE_2D, rockTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, rockImg->width, rockImg->height, 0, 
                     GL_RGB, GL_UNSIGNED_BYTE, rockImg->pixels);
        delete rockImg;
        printf("batu.bmp loaded\n");
    } else {
        printf("GAGAL load batu.bmp - cek file!\n");
    }


    texturesLoaded = (woodTextureID != 0 && metalTextureID != 0 && cartTextureID != 0 && caveTextureID != 0 && floorTextureID != 0 
                  && torchStickTextureID != 0 && caveVoidTextureID != 0 && rockTextureID != 0 && flameSuccess);

}


void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); 
    glEnable(GL_DEPTH_TEST); 
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL); 
    glEnable(GL_NORMALIZE);

	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Light 0 - Ambient light
    glEnable(GL_LIGHT0);
    GLfloat ambientLight[] = { 0.01f, 0.01f, 0.01f, 1.0f };
    GLfloat diffuseLight0[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat light_pos0[] = { 0.0f, 5.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);

    // Light 1, 2, 3 - Torch lights (Obor)
    glEnable(GL_LIGHT1); 
    glEnable(GL_LIGHT2); 
    glEnable(GL_LIGHT3);
    
    GLfloat torchDiffuse[] = { 1.0f, 0.65f, 0.2f, 1.0f }; 
    GLfloat torchAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    
    glLightfv(GL_LIGHT1, GL_DIFFUSE, torchDiffuse); 
    glLightfv(GL_LIGHT1, GL_AMBIENT, torchAmbient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, torchDiffuse); 
    glLightfv(GL_LIGHT2, GL_AMBIENT, torchAmbient);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, torchDiffuse); 
    glLightfv(GL_LIGHT3, GL_AMBIENT, torchAmbient);

    // Atenuasi untuk torch lights
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.008f);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.008f);
    glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.008f);

    glShadeModel(GL_SMOOTH);
    
    glEnable(GL_TEXTURE_2D);
    
    loadTextures();
    
    glutTimerFunc(100, timerApi, 0);
}

void resetGame() {
    // jumlah papan berdasarkan level
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
    
    camAngleY = 0.0f;
    
    // Reset banner
    isBannerActive = true; 
    bannerY = 600.0f;      
    bannerWaitTime = 0;
    
    // Reset sistem hint
    hintPapanIdx = -1;
    hintDisplayTimer = 0;
    simStepCount = 0;
    
    for(int i = 0; i < 15; i++) {
        puzzleAlpha[i] = 1.0f; 
    }
    
    sisaHint = 3;
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
        worldZ += 1.2f;       
        wheelRot -= 20.0f;   
        
        simStepCount++;
        if (simStepCount > 60) { 
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

	// Animasi Papan Jatuh & Menghilang 
    for(int i = 0; i < totalPapan; i++) {
        if (puzzle[i].isSolved && puzzleAlpha[i] > 0.0f) {
            puzzle[i].y -= 0.2f;      
            puzzleAlpha[i] -= 0.1f;  

            if (puzzle[i].y <= -1.5f) {
                puzzleAlpha[i] = 0.0f;
            } else if (puzzleAlpha[i] < 0.0f) {
                puzzleAlpha[i] = 0.0f; 
            }
        }
    }
	
    if (alertTimer > 0) alertTimer--;
    
    if (hintDisplayTimer > 0) hintDisplayTimer--;
    
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void timerApi(int value) {
    currentFrame = (currentFrame + 1) % 3;
    glutPostRedisplay();                   
    glutTimerFunc(100, timerApi, 0);       
}
