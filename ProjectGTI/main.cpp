#include <GL/glut.h>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <stdio.h>

// ==========================================
// VARIABEL GLOBAL
// ==========================================
float worldZ = -35.0f;  // Posisi dunia (kamera mulai dari luar)
int deltaMove = 0;      // Kontrol maju/mundur
float moveSpeed = 0.5f; // Kecepatan jalan
float wheelRot = 0;     // Animasi roda kereta
// Menyimpan warna baut yang sudah diambil ke dalam kereta (maksimal 3 sesuai jumlah indikator)
float cartColors[3][3] = {{0,0,0}, {0,0,0}, {0,0,0}}; 
int boltInCart = 0;
bool isGameOver = false;
char alertMsg[100] = ""; 
int alertTimer = 0; // Durasi pesan muncul di layar
bool isGameFinished = false;
float creditY = 700.0f; // Start di atas layar (layar kita 600)

// Tambahkan di dalam Struct Bilah yang tadi:
// bool bautKiriLepas = false;
// bool bautKananLepas = false;

// ==========================================
// FUNGSI GUA & LINGKUNGAN (TANPA TEKSTUR)
// ==========================================
void drawCave() {
    // 1. Dinding Kiri
    glPushMatrix();
        glColor3f(0.3f, 0.2f, 0.1f); // Cokelat
        glTranslatef(-6.0f, 0.0f, 0.0f);
        glScalef(0.5f, 6.0f, 50.0f);
        glutSolidCube(1.0);
    glPopMatrix();

    // 2. Dinding Kanan
    glPushMatrix();
        glColor3f(0.3f, 0.2f, 0.1f); // Cokelat
        glTranslatef(6.0f, 0.0f, 0.0f);
        glScalef(0.5f, 6.0f, 50.0f);
        glutSolidCube(1.0);
    glPopMatrix();

    // 3. Atap Setengah Lingkaran (Quad Strip)
    glPushMatrix();
        glColor3f(0.2f, 0.15f, 0.1f); // Cokelat gelap
        float radius = 6.0f;       
        float centerY = 3.0f;      
        int segments = 20;         
        float startZ = 25.0f;      
        float endZ = -25.0f;       

        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= segments; i++) {
            float angle = i * 3.14159f / segments;
            float x = radius * cos(angle);
            float y = centerY + (radius * sin(angle));
            glVertex3f(x, y, startZ);
            glVertex3f(x, y, endZ);
        }
        glEnd();
    glPopMatrix();

    // 4. Lantai
    glPushMatrix();
        glColor3f(0.15f, 0.1f, 0.05f); // Cokelat sangat gelap
        glTranslatef(0.0f, -3.0f, 0.0f); 
        glScalef(12.5f, 0.1f, 50.0f); 
        glutSolidCube(1.0);
    glPopMatrix();
}

void PapanKayu(float x, float y, float z) {
    // Kita netralin lighting sedikit biar warnanya gak kegelapan
    glPushAttrib(GL_LIGHTING_BIT);
    
    // Warna Cokelat Kayu (sedikit lebih terang biar kontras sama gua)
    glColor3f(0.5f, 0.35f, 0.2f); 
    
    glPushMatrix();
        glScalef(x, y, z);
        glutSolidCube(1.0);
    glPopMatrix();
    
    glPopAttrib();
}

//MEMBUAT BAUT
void Baut(float r, float g, float b) {
    GLUquadric *quad = gluNewQuadric();
    //Kepala baut
    glColor3f(r, g, b);
    glPushMatrix();
        gluCylinder(quad, 0.22, 0.22, 0.25, 32, 32);  
        gluDisk(quad, 0, 0.22, 32, 1); //Tutup silinder bagian belakang
        glTranslatef(0, 0, 0.25f); 
        gluDisk(quad, 0, 0.22, 32, 1); //Tutup silinder bagian depan
    glPopMatrix();
    //Lubang +
    glColor3f(r * 0.15f, g * 0.15f, b * 0.15f); 
    for(int i = 0; i < 2; i++) {
        glPushMatrix();
            glRotatef(i * 90, 0, 0, 1);
            glTranslatef(0, 0, 0.251f); 
            glScalef(0.3f, 0.07f, 0.02f); 
            glutSolidCube(1.0); 
        glPopMatrix();
    }
    //Batang baut
	glColor3f(r * 0.6f, g * 0.6f, b * 0.8f); 
    float stemLength = 0.6f;
    float stemRadius = 0.06f;

    glPushMatrix();
        glTranslatef(0, 0, -stemLength); 
        gluCylinder(quad, stemRadius, stemRadius, stemLength, 16, 16);
        //Ulir
        for(float z = 0.05f; z < stemLength; z += 0.08f) {
            glPushMatrix();
                glTranslatef(0, 0, z);
                gluDisk(quad, stemRadius, stemRadius + 0.015f, 16, 1); 
            glPopMatrix();
        }
    glPopMatrix();
    gluDeleteQuadric(quad);
}

