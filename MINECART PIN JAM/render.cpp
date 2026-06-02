#include "render.h"
#include "globals.h"
#include "puzzle.h"
#include "ui.h"
#include <GL/glut.h>
#include <cmath>
#include <cstdio>

// Petunjuk
void gambarTeksUI(float x, float y, const char* teks) {
    glRasterPos2f(x, y);
    for (int i = 0; teks[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, teks[i]);
    }
}

// Cube untuk texture
void drawTexturedCube() {
    glBegin(GL_QUADS);
    // Sisi Depan
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
    // Sisi Belakang
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
    // Sisi Atas
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
    // Sisi Bawah
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
    // Sisi Kanan
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
    // Sisi Kiri
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
    glEnd();
}

// Obor
void drawObor(float x, float y, float z, int lightID) {
    glPushMatrix();
        glTranslatef(x, y, z);
        
        GLfloat pos[] = { 0.0f, 0.2f, 0.0f, 1.0f }; 
        glLightfv(lightID, GL_POSITION, pos);

		//======Tongkat obor=======
        glColor3f(1.0f, 1.0f, 1.0f); 
        
        GLfloat no_em[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glMaterialfv(GL_FRONT, GL_EMISSION, no_em);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, torchStickTextureID);

        glPushMatrix();
            glTranslatef(0.0f, -0.2f, -0.05f); 
            glScalef(0.15f, 0.6f, 0.15f); 
            drawTexturedCube(); 
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0f, -0.3f, -0.1f);
            glScalef(0.25f, 0.15f, 0.08f);
            drawTexturedCube();
        glPopMatrix();
        
        glDisable(GL_TEXTURE_2D);

        // ==========GAMBAR API OBOR DINAMIS (DI ATAS TONGKAT) ==============
        GLfloat emApi[] = { 1.0f, 0.6f, 0.2f, 1.0f }; 
        glMaterialfv(GL_FRONT, GL_EMISSION, emApi);

        // TEXTURE API DINAMIS
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, flameTextureIDs[currentFrame]);
        
        glPushMatrix();
            glTranslatef(0.0f, 0.15f, 0.0f); 
            glScalef(0.25f, 0.25f, 0.25f);
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, 0.0f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, 0.0f);
            glEnd();
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
        glMaterialfv(GL_FRONT, GL_EMISSION, no_em);
    glPopMatrix();
}

