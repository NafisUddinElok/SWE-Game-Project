#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
using namespace std;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
int ROAD_SPEED = 5;
int ROAD_WIDTH = 400;
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

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
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
    if (!roadTexture || !grassTexture) {
        cout << "Road texture or grass texture has not been loaded";
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_Rect road1 = {(WINDOW_WIDTH - ROAD_WIDTH)/2, 0, ROAD_WIDTH, WINDOW_HEIGHT};
    SDL_Rect road2 = { (WINDOW_WIDTH - ROAD_WIDTH) / 2, -WINDOW_HEIGHT, ROAD_WIDTH, WINDOW_HEIGHT};
    
    SDL_Rect grass1Left = {0, 0, (WINDOW_WIDTH - ROAD_WIDTH) / 2, WINDOW_HEIGHT};
    SDL_Rect grass2Left = {0, -WINDOW_HEIGHT, (WINDOW_WIDTH- ROAD_WIDTH)/2, WINDOW_HEIGHT};


    SDL_Rect grass1Right = { (WINDOW_WIDTH + ROAD_WIDTH) / 2, 0, (WINDOW_WIDTH - ROAD_WIDTH)/ 2, WINDOW_HEIGHT };
    SDL_Rect grass2Right= { (WINDOW_WIDTH + ROAD_WIDTH) / 2, -WINDOW_HEIGHT, (WINDOW_WIDTH - ROAD_WIDTH) / 2, WINDOW_HEIGHT };

    // game loop 
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // move road (scrolling logic)
        /**
        */
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
        if(grass2Left.y >= WINDOW_HEIGHT) {
            grass2Left.y = grass1Left.y - WINDOW_HEIGHT;
        }

        if (grass1Right.y >= WINDOW_HEIGHT) {
            grass1Right.y = grass2Right.y - WINDOW_HEIGHT;
        }
        if (grass2Right.y >= WINDOW_HEIGHT) {
            grass2Right.y = grass1Right.y - WINDOW_HEIGHT;
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

        SDL_RenderPresent(renderer);

        SDL_Delay(16); // approx 60 frame per second
    }
    SDL_DestroyTexture(roadTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();

    SDL_Delay(2000);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
