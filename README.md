# CS32 Arcade - Bunny Hop

# Description
Our game takes inspiration from the classic arcade game Frogger and a similar modernized version called Crossy Road. Our take on these games follows a new avatar in the form of a Bunny. The user has to control the bunny using simple left, right, upward, and downward movements with the arrow keys and cross environments like roads and grass terrains while avoiding obstacles like vehicles which may move at different speeds, or other animals like foxes. As the player makes it to the top of the screen, their score will increase by 100 and they will be sent back to the bottom of the screen where the obstacles will have gained speed and progressively make traversering more challenging. Once a player is hit by an obstacle they will lose a life and be brought back to the bottom of the screen. Once the players loses 3 lives they will lose and have to restart from 0. We also added bonuses in the form of carrots that the user can pick up on their way to the top of the screen. By picking up a carrot the player can gain 20 points. There will be 3 carrots in random locations each new screen the player makes it to. We also added a small 5% chance that a carrot can be a golden carrot, where the player will gain a whopping 250 points, which gives the incentive for players to track down these carrots but adds a higher risk factor when traversing.

# Main:
The main.cpp creates the window with a 500 x 500 frame. It then proceeds to call all the important functions to run our game. First it sets the background, then it creates the display and tracks keyboard input for character movement. It starts the timer, initializes the carrots on the screen, and then runs the main loop. This main loop will continue running until the player presses the 'Esc' key.

# Game:
Our Game header and cpp files hold all the important functions that will run our game. These functions that are written out in Game.cpp will be called from our main.cpp when initalizing the game.
We have functions like timer and display which carry out core proccess and other smaller functions like drawHearts or DrawPlayer that takes care of building our objects.
