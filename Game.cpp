#define STB_IMAGE_IMPLEMENTATION
#include "Game.h"
#include "stb_image.h"


// Set player positions and speed
Player player;


void initializePlayer() {
   player.x = 275.0f;
   player.y = 25.0f;
   player.speed = 50.0f;
   player.score = 0;
   player.lives = 3;
   player.lookingLeft = true;
}

mt19937 rng(std::random_device{}());
uniform_real_distribution<float> dist(0.0f, 500.0f);


// Set initial Car x,y positions and speed
const int numCars = 3;
Car cars[numCars];


void initializeCars() {
   for (int i = 0; i < numCars; ++i) {
       cars[i].x = dist(rng);
       if (i == 0) {
           cars[i].y = 75.0f;
           cars[i].initialCarSpeed = 3.0f;
       } else if (i == 1) {
           cars[i].y = 125.0f;
           cars[i].initialCarSpeed = 5.0f;
       } else if (i == 2) {
           cars[i].y = 175.0f;
           cars[i].initialCarSpeed = 4.0f;
       }
       cars[i].CarSpeed = cars[i].initialCarSpeed;
   }
}

// Set intial Carrot x,y positions
const int numCarrots = 3;
Carrot Carrots[numCarrots];


std::unordered_set<float> occupiedXLevels;
std::unordered_set<float> occupiedYLevels;


void initializeCarrots() {
   srand(static_cast<unsigned>(time(nullptr)));


   occupiedXLevels.clear();  // Reset the set of occupied X-levels
   occupiedYLevels.clear();  // Reset the set of occupied Y-levels


   for (int i = 0; i < numCarrots; ++i) {
       // Generate a random X-level that is not already occupied
       float newXLevel;
       do {
           newXLevel = static_cast<float>(rand() % 10 * 50 + 25);  // Random multiple of 50 plus 25
       } while (occupiedXLevels.count(newXLevel) > 0);


       // Generate a random Y-level that is not already occupied
       float newYLevel;
       do {
           newYLevel = static_cast<float>(rand() % 8 * 50 + 75);  // Random multiple of 50 plus 75
       } while (occupiedYLevels.count(newYLevel) > 0);


       Carrots[i].x = newXLevel;
       Carrots[i].y = newYLevel;
       Carrots[i].isVisible = true;


       // Randomly determine whether the carrot should be Golden
       std::random_device rd;
       std::mt19937 gen(rd());
       std::uniform_real_distribution<> dis(0.0, 1.0);
       bool isGolden = dis(gen) < 0.05;  // Adjust the probability as needed


       if (isGolden) {
           Carrots[i].isGolden = true;
       } else {
           Carrots[i].isGolden = false;
       }


       // Mark the X and Y-levels as occupied
       occupiedXLevels.insert(newXLevel);
       occupiedYLevels.insert(newYLevel);
   }
}

bool isMoving;


void drawPlayer() {
   glEnable(GL_TEXTURE_2D);
   if (player.selectedBunnyColor == BLACK) {
       playerTextureID = blackBunnyTextureID;
   } else if (player.selectedBunnyColor == BROWN) {
       playerTextureID = brownBunnyTextureID;
   } else {
       playerTextureID = whiteBunnyTextureID;
   }
  
   if (isMoving){
       glBindTexture(GL_TEXTURE_2D, playerFrames[currentFrame]);
       if (frameCount == frameDelay) {
           if(currentFrame < 3) {
               currentFrame++;
           } else {
               currentFrame = 0;
               isMoving = false;
           }
           frameCount = 0;
       } else {
           frameCount++;
       }
   } else {
       glBindTexture(GL_TEXTURE_2D, playerTextureID);
   }
  
   glColor4f(1.0f, 1.0f, 1.0f, 1.0f);


   glPushMatrix(); // Save the current transformation matrix


   // Move to the player's position
   glTranslatef(player.x, player.y, 0.0f);
   if (!player.lookingLeft) { // Moving right
       glScalef(-1.0f, 1.0f, 1.0f); // Flip the texture horizontally
   }


   glBegin(GL_QUADS);
   glTexCoord2f(0.0f, 1.0f); glVertex2f(-25, -25);
   glTexCoord2f(1.0f, 1.0f); glVertex2f(25, -25);
   glTexCoord2f(1.0f, 0.0f); glVertex2f(25, 25);
   glTexCoord2f(0.0f, 0.0f); glVertex2f(-25, 25);
   glEnd();
  
   glPopMatrix(); // Restore the original transformation matrix
  
   glDisable(GL_TEXTURE_2D);
}