void GambarBilahPuzzle(float w, float h, float tebal, float rL, float gL, float bL, float rR, float gR, float bR, bool kiriLps, bool kananLps) {
    glPushMatrix();
        PapanKayu(w, h, tebal);
        // Paku Kiri
        if (!kiriLps) {
            glPushMatrix();
            glTranslatef(-(w/2.0f) + 0.5f, 0.0f, tebal/2.0f + 0.1f);
            Baut(rL, gL, bL);
            glPopMatrix();
        }
        // Paku Kanan
        if (!kananLps) {
            glPushMatrix();
            glTranslatef((w/2.0f) - 0.5f, 0.0f, tebal/2.0f + 0.1f);
            Baut(rR, gR, bR);
            glPopMatrix();
        }
    glPopMatrix();
}

struct Bilah {
    float x, y, z, rot, w, h;
    // Warna paku kiri dan kanan dipisah
    float rL, gL, bL; 
    float rR, gR, bR; 
    bool isSolved, bautKiriKlik, bautKananKlik;
    int id;
};

// Kita buat 6 papan dengan posisi "acak" yang kita tentukan sendiri
Bilah puzzle[6] = {
    // x,    y,    z,   rot,    w,    h,    r,   g,   b,    solved, id, kiri, kanan
    { 0.0, -2.0,  0.1,  15.0,  8.0,  1.5,  1.0, 0.0, 0.0,  false,  1,  false, false}, 
    {-1.0,  0.5,  0.2, -20.0,  7.0,  1.5,  0.0, 1.0, 0.0,  false,  2,  false, false}, 
    { 1.5,  3.0,  0.3,  45.0,  6.0,  1.5,  0.0, 0.0, 1.0,  false,  3,  false, false}, 
    {-2.0,  5.0,  0.4,  -5.0,  9.0,  1.5,  1.0, 1.0, 0.0,  false,  4,  false, false}, 
    { 0.5,  7.0,  0.5,  10.0,  7.5,  1.5,  1.0, 0.0, 1.0,  false,  5,  false, false}, 
    {-0.5,  9.0,  0.6, -30.0,  8.5,  1.5,  0.0, 1.0, 1.0,  false,  6,  false, false}
};

