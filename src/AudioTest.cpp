#include "InputManager.h"
#include "AudioManager.h"
#include <iostream>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


InputManager test;
AudioManager manager;
AudioStream* audio_stream_pointer = manager.CreateAudioStream("game_music.wav");

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_CreateWindowAndRenderer("examples/audio/multiple-streams", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer);  

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT) {
            return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
        }
        return SDL_APP_CONTINUE;  /* carry on with the program! */
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    test.UpdateInput();
    std::cout << "I am listening" << std::endl;
    std::cout << "isPressed" << test.IsPressed('a') << std::endl;
    std::cout << "wasPressedDown" << test.WasPressedDown('a') << std::endl;
    std::cout << "wasReleased" << test.WasReleased('a') << std::endl;

    if (test.IsPressed('a'))
    {
        std::cout << "the a key was pressed" << std::endl;
        audio_stream_pointer->PlaySound();
    }

    return SDL_APP_CONTINUE;
}


void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}