// Gua
void drawCave() {
    if (texturesLoaded && caveTextureID != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, caveTextureID);  
    }
    glColor3f(0.3f, 0.3f, 0.3f); 

    float startZ = 25.0f;      
    float endZ = -25.0f;
    
    float repeatU = 3.0f;  
    float repeatZ = 5.0f; 

    float wallLen = 6.0f; 
    float archLen = 3.14159f * 6.0f; 
    float totalLen = wallLen + archLen + wallLen;

    float uTopLeft = (wallLen / totalLen) * repeatU;
    float uTopRight = ((wallLen + archLen) / totalLen) * repeatU;

    glBegin(GL_QUAD_STRIP);
    
    // Dinding Kiri
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);        glVertex3f(-6.0f, -3.0f, startZ);
    glTexCoord2f(repeatZ, 0.0f);     glVertex3f(-6.0f, -3.0f, endZ);
    
    glTexCoord2f(0.0f, uTopLeft);    glVertex3f(-6.0f, 3.0f, startZ);
    glTexCoord2f(repeatZ, uTopLeft); glVertex3f(-6.0f, 3.0f, endZ);

    // Atap Lengkung
    int segments = 20;
    float radius = 6.0f;
    float centerY = 3.0f;
    for (int i = 1; i < segments; i++) {
        float fraction = (float)i / segments;
        float angle = 3.14159f - (fraction * 3.14159f); 
        
        float xv = radius * cos(angle);
        float yv = centerY + radius * sin(angle);
        
        glNormal3f(-cos(angle), -sin(angle), 0.0f); 
        
        float u = uTopLeft + (fraction * (uTopRight - uTopLeft));
        
        glTexCoord2f(0.0f, u);    glVertex3f(xv, yv, startZ);
        glTexCoord2f(repeatZ, u); glVertex3f(xv, yv, endZ);
    }

    // Dinding Kanan
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, uTopRight);    glVertex3f(6.0f, 3.0f, startZ);
    glTexCoord2f(repeatZ, uTopRight); glVertex3f(6.0f, 3.0f, endZ);

    glTexCoord2f(0.0f, repeatU);    glVertex3f(6.0f, -3.0f, startZ);
    glTexCoord2f(repeatZ, repeatU); glVertex3f(6.0f, -3.0f, endZ);
    
    glEnd();

    // Lantai Gua
    if (texturesLoaded && floorTextureID != 0) {
        glBindTexture(GL_TEXTURE_2D, floorTextureID); 
    }

    float repeatFloorX = 2.0f; 
    float repeatFloorZ = 8.0f; 
    
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, -3.0f, startZ);
        glTexCoord2f(repeatFloorX, 0.0f); glVertex3f(6.0f, -3.0f, startZ);
        glTexCoord2f(repeatFloorX, repeatFloorZ); glVertex3f(6.0f, -3.0f, endZ);
        glTexCoord2f(0.0f, repeatFloorZ); glVertex3f(-6.0f, -3.0f, endZ);
    glEnd();

    // ===========DINDING BELAKANG PAPAN============ 
    if (texturesLoaded && caveVoidTextureID != 0) {
        glBindTexture(GL_TEXTURE_2D, caveVoidTextureID); 
        glColor3f(1.0f, 1.0f, 1.0f); 
    } else {
        glDisable(GL_TEXTURE_2D);
        glColor3f(0.05f, 0.05f, 0.05f); 
    }
    
    glBegin(GL_POLYGON);
        glNormal3f(0.0f, 0.0f, 1.0f);
        
        glTexCoord2f(0.0f, 1.0f); 
        glVertex3f(-6.0f, -3.0f, endZ);
        
        glTexCoord2f(1.0f, 1.0f); 
        glVertex3f(6.0f, -3.0f, endZ);
        
        glTexCoord2f(1.0f, 0.5f); 
        glVertex3f(6.0f, 3.0f, endZ);
        
        // Penutup atap melengkung
        for (int i = segments; i >= 0; i--) {
            float fraction = (float)i / segments;
            float angle = 3.14159f - (fraction * 3.14159f);
            
            float xv = radius * cos(angle);
            float yv = centerY + radius * sin(angle);
            
            float u_wall = 0.1f + (fraction * 0.8f); 
            float v_wall = 0.5f - (sin(angle) * 0.5f); 
            
            glTexCoord2f(u_wall, v_wall); 
            glVertex3f(xv, yv, endZ);
        }
        
        glTexCoord2f(0.0f, 0.5f); 
        glVertex3f(-6.0f, 3.0f, endZ);
    glEnd();
    
    if (texturesLoaded) glDisable(GL_TEXTURE_2D);

    // Obor
    drawObor(5.6f, 0.5f, 15.0f, GL_LIGHT1);
    drawObor(-5.6f, 0.5f, 5.0f, GL_LIGHT2);
    drawObor(5.6f, 0.5f, -5.0f, GL_LIGHT3);

    // Tampilan Petunjuk Kontrol 
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0f, 1.0f, 1.0f);
    
    gambarTeksUI(20.0f, 55.0f, "PETUNJUK KONTROL");
    gambarTeksUI(20.0f, 30.0f, "Panah Atas: Maju  |  Panah Bawah: Mundur");
    gambarTeksUI(420.0f, 30.0f, "Tombol L: Hadap Kiri  |  Tombol R: Hadap Kanan");

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
}

