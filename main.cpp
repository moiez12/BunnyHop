#include "Game.h"

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Bunny Hop");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // Set clear color to white

    Draw draw;

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, timer, 0);  // Start the timer

    // Initialize blue squares
    draw.initializeBlueSquares();

    glutMainLoop();
    return 0;
}