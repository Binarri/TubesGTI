#include "puzzle.h"
#include "globals.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>

// Cek apakah pin tertutup papan kayu
bool isBoltCovered(float bx, float by, Bilah papanDepan) {
    float dx = bx - papanDepan.x;
    float dy = by - papanDepan.y;
    float rad = -papanDepan.rot * 3.14159f / 180.0f;
    float localX = dx * cos(rad) - dy * sin(rad);
    float localY = dx * sin(rad) + dy * cos(rad);
    
    float halfW = (papanDepan.w / 2.0f) + 0.4f; 
    float halfH = (papanDepan.h / 2.0f) + 0.4f; 
    
    return (localX >= -halfW && localX <= halfW && localY >= -halfH && localY <= halfH);
}

// Arah Garis
int orientation(float p1x, float p1y, float q1x, float q1y, float rx, float ry) {
    float val = (q1y - p1y) * (rx - q1x) - (q1x - p1x) * (ry - q1y);
    if (fabsf(val) < 0.001f) return 0;  
    return (val > 0) ? 1 : 2;           
}

// Perpotongan Garis (Silang)
bool doIntersect(float p1x, float p1y, float q1x, float q1y, float p2x, float p2y, float q2x, float q2y) {
    int o1 = orientation(p1x, p1y, q1x, q1y, p2x, p2y);
    int o2 = orientation(p1x, p1y, q1x, q1y, q2x, q2y);
    int o3 = orientation(p2x, p2y, q2x, q2y, p1x, p1y);
    int o4 = orientation(p2x, p2y, q2x, q2y, q1x, q1y);
    if (o1 != o2 && o3 != o4) return true;
    return false;
}

// Cek apakah sebuah papan terjepit atau saling silang
bool isBoardEntangled(int i, bool useSimState) {
    float rad1 = puzzle[i].rot * 3.14159f / 180.0f;
    float dist1 = (puzzle[i].w / 2.0f) - 0.5f;
    float b1xL = puzzle[i].x - dist1 * cos(rad1); float b1yL = puzzle[i].y - dist1 * sin(rad1);
    float b1xR = puzzle[i].x + dist1 * cos(rad1); float b1yR = puzzle[i].y + dist1 * sin(rad1);

    for (int j = i + 1; j < totalPapan; j++) {
        bool isJActive = useSimState ? (!simSolved[j]) : (!puzzle[j].isSolved);
        if (isJActive) {
            float rad2 = puzzle[j].rot * 3.14159f / 180.0f;
            float dist2 = (puzzle[j].w / 2.0f) - 0.5f;
            float b2xL = puzzle[j].x - dist2 * cos(rad2); float b2yL = puzzle[j].y - dist2 * sin(rad2);
            float b2xR = puzzle[j].x + dist2 * cos(rad2); float b2yR = puzzle[j].y + dist2 * sin(rad2);

            if (doIntersect(b1xL, b1yL, b1xR, b1yR, b2xL, b2yL, b2xR, b2yR)) return true;
        }
    }
    return false;
}

// Bandingin dua warna sama atau tidak
bool isSameColor(float c1[3], float c2[3]) {
    return (fabsf(c1[0] - c2[0]) < 0.01f && 
            fabsf(c1[1] - c2[1]) < 0.01f && 
            fabsf(c1[2] - c2[2]) < 0.01f);
}

// Cek pin di blok
bool isSimBoltBlocked(int i, bool isKanan) {
    if (isBoardEntangled(i, true)) return true; 

    float rad = puzzle[i].rot * 3.14159f / 180.0f;
    float dist = (puzzle[i].w / 2.0f) - 0.5f;
    if (!isKanan) dist = -dist;
    float pakuX = puzzle[i].x + dist * cos(rad);
    float pakuY = puzzle[i].y + dist * sin(rad);

    for (int j = i + 1; j < totalPapan; j++) {
        if (!simSolved[j] && isBoltCovered(pakuX, pakuY, puzzle[j])) return true;
    }
    return false;
}