void acakPuzzle() {
    srand(time(NULL));

    // 1. Siapkan 4 warna berbeda
    float colors[4][3];
    for (int i = 0; i < 4; i++) {
        colors[i][0] = (float)(rand() % 10) / 10.0f;
        colors[i][1] = (float)(rand() % 10) / 10.0f;
        colors[i][2] = (float)(rand() % 10) / 10.0f;
    }

    // 2. Siapkan dua Pool Warna (Pool Luar & Pool Dalam)
    // Masing-masing pool berisi 6 slot warna (untuk 2 set triplet)
    float poolLuar[6][3];
    float poolDalam[6][3];

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            // Warna index 0 & 1 masuk ke Pool Luar (Papan 3, 4, 5)
            poolLuar[i * 3 + j][0] = colors[i][0];
            poolLuar[i * 3 + j][1] = colors[i][1];
            poolLuar[i * 3 + j][2] = colors[i][2];

            // Warna index 2 & 3 masuk ke Pool Dalam (Papan 0, 1, 2)
            poolDalam[i * 3 + j][0] = colors[i + 2][0];
            poolDalam[i * 3 + j][1] = colors[i + 2][1];
            poolDalam[i * 3 + j][2] = colors[i + 2][2];
        }
    }

    // 3. Kocok (Shuffle) masing-masing pool secara terpisah
    for (int i = 5; i > 0; i--) {
        int jL = rand() % (i + 1);
        int jD = rand() % (i + 1);
        for (int k = 0; k < 3; k++) {
            // Shuffle Pool Luar
            float tempL = poolLuar[i][k];
            poolLuar[i][k] = poolLuar[jL][k];
            poolLuar[jL][k] = tempL;
            // Shuffle Pool Dalam
            float tempD = poolDalam[i][k];
            poolDalam[i][k] = poolDalam[jD][k];
            poolDalam[jD][k] = tempD;
        }
    }

    // 4. Masukkan ke tiap papan berdasarkan kedalaman (Z-Order)
    for (int i = 0; i < 6; i++) {
        puzzle[i].x = (float)(rand() % 40 - 20) / 10.0f;
        puzzle[i].y = (float)(rand() % 60 + 10) / 10.0f;
        puzzle[i].z = (float)i * 0.15f; // Semakin besar i, semakin di depan
        puzzle[i].rot = (float)(rand() % 60 - 30);
        puzzle[i].w = (float)(rand() % 20 + 60) / 10.0f;
        puzzle[i].h = 1.2f;

        // Jika i = 0, 1, 2 (LAPISAN DALAM) -> Ambil dari poolDalam
        if (i < 3) {
            puzzle[i].rL = poolDalam[i * 2][0];
            puzzle[i].gL = poolDalam[i * 2][1];
            puzzle[i].bL = poolDalam[i * 2][2];
            puzzle[i].rR = poolDalam[i * 2 + 1][0];
            puzzle[i].gR = poolDalam[i * 2 + 1][1];
            puzzle[i].bR = poolDalam[i * 2 + 1][2];
        }
        // Jika i = 3, 4, 5 (LAPISAN LUAR) -> Ambil dari poolLuar
        else {
            int offset = (i - 3) * 2;
            puzzle[i].rL = poolLuar[offset][0];
            puzzle[i].gL = poolLuar[offset][1];
            puzzle[i].bL = poolLuar[offset][2];
            puzzle[i].rR = poolLuar[offset + 1][0];
            puzzle[i].gR = poolLuar[offset + 1][1];
            puzzle[i].bR = poolLuar[offset + 1][2];
        }

        puzzle[i].isSolved = false;
        puzzle[i].bautKiriKlik = false;
        puzzle[i].bautKananKlik = false;
    }
}

void drawEndWall() {
    // Selalu reset warna ke netral agar tembok tidak ikut warna paku terakhir
    glColor3f(0.2f, 0.2f, 0.2f); 

    glPushMatrix();
        // Tempel di ujung gua (Z = -24.8)
        glTranslatef(0.0f, 0.0f, -24.8f); 

        for (int i = 0; i < 6; i++) {
            // Papan cuma digambar kalau belum "Solved" (dua paku belum lepas semua)
            if (!puzzle[i].isSolved) {
                glPushMatrix();
                    glTranslatef(puzzle[i].x, puzzle[i].y, puzzle[i].z);
                    glRotatef(puzzle[i].rot, 0.0f, 0.0f, 1.0f);
                    
                    // Panggil fungsi Bilah dengan parameter warna Kiri (L) dan Kanan (R)
                    // Serta kirim status apakah pakunya sudah diklik atau belum
                    GambarBilahPuzzle(
                        puzzle[i].w, puzzle[i].h, 0.5f, 
                        puzzle[i].rL, puzzle[i].gL, puzzle[i].bL, // Warna Kiri
                        puzzle[i].rR, puzzle[i].gR, puzzle[i].bR, // Warna Kanan
                        puzzle[i].bautKiriKlik, puzzle[i].bautKananKlik // Status Klik
                    );
                glPopMatrix();
            }
        }
    glPopMatrix();
}

void drawRel() {
    // Rel kereta sederhana
    glPushMatrix();
        glColor3f(0.5f, 0.5f, 0.5f); // Warna besi polos
        float relWidth = 0.2f; 
        float yRel = -2.9f;    
        float startZ = 25.0f;  
        float endZ = -25.0f;   

        // Rel Kiri
        glBegin(GL_QUADS);
            glVertex3f(-1.5f - relWidth, yRel, startZ);
            glVertex3f(-1.5f + relWidth, yRel, startZ);
            glVertex3f(-1.5f + relWidth, yRel, endZ);
            glVertex3f(-1.5f - relWidth, yRel, endZ);
        glEnd();

        // Rel Kanan
        glBegin(GL_QUADS);
            glVertex3f(1.5f - relWidth, yRel, startZ);
            glVertex3f(1.5f + relWidth, yRel, startZ);
            glVertex3f(1.5f + relWidth, yRel, endZ);
            glVertex3f(1.5f - relWidth, yRel, endZ);
        glEnd();
    glPopMatrix();
}

