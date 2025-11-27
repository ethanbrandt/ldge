#include <SDL3/SDL.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "FileHandler.h"
#include "../rendering/RenderManager.h"
#include "../rendering/UIElement.h"
#include "../audio/AudioManager.h"

int main()
{
    SDL_Window* window;
    SDL_Renderer* renderer;

    //intializes a window, specifically one that plays audio
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    //creates the window so that it takes inputs and does sounds
    SDL_CreateWindowAndRenderer("examples/audio/multiple-streams", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer);  
	SDL_SetWindowResizable(window, false);
    
	AudioManager audioManager;
	RenderManager renderManager(renderer);

	std::string songFilePath = "assets/game_music.json";
	std::string sfxFilePath = "assets/game_sword.json";

	FileHandler fileHandler(&renderManager, &audioManager);

	fileHandler.LoadAudioFromFile(songFilePath);
	fileHandler.LoadAudioFromFile(sfxFilePath);

	audioManager.PlayAllSongs();

	std::string fontFilePath = "assets/bytesized-mono.json";

	fileHandler.LoadFontFromFile(fontFilePath);

	UIText uiText;
	uiText.SetText("This is a test of the ui text rendering. RAAAAAAAAAAHHHH!!!!?!?!?!");
	uiText.SetScreenPosition(50, 50);
	uiText.SetScale(4.0f, 4.0f);

	renderManager.RegisterUIElement(&uiText);
	renderManager.SetUITextFont(uiText, "assets/bytesized-mono.bmp");

	while (true)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
			{
				SDL_Quit();
				return 0;
			}
		}
		
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		renderManager.RenderAll(w, h);

		std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
	}
}