void drawCar(float x, float y, int color) {
   glEnable(GL_TEXTURE_2D);
   if (color == 2) {
       glBindTexture(GL_TEXTURE_2D, bluecarTextureID);
   } else if (color == 1) {
       glBindTexture(GL_TEXTURE_2D, redcarTextureID);
   } else {
       glBindTexture(GL_TEXTURE_2D, greencarTextureID);
   }
   glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
   glBegin(GL_QUADS);
   glTexCoord2f(0.0f, 1.0f); glVertex2f(x - 25, y - 25);
   glTexCoord2f(1.0f, 1.0f); glVertex2f(x + 25, y - 25);
   glTexCoord2f(1.0f, 0.0f); glVertex2f(x + 25, y + 25);
   glTexCoord2f(0.0f, 0.0f); glVertex2f(x - 25, y + 25);
   glEnd();
   glDisable(GL_TEXTURE_2D);
}

void drawCarrot(float x, float y, bool isGolden) {
  
   glEnable(GL_TEXTURE_2D);
  
   if (isGolden) {
       glBindTexture(GL_TEXTURE_2D, goldcarrotTextureID);
   } else {
       glBindTexture(GL_TEXTURE_2D, carrotTextureID);
   }
   glColor4f(1.0f, 1.0f, 1.0f, 1.0f);


   glBegin(GL_QUADS);
   glTexCoord2f(0.0f, 1.0f); glVertex2f(x - 12.5, y - 12.5);
   glTexCoord2f(1.0f, 1.0f); glVertex2f(x + 12.5, y - 12.5);
   glTexCoord2f(1.0f, 0.0f); glVertex2f(x + 12.5, y + 12.5);
   glTexCoord2f(0.0f, 0.0f); glVertex2f(x - 12.5, y + 12.5);
   glEnd();
   glDisable(GL_TEXTURE_2D);
}

Draw draw;

