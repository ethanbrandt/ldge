#include <SDL3/SDL.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Actor.h"
#include "TileRenderer.h"
using namespace std;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL_Init Error: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Tiles", 960, 700, 0);
    if (!window) {
        cerr << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    //load up tileset
    SDL_Surface* source = SDL_LoadBMP("lettuce.bmp");
    if (!source) {
        cerr << "SDL_LoadBMP Error: " << SDL_GetError() << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    //load up the images and map layout we will use.
    TileRenderer tileRenderer;
    tileRenderer.CreateTileTextures(renderer, source);
    tileRenderer.LoadTileMap("map.txt");
    Actor player;
    SDL_Rect frame = { 0, 0, 32, 32 };   // first frame in spritesheet
    player.Load(renderer, "sshot0002.bmp", frame);
    
    float playerx = 100, playery = 150;
    player.SetPosition(playerx, playery);
    player.SetSize(64, 64);

    SDL_DestroySurface(source);
    //the following is my test game loop. We can change variables/move things around as necessary but
    //we need this to be sure it works.
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT)
                quit = true;
        }

        SDL_RenderClear(renderer);
        tileRenderer.Render(renderer, 960, 700);
        player.Render(renderer);
        playerx++;
        player.SetPosition(playerx, playery);
        SDL_RenderPresent(renderer);
    }
    //close window and cleanup.
    tileRenderer.Cleanup();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
