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

// Set intial Log x,y positions and speed
const int numLogs = 3;
Log logs[numLogs];

void initializeLogs() {
    for (int i = 0; i < numLogs; ++i) {
        logs[i].x = dist(rng);
        if (i == 0) {
            logs[i].y = 275.0f;
            logs[i].speed = 1.5f;
            logs[i].length = 2;
        } else if (i == 1) {
            logs[i].y = 325.0f;
            logs[i].speed = 1.0f;
            logs[i].length = 3;
        } else if (i == 2) {
            logs[i].y = 375.0f;
            logs[i].speed = 2.0f;
            logs[i].length = 2;
        }
        
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

bool gameOver = false;

// Texture IDs
unsigned int textureID;
unsigned int playerTextureID;
unsigned int backgroundTextureID;
unsigned int redcarTextureID;
unsigned int bluecarTextureID;
unsigned int greencarTextureID;
unsigned int carrotTextureID;
unsigned int goldcarrotTextureID;
unsigned int LogoTextureID;
unsigned int blackBunnyTextureID;
unsigned int brownBunnyTextureID;
unsigned int whiteBunnyTextureID;
unsigned int selectionTextureID;
unsigned int gameoverTextureID;
unsigned int logTextureID;
unsigned int longlogTextureID;

unsigned int loadTexture(const char* filename) {
    int width, height, numChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &numChannels, 0);
    if (data) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    } else {
        std::cerr << "Failed to load player texture" << std::endl;
    }
    return textureID;
}

// Texture IDs for player animation frames
std::vector<unsigned int> playerFrames;
int currentFrame = 0;
int frameCount = 0;
int frameDelay = 5;  // Delay between frames

// Load player animation frames
void loadPlayerFrames() {
    if (player.selectedBunnyColor == BLACK) {
        loadTexture("lib/Sprites/BlackBunny2.PNG");
        playerFrames.push_back(textureID);

        loadTexture("lib/Sprites/BlackBunny3.PNG");
        playerFrames.push_back(textureID);

        loadTexture("lib/Sprites/BlackBunny4.PNG");
        playerFrames.push_back(textureID);
        
        loadTexture("lib/Sprites/BlackBunny5.PNG");
        playerFrames.push_back(textureID);
    } else if (player.selectedBunnyColor == BROWN) {
        loadTexture("lib/Sprites/BrownBunny2.PNG");
        playerFrames.push_back(textureID);

        loadTexture("lib/Sprites/BrownBunny3.PNG");
        playerFrames.push_back(textureID);

        loadTexture("lib/Sprites/BrownBunny4.PNG");
        playerFrames.push_back(textureID);
        
        loadTexture("lib/Sprites/BrownBunny5.PNG");
        playerFrames.push_back(textureID);
    } else if (player.selectedBunnyColor == WHITE) {
        loadTexture("lib/Sprites/WhiteBunny2.PNG");
        playerFrames.push_back(textureID);

        loadTexture("lib/Sprites/WhiteBunny3.PNG");
        playerFrames.push_back(textureID);

        loadTexture("lib/Sprites/WhiteBunny4.PNG");
        playerFrames.push_back(textureID);
        
        loadTexture("lib/Sprites/WhiteBunny5.PNG");
        playerFrames.push_back(textureID);
    }
}

