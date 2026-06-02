#ifndef RENDER_H
#define RENDER_H

void gambarTeksUI(float x, float y, const char* teks);
void drawObor(float x, float y, float z, int lightID);
void drawCave();
void PapanKayu(float x, float y, float z, float alpha);
void Baut(float r, float g, float b, float alpha);
void GambarBilahPuzzle(float w, float h, float tebal, float rL, float gL, float bL, float rR, float gR, float bR, bool kiriLps, bool kananLps, float alpha);
void drawEndWall();
void drawRel();
void drawCartBody();
void drawAxle();
void drawWheel();
void drawWheels();
void drawIndicators();
void drawShadow(float keretaX, float keretaZ, float oborX, float oborZ);
void renderForPicking();
void drawEnvironmentDetails();

#endif