// ==========================================
// FUNGSI KERETA DOSEN (SUDAH DIPERSIMPEL)
// ==========================================
void drawCartBody() {
    float t = 0.2;
    float listT = 0.25;
    float listH = 0.1;
    
    // --- MATERIAL BESI DASAR (GELAP) ---
    glColor3f(0.15f, 0.15f, 0.15f); 
    glPushMatrix(); glTranslatef(0, -0.3, 0); glScalef(1.4, t, 1.8); glutSolidCube(1.0); glPopMatrix();

    // --- MATERIAL DINDING (KAYU TUA) ---
    // Warna cokelat gelap biar mirip dinding gua
    glColor3f(0.25f, 0.15f, 0.1f); 
    glPushMatrix(); glTranslatef(-0.7, 0.2, 0); glScalef(t, 0.9, 1.8); glutSolidCube(1.0); glPopMatrix();
    glPushMatrix(); glTranslatef(0.7, 0.2, 0); glScalef(t, 0.9, 1.8); glutSolidCube(1.0); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 0.2, -0.9); glScalef(1.6, 0.9, t); glutSolidCube(1.0); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 0.2, 0.9); glScalef(1.6, 0.9, t); glutSolidCube(1.0); glPopMatrix();

    // --- MATERIAL LIST/RIM (BESI KILAP) ---
    glColor3f(0.3f, 0.3f, 0.3f); 
    glPushMatrix(); glTranslatef(-0.7, 0.65, 0); glScalef(listT, listH, 1.9); glutSolidCube(1.0); glPopMatrix();
    glPushMatrix(); glTranslatef(0.7, 0.65, 0); glScalef(listT, listH, 1.9); glutSolidCube(1.0); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 0.65, -0.9); glScalef(1.6, listH, listT); glutSolidCube(1.0); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 0.65, 0.9); glScalef(1.6, listH, listT); glutSolidCube(1.0); glPopMatrix();
}

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

void drawWheel() {
    glPushMatrix();
        glRotatef(90, 0, 1, 0);               
        glRotatef(wheelRot, 0, 0, 1);         // Roda berputar

        // 1. Pinggiran roda (Donat)
        glColor3f(0.1f, 0.1f, 0.1f);             
        glutSolidTorus(0.08, 0.2, 20, 20);    

        // 2. Palang / Jari-jari roda biar kelihatan muter
        glColor3f(0.4f, 0.4f, 0.4f); // Warna abu-abu besi terang
        
        // Palang vertikal
        glPushMatrix();
            glScalef(0.05f, 0.35f, 0.05f);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Palang horizontal
        glPushMatrix();
            glScalef(0.35f, 0.05f, 0.05f);
            glutSolidCube(1.0f);
        glPopMatrix();

    glPopMatrix();
}

void drawWheels() {
    float pos[4][3] = {{0.85,-0.5,0.6}, {0.85,-0.5,-0.6}, {-0.85,-0.5,0.6}, {-0.85,-0.5,-0.6}};
    for(int i=0; i<4; i++) {
        glPushMatrix();
        glTranslatef(pos[i][0], pos[i][1], pos[i][2]);
        drawWheel();
        glPopMatrix();
    }
    glPushMatrix(); glTranslatef(0, -0.5, 0.6); drawAxle(); glPopMatrix();
    glPushMatrix(); glTranslatef(0, -0.5, -0.6); drawAxle(); glPopMatrix();
}

