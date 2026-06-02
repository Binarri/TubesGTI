#include <GL/glut.h>
#include <cmath>     
#include <cstdio>     
#include "globals.h"
#include "render.h"
#include "input.h"
#include "ui.h"
#include "game.h"

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();    
    
    if (isGameFinished) {
        float radius = 12.0f; 
        float radians = endCamAngle * 3.14159f / 180.0f; 
        float camX = radius * sin(radians);
        float camZ = -2.0f + (radius * cos(radians)); 
        gluLookAt(camX, 0.0f, camZ, 0.0f, -3.2f, -2.0f, 0.0f, 1.0f, 0.0f);
    } else {
        gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f,  0.0f, 0.0f,  1.0f,  0.0f);
        glRotatef(camAngleY, 0.0f, 1.0f, 0.0f); 
    }

    if (isGameFinished) glClearColor(0.0f, 0.2f, 0.0f, 1.0f); 
    else glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 

    // Render Lingkungan Gua dan Rel
    glPushMatrix();
        glTranslatef(0.0f, -1.5f, worldZ); 
        
        if (!isGameFinished) {
            drawCave(); // Gua
            drawEnvironmentDetails();
        }
        
        drawRel(); // Rel

        if (!isGameFinished && !isTransitioning) {
            drawEndWall(); // Papan TERAKHIR
        }
    glPopMatrix();

    // Render Kereta dan Bayangannya
    glPushMatrix();

        // --- BAYANGAN ---
        float oborX[] = {5.6f, -5.6f, 5.6f};
        float oborZ[] = {15.0f, 5.0f, -5.0f};

        for(int i = 0; i < 3; i++) {
            float realOborZ = worldZ + oborZ[i];
            if (fabs((-2.0f) - realOborZ) < 8.0f) {
                drawShadow(0.0f, -2.0f, oborX[i], realOborZ);
            }
        }

        // --- LORI ---
        glTranslatef(0.0f, -4.0f + cartDropY, -2.0f);
        glPushMatrix();
            glTranslatef(0.0f, 0.8f, 0.0f);
            glRotatef(180.0f, 0.0f, 1.0f, 0.0f); 
            glScalef(1.7f, 1.7f, 1.7f); 
            drawCartBody();
            drawWheels();
            drawIndicators();
        glPopMatrix();

    glPopMatrix();

    // Alert Message
    if (alertTimer > 0) {
        glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0, 800, 0, 600);
        glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

        float aw = getTextWidth(0.13f, alertMsg);
        float ax = (800.0f - aw) / 2.0f;
        float panX1 = ax - 18, panX2 = ax + aw + 18;
        float panY1 = 455, panY2 = 490;
        drawPanel(panX1, panY1, panX2, panY2, 0.0f,0.0f,0.0f,0.78f, 1.0f,0.85f,0.0f, 1.5f);
        glColor3f(1.0f, 0.95f, 0.2f);
        renderTextJumbo(ax, 463, 0.13f, 2.0f, alertMsg);

        glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix();
        glMatrixMode(GL_MODELVIEW); glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
    }
    
    // Credit Screen Setelah Game Finish
    if (isGameFinished) {
        glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0, 800, 0, 600);
        glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

        glColor3f(1.0f, 1.0f, 1.0f);
        renderTextCentered(creditY, 0.22f, 3.0f, "YOU WIN! EZZ YA CIK? XIXI");
        if (creditY <= 300.0f) {
            glColor3f(0.6f, 0.6f, 0.6f); glLineWidth(1.5f);
            glBegin(GL_LINES); glVertex2f(270,270); glVertex2f(530,270); glEnd();
            glLineWidth(1.0f);
            glColor3f(0.8f, 0.8f, 0.8f);
            renderTextCentered(245, 0.13f, 1.8f, "Created by Kelompok 6");
            glColor3f(0.6f, 0.8f, 1.0f);
            renderTextCentered(210, 0.11f, 1.5f, "Informatics UNDIP 2026");
        }

        glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix();
        glMatrixMode(GL_MODELVIEW); glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
    }

    // Level Prompt
    if (showLevelPrompt) {
        glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0, 800, 0, 600); 
        glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

        drawPanel(170, 240, 630, 370, 0.05f,0.05f,0.15f,0.92f, 0.4f,0.6f,1.0f, 2.0f);

        glColor3f(0.4f, 0.6f, 1.0f); glLineWidth(3.0f);
        glBegin(GL_LINES); glVertex2f(170,368); glVertex2f(630,368); glEnd();
        glLineWidth(1.0f);

        glColor3f(0.6f, 0.8f, 1.0f);
        renderTextCentered(338, 0.10f, 1.5f, "LEVEL SELESAI!");

        char promptText[60];
        sprintf(promptText, "Lanjut ke Level %d?", currentLevel + 1);
        glColor3f(1.0f, 1.0f, 1.0f);
        renderTextCentered(296, 0.17f, 2.5f, promptText);

        glColor3f(0.0f, 0.85f, 0.3f);
        renderTextCentered(258, 0.12f, 2.0f, "[Y] Lanjut          [N] Berhenti");

        glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix();
        glMatrixMode(GL_MODELVIEW); glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
    }

    // Screen Game Over
    if (isGameOver) {
        glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST); 
        glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0, 800, 0, 600); 
        glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

        glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0f, 0.0f, 0.0f, 0.80f); 
        glBegin(GL_QUADS);
            glVertex2f(0,0); glVertex2f(800,0); glVertex2f(800,600); glVertex2f(0,600);
        glEnd();
        glDisable(GL_BLEND);

        drawPanel(160, 145, 640, 460, 0.08f,0.02f,0.02f,0.95f, 0.7f,0.1f,0.1f, 2.5f);

        glColor3f(0.8f, 0.1f, 0.1f); glLineWidth(4.0f);
        glBegin(GL_LINES); glVertex2f(160,455); glVertex2f(640,455); glEnd();
        glBegin(GL_LINES); glVertex2f(160,150); glVertex2f(640,150); glEnd();
        glLineWidth(1.0f);

        glColor3f(1.0f, 0.15f, 0.15f);
        renderTextCentered(350, 0.42f, 5.0f, "GAME OVER");

        glColor3f(0.9f, 0.9f, 0.9f);
        renderTextCentered(295, 0.13f, 2.0f, "Lori penuh & jalan lu buntu, Cik!");

        glColor3f(0.4f, 0.4f, 0.4f); glLineWidth(1.0f);
        glBegin(GL_LINES); glVertex2f(240,280); glVertex2f(560,280); glEnd();

        drawPanel(300, 200, 500, 260, 0.0f,0.0f,0.0f,0.75f, 0.2f,0.9f,0.2f, 2.5f);
        glColor3f(0.2f, 1.0f, 0.2f);
        renderTextCentered(219, 0.15f, 2.5f, "[ RESTART ]");

        glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix();
        glMatrixMode(GL_MODELVIEW); glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);   
    }
    
    // Top Level Banner Alert
    if (isBannerActive) {
        glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0, 800, 0, 600); 
        glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

        float bH = 70.0f;
        drawPanel(0, bannerY - bH/2, 800, bannerY + bH/2, 0.0f,0.05f,0.15f,0.92f, 0.3f,0.6f,1.0f, 2.0f);

        glColor3f(0.3f, 0.6f, 1.0f); glLineWidth(3.0f);
        glBegin(GL_LINES); glVertex2f(0, bannerY + bH/2 - 1); glVertex2f(800, bannerY + bH/2 - 1); glEnd();
        glBegin(GL_LINES); glVertex2f(0, bannerY - bH/2 + 1); glVertex2f(800, bannerY - bH/2 + 1); glEnd();
        glLineWidth(1.0f);

        char lvlText[50];
        sprintf(lvlText, "LEVEL  %d", currentLevel);
        glColor3f(1.0f, 1.0f, 1.0f);
        renderTextCentered(bannerY + 2, 0.28f, 3.5f, lvlText);

        char subText[60];
        sprintf(subText, "%d Papan Puzzle", totalPapan);
        glColor3f(0.6f, 0.85f, 1.0f);
        renderTextCentered(bannerY - 28, 0.10f, 1.5f, subText);

        glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix();
        glMatrixMode(GL_MODELVIEW); glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
    }

    // Buttons Hint dan Restart
    if (!isGameOver && !isGameFinished && !isBannerActive && !showLevelPrompt) {
        glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0, 800, 0, 600);
        glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

        drawPanel(15, 535, 115, 578, 0.0f,0.4f,0.15f,1.0f, 0.0f,0.9f,0.4f, 2.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        renderTextJumbo(30, 547, 0.13f, 2.5f, "HINT");

        drawPanel(15, 490, 115, 533, 0.0f,0.0f,0.0f,0.75f, 0.85f,0.2f,0.2f, 2.0f);
        glColor3f(1.0f, 0.4f, 0.4f);
        renderTextJumbo(18, 502, 0.10f, 2.0f, "RESTART");

        if (hintDisplayTimer > 0 && hintPapanIdx != -1) {
            char hintTxt[100];
            sprintf(hintTxt, "Cabut Paku %s di Lapisan Depan!", hintIsKanan ? "KANAN" : "KIRI");
            float hw = getTextWidth(0.12f, hintTxt);
            float hx = 130.0f;
            drawPanel(hx - 8, 535, hx + hw + 8, 578, 0.0f,0.0f,0.0f,0.75f, 0.0f,0.9f,0.4f, 1.5f);
            glColor3f(0.3f, 1.0f, 0.6f);
            renderTextJumbo(hx, 549, 0.12f, 2.0f, hintTxt);
        }

        glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix();
        glMatrixMode(GL_MODELVIEW); glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
    }

    glutSwapBuffers(); 
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w/h, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Tubes Kereta Gua");
    
    init();
    
    glutIgnoreKeyRepeat(1);
    
    glutSpecialFunc(pressKey);
    glutSpecialUpFunc(releaseKey);
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    
    glutMouseFunc(mouse); 
    
    glutKeyboardFunc(keyboardKeys);
    
    resetGame();
    glutMainLoop();
    return 0;
}
