#include "input.h"
#include "globals.h"
#include "puzzle.h"
#include "render.h"
#include "game.h"
#include <GL/glut.h>
#include <cstdio>
#include <cmath>

void mouse(int button, int state, int x, int y) {
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    int flipY = windowHeight - y;

    // Tombol Restart saat Game Over
    if (isGameOver && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float scaleX = 800.0f / glutGet(GLUT_WINDOW_WIDTH);
        float scaleY = 600.0f / glutGet(GLUT_WINDOW_HEIGHT);
        float mouseX = x * scaleX;
        float mouseY = flipY * scaleY;

        // Hitbox Tombol Restart
        if (mouseX >= 300 && mouseX <= 500 && mouseY >= 200 && mouseY <= 260) {
            resetGame(); 
            glutPostRedisplay();
            return; 
        }
    }

    // === INPUT saat game aktif ===
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && 
        !isGameOver && !isGameFinished && !isBannerActive && !showLevelPrompt) {
        
        float scaleX = 800.0f / glutGet(GLUT_WINDOW_WIDTH);
        float scaleY = 600.0f / glutGet(GLUT_WINDOW_HEIGHT);
        float mouseX = x * scaleX;
        float mouseY = flipY * scaleY;

        // Tombol Restart in Game
        if (mouseX >= 15 && mouseX <= 115 && mouseY >= 490 && mouseY <= 533) {
            resetGame();
            glutPostRedisplay();
            return;
        }

        // Tombol Hint
        if (mouseX >= 20 && mouseX <= 120 && mouseY >= 540 && mouseY <= 575) {
            if (sisaHint <= 0) {
                sprintf(alertMsg, "Abis cik! Hint cuma boleh 3x per level.");
                alertTimer = 90;
            } 
            else {
                if (cariHintLangkahSelanjutnya()) {
                    sisaHint--;
                    hintDisplayTimer = 150;
                    printf("Hint: Klik papan %d (%s) | Sisa Hint: %d\n", hintPapanIdx, hintIsKanan ? "Kanan" : "Kiri", sisaHint);
                    sprintf(alertMsg, "Hint aktif! Sisa jatah: %d kali", sisaHint);
                    alertTimer = 60;
                } else {
                    sprintf(alertMsg, "Zonk cik, urutan lu udh salah total, mending RESTART!");
                    alertTimer = 90;
                }
            }
            glutPostRedisplay();
            return;
        }

		if (camAngleY != 0.0f) {
            sprintf(alertMsg, "Madep lurus dulu kalo mau cabut paku ya!");
            alertTimer = 90;
            glutPostRedisplay();
            return;
        }
		
        // Pengaturan Jarak untuk Interaksi
        if (worldZ < 15.0f) {
            sprintf(alertMsg, "maju lagi cik, tangan lu ga nyampe!");
            alertTimer = 90; 
            glutPostRedisplay(); 
            return; 
        }

        // Color-based picking untuk deteksi paku yang diklik
        unsigned char pixel[3];
        renderForPicking();
        glReadPixels(x, flipY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

        int pickedID = pixel[0]; 
        
        if (pickedID > 0 && pickedID <= (totalPapan * 2)) {
            int pakuIdx = (pickedID - 1) / 2; 
            bool isKanan = (pickedID % 2 == 0);

            // Hitung posisi paku di world space
            float rad = puzzle[pakuIdx].rot * 3.14159f / 180.0f;
            float dist = (puzzle[pakuIdx].w / 2.0f) - 0.5f;
            if (!isKanan) dist = -dist;
            float pakuX = puzzle[pakuIdx].x + dist * cos(rad);
            float pakuY = puzzle[pakuIdx].y + dist * sin(rad);

            // Cek ada papan depan yang nutupin titik paku atau tidak
            int penghalangIdx = -1;
            for (int j = pakuIdx + 1; j < totalPapan; j++) {
                if (!puzzle[j].isSolved && isBoltCovered(pakuX, pakuY, puzzle[j])) {
                    penghalangIdx = j;
                    break;
                }
            }

            // Cek badan papan yang ketindihan silang
            bool ketindihan = isBoardEntangled(pakuIdx, false);

            if (penghalangIdx != -1) {
                sprintf(alertMsg, "Selesaikan papan depan dulu, Cik!");
                alertTimer = 90;
            } else if (ketindihan) {
                sprintf(alertMsg, "Papannya ketindihan silang! Cabut yg depan dulu.");
                alertTimer = 90;
            } else {
                if (isKanan) {
                    if (!puzzle[pakuIdx].bautKananKlik) {
                        puzzle[pakuIdx].bautKananKlik = true;
                        tambahBautKeKereta(puzzle[pakuIdx].rR, puzzle[pakuIdx].gR, puzzle[pakuIdx].bR);
                    }
                } else {
                    if (!puzzle[pakuIdx].bautKiriKlik) {
                        puzzle[pakuIdx].bautKiriKlik = true;
                        tambahBautKeKereta(puzzle[pakuIdx].rL, puzzle[pakuIdx].gL, puzzle[pakuIdx].bL);
                    }
                }

                // Clear hint setelah klik berhasil
                hintPapanIdx = -1;
                hintDisplayTimer = 0;

                // Cek apakah papan sudah selesai
                if (puzzle[pakuIdx].bautKiriKlik && puzzle[pakuIdx].bautKananKlik) {
                    puzzle[pakuIdx].isSolved = true;
                    
                    // Cek apakah semua papan sudah selesai
                    bool allSolved = true;
                    for (int j = 0; j < totalPapan; j++) {
                        if (!puzzle[j].isSolved) {
                            allSolved = false;
                            break;
                        }
                    }

                    if (allSolved) {
                        if (currentLevel < 3) {
                            showLevelPrompt = true; 
                        } else {
                            isGameFinished = true;
                        }
                    }
                }
            }
        } 
        else if (pickedID == 0 && pixel[1] == 0 && pixel[2] == 0) {
            sprintf(alertMsg, "Sabar cik, pakunya ketutupan papan depan!");
            alertTimer = 90;
        }
        glutPostRedisplay();
    }
}

// Kontrol kamera kekiri dan kekanan
void keyboardKeys(unsigned char key, int x, int y) {
    if (!isBannerActive && !isGameOver && !isGameFinished) {
        if (key == 'l' || key == 'L') {
            if (camAngleY > -15.0f) {
                camAngleY -= 3.0f;
                glutPostRedisplay();
            }
        } else if (key == 'r' || key == 'R') {
            if (camAngleY < 15.0f) {
                camAngleY += 3.0f; 
                glutPostRedisplay();
            }
        }
    }

    // Lanjut level atau Ending
    if (showLevelPrompt) {
        if (key == 'y' || key == 'Y') {
            showLevelPrompt = false;
            isTransitioning = true;
        } else if (key == 'n' || key == 'N') {
            showLevelPrompt = false;
            isGameFinished = true;
        }
        glutPostRedisplay();
    }
}

void pressKey(int key, int x, int y) {
    if (isBannerActive || isGameOver || isGameFinished || showLevelPrompt) return; 
    
    switch (key) {
        case GLUT_KEY_UP:
            deltaMove = 1;   // Maju
            break;   
        case GLUT_KEY_DOWN:
            deltaMove = -1;  // Mundur
            break; 
    }
}

void releaseKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP: 
        case GLUT_KEY_DOWN:
            deltaMove = 0;  // Berhenti
            break;
    }
}