void renderForPicking() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    glDisable(GL_LIGHTING); 
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DITHER); // TAMBAHKAN INI CIK!
    glDisable(GL_BLEND);  // DAN INI!

    // 1. WAJIB SAMA DENGAN DISPLAY
    gluLookAt(0.0f, 0.0f, 10.0f, 
              0.0f, 0.0f, 0.0f, 
              0.0f, 1.0f, 0.0f);

    glPushMatrix();
        // 2. WAJIB SAMA DENGAN DISPLAY (Biar paku ikut gerak bareng gua)
        glTranslatef(0.0f, -1.5f, worldZ); 
        
        // 3. Masuk ke koordinat EndWall
        glTranslatef(0.0f, 0.0f, -24.8f); 

        for (int i = 0; i < 6; i++) {
            if (!puzzle[i].isSolved) {
                glPushMatrix();
                    glTranslatef(puzzle[i].x, puzzle[i].y, puzzle[i].z);
                    glRotatef(puzzle[i].rot, 0.0f, 0.0f, 1.0f);
                    
                    // Hitbox Kiri (Gedein bolanya biar gampang diklik)
                    if (!puzzle[i].bautKiriKlik) {
                        glColor3ub(i * 2 + 1, 0, 0); 
                        glPushMatrix();
                            glTranslatef(-(puzzle[i].w/2.0f) + 0.5f, 0.0f, 0.35f);
                            glutSolidSphere(1.2, 10, 10); 
                        glPopMatrix();
                    }
                    // Hitbox Kanan
                    if (!puzzle[i].bautKananKlik) {
                        glColor3ub(i * 2 + 2, 0, 0); 
                        glPushMatrix();
                            glTranslatef((puzzle[i].w/2.0f) - 0.5f, 0.0f, 0.35f);
                            glutSolidSphere(1.2, 10, 10);
                        glPopMatrix();
                    }
                glPopMatrix();
            }
        }
    glPopMatrix();
    glEnable(GL_DITHER); // Nyalain lagi setelah beres picking
    glEnable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

// 1. Gunakan fabsf agar selisih desimal tetap terbaca
bool isSameColor(float c1[3], float c2[3]) {
    return (fabsf(c1[0] - c2[0]) < 0.01f && 
            fabsf(c1[1] - c2[1]) < 0.01f && 
            fabsf(c1[2] - c2[2]) < 0.01f);
}

void tambahBautKeKereta(float r, float g, float b) {
    if (boltInCart < 3) {
        cartColors[boltInCart][0] = r;
        cartColors[boltInCart][1] = g;
        cartColors[boltInCart][2] = b;
        boltInCart++;

        if (boltInCart == 3) {
            // CEK: Apakah ketiga baut di kereta warnanya SAMA SEMUA?
            bool matchAll = isSameColor(cartColors[0], cartColors[1]) && 
                            isSameColor(cartColors[1], cartColors[2]);

            if (matchAll) {
                // HORE! 3 baut sama, kereta kosongkan, lanjut panen paku
                boltInCart = 0; 
                printf("SUCCESS: Triplet ditemukan! Kereta kosong lagi.\n");
            } 
            else {
                // Kereta penuh tapi warnanya campur-campur -> GAME OVER
                // Biar solvable, pemain harus mikir urutan nyabut bautnya
                isGameOver = true; 
                printf("GAME OVER: Kereta penuh paku beda warna!\n");
            }
        }
    }
}

bool isBlocked(int pakuIdx) {
    // Cek semua papan yang index-nya lebih tinggi (lapisan luar)
    for (int j = pakuIdx + 1; j < 6; j++) {
        // Kalau papan di depannya belum lepas (isSolved == false)
        if (!puzzle[j].isSolved) {
            // Kita pakai logika simple: kalau paku berada di area papan j
            // Ini perkiraan area tumpukan (Informatics bounding box check)
            float px = puzzle[pakuIdx].x;
            float py = puzzle[pakuIdx].y;
            
            // Cek apakah koordinat paku pakuIdx berada di dalam kotak papan j
            if (px > (puzzle[j].x - puzzle[j].w/2) && px < (puzzle[j].x + puzzle[j].w/2) &&
                py > (puzzle[j].y - puzzle[j].h/2) && py < (puzzle[j].y + puzzle[j].h/2)) {
                return true; // Ketutupan cik!
            }
        }
    }
    return false; // Aman, bisa dicabut
}

