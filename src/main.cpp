#include "lib/Game.h"

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Bunny Hop");

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // Set clear color to black

    initiliazeTime();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, timer, 0);  // Start the timer

    // Initialize Player
    initializePlayer();

    // Initialize Cars
    initializeCars();

    // Initialize Logs
    initializeLogs();

    // Initialize Carrots
    initializeCarrots();

    // Initialize Textures
    initializeTextures();

    glutMainLoop();
    return 0;
}