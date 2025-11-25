#include <SDL3/SDL.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>

#include "Actor.h"
#include "TileRenderer.h"
#include "RenderManager.h"

int main(int argc, char* argv[])
{
    SDL_Window* window;
    SDL_Renderer* renderer;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_CreateWindowAndRenderer("Render Test", 640, 360, 0, &window, &renderer);

    if (!renderer || !window)
	{
        std::cerr << "SDL_CreateWindowAndRender Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    //load up the images and map layout we will use.
	RenderManager renderManager(renderer);
	renderManager.LoadTileSet("assets/test-tileset.bmp", 8);
	renderManager.LoadTileMap("assets/map.txt");
    
	Actor player;
    player.LoadSprite(renderer, "assets/sshot0002.bmp");
    
    float playerx = 100, playery = 150;
    player.SetScreenPosition(playerx, playery);
    player.SetScreenSize(64, 64);
	
	renderManager.RegisterActor(&player);

    //the following is my test game loop. We can change variables/move things around as necessary but
    //we need this to be sure it works.
	SDL_Event e;
    while (true)
	{
        while (SDL_PollEvent(&e))
		{
            if (e.type == SDL_EVENT_QUIT)
			{
				SDL_Quit();
				return 0;
			}
		}

		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		renderManager.RenderAll(w, h);

        playerx++;
        player.SetScreenPosition(playerx, playery);
		std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }

    SDL_Quit();

    return 0;
}
