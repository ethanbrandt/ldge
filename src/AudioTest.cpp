#include "InputManager.h"
#include "AudioManager.h"
#include "EventHandler.h"

#include <SDL3/SDL.h>
#include <iostream>
#include <chrono>
#include <thread>

InputManager inputManager;
AudioManager* audioManager;
EventHandler* eventHandler;

void HandleQuit();

int main()
{
    SDL_Window* window;
    SDL_Renderer* renderer;

    //intializes a window, specifically one that plays audio
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    
    //creates an instance of the audioaudioManager 
    audioManager = new AudioManager();

	eventHandler = new EventHandler(&inputManager);

    //creates the window so that it takes inputs and does sounds
    SDL_CreateWindowAndRenderer("examples/audio/multiple-streams", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer);  
    
    //adding a inputManager song
    audioManager->AddSong("game_music.wav");
    
    //adding a inputManager sound effect
    audioManager->AddSound("game_sword.wav");

	while (true)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
			{
				HandleQuit();
				return 0;
			}

			if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP)
			{
				std::cout << "Key event detected: " << event.key.key << std::endl;
				inputManager.HandleInputEvent(&event);
			}
		}
		
		//plays all the songs/sounds that are meant to be looping. e.g. bgms/persistent sound effects
		audioManager->PlayAllSongs();

		//tests swinging a sword sound over the BGM
		if (inputManager.WasPressedDown('a'))
		{
			//std::cout << "the a key is pressed" << std::endl;
			audioManager->PlaySound("game_sword.wav"); //waits for every sound to finish before playing the next one if held down
		}
		//reduces the volume of the inputManager bgm
		if (inputManager.WasPressedDown('s'))
		{
			std::cout << "the s key is pressed" << std::endl;
			audioManager->SetSongVolume("game_music.wav", .2);
		}
		//pauses all the audio
		if (inputManager.WasPressedDown('d'))
		{
			std::cout << "the d key is pressed" << std::endl;
			audioManager->PauseAllAudio(); 
		}
		//resumes all the audio
		if (inputManager.WasPressedDown('f'))
		{
			std::cout << "the f key is pressed" << std::endl;
			audioManager->ResumeAllAudio(); 
		}
		
		//deletes all the songs, so this will stop playing the bgm
		if (inputManager.WasPressedDown('g'))
		{
			std::cout << "the g key is pressed" << std::endl;
			audioManager->DeleteAllSongs(); 
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	return 0;
}

void HandleQuit()
{
	audioManager->DeleteAllSongs();
    audioManager->DeleteAllSounds();

	delete audioManager;
	delete eventHandler;
}