void initializeTextures() {
    loadTexture("lib/Sprites/Background.PNG");
    backgroundTextureID = textureID;
    loadTexture("lib/Sprites/Carrot.PNG");
    carrotTextureID = textureID;
    loadTexture("lib/Sprites/GoldCarrot.PNG");
    goldcarrotTextureID = textureID;
    loadTexture("lib/Sprites/RedCar.PNG");
    redcarTextureID = textureID;
    loadTexture("lib/Sprites/BlueCar.PNG");
    bluecarTextureID = textureID;
    loadTexture("lib/Sprites/GreenCar.PNG");
    greencarTextureID = textureID;
    loadTexture("lib/Sprites/Logo.PNG");
    LogoTextureID = textureID;
    loadTexture("lib/Sprites/BlackBunny.PNG");
    blackBunnyTextureID = textureID;
    loadTexture("lib/Sprites/BrownBunny.PNG");
    brownBunnyTextureID = textureID;
    loadTexture("lib/Sprites/WhiteBunny.PNG");
    whiteBunnyTextureID = textureID;
    loadTexture("lib/Sprites/Selection.PNG");
    selectionTextureID = textureID;
    loadTexture("lib/Sprites/GameOver.PNG");
    gameoverTextureID = textureID;
    loadTexture("lib/Sprites/Log.PNG");
    logTextureID = textureID;
    loadTexture("lib/Sprites/LongLog.PNG");
    longlogTextureID = textureID;
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

void drawLog(float x, float y, int length) {
    glEnable(GL_TEXTURE_2D);
    

    if (length == 3) {
        // Long log texture mapping
        glBindTexture(GL_TEXTURE_2D, longlogTextureID);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y - 25);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(x + 150, y - 25);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(x + 150, y + 25);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y + 25);
    } else {
        // Regular log texture mapping
        glBindTexture(GL_TEXTURE_2D, logTextureID);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y - 25);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(x + 100, y - 25);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(x + 100, y + 25);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y + 25);
    }

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

void drawText(const std::string& text, float x, float y) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);

    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void drawHearts() {
    glColor3f(1.0f, 0.0f, 0.0f);  // Set color to red for hearts
    for (int i = 0; i < player.lives; ++i) {
        glBegin(GL_TRIANGLES);
        glVertex2f(30.0f + i * 30, 485.0f);
        glVertex2f(40.0f + i * 30, 475.0f);
        glVertex2f(50.0f + i * 30, 485.0f);

        glVertex2f(30.0f + i * 30, 485.0f);
        glVertex2f(35.0f + i * 30, 490.0f);
        glVertex2f(40.0f + i * 30, 485.0f);

        glVertex2f(40.0f + i * 30, 485.0f);
        glVertex2f(45.0f + i * 30, 490.0f);
        glVertex2f(50.0f + i * 30, 485.0f);

        glEnd();
    }
}