// Puzzle Piece
void PapanKayu(float x, float y, float z, float alpha) {
    if (texturesLoaded && woodTextureID != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, woodTextureID);
    }
    
    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    
    glPushMatrix();
        glScalef(x, y, z);
        drawTexturedCube();
    glPopMatrix();
    
    if (texturesLoaded) glDisable(GL_TEXTURE_2D);
}

// Pin (baut)
void Baut(float r, float g, float b, float alpha) {
    GLUquadric *quad = gluNewQuadric();
    
    // Kepala baut
    glColor4f(r, g, b, alpha);
    glPushMatrix();
        gluCylinder(quad, 0.22, 0.22, 0.25, 32, 32);  
        gluDisk(quad, 0, 0.22, 32, 1); 
        glTranslatef(0, 0, 0.25f); 
        gluDisk(quad, 0, 0.22, 32, 1); 
    glPopMatrix();
    
    // Lubang + (cross slot)
    glColor4f(r * 0.15f, g * 0.15f, b * 0.15f, alpha);
    for(int i = 0; i < 2; i++) {
        glPushMatrix();
            glRotatef(i * 90, 0, 0, 1);
            glTranslatef(0, 0, 0.251f); 
            glScalef(0.3f, 0.07f, 0.02f); 
            glutSolidCube(1.0); 
        glPopMatrix();
    }
    
    // Batang baut 
    glColor4f(r * 0.6f, g * 0.6f, b * 0.8f, alpha);
    float stemLength = 0.6f;
    float stemRadius = 0.06f;

    glPushMatrix();
        glTranslatef(0, 0, -stemLength); 
        gluCylinder(quad, stemRadius, stemRadius, stemLength, 16, 16);
        for(float z = 0.05f; z < stemLength; z += 0.08f) {
            glPushMatrix();
                glTranslatef(0, 0, z);
                gluDisk(quad, stemRadius, stemRadius + 0.015f, 16, 1); 
            glPopMatrix();
        }
    glPopMatrix();
    gluDeleteQuadric(quad);
}

// Papan puzzle
void GambarBilahPuzzle(float w, float h, float tebal, float rL, float gL, float bL, float rR, float gR, float bR, bool kiriLps, bool kananLps, float alpha) {
    glPushMatrix();
        PapanKayu(w, h, tebal, alpha);
        
        // Paku Kiri (jika belum lepas)
        if (!kiriLps) {
            glPushMatrix();
            glTranslatef(-(w/2.0f) + 0.5f, 0.0f, tebal/2.0f + 0.1f);
            Baut(rL, gL, bL, alpha);
            glPopMatrix();
        }
        // Paku Kanan (jika belum lepas)
        if (!kananLps) {
            glPushMatrix();
            glTranslatef((w/2.0f) - 0.5f, 0.0f, tebal/2.0f + 0.1f);
            Baut(rR, gR, bR, alpha);
            glPopMatrix();
        }
    glPopMatrix();
}

void drawEndWall() {
    glColor3f(0.2f, 0.2f, 0.2f); 
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, -24.8f); 

        for (int i = 0; i < totalPapan; i++) {
            if (!puzzle[i].isSolved || puzzleAlpha[i] > 0.0f) {
                glPushMatrix();
                    glTranslatef(puzzle[i].x, puzzle[i].y, puzzle[i].z);
                    glRotatef(puzzle[i].rot, 0.0f, 0.0f, 1.0f);
                    
                    GambarBilahPuzzle(
                        puzzle[i].w, puzzle[i].h, 0.5f, 
                        puzzle[i].rL, puzzle[i].gL, puzzle[i].bL, 
                        puzzle[i].rR, puzzle[i].gR, puzzle[i].bR, 
                        puzzle[i].bautKiriKlik, puzzle[i].bautKananKlik,
                        puzzleAlpha[i]
                    );

                    if (hintDisplayTimer > 0 && i == hintPapanIdx) {
                        glPushMatrix();
        
                            float posX = -(puzzle[i].w / 2.0f) + 0.5f;
                            if (hintIsKanan) posX = (puzzle[i].w / 2.0f) - 0.5f;
                            
                            glTranslatef(posX, 0.0f, 0.5f / 2.0f + 0.15f); 
                            
                            glDisable(GL_LIGHTING); 
                            glColor3f(1.0f, 1.0f, 0.0f); 
                            glLineWidth(4.0f);
                        
                            glBegin(GL_LINE_LOOP);
                            for (int a = 0; a < 360; a += 10) {
                                float angleRad = a * 3.14159f / 180.0f;
                                glVertex3f(cos(angleRad) * 0.45f, sin(angleRad) * 0.45f, 0.0f);
                            }
                            glEnd();
                            
                            glLineWidth(1.0f); 
                            glEnable(GL_LIGHTING); 
                        glPopMatrix();
                    }
                glPopMatrix();
            }
        }
    glPopMatrix();
}

