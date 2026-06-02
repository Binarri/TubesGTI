#include "ui.h"
#include <GL/glut.h>

void renderTextJumbo(float x, float y, float scale, float thickness, const char* text) {
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    
    glTranslatef(x, y, 0);
    glScalef(scale, scale, scale); 
    glLineWidth(thickness); 
   
    for (const char* c = text; *c != '\0'; c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }
    
    glLineWidth(1.0f); glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW); glEnable(GL_LIGHTING);
}

float getTextWidth(float scale, const char* text) {
    float w = 0;
    for (const char* c = text; *c != '\0'; c++)
        w += glutStrokeWidth(GLUT_STROKE_ROMAN, *c);
    return w * scale;
}

void renderTextCentered(float y, float scale, float thickness, const char* text) {
    float w = getTextWidth(scale, text);
    float x = (800.0f - w) / 2.0f;
    renderTextJumbo(x, y, scale, thickness, text);
}

void drawPanel(float x1, float y1, float x2, float y2, 
               float br, float bg, float bb, float ba,
               float lr, float lg, float lb, float lineW) {
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(br, bg, bb, ba);
    glBegin(GL_QUADS);
        glVertex2f(x1,y1); glVertex2f(x2,y1); glVertex2f(x2,y2); glVertex2f(x1,y2);
    glEnd();
    glDisable(GL_BLEND);
    glColor3f(lr, lg, lb); glLineWidth(lineW);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x1,y1); glVertex2f(x2,y1); glVertex2f(x2,y2); glVertex2f(x1,y2);
    glEnd();
    glLineWidth(1.0f);
}