void mouse(int button, int state, int x, int y) {
    // 1. Pastikan klik hanya diproses kalau game belum selesai/over
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !isGameOver && !isGameFinished) {
        unsigned char pixel[3];
        int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

        // Render "rahasia" untuk deteksi picking warna ID
        renderForPicking();

        int flipY = windowHeight - y;
        glReadPixels(x, flipY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

        int pickedID = pixel[0]; 
        
        if (pickedID > 0 && pickedID <= 12) {
            int pakuIdx = (pickedID - 1) / 2; 
            
            // --- VALIDASI TUMPUKAN (Layering Logic) ---
            if (isBlocked(pakuIdx)) {
                // Beri peringatan jika mencoba mencabut paku yang tertutup
                sprintf(alertMsg, "tidak bisa, paku yg atas dikerjain dulu");
                alertTimer = 90; 
                printf("Aksi diblokir: Papan masih tertutup tumpukan luar.\n");
            } 
            else {
                // --- PROSES CABUT PAKU ---
                bool isKanan = (pickedID % 2 == 0);

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

                // --- CEK STATUS PAPAN ---
                if (puzzle[pakuIdx].bautKiriKlik && puzzle[pakuIdx].bautKananKlik) {
                    puzzle[pakuIdx].isSolved = true;

                    // --- LOGIKA KEMENANGAN (FINAL CHECK) ---
                    // Cek apakah semua papan (0-5) sudah dilepaskan
                    bool allSolved = true;
                    for (int j = 0; j < 6; j++) {
                        if (!puzzle[j].isSolved) {
                            allSolved = false;
                            break;
                        }
                    }

                    if (allSolved) {
                        isGameFinished = true; // Trigger mode ending
                        printf("SEMUA PAPAN LEPAS! Kereta meluncur!\n");
                    }
                }
            }
        }
        
        glutPostRedisplay();
    }
}

void drawIndicators() {
    float pos[3][3] = {{-0.4f, 0.2f, -1.0f}, {0.0f, 0.2f, -1.0f}, {0.4f, 0.2f, -1.0f}};
    
    for(int i = 0; i < 3; i++) {
        glPushMatrix();
            glTranslatef(pos[i][0], pos[i][1], pos[i][2]);
            
            // Jika baut sudah masuk, gunakan warnanya. Jika belum, biarkan hitam/redup.
            if (i < boltInCart) {
                glColor3f(cartColors[i][0], cartColors[i][1], cartColors[i][2]);
                // Tambahkan pendaran (emission) agar nyala
                GLfloat em[] = {cartColors[i][0], cartColors[i][1], cartColors[i][2], 1.0f};
                glMaterialfv(GL_FRONT, GL_EMISSION, em);
            } else {
                glColor3f(0.1f, 0.1f, 0.1f);
                GLfloat no_em[] = {0,0,0,1};
                glMaterialfv(GL_FRONT, GL_EMISSION, no_em);
            }

            glutSolidSphere(0.12, 20, 20);
        glPopMatrix();
    }
}

void renderTextJumbo(float x, float y, float scale, float thickness, const char* text) {
    glDisable(GL_LIGHTING);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    // 1. Posisikan teks
    glTranslatef(x, y, 0);
    
    // 2. Atur ukuran (Scale) dan ketebalan garis
    glScalef(scale, scale, scale); 
    glLineWidth(thickness); 
    
    // 3. Gambar per karakter menggunakan Stroke
    for (const char* c = text; *c != '\0'; c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }
    
    glLineWidth(1.0f); // Reset ketebalan garis ke normal
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    glEnable(GL_LIGHTING);
}