// Rel Kereta
void drawRel() {
    glPushMatrix();
        glColor3f(0.5f, 0.5f, 0.5f); 
        float relWidth = 0.2f; 
        float yRel = -2.9f;    
        float startZ = 25.0f;  
        float endZ = -25.0f;   

        glBegin(GL_QUADS);
            // Rel kiri
            glVertex3f(-1.5f - relWidth, yRel, startZ);
            glVertex3f(-1.5f + relWidth, yRel, startZ);
            glVertex3f(-1.5f + relWidth, yRel, endZ);
            glVertex3f(-1.5f - relWidth, yRel, endZ);
            // Rel kanan
            glVertex3f(1.5f - relWidth, yRel, startZ);
            glVertex3f(1.5f + relWidth, yRel, startZ);
            glVertex3f(1.5f + relWidth, yRel, endZ);
            glVertex3f(1.5f - relWidth, yRel, endZ);
        glEnd();
    glPopMatrix();
}


void BagianKereta(float scX, float scY, float scZ, GLuint texID, float repX, float repY) {
    if (texturesLoaded && texID != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texID);
        
        glMatrixMode(GL_TEXTURE);
        glPushMatrix();
        glLoadIdentity();
        glScalef(repX, repY, 1.0f);
        glMatrixMode(GL_MODELVIEW);
    }
    
    glColor3f(1.0f, 1.0f, 1.0f); 

    glPushMatrix();
    glScalef(scX, scY, scZ);
    drawTexturedCube();
    glPopMatrix();

    if (texturesLoaded && texID != 0) {
        glMatrixMode(GL_TEXTURE);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glDisable(GL_TEXTURE_2D);
    }
}


