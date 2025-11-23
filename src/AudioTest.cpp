#include "InputManager.h"
#include "AudioManager.h"
#include <iostream>


InputManager test;
AudioManager* manager;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_Window* window;
    SDL_Renderer* renderer;

    //intializes a window, specifically one that plays audio
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    
    //creates an instance of the audio manager
    manager = new AudioManager();

    //creates the window so that it takes inputs and does sounds
    SDL_CreateWindowAndRenderer("examples/audio/multiple-streams", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer);  
    
    //adding a test song
    manager->AddSong("game_music.wav");
    
    //adding a test sound effect
    manager->AddSound("game_sword.wav");

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_KEY_UP)
    {
        //checks for a specific keyboard event and sends that to the InputManager's UpdateInput function
        test.UpdateInput(event);
    }
    if (event->type == SDL_EVENT_QUIT) {
            return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
        }
        return SDL_APP_CONTINUE;  /* carry on with the program! */
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    //plays all the songs/sounds that are meant to be looping. e.g. bgms/persistent sound effects
    manager->PlayAllSongs();

    //tests swinging a sword sound over the BGM
    if (test.WasPressedDown('a'))
    {
        //std::cout << "the a key is pressed" << std::endl;
        manager->PlaySound("game_sword.wav"); //waits for every sound to finish before playing the next one if held down
    }
    //reduces the volume of the test bgm
    if (test.WasPressedDown('s'))
    {
        std::cout << "the s key is pressed" << std::endl;
        manager->SetSongVolume("game_music.wav", .2);
    }
    //pauses all the audio
    if (test.WasPressedDown('d'))
    {
        std::cout << "the d key is pressed" << std::endl;
        manager->PauseAllAudio(); 
    }
    //resumes all the audio
    if (test.WasPressedDown('f'))
    {
        std::cout << "the f key is pressed" << std::endl;
        manager->ResumeAllAudio(); 
    }
    
    //deletes all the songs, so this will stop playing the bgm
    if (test.WasPressedDown('g'))
    {
        std::cout << "the g key is pressed" << std::endl;
        manager->DeleteAllSongs(); 
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
    manager->DeleteAllSongs();
    manager->DeleteAllSounds();
}