// ==========================================
// RENDER UTAMA
// ==========================================
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // ==========================================
    // KAMERA LURUS (POV Penumpang)
    // ==========================================
    gluLookAt(0.0f, 0.0f, 10.0f,   
              0.0f, 0.0f,  0.0f,   
              0.0f,  1.0f,  0.0f);

    // ==========================================
    // 1. DUNIA YANG BERGERAK (Gua + Rel + Tembok)
    // ==========================================
    if (isGameOver) {
        glClearColor(0.5f, 0.0f, 0.0f, 1.0f); // Merah kalau Game Over
    } else if (isGameFinished) {
        glClearColor(0.0f, 0.2f, 0.0f, 1.0f); // Hijau gelap kalau Menang (Opsional)
    } else {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }

    glPushMatrix();
        glTranslatef(0.0f, -1.5f, worldZ); 
        drawCave();
        
        // --- LOGIKA TEMBOK HANCUR ---
        // Tembok (dan paku) hanya digambar jika game BELUM selesai
        if (!isGameFinished) {
            drawEndWall();
        }
        
        drawRel();
    glPopMatrix();

    // ==========================================
    // 2. KERETA LORI
    // ==========================================
    glPushMatrix();
        glTranslatef(0.0f, -4.0f, -2.0f);
        glPushMatrix();
            glTranslatef(0.0f, 0.8f, 0.0f);
            glRotatef(180.0f, 0.0f, 1.0f, 0.0f); 
            glScalef(1.7f, 1.7f, 1.7f); 
            
            drawCartBody();
            drawWheels();
            drawIndicators();
        glPopMatrix();
    glPopMatrix();
    
    // ==========================================
    // 3. UI & TEXT OVERLAY (Render Paling Depan)
    // ==========================================

    // --- RENDER PESAN PERINGATAN (Dependency Check) ---
    if (alertTimer > 0) {
        glColor3f(1.0f, 1.0f, 0.0f); // Kuning
        renderTextJumbo(200, 450, 0.15f, 2.0f, alertMsg);
    }
    
    // --- RENDER KEMENANGAN (Credits) ---
    if (isGameFinished) {
        glColor3f(1.0f, 1.0f, 1.0f); // Putih
        // Teks meluncur dari atas ke creditY yang sudah diupdate di timer
        renderTextJumbo(180, creditY, 0.2f, 3.0f, "Game selesai.");
        
        if (creditY <= 300.0f) {
            renderTextJumbo(250, 250, 0.12f, 1.5f, "Created by Kelompok 6");
            renderTextJumbo(300, 210, 0.1f, 1.2f, "Informatics UNDIP 2026");
        }
    }

    // --- RENDER GAME OVER ---
    if (isGameOver) {
        glClearColor(0.5f, 0.0f, 0.0f, 1.0f); 
        glColor3f(1.0f, 1.0f, 1.0f); // Teks putih biar kebaca di latar merah
        renderTextJumbo(180, 300, 0.4f, 4.0f, "GAME OVER");
        renderTextJumbo(230, 250, 0.15f, 2.0f, "YOU FAILED TO MATCH THE BOLTS");
        renderTextJumbo(250, 200, 0.12f, 1.5f, "MULAI ULANG");
    }

    glutSwapBuffers();
}

// ==========================================
// KONTROL & ANIMASI
// ==========================================
void timer(int value) {
    // 1. LOGIKA ANIMASI ENDING (Otomatis)
    if (isGameFinished) {
        // Kereta meluncur otomatis ke depan (Z bertambah)
        worldZ += 0.8f; 
        
        // Roda berputar otomatis karena kereta meluncur
        wheelRot -= 7.0f; 

        // Teks credit turun perlahan dari atas (700) ke tengah (300)
        if (creditY > 300.0f) {
            creditY -= 3.0f;
        }
    } 
    // 2. LOGIKA PERGERAKAN MANUAL (Jika Belum Selesai)
    else if (deltaMove != 0) {
        worldZ += deltaMove * moveSpeed;
        
        // Batas pergerakan agar tidak keluar map saat main
        if (worldZ < -35.0f) worldZ = -35.0f; // Mentok luar gua
        if (worldZ > 19.0f) worldZ = 19.0f;   // Mentok tembok ujung
        
        // Animasi roda saat ditekan tombol maju/mundur
        if (deltaMove == 1) { 
            wheelRot -= 5.0f; // Maju
        } else if (deltaMove == -1) {
            wheelRot += 5.0f; // Mundur
        }
    }

    // Normalisasi rotasi roda agar tidak overflow
    if(wheelRot > 360) wheelRot -= 360;
    if(wheelRot < -360) wheelRot += 360;

    // Kurangi timer pesan "gabisa gini cik" secara bertahap
    if (alertTimer > 0) alertTimer--; 
    
    // Render ulang dan panggil timer lagi (60 FPS)
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void pressKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP : deltaMove = 1; break;   // Maju
        case GLUT_KEY_DOWN : deltaMove = -1; break; // Mundur
    }
}

void releaseKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP : 
        case GLUT_KEY_DOWN : deltaMove = 0; break;
    }
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w/h, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); 
    glEnable(GL_DEPTH_TEST);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL); // Biar glColor tetap narik warna dasar

    // --- CAHAYA LEBIH REALISTIK ---
    GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // Cahaya dasar gelap
    GLfloat diffuseLight[] = { 0.8f, 0.7f, 0.5f, 1.0f }; // Cahaya obor agak kekuningan
    GLfloat light_pos[] = { 0.0f, 5.0f, 5.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    // Aktifkan fitur kilap material
    glShadeModel(GL_SMOOTH);
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
    acakPuzzle();
    glutMainLoop();
    return 0;
}