void drawCartBody() {
    float t = 0.2f;
    float listT = 0.25f;
    float listH = 0.1f;

    // 1. Lantai Kereta 
    glPushMatrix(); glTranslatef(0, -0.3f, 0); BagianKereta(1.4f, t, 1.8f, cartTextureID, 2.0f, 2.0f); glPopMatrix();

    // 2. Dinding Kereta Kiri, Kanan, Depan, Belakang
    glPushMatrix(); glTranslatef(-0.7f, 0.2f, 0); BagianKereta(t, 0.9f, 1.8f, cartTextureID, 1.0f, 1.0f); glPopMatrix(); 
    glPushMatrix(); glTranslatef(0.7f, 0.2f, 0);  BagianKereta(t, 0.9f, 1.8f, cartTextureID, 1.0f, 1.0f); glPopMatrix(); 
    glPushMatrix(); glTranslatef(0, 0.2f, -0.9f); BagianKereta(1.6f, 0.9f, t, cartTextureID, 1.0f, 1.0f); glPopMatrix(); 
    glPushMatrix(); glTranslatef(0, 0.2f, 0.9f);  BagianKereta(1.6f, 0.9f, t, cartTextureID, 1.0f, 1.0f); glPopMatrix(); 

    // 3. List Kereta 
    glPushMatrix(); glTranslatef(-0.7f, 0.65f, 0); BagianKereta(listT, listH, 1.9f, metalTextureID, 1.0f, 4.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(0.7f, 0.65f, 0);  BagianKereta(listT, listH, 1.9f, metalTextureID, 1.0f, 4.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 0.65f, -0.9f); BagianKereta(1.6f, listH, listT, metalTextureID, 4.0f, 1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 0.65f, 0.9f);  BagianKereta(1.6f, listH, listT, metalTextureID, 4.0f, 1.0f); glPopMatrix();
}

// Penghubung Roda
void drawAxle() {
    glColor3f(0.2, 0.2, 0.2);              
    glPushMatrix();
    glRotatef(90, 0, 1, 0);                
    glTranslatef(0, 0, -1.0);
    GLUquadric* q = gluNewQuadric();       
    gluCylinder(q, 0.05, 0.05, 2.0, 20, 1);
    gluDeleteQuadric(q);
    glPopMatrix();
}

// Roda
void drawWheel() {
    glPushMatrix();
        glRotatef(90, 0, 1, 0);               
        glRotatef(-wheelRot, 0, 0, 1);         

        // Ban hitam
        glColor3f(0.1f, 0.1f, 0.1f);             
        glutSolidTorus(0.08, 0.2, 20, 20);    

        // Velg silver 
        glColor3f(0.4f, 0.4f, 0.4f); 
        glPushMatrix(); glScalef(0.05f, 0.35f, 0.05f); glutSolidCube(1.0f); glPopMatrix();
        glPushMatrix(); glScalef(0.35f, 0.05f, 0.05f); glutSolidCube(1.0f); glPopMatrix();
    glPopMatrix();
}

void drawWheels() {
    float pos[4][3] = {{0.85,-0.5,0.6}, {0.85,-0.5,-0.6}, {-0.85,-0.5,0.6}, {-0.85,-0.5,-0.6}};
    for(int i=0; i<4; i++) {
        glPushMatrix(); glTranslatef(pos[i][0], pos[i][1], pos[i][2]); drawWheel(); glPopMatrix();
    }
    glPushMatrix(); glTranslatef(0, -0.5, 0.6); drawAxle(); glPopMatrix();
    glPushMatrix(); glTranslatef(0, -0.5, -0.6); drawAxle(); glPopMatrix();
}

// Slot indikator lampu
void drawIndicators() {
    float startX = -0.6f;
    float gap = 0.3f; 
    
    for(int i = 0; i < 5; i++) {
        glPushMatrix();
            glTranslatef(startX + (i * gap), 0.2f, -1.0f);
            if (i < boltInCart) {
                glColor3f(cartColors[i][0], cartColors[i][1], cartColors[i][2]);
                GLfloat em[] = {cartColors[i][0], cartColors[i][1], cartColors[i][2], 1.0f};
                glMaterialfv(GL_FRONT, GL_EMISSION, em);
            } else {
                glColor3f(0.1f, 0.1f, 0.1f);
                GLfloat no_em[] = {0,0,0,1};
                glMaterialfv(GL_FRONT, GL_EMISSION, no_em);
            }
            glutSolidSphere(0.1, 20, 20); 
        glPopMatrix();
    }
}

void drawShadow(float keretaX, float keretaZ, float oborX, float oborZ) {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D); 
    
    glDepthMask(GL_FALSE);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
        float lantaiY = -2.95f; 

        float arahX = keretaX - oborX;
        float arahZ = keretaZ - oborZ;

        float stretchX = 1.0f + fabs(arahX) * 0.15f;
        float stretchZ = 1.0f + fabs(arahZ) * 0.15f;

        glTranslatef(keretaX + (arahX * 0.2f), lantaiY, keretaZ + (arahZ * 0.2f));

        glBegin(GL_TRIANGLE_FAN);
            glColor4f(0.0f, 0.0f, 0.0f, 0.65f);
            glVertex3f(0.0f, 0.0f, 0.0f);

            glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
            for (int i = 0; i <= 360; i += 10) {
                float rad = i * 3.14159f / 180.0f;
                
                float x = cos(rad) * 1.4f * stretchX;
                float z = sin(rad) * 2.2f * stretchZ;
                
                glVertex3f(x, 0.0f, z);
            }
        glEnd();
    glPopMatrix();

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE); 
    glEnable(GL_LIGHTING);
}

