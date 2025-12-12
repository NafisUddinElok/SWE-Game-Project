#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <sstream>
using namespace std;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
int ROAD_SPEED = 5;
int PLAYER_SPEED = 10;
int OPPONENT_SPEED = 8;
int SCORE = 0;
int ROAD_WIDTH = 400;
bool collisionOccured = false;
bool quit = false;
SDL_Event event;

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Surface *surface = IMG_Load(path);
    if (!surface) {
        cout << "problem creating an image on surface";
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}

void renderText(const string& text, int x, int y, SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color white = {255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), white);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect destRect = {x, y, 50, 30};
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

}

string intToString(int value) {
    ostringstream oss;
    oss << value;
    return oss.str();
}

bool checkCollision(const SDL_Rect& rect1, const SDL_Rect& rect2) {
    return SDL_HasIntersection(&rect1, &rect2);
}
int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() < 0) {
        cout << "SDL Init Error: " << SDL_GetError() << endl;
        return 1;
    }
    if (!(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG)) {
        cout << "problem initializing image library" << endl;

        SDL_Quit();
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Car racing game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer*  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!window || !renderer) {
        cout << "SDL Window Error: " << SDL_GetError() << endl;
        SDL_Quit();

        return 1;
    }


    SDL_Texture *roadTexture = loadTexture("Images/road1.jpg", renderer); // also 
    SDL_Texture *grassTexture = loadTexture("Images/longtree1.jpg", renderer);
    SDL_Texture* carTexture = loadTexture("Images/car.png", renderer);
    SDL_Texture *carOpp1Texture = loadTexture("Images/caropp1.png", renderer);
    SDL_Texture *carOpp2Texture = loadTexture("Images/caropp2.png", renderer);
    SDL_Texture *explosionTexture = loadTexture("Images/explosion.png", renderer);
    
    TTF_Font *font =  TTF_OpenFont("ARIAL.TTF", 25);
    
    if (!font) {
        cout << "Failed to open font ttf" << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    if (!roadTexture || !grassTexture || !carTexture || !carOpp1Texture || !carOpp2Texture || !explosionTexture) {
        cout << "some texture might not have been loaded";
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_Rect road1 = { (WINDOW_WIDTH - ROAD_WIDTH) / 2, 0, ROAD_WIDTH, WINDOW_HEIGHT};
    SDL_Rect road2 = { (WINDOW_WIDTH - ROAD_WIDTH) / 2, -WINDOW_HEIGHT, ROAD_WIDTH, WINDOW_HEIGHT};
    
    SDL_Rect grass1Left = {0, 0, (WINDOW_WIDTH - ROAD_WIDTH) / 2, WINDOW_HEIGHT};
    SDL_Rect grass2Left = {0, -WINDOW_HEIGHT, (WINDOW_WIDTH- ROAD_WIDTH)/2, WINDOW_HEIGHT};


    SDL_Rect grass1Right = { (WINDOW_WIDTH + ROAD_WIDTH) / 2, 0, (WINDOW_WIDTH - ROAD_WIDTH)/ 2, WINDOW_HEIGHT };
    SDL_Rect grass2Right= { (WINDOW_WIDTH + ROAD_WIDTH) / 2, -WINDOW_HEIGHT, (WINDOW_WIDTH - ROAD_WIDTH) / 2, WINDOW_HEIGHT };


    SDL_Rect playerCar = {(WINDOW_WIDTH-50)/2, WINDOW_HEIGHT - 120, 50, 100};
    SDL_Rect opponentCar1 = {(WINDOW_WIDTH-ROAD_WIDTH)/2 + rand() % (ROAD_WIDTH - 50), -120, 50, 100};
    SDL_Rect opponentCar2 = { (WINDOW_WIDTH - ROAD_WIDTH) / 2 + rand() % (ROAD_WIDTH - 50), -300, 50, 100 };

    SDL_Rect explosionRect;
    // game loop 
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN) {
                
                if (!collisionOccured) {
                    switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        if (playerCar.x > (WINDOW_WIDTH - ROAD_WIDTH) / 2) {
                            playerCar.x -= PLAYER_SPEED;
                        }
                        break;

                    case SDLK_RIGHT:
                        if (playerCar.x < (WINDOW_WIDTH + ROAD_WIDTH) / 2 - playerCar.w) {
                            playerCar.x += PLAYER_SPEED;
                        }
                        break;

                    }
                }
                
            }
        }
        if (playerCar.x < (WINDOW_WIDTH - ROAD_WIDTH) / 2) {
            collisionOccured = true;
            explosionRect = playerCar;
        }

        if (playerCar.x + playerCar.w > (WINDOW_WIDTH + ROAD_WIDTH) / 2) {
            collisionOccured = true;
            explosionRect = playerCar;
        }

        if (!collisionOccured) {
            // move road (scrolling logic)
            road1.y += ROAD_SPEED;
            road2.y += ROAD_SPEED;

            grass1Left.y += ROAD_SPEED;
            grass2Left.y += ROAD_SPEED;

            grass1Right.y += ROAD_SPEED;
            grass2Right.y += ROAD_SPEED;



            if (road1.y >= WINDOW_HEIGHT) {
                road1.y = road2.y - WINDOW_HEIGHT;
            }
            if (road2.y >= WINDOW_HEIGHT) {
                road2.y = road1.y - WINDOW_HEIGHT;
            }

            if (grass1Left.y >= WINDOW_HEIGHT) {
                grass1Left.y = grass2Left.y - WINDOW_HEIGHT;
            }
            if (grass2Left.y >= WINDOW_HEIGHT) {
                grass2Left.y = grass1Left.y - WINDOW_HEIGHT;
            }

            if (grass1Right.y >= WINDOW_HEIGHT) {
                grass1Right.y = grass2Right.y - WINDOW_HEIGHT;
            }
            if (grass2Right.y >= WINDOW_HEIGHT) {
                grass2Right.y = grass1Right.y - WINDOW_HEIGHT;
            }

            opponentCar1.y += OPPONENT_SPEED;
            opponentCar2.y += OPPONENT_SPEED;

            if (opponentCar1.y > WINDOW_HEIGHT) {
                SCORE++;
                opponentCar1.y = -120;
                opponentCar1.x = (WINDOW_WIDTH - ROAD_WIDTH) / 2 + rand() % (ROAD_WIDTH - 50);
            }

            if (opponentCar2.y > WINDOW_HEIGHT) {
                SCORE++;
                opponentCar2.y = -300;
                opponentCar2.x = (WINDOW_WIDTH - ROAD_WIDTH) / 2 + rand() % (ROAD_WIDTH - 50);
            }

            if (checkCollision(playerCar, opponentCar1) || checkCollision(playerCar, opponentCar2)) {
                
                collisionOccured = true;
                explosionRect = playerCar;
            }

        }
        

        //Render out everything
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, roadTexture, NULL, &road1);
        SDL_RenderCopy(renderer, roadTexture, NULL, &road2);

        SDL_RenderCopy(renderer, grassTexture, NULL, &grass1Left);
        SDL_RenderCopy(renderer, grassTexture, NULL, &grass2Left);

        SDL_RenderCopy(renderer, grassTexture, NULL, &grass1Right);
        SDL_RenderCopy(renderer, grassTexture, NULL, &grass2Right);

        SDL_RenderCopy(renderer, carTexture, NULL, &playerCar);

        SDL_RenderCopy(renderer, carOpp1Texture, NULL, &opponentCar1);
        SDL_RenderCopy(renderer, carOpp2Texture, NULL, &opponentCar2);

        if (collisionOccured) {
            SDL_RenderCopy(renderer, explosionTexture, NULL, &explosionRect);
        }
        else {
            renderText("Score : " + intToString(SCORE), WINDOW_WIDTH-200,10, renderer, font);
        }
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // approx 60 frame per second
    }
    SDL_DestroyTexture(grassTexture);
    SDL_DestroyTexture(carTexture);
    SDL_DestroyTexture(carOpp1Texture);
    SDL_DestroyTexture(carOpp2Texture);

    SDL_DestroyTexture(roadTexture);
    SDL_DestroyTexture(carTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();

    SDL_Delay(2000);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