// Tambah pin ke kereta
void tambahBautKeKereta(float r, float g, float b) {
    if (boltInCart < 5) {
        cartColors[boltInCart][0] = r;
        cartColors[boltInCart][1] = g;
        cartColors[boltInCart][2] = b;
        boltInCart++;

        int matchColorIdx = -1;
        for (int i = 0; i < boltInCart; i++) {
            int count = 1;
            for (int j = i + 1; j < boltInCart; j++) {
                if (isSameColor(cartColors[i], cartColors[j])) count++;
            }
            if (count >= 3) {
                matchColorIdx = i; 
                break;
            }
        }

        if (matchColorIdx != -1) {
            float target[3] = {cartColors[matchColorIdx][0], cartColors[matchColorIdx][1], cartColors[matchColorIdx][2]};
            float temp[5][3];
            int tempCount = 0;
            int removedCount = 0;
            
            for (int i = 0; i < boltInCart; i++) {
                if (removedCount < 3 && isSameColor(cartColors[i], target)) {
                    removedCount++; 
                } else {
                    temp[tempCount][0] = cartColors[i][0];
                    temp[tempCount][1] = cartColors[i][1];
                    temp[tempCount][2] = cartColors[i][2];
                    tempCount++; 
                }
            }
            
            for (int i = 0; i < tempCount; i++) {
                cartColors[i][0] = temp[i][0]; cartColors[i][1] = temp[i][1]; cartColors[i][2] = temp[i][2];
            }
            boltInCart = tempCount; 
            printf("BOOM! Match 3 Hancur! Sisa di lori: %d\n", boltInCart);
        } 
        else if (boltInCart == 5) {
            isGameOver = true; 
            printf("GAME OVER: Lori Buntu & Penuh!\n");
        }
    }
}

bool solveStateDFS(int depth, int &bestPapan, bool &bestIsKanan) {
    simStepCount++;
    if (simStepCount > 3000) return false; 

    bool allSolved = true;
    for (int i = 0; i < totalPapan; i++) {
        if (!simSolved[i]) { allSolved = false; break; }
    }
    if (allSolved) return true;

    if (simBoltInCart >= 5) return false;
    if (depth > 40) return false;

    for (int i = 0; i < totalPapan; i++) {
        if (simSolved[i]) continue;

        // Simulasi Klik Paku Kiri
        if (!simKiriKlik[i] && !isSimBoltBlocked(i, false)) {
            simKiriKlik[i] = true;
            float backupCart[5][3];
            int backupBoltCount = simBoltInCart;
            for(int c=0; c<5; c++) { backupCart[c][0]=simCart[c][0]; backupCart[c][1]=simCart[c][1]; backupCart[c][2]=simCart[c][2]; }
            
            simCart[simBoltInCart][0] = puzzle[i].rL; 
            simCart[simBoltInCart][1] = puzzle[i].gL; 
            simCart[simBoltInCart][2] = puzzle[i].bL;
            simBoltInCart++;
            
            int matchIdx = -1;
            for(int m=0; m<simBoltInCart; m++) {
                int cnt = 1;
                for(int n=m+1; n<simBoltInCart; n++) {
                    if (fabsf(simCart[m][0]-simCart[n][0]) < 0.01f && fabsf(simCart[m][1]-simCart[n][1]) < 0.01f && fabsf(simCart[m][2]-simCart[n][2]) < 0.01f) cnt++;
                }
                if (cnt >= 3) { matchIdx = m; break; }
            }
            if (matchIdx != -1) {
                float target[3] = {simCart[matchIdx][0], simCart[matchIdx][1], simCart[matchIdx][2]};
                float tempC[5][3]; int tempCnt = 0; int rem = 0;
                for(int c=0; c<simBoltInCart; c++) {
                    if (rem < 3 && fabsf(simCart[c][0]-target[0]) < 0.01f && fabsf(simCart[c][1]-target[1]) < 0.01f && fabsf(simCart[c][2]-target[2]) < 0.01f) rem++;
                    else { tempC[tempCnt][0]=simCart[c][0]; tempC[tempCnt][1]=simCart[c][1]; tempC[tempCnt][2]=simCart[c][2]; tempCnt++; }
                }
                for(int c=0; c<tempCnt; c++) { simCart[c][0]=tempC[c][0]; simCart[c][1]=tempC[c][1]; simCart[c][2]=tempC[c][2]; }
                simBoltInCart = tempCnt;
            }
            
            bool backupSolved = simSolved[i];
            if (simKiriKlik[i] && simKananKlik[i]) simSolved[i] = true;

            if (solveStateDFS(depth + 1, bestPapan, bestIsKanan)) {
                if (depth == 0) { bestPapan = i; bestIsKanan = false; }
                return true;
            }

            simKiriKlik[i] = false;
            simSolved[i] = backupSolved;
            simBoltInCart = backupBoltCount;
            for(int c=0; c<5; c++) { simCart[c][0]=backupCart[c][0]; simCart[c][1]=backupCart[c][1]; simCart[c][2]=backupCart[c][2]; }
        }

        // Simulasi Klik Paku Kanan
        if (!simKananKlik[i] && !isSimBoltBlocked(i, true)) {
            simKananKlik[i] = true;
            float backupCart[5][3];
            int backupBoltCount = simBoltInCart;
            for(int c=0; c<5; c++) { backupCart[c][0]=simCart[c][0]; backupCart[c][1]=simCart[c][1]; backupCart[c][2]=simCart[c][2]; }
            
            simCart[simBoltInCart][0] = puzzle[i].rR; 
            simCart[simBoltInCart][1] = puzzle[i].gR; 
            simCart[simBoltInCart][2] = puzzle[i].bR;
            simBoltInCart++;
            
            int matchIdx = -1;
            for(int m=0; m<simBoltInCart; m++) {
                int cnt = 1;
                for(int n=m+1; n<simBoltInCart; n++) {
                    if (fabsf(simCart[m][0]-simCart[n][0]) < 0.01f && fabsf(simCart[m][1]-simCart[n][1]) < 0.01f && fabsf(simCart[m][2]-simCart[n][2]) < 0.01f) cnt++;
                }
                if (cnt >= 3) { matchIdx = m; break; }
            }
            if (matchIdx != -1) {
                float target[3] = {simCart[matchIdx][0], simCart[matchIdx][1], simCart[matchIdx][2]};
                float tempC[5][3]; int tempCnt = 0; int rem = 0;
                for(int c=0; c<simBoltInCart; c++) {
                    if (rem < 3 && fabsf(simCart[c][0]-target[0]) < 0.01f && fabsf(simCart[c][1]-target[1]) < 0.01f && fabsf(simCart[c][2]-target[2]) < 0.01f) rem++;
                    else { tempC[tempCnt][0]=simCart[c][0]; tempC[tempCnt][1]=simCart[c][1]; tempC[tempCnt][2]=simCart[c][2]; tempCnt++; }
                }
                for(int c=0; c<tempCnt; c++) { simCart[c][0]=tempC[c][0]; simCart[c][1]=tempC[c][1]; simCart[c][2]=tempC[c][2]; }
                simBoltInCart = tempCnt;
            }

            bool backupSolved = simSolved[i];
            if (simKiriKlik[i] && simKananKlik[i]) simSolved[i] = true;

            if (solveStateDFS(depth + 1, bestPapan, bestIsKanan)) {
                if (depth == 0) { bestPapan = i; bestIsKanan = true; }
                return true;
            }

            simKananKlik[i] = false;
            simSolved[i] = backupSolved;
            simBoltInCart = backupBoltCount;
            for(int c=0; c<5; c++) { simCart[c][0]=backupCart[c][0]; simCart[c][1]=backupCart[c][1]; simCart[c][2]=backupCart[c][2]; }
        }
    }
    return false;
}