void renderForPicking() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    glDisable(GL_LIGHTING); 
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DITHER); 
    glDisable(GL_BLEND);  
    
    gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
        glTranslatef(0.0f, -1.5f, worldZ); 
        glTranslatef(0.0f, 0.0f, -24.8f); 

        for (int i = 0; i < totalPapan; i++) {
            if (!puzzle[i].isSolved) {
                glPushMatrix();
                    glTranslatef(puzzle[i].x, puzzle[i].y, puzzle[i].z);
                    glRotatef(puzzle[i].rot, 0.0f, 0.0f, 1.0f);
                    
                    // Papan penghalang: warna hitam
                    glColor3ub(0, 0, 0); 
                    glPushMatrix();
                        glScalef(puzzle[i].w, puzzle[i].h, 0.5f);
                        glutSolidCube(1.0);
                    glPopMatrix();

                    // Hitbox Paku Kiri
                    if (!puzzle[i].bautKiriKlik) {
                        glColor3ub(i * 2 + 1, 0, 0); 
                        glPushMatrix();
                            glTranslatef(-(puzzle[i].w/2.0f) + 0.5f, 0.0f, 0.35f);
                            glutSolidSphere(0.35, 10, 10); 
                        glPopMatrix();
                    }
                    // Hitbox Paku Kanan
                    if (!puzzle[i].bautKananKlik) {
                        glColor3ub(i * 2 + 2, 0, 0); 
                        glPushMatrix();
                            glTranslatef((puzzle[i].w/2.0f) - 0.5f, 0.0f, 0.35f);
                            glutSolidSphere(0.35, 10, 10);
                        glPopMatrix();
                    }
                glPopMatrix();
            }
        }
    glPopMatrix();
    
    glEnable(GL_DITHER); 
    glEnable(GL_BLEND); 
    glEnable(GL_LIGHTING);
}

