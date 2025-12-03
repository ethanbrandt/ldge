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
#include "UIElement.h"

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

	RenderManager renderManager(renderer);
	renderManager.LoadTileSet("assets/test-tileset.bmp", 8);
	renderManager.LoadTileMap("assets/map.txt");
    
	Actor player;
    player.LoadSprite(renderer, "assets/sshot0002.bmp");
    
    float playerx = 250, playery = 150;
    player.SetScreenPosition(playerx, playery);
    player.SetScreenSize(64, 64);
	
	renderManager.RegisterActor(&player);

	std::string fontAtlasOrder = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,;:?!-_~#";
	std::string fontFilePath = "assets/awe-mono-gold-v2.bmp";
	
	renderManager.LoadFontAtlas(fontFilePath, fontAtlasOrder, 8, 16, 8);

	UIImage uiImage;
	uiImage.LoadUIImage(renderer, "assets/lettuce.bmp");
	uiImage.SetScreenPosition(-600, 50);
	uiImage.SetScale(2.5f, 1.0f);

	renderManager.RegisterUIElement(&uiImage);

	UIText uiText;
	uiText.SetText("This is a test of the ui text rendering. RAAAAAAAAAAHHHH!!!!?!?!?!");
	uiText.SetScreenPosition(50, 50);
	uiText.SetScale(2.0f, 2.0f);

	renderManager.RegisterUIElement(&uiText);
	renderManager.SetUITextFont(uiText, fontFilePath);

	int i = 0;
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

		if (i == 200)
			renderManager.UnregisterActor(&player);
		else if (i == 120)
			renderManager.UnregisterUIElement(&uiText);
		else if (i == 160)
			renderManager.UnregisterUIElement(&uiImage);

		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		renderManager.RenderAll(w, h);

        playerx++;
        player.SetScreenPosition(playerx, playery);
		i++;
		std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }

    SDL_Quit();

    return 0;
}