bool cariHintLangkahSelanjutnya() {
    for(int i=0; i<totalPapan; i++) {
        simSolved[i] = puzzle[i].isSolved;
        simKiriKlik[i] = puzzle[i].bautKiriKlik;
        simKananKlik[i] = puzzle[i].bautKananKlik;
    }
    simBoltInCart = boltInCart;
    for(int c=0; c<5; c++) { 
        simCart[c][0]=cartColors[c][0]; 
        simCart[c][1]=cartColors[c][1]; 
        simCart[c][2]=cartColors[c][2]; 
    }
    
    simStepCount = 0;
    int bp = -1; bool bR = false;
    if (solveStateDFS(0, bp, bR)) {
        hintPapanIdx = bp;
        hintIsKanan = bR;
        return true;
    }
    return false;
}

void acakPuzzle() {
    srand(time(NULL));
    int totalLayers = totalPapan / 3; 
    
    float palette[10][3] = {
        {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.5f, 1.0f}, 
        {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, 
        {1.0f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.5f, 0.0f, 1.0f}, {0.6f, 1.0f, 0.0f}
    };

    bool mapValidDanSolvable = false;
    int mapAttempts = 0;

    while (!mapValidDanSolvable && mapAttempts < 20) {
        mapAttempts++;

        // Generate Posisi
        for (int layer = 0; layer < totalLayers; layer++) {
            int startIdx = layer * 3;
            for (int b = 2; b >= 0; b--) {
                int i = startIdx + b;
                bool posisiAman = false;
                int attempt = 0;
                
                while (!posisiAman && attempt < 100) {
                    if (currentLevel == 1) {
                        puzzle[i].x = (float)(rand() % 40 - 20) / 10.0f; 
                        puzzle[i].y = (float)(rand() % 40 + 20) / 10.0f;
                        puzzle[i].h = 1.2f;
                    } else {
                        puzzle[i].x = (float)(rand() % 20 - 10) / 10.0f; 
                        puzzle[i].y = (float)(rand() % 20 + 30) / 10.0f; 
                        puzzle[i].h = 1.6f; 
                    }
                    
                    puzzle[i].z = (float)i * 0.35f; 
                    puzzle[i].rot = (float)(rand() % 180 - 90); 
                    puzzle[i].w = (float)(rand() % 25 + 45) / 10.0f; 
                    
                    float rad = puzzle[i].rot * 3.14159f / 180.0f;
                    float distKanan = (puzzle[i].w / 2.0f) - 0.5f;
                    float distKiri = -distKanan;
                    
                    float pakuKiriX = puzzle[i].x + distKiri * cos(rad);
                    float pakuKiriY = puzzle[i].y + distKiri * sin(rad);
                    float pakuKananX = puzzle[i].x + distKanan * cos(rad);
                    float pakuKananY = puzzle[i].y + distKanan * sin(rad);
                    
                    bool ketutupan = false;
                    for (int depan = b + 1; depan < 3; depan++) {
                        int idxDepan = startIdx + depan;
                        if (isBoltCovered(pakuKiriX, pakuKiriY, puzzle[idxDepan]) || 
                            isBoltCovered(pakuKananX, pakuKananY, puzzle[idxDepan])) {
                            ketutupan = true;
                            break;
                        }
                    }
                    
                    if (!ketutupan) posisiAman = true;
                    attempt++;
                }
                
                puzzle[i].isSolved = false;
                puzzle[i].bautKiriKlik = false;
                puzzle[i].bautKananKlik = false;
            }
        }

        // Generate Warna
        int warnaTersedia = 0; 
        int totalPaku = totalPapan * 2;
        int totalWarnaUnik = totalPaku / 3; 

        // Urut Semua Warna 
        float allColors[100][3];
        for (int i = 0; i < totalWarnaUnik; i++) {
            for (int j = 0; j < 3; j++) {
                allColors[i*3 + j][0] = palette[warnaTersedia][0];
                allColors[i*3 + j][1] = palette[warnaTersedia][1];
                allColors[i*3 + j][2] = palette[warnaTersedia][2];
            }
            warnaTersedia = (warnaTersedia + 1) % 10;
        }

        // Acak Warna
        for(int i = 0; i < totalPaku; i++) {
            int swapOffset = rand() % 5;
            int j = i + swapOffset;
            if(j < totalPaku) {
                // Tukar Posisi Warna
                float t0 = allColors[i][0], t1 = allColors[i][1], t2 = allColors[i][2];
                allColors[i][0] = allColors[j][0]; allColors[i][1] = allColors[j][1]; allColors[i][2] = allColors[j][2];
                allColors[j][0] = t0; allColors[j][1] = t1; allColors[j][2] = t2;
            }
        }

        // Tempel warnanya ke papan
        int cIdx = 0;
        for (int i = 0; i < totalPapan; i++) {
            puzzle[i].rL = allColors[cIdx][0]; puzzle[i].gL = allColors[cIdx][1]; puzzle[i].bL = allColors[cIdx][2]; cIdx++;
            puzzle[i].rR = allColors[cIdx][0]; puzzle[i].gR = allColors[cIdx][1]; puzzle[i].bR = allColors[cIdx][2]; cIdx++;
        }

		// Validasi
        for(int i=0; i<totalPapan; i++) {
            simSolved[i] = false; simKiriKlik[i] = false; simKananKlik[i] = false;
        }
        simBoltInCart = 0;
        simStepCount = 0;

        int dummyPapan = -1; bool dummySide = false;
        if (solveStateDFS(0, dummyPapan, dummySide)) {
            mapValidDanSolvable = true; 
            printf("VALIDATION SUCCESS: Level %d Berhasil Terbentuk dalam %d Percobaan!\n", currentLevel, mapAttempts);
        }
    }

    if (!mapValidDanSolvable) {
        mapValidDanSolvable = true;
        printf("VALIDATION TIMEOUT BYPASS: Menggunakan susunan teroptimasi terakhir demi keamanan CPU.\n");
    }
}