// ============LINGKUNGAN GUA (Batu, Sarang Laba-laba, Kelelawar)==================
void drawEnvironmentDetails() {
    float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    
	// -----RENDER BEBATUAN & BAYANGANNYA------
    float rockZ[] = { 18.0f, 5.0f, -8.0f, -20.0f }; 

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int i = 0; i < 4; i++) {
        float lantaiY = -2.95f; 
        
        // Bayangan Batu Kiri
        glPushMatrix();
            glTranslatef(-5.0f, lantaiY, rockZ[i]);
            glBegin(GL_TRIANGLE_FAN);
                glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
                glVertex3f(0.0f, 0.0f, 0.0f);
                glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
                for(int a = 0; a <= 360; a += 15) {
                    float rad = a * 3.14159f / 180.0f;
                    glVertex3f(cos(rad) * 2.0f, 0.0f, sin(rad) * 1.5f); // Elips bayangan kiri
                }
            glEnd();
        glPopMatrix();

        // Bayangan Batu Kanan 
        if (i != 1) {
            glPushMatrix();
                glTranslatef(5.0f, lantaiY, rockZ[i] - 3.0f);
                glBegin(GL_TRIANGLE_FAN);
                    glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
                    glVertex3f(0.0f, 0.0f, 0.0f);
                    glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
                    for(int a = 0; a <= 360; a += 15) {
                        float rad = a * 3.14159f / 180.0f;
                        glVertex3f(cos(rad) * 1.8f, 0.0f, sin(rad) * 1.8f); // Elips bayangan kanan
                    }
                glEnd();
            glPopMatrix();
        }
    }

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glEnable(GL_LIGHTING);


    if (texturesLoaded && rockTextureID != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, rockTextureID);
        
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    }
    
    glColor3f(0.8f, 0.8f, 0.8f); 
    
    for (int i = 0; i < 4; i++) {
        // Fisik Batu Kiri
        glPushMatrix();
            glTranslatef(-5.0f, -2.8f, rockZ[i]);
            glRotatef(rockZ[i] * 15.0f, 0.0f, 1.0f, 0.0f); 
            glScalef(1.5f, 0.8f, 1.2f);
            glutSolidDodecahedron(); 
        glPopMatrix();

        // Fisik Batu Kanan
        if (i != 1) { 
            glPushMatrix();
                glTranslatef(5.0f, -2.8f, rockZ[i] - 3.0f); 
                glRotatef(rockZ[i] * -20.0f, 0.0f, 1.0f, 0.0f);
                glScalef(1.2f, 0.7f, 1.5f);
                glutSolidDodecahedron();
            glPopMatrix();
        }
    }

    if (texturesLoaded) {
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);
    }

    // -----------RENDER SARANG LABA-LABA--------------
    glDisable(GL_TEXTURE_2D); 

    glColor3f(0.9f, 0.9f, 0.9f); 
    glLineWidth(3.0f); 

    float webZ[] = { 10.0f, -5.0f, -15.0f };
    for (int i = 0; i < 3; i++) {
        glPushMatrix();
            glTranslatef(-5.0f, 2.5f, webZ[i]); 
            
            glBegin(GL_LINES);
            // Garis rangka sarang
            for (int j = 0; j < 5; j++) {
                float angle = j * 3.14159f / 8.0f;
                glVertex3f(0.0f, 0.0f, 0.0f);
                glVertex3f(cos(angle) * 2.0f, -sin(angle) * 2.0f, 0.0f);
            }
            // Garis melingkar sarang
            for (float r = 0.5f; r <= 1.5f; r += 0.5f) {
                for (int j = 0; j < 4; j++) {
                    float a1 = j * 3.14159f / 8.0f;
                    float a2 = (j + 1) * 3.14159f / 8.0f;
                    glVertex3f(cos(a1) * r, -sin(a1) * r, 0.0f);
                    glVertex3f(cos(a2) * r, -sin(a2) * r, 0.0f);
                }
            }
            glEnd();
        glPopMatrix();
    }
    glLineWidth(1.0f);

    // --------RENDER KELELAWAR ANIMASI KEPAK SAYAP--------------
    glColor3f(0.02f, 0.02f, 0.02f); 

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
            float offsetZ = i * 8.0f;

            float bobbingY = sin(time * 2.0f + i) * 0.3f; 
            
            glTranslatef(-2.5f + (i % 2) * 5.0f, 2.0f + bobbingY, 15.0f - offsetZ);
            
            float wingAngle = sin(time * 12.0f + i) * 35.0f;

            // Badan kelelawar
            glPushMatrix();
                glScalef(0.15f, 0.15f, 0.15f);
                glutSolidCube(1.0f);
            glPopMatrix();

            // Sayap Kiri
            glPushMatrix();
                glRotatef(wingAngle, 0.0f, 0.0f, 1.0f); 
                glBegin(GL_TRIANGLES);
                    glVertex3f(0.0f, 0.0f, 0.0f); // Titik pangkal sayap
                    glVertex3f(-1.0f, 0.2f, -0.2f); // Ujung sayap atas
                    glVertex3f(-0.8f, -0.3f, 0.2f); // Ujung sayap bawah
                glEnd();
            glPopMatrix();

            // Sayap Kanan
            glPushMatrix();
                glRotatef(-wingAngle, 0.0f, 0.0f, 1.0f);
                glBegin(GL_TRIANGLES);
                    glVertex3f(0.0f, 0.0f, 0.0f);
                    glVertex3f(1.0f, 0.2f, -0.2f);
                    glVertex3f(0.8f, -0.3f, 0.2f);
                glEnd();
            glPopMatrix();

        glPopMatrix();
    }
    glEnable(GL_LIGHTING); 
}
