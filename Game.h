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
#include <chrono>
using namespace std;

// Enum to represent bunny color options
enum BunnyColor {
   BLACK = 1,
   BROWN = 2,
   WHITE = 3
};

struct Player {
   float x;
   float y;
   float speed;
   int score;
   int lives;
   bool lookingLeft;
   BunnyColor selectedBunnyColor = BLACK; // Default bunny color
};

struct Car {
   float x;
   float y;
   float initialCarSpeed;
   float CarSpeed;
};

struct Carrot {
   float x;
   float y;
   bool isVisible;
   bool isGolden;
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