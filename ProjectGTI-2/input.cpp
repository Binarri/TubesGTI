
#include "input.h"
#include "globals.h"
#include "puzzle.h"
#include "render.h"
#include "game.h"
#include <GL/glut.h>
#include <cstdio>
#include <cmath>

// =================MOUSE CLICK HANDLER===================
void mouse(int button, int state, int x, int y) {
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    int flipY = windowHeight - y; 

    // A. RESTART BUTTON (saat Game Over)
    if (isGameOver && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float scaleX = 800.0f / glutGet(GLUT_WINDOW_WIDTH);
        float scaleY = 600.0f / glutGet(GLUT_WINDOW_HEIGHT);
        float mouseX = x * scaleX;
        float mouseY = flipY * scaleY;

        // Hitbox tombol RESTART: x[300-500], y[200-260]
        if (mouseX >= 300 && mouseX <= 500 && mouseY >= 200 && mouseY <= 260) {
            resetGame(); 
            glutPostRedisplay();
            return; 
        }
    }

    // B. INPUT NORMAL (saat game aktif)
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && 
        !isGameOver && !isGameFinished && !isBannerActive && !showLevelPrompt) {
        
        float scaleX = 800.0f / glutGet(GLUT_WINDOW_WIDTH);
        float scaleY = 600.0f / glutGet(GLUT_WINDOW_HEIGHT);
        float mouseX = x * scaleX;
        float mouseY = flipY * scaleY;

        // B0. Tombol RESTART in-game (x:15-115, y:490-533)
        if (mouseX >= 15 && mouseX <= 115 && mouseY >= 490 && mouseY <= 533) {
            resetGame();
            glutPostRedisplay();
            return;
        }

        // B1. Tombol HINT (x:20-120, y:540-575)
        if (mouseX >= 20 && mouseX <= 120 && mouseY >= 540 && mouseY <= 575) {
            // Cek sisa hint masih ada?
            if (sisaHint <= 0) {
                sprintf(alertMsg, "Abis cik! Hint cuma boleh 3x per level.");
                alertTimer = 90;
            } 
            else {
                // Kalau masih ada jatah, baru jalankan DFS
                if (cariHintLangkahSelanjutnya()) {
                    sisaHint--; // Kurangi jatah hint!
                    hintDisplayTimer = 150;
                    printf("AI Hint: Klik papan %d (%s) | Sisa Hint: %d\n", hintPapanIdx, hintIsKanan ? "Kanan" : "Kiri", sisaHint);
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

        // B2. Proximity check: harus cukup dekat untuk interaksi
        if (worldZ < 15.0f) {
            sprintf(alertMsg, "maju lagi cik, tangan lu ga nyampe!");
            alertTimer = 90; 
            glutPostRedisplay(); 
            return; 
        }

        // B3. Color-based picking untuk deteksi paku yang diklik
        unsigned char pixel[3];
        renderForPicking();  // Render scene dengan warna ID
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

            // ATURAN 1: CEK APAKAH ADA PAPAN DEPAN MENUTUPI TITIK PAKU
            int penghalangIdx = -1;
            for (int j = pakuIdx + 1; j < totalPapan; j++) {
                if (!puzzle[j].isSolved && isBoltCovered(pakuX, pakuY, puzzle[j])) {
                    // Melanggar Aturan 1
                    penghalangIdx = j;
                    break;
                }
            }

            // ATURAN 2: CEK APAKAH BADAN PAPAN KETINDIHAN SILANG
            bool ketindihan = isBoardEntangled(pakuIdx, false);

            if (penghalangIdx != -1) {
                sprintf(alertMsg, "Selesaikan papan depan dulu, Cik!");
                alertTimer = 90;
            } else if (ketindihan) {
                // Melanggar Aturan 2 (Baru ditambah)
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

                // Cek apakah papan sudah selesai (kedua paku dicabut)
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
                            showLevelPrompt = true;  // Tanya mau lanjut level berikutnya?
                        } else {
                            isGameFinished = true;   // Game selesai!
                        }
                    }
                }
            }
        } 
        else if (pickedID == 0 && pixel[1] == 0 && pixel[2] == 0) {
            // Klik di area hitam = klik papan penghalang
            sprintf(alertMsg, "Sabar cik, pakunya ketutupan papan depan!");
            alertTimer = 90;
        }
        glutPostRedisplay();
    }
}

// ============KEYBOARD HANDLER (ASCII keys: 'y', 'n', 'a', 'd')=====================
void keyboardKeys(unsigned char key, int x, int y) {
    // --- KONTROL KAMERA INTERAKTIF  ---
    if (!isBannerActive && !isGameOver && !isGameFinished) {
        if (key == 'l' || key == 'L') {
            camAngleY -= 3.0f;   // Kamera berputar mengorbit ke kiri
            glutPostRedisplay();
        } else if (key == 'r' || key == 'R') {
            camAngleY += 3.0f;   // Kamera berputar mengorbit ke kanan
            glutPostRedisplay();
        }
    }

    if (showLevelPrompt) {
        if (key == 'y' || key == 'Y') {
            showLevelPrompt = false;
            isTransitioning = true;  // Animasi kereta jatuh ke level berikutnya
        } else if (key == 'n' || key == 'N') {
            showLevelPrompt = false;
            isGameFinished = true; 
        }
        glutPostRedisplay();
    }
}

//=============== SPECIAL KEY PRESSED (arrow keys, function keys)===================
void pressKey(int key, int x, int y) {
    // Ignore input jika UI overlay aktif
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

//======================== SPECIAL KEY RELEASED===========================
void releaseKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP: 
        case GLUT_KEY_DOWN:
            deltaMove = 0;  // Berhenti bergerak
            break;
    }
}
