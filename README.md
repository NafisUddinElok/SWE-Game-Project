🚗 SDL2 Car Racing Game

A simple 2D car-racing game built using SDL2, SDL_image, and SDL_ttf.
The player controls a car, avoids opponent cars, and scores points by surviving as long as possible.
Includes road scrolling, collision detection, score display, and explosion animation.

🎮 Features

Smooth road scrolling animation

Player car movement (Left/Right)

Two opponent cars with random lane generation

Collision detection

Explosion animation on crash

Score system rendered using SDL_ttf

Textures loaded using SDL_image

60 FPS game loop (SDL_Delay(16))


🛠️ Technologies Used

C++

SDL2

SDL_image

SDL_ttf


📦 Requirements

Install the following SDL libraries:

Library	    Purpose

SDL2	      Window, renderer, events

SDL_image  	Loading PNG/JPG textures

SDL_ttf	    Rendering text (score)



▶️ Controls

Key	            Action

Left Arrow	    Move left

Right Arrow	    Move right

Close Window	  Quit

🧨 Game Over

If the player:

Hits an opponent car

Leaves the road

…an explosion animation appears and the game stops updating score.

▶️ Watch the Gameplay



https://github.com/user-attachments/assets/2c93d678-927d-4ac0-a54e-373d6789db97


