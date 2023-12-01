#ifndef Game_h
#define Game_h
#define GL_SILENCE_DEPRECATION
#include <GL/glut.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <unordered_set>
#include <random>

struct BlueSquare {
    float x;
    float y;
    bool isVisible;
    bool isPurple;  // New field to track if the square is purple
};

struct Draw {
    void initializeBlueSquares();
    void drawPlayer();
    void drawObstacle(float x, float y);
    void drawBlueSquare(float x, float y, bool isPurple);
    void drawText(const std::string& text, float x, float y);
    void drawHearts();
};

void display();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void timer(int value);

#endif