void drawBunnySelectionMenu() {
    glEnable(GL_TEXTURE_2D);

    // Draw background
    glBindTexture(GL_TEXTURE_2D, selectionTextureID);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(500.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(500.0f, 500.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 500.0f);
    glEnd();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    // Black bunny
    glBindTexture(GL_TEXTURE_2D, blackBunnyTextureID);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(50.0f, 150.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(150.0f, 150.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(150.0f, 250.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(50.0f, 250.0f);
    glEnd();

    // Brown bunny
    glBindTexture(GL_TEXTURE_2D, brownBunnyTextureID);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(200.0f, 150.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(300.0f, 150.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(300.0f, 250.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(200.0f, 250.0f);
    glEnd();

    // White bunny
    glBindTexture(GL_TEXTURE_2D, whiteBunnyTextureID);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(350.0f, 150.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(450.0f, 150.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(450.0f, 250.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(350.0f, 250.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    glutSwapBuffers();
}

std::chrono::steady_clock::time_point startTime;

void initiliazeTime() {
    startTime = std::chrono::steady_clock::now();  // Record the start time
}

// State variable to track if the menu should be displayed
bool displayMenu = true;

// Set onLog State
bool onLog;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

    // Display Logo for the first 5 seconds
    if (elapsedTime < 5) {
        // Draw logo
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, LogoTextureID);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(500.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(500.0f, 500.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 500.0f);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glutSwapBuffers();
    } 
    else if (displayMenu) {
        // Display Bunny Color Selection Menu
        drawBunnySelectionMenu();
    } 
    else {
        // Draw background
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, backgroundTextureID);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(500.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(500.0f, 500.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 500.0f);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        if (!gameOver) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // Draw Logs
            for (int i = 0; i < numLogs; ++i) {
                drawLog(logs[i].x, logs[i].y, logs[i].length);
                
            }

            // Update player animation frame
            if (!onLog && ((static_cast<int>(player.x) % 50 == 0))) {
                player.x -= 25;
            } else if (!onLog && (static_cast<int>(player.x) % 25 != 0)) {
                if (static_cast<int>(player.x) % 25 > 12.5) {
                    player.x += 25 - static_cast<int>(player.x) % 25;
                } else {
                    player.x -= 25 - static_cast<int>(player.x) % 25;
                }
            }

            drawPlayer();

            // Draw Cars
            for (int i = 0; i < numCars; ++i) {
                drawCar(cars[i].x, cars[i].y, i);

                // Update Car Position and Check if Cars went off-screen
                if (i == 0 || i == 2) {
                    cars[i].x += cars[i].CarSpeed;
                    if (cars[i].x > 500 + 25) {
                        cars[i].x = -25;  // Reset Car1 to the left side
                    }
                } else {
                    cars[i].x -= cars[i].CarSpeed;
                    if (cars[i].x < -25) {
                        cars[i].x = 500 + 25;  // Reset Car2 to the right side
                    }
                }

                // Check for collision with Cars
                if (player.x - 25 < cars[i].x + 25 &&
                    player.x + 25 > cars[i].x - 25 &&
                    player.y - 25 < cars[i].y + 25 &&
                    player.y + 25 > cars[i].y - 25) {
                    std::cout << "Hit by a Car! -1 Life\n";
                    player.lives--;

                    // Reset player position if there are remaining lives
                    if (player.lives > 0) {
                        player.y = 25.0f;
                        player.x = 275.0f;
                    } else {
                        std::cout << "Game Over! Press Space to Play Again.\n";
                        gameOver = true;
                    }
                }
            }

            // Draw and update carrots
            for (int i = 0; i < numCarrots; ++i) {
                if (Carrots[i].isVisible) {
                    drawCarrot(Carrots[i].x, Carrots[i].y, Carrots[i].isGolden);
                }  

                // Check for collision with Carrots
                if (Carrots[i].isVisible &&
                    player.x - 25 < Carrots[i].x + 12.5 &&
                    player.x + 25 > Carrots[i].x - 12.5 &&
                    player.y - 25 < Carrots[i].y + 12.5 &&
                    player.y + 25 > Carrots[i].y - 12.5) {
                    if (Carrots[i].isGolden) {
                        // Player touched a golden carrot
                        std::cout << "Grabbed a Golden Carrot! +250 Score\n";
                        player.score += 250;
                    } else {
                        // Player touched a carrot
                        std::cout << "Grabbed a Carrot! +20 Score\n";
                        player.score += 20;
                    }
                    Carrots[i].isVisible = false;
                }
            }

            onLog = false;

            // Check for collision with Logs
            for (int i = 0; i < numLogs; ++i) {
                if (i == 1) {
                    if ((player.y == logs[i].y) && (player.x >= logs[i].x && player.x < logs[i].x + 150)) {
                        // Player is on a Log, move with it
                        if (player.x < logs[i].x + 50) {
                            player.x = logs[i].x + 25;
                        } else if (player.x < logs[i].x + 100) {
                            player.x = logs[i].x + 75;
                        } else {
                            player.x = logs[i].x + 125;
                        }
                        player.x += logs[i].speed;
                        onLog = true;
                    }
                    // Update Log positions
                    logs[i].x += logs[i].speed;
                    // Check if Logs are out of bounds, reset their positions
                    if (logs[i].x > 500) {
                        logs[i].x = -150.0f;
                    }
                } else {
                    if ((player.y == logs[i].y) && (player.x >= logs[i].x && player.x < logs[i].x + 100)) {
                        // Player is on a Log, move with it
                        if (player.x < logs[i].x + 50) {
                            player.x = logs[i].x + 25;
                        } else {
                            player.x = logs[i].x + 75;
                        }
                        player.x -= logs[i].speed;
                        onLog = true;
                    }
                    // Update Log positions
                    logs[i].x -= logs[i].speed;
                    // Check if Logs are out of bounds, reset their positions
                    if (logs[i].x < -100) {
                        logs[i].x = 500.0f;
                    }
                }
            }

            // Check for collision with water
            if (player.y >= 275 && player.y < 425 && (!onLog)) {
                std::cout << "Fell in water! -1 Life\n";
                player.lives--;

                // Reset player position if there are remaining lives
                if (player.lives > 0) {
                    player.y = 25.0f;
                    player.x = 275.0f;
                } else {
                    std::cout << "Game Over! Press Space to Play Again.\n";
                    gameOver = true;
                }
            }

            // Check if player reached the top
            if (player.y + 50 > 500) {
                // Increment score and reset player position
                std::cout << "You made it to the top! +100 Score" << std::endl;
                player.score += 100;
                player.y = 25.0f;
                player.x = 275.0f;

                // Increase Car speed by 0.5; Maxed out at 10
                for (int i = 0; i < numCars; ++i) {
                    if (cars[i].CarSpeed < 10.0f) {
                        cars[i].CarSpeed += 0.5f;
                        std::cout << "Car speed increased to " << cars[i].CarSpeed << std::endl;
                    }
                }
                
                // Generate new carrots
                initializeCarrots();
            }
            glDisable(GL_BLEND);
        } 
        if (gameOver){
            // Draw game over screen
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, gameoverTextureID);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 0.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(500.0f, 0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(500.0f, 500.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 500.0f);
            glEnd();
            glDisable(GL_TEXTURE_2D);
        }

        // Display score on the top right
        std::string scoreText = "Score: " + std::to_string(player.score);
        drawText(scoreText, 385, 475);

        // Display hearts on the top left
        drawHearts();

        glutSwapBuffers();
    }
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
        case '1':
            player.selectedBunnyColor = BLACK;
            displayMenu = false;
            // Load player animation frames
            loadPlayerFrames();
            break;
        case '2':
            player.selectedBunnyColor = BROWN;
            displayMenu = false;
            // Load player animation frames
            loadPlayerFrames();
            break;
        case '3':
            player.selectedBunnyColor = WHITE;
            displayMenu = false;
            // Load player animation frames
            loadPlayerFrames();
            break;
        case 27:  // ASCII code for the 'Esc' key
            exit(0);
            break;
        case 32:  // ASCII code for the space bar
            if (gameOver) {
                // Reset the game
                std::cout << "Game Reset.\n";
                player.y = 25.0f;
                player.x = 275.0f;
                float Car1X = dist(rng);
                float Car2X = dist(rng);
                float Car3X = dist(rng);
                float Car4X = dist(rng);
                gameOver = false;
                player.score = 0;
                player.lives = 3;

                // Reset Car speed to the initial value
                for (int i = 0; i < numCars; ++i) {
                    cars[i].CarSpeed = cars[i].initialCarSpeed;
                }

                // Initialize carrots
                initializeCarrots();
            }
            break;
    }
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            if (!gameOver)
                player.y += player.speed;
                isMoving = true;

            // Allow the character to move off-screen at the top
            break;
        case GLUT_KEY_DOWN:
            if (!gameOver)
                player.y -= player.speed;
                isMoving = true;

            // Ensure the character stays within the bottom boundary
            if (player.y - 25 < 0)
                player.y = 0 + 25;

            break;
        case GLUT_KEY_LEFT:
            if (!gameOver)
                player.x -= player.speed;
                player.lookingLeft = true;
                isMoving = true;

            // Ensure the character stays within the left boundary
            if (player.x - 25 < 0)
                player.x = 0 + 25;

            break;
        case GLUT_KEY_RIGHT:
            if (!gameOver)
                player.x += player.speed;
                player.lookingLeft = false;
                isMoving = true;

            // Ensure the character stays within the right boundary
            if (player.x + 25 > 500)
                player.x = 500 - 25;

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