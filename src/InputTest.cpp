#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "InputManager.h"
#include <iostream>
#include <chrono>
#include <thread>

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window;
    window = SDL_CreateWindow("Test", 320, 240, SDL_WINDOW_RESIZABLE);

    //SDL_Event event;
  
    InputManager test;
    
    bool running = true;
    while (running)
    {
        test.UpdateInput();
        std::cout << "I am listening" << std::endl;
        std::cout << "isPressed" << test.IsPressed('a') << std::endl;
        std::cout << "wasPressedDown" << test.WasPressedDown('a') << std::endl;
        std::cout << "wasReleased" << test.WasReleased('a') << std::endl;

        if(test.IsPressed('a'))
        {
            std::cout << "the a key was pressed" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    
	return 0;
}
