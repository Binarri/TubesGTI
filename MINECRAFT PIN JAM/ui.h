#ifndef UI_H
#define UI_H

void renderTextJumbo(float x, float y, float scale, float thickness, const char* text);
float getTextWidth(float scale, const char* text);
void renderTextCentered(float y, float scale, float thickness, const char* text);
void drawPanel(float x1, float y1, float x2, float y2, float br, float bg, float bb, float ba, float lr, float lg, float lb, float lineW);

#endif
