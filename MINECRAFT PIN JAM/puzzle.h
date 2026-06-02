#ifndef PUZZLE_H
#define PUZZLE_H

#include "globals.h"
#include <cmath>

// === UTILS ===
bool isBoltCovered(float bx, float by, Bilah papanDepan);
bool isSameColor(float c1[3], float c2[3]);
bool isSimBoltBlocked(int i, bool isKanan);

int orientation(float p1x, float p1y, float q1x, float q1y, float rx, float ry);
bool doIntersect(float p1x, float p1y, float q1x, float q1y, float p2x, float p2y, float q2x, float q2y);
bool isBoardEntangled(int i, bool useSimState);

// === AI SOLVER ===
bool solveStateDFS(int depth, int &bestPapan, bool &bestIsKanan);
bool cariHintLangkahSelanjutnya();

// === PUZZLE GENERATOR ===
void acakPuzzle();

// === BOLT LOGIC ===
void tambahBautKeKereta(float r, float g, float b);

#endif