void Draw::drawText(const std::string& text, float x, float y) {
    glColor3f(0.0f, 0.0f, 0.0f);  // Set color to black
    glRasterPos2f(x, y);

    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void Draw::drawHearts() {
    glColor3f(1.0f, 0.0f, 0.0f);  // Set color to red for hearts
    for (int i = 0; i < lives; ++i) {
        glBegin(GL_TRIANGLES);
        glVertex2f(30.0f + i * 30, 470.0f);
        glVertex2f(40.0f + i * 30, 480.0f);
        glVertex2f(50.0f + i * 30, 470.0f);
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (!gameOver) {
        draw.drawPlayer();
        draw.drawObstacle(obstacle1X, obstacle1Y);
        draw.drawObstacle(obstacle2X, obstacle2Y);
        draw.drawObstacle(obstacle3X, obstacle3Y);
        draw.drawObstacle(obstacle4X, obstacle4Y);

        // Draw and update blue squares
        for (int i = 0; i < numBlueSquares; ++i) {
            if (blueSquares[i].isVisible) {
                draw.drawBlueSquare(blueSquares[i].x, blueSquares[i].y, blueSquares[i].isPurple);
            }
        }

        obstacle1X += obstacleSpeed;
        obstacle2X -= obstacleSpeed;
        obstacle3X += obstacleSpeed;
        obstacle4X -= obstacleSpeed;

        // Check if obstacles went off-screen
        if (obstacle1X > 500 + 25) {
            obstacle1X = -25;  // Reset obstacle1 to the left side
        }

        if (obstacle2X < -25) {
            obstacle2X = 500 + 25;  // Reset obstacle2 to the right side
        }

        if (obstacle3X > 500 + 25) {
            obstacle3X = -25;  // Reset obstacle3 to the left side
        }

        if (obstacle4X < -25) {
            obstacle4X = 500 + 25;  // Reset obstacle4 to the right side
        }

        // Check for collision with obstacles
        for (int i = 0; i < numBlueSquares; ++i) {
            if (blueSquares[i].isVisible &&
                playerX - 25 < blueSquares[i].x + 12.5 &&
                playerX + 25 > blueSquares[i].x - 12.5 &&
                playerY - 25 < blueSquares[i].y + 12.5 &&
                playerY + 25 > blueSquares[i].y - 12.5) {
                if (blueSquares[i].isPurple) {
                    // Player touched a blue square
                    std::cout << "Touched a purple square! +250 Score\n";
                    score += 250;
                } else {
                    // Player touched a blue square
                    std::cout << "Touched a blue square! +20 Score\n";
                    score += 20;
                }
                blueSquares[i].isVisible = false;
            }
        }

        // Check for collision with obstacles
        if (playerX - 25 < obstacle1X + 25 &&
            playerX + 25 > obstacle1X - 25 &&
            playerY - 25 < obstacle1Y + 25 &&
            playerY + 25 > obstacle1Y - 25) {
            std::cout << "Hit an obstacle! -1 Life\n";
            lives--;

            // Reset player position if there are remaining lives
            if (lives > 0) {
                playerY = 25.0f;
                playerX = 275.0f;
            } else {
                std::cout << "Game Over! Press Space to Play Again.\n";
                gameOver = true;
            }
        }

        // Check for collision with obstacle2
        if (playerX - 25 < obstacle2X + 25 &&
            playerX + 25 > obstacle2X - 25 &&
            playerY - 25 < obstacle2Y + 25 &&
            playerY + 25 > obstacle2Y - 25) {
            std::cout << "Hit an obstacle! -1 Life\n";
            lives--;

            // Reset player position if there are remaining lives
            if (lives > 0) {
                playerY = 25.0f;
                playerX = 275.0f;
            } else {
                std::cout << "Game Over! Press Space to Play Again.\n";
                gameOver = true;
            }
        }

        // Check for collision with obstacle3
        if (playerX - 25 < obstacle3X + 25 &&
            playerX + 25 > obstacle3X - 25 &&
            playerY - 25 < obstacle3Y + 25 &&
            playerY + 25 > obstacle3Y - 25) {
            std::cout << "Hit an obstacle! -1 Life\n";
            lives--;

            // Reset player position if there are remaining lives
            if (lives > 0) {
                playerY = 25.0f;
                playerX = 275.0f;
            } else {
                std::cout << "Game Over! Press Space to Play Again.\n";
                gameOver = true;
            }
        }

        // Check for collision with obstacle4
        if (playerX - 25 < obstacle4X + 25 &&
            playerX + 25 > obstacle4X - 25 &&
            playerY - 25 < obstacle4Y + 25 &&
            playerY + 25 > obstacle4Y - 25) {
            std::cout << "Hit an obstacle! -1 Life\n";
            lives--;

            // Reset player position if there are remaining lives
            if (lives > 0) {
                playerY = 25.0f;
                playerX = 275.0f;
            } else {
                std::cout << "Game Over! Press Space to Play Again.\n";
                gameOver = true;
            }
        }

        // Check if player reached the top
        if (playerY + 25 > 500) {
            // Increment score and reset player position
            score += 100;
            playerY = 25.0f;
            playerX = 275.0f;

            // Increase obstacle speed by 1
            if (obstacleSpeed < 10.0f) {
                obstacleSpeed += 0.5f;
                std::cout << "Obstacle speed increased to " << obstacleSpeed << std::endl;
            }

            // Generate new blue squares
            draw.initializeBlueSquares();
        }
    } else {
        // Display "Game Over Press Space to Play Again" message
        draw.drawText("Game Over Press Space to Play Again", 80, 250);
    }

    // Display score on the top right
    std::string scoreText = "Score: " + std::to_string(score);
    draw.drawText(scoreText, 400, 480);

    // Display hearts on the top left
    draw.drawHearts();

    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27:  // ASCII code for the 'Esc' key
            exit(0);
            break;
        case 32:  // ASCII code for the space bar
            if (gameOver) {
                // Reset the game
                std::cout << "Game Reset.\n";
                playerY = 25.0f;
                playerX = 275.0f;
                obstacle1X = 0.0f;
                obstacle2X = 500.0f;
                obstacle3X = 200.0f;
                obstacle4X = 300.0f;
                gameOver = false;
                score = 0;
                lives = 3;

                // Reset obstacle speed to the initial value
                obstacleSpeed = initialObstacleSpeed;

                // Initialize blue squares
                draw.initializeBlueSquares();
            }
            break;
    }
}


void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            if (!gameOver)
                playerY += playerSpeed;

            // Allow the character to move off-screen at the top
            break;
        case GLUT_KEY_DOWN:
            if (!gameOver)
                playerY -= playerSpeed;

            // Ensure the character stays within the bottom boundary
            if (playerY - 25 < 0)
                playerY = 0 + 25;

            break;
        case GLUT_KEY_LEFT:
            if (!gameOver)
                playerX -= playerSpeed;

            // Ensure the character stays within the left boundary
            if (playerX - 25 < 0)
                playerX = 0 + 25;

            break;
        case GLUT_KEY_RIGHT:
            if (!gameOver)
                playerX += playerSpeed;

            // Ensure the character stays within the right boundary
            if (playerX + 25 > 500)
                playerX = 500 - 25;

            break;
    }

    glutPostRedisplay();  // Request a redraw
}

void timer(int value) {
    glutTimerFunc(16, timer, 0);  // 60 frames per second

    if (!gameOver) {
        glutPostRedisplay();  // Request a redraw
    }
}