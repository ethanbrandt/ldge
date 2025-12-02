#include <SDL3/SDL.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <typeinfo>

#include "FileHandler.h"
#include "../input/InputManager.h"
#include "../rendering/RenderManager.h"
#include "../rendering/UIElement.h"
#include "../audio/AudioManager.h"
#include "../gorm/ScriptManager.h"

InputManager* InputManager::instance = nullptr;
RenderManager* RenderManager::instance = nullptr;
FileHandler* FileHandler::instance = nullptr;
ScriptManager* ScriptManager::instance = nullptr;
AudioManager* AudioManager::instance = nullptr;

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
	ScriptManager scriptManager;
	InputManager inputManager;
	FileHandler fileHandler;

	//std::string songFilePath = "assets/game_music.json";
	//std::string sfxFilePath = "assets/game_sword.json";

	//fileHandler.LoadAudioFromFile(songFilePath);
	//fileHandler.LoadAudioFromFile(sfxFilePath);


	std::string fontFilePath = "assets/bytesized-mono.json";

	fileHandler.LoadFontFromFile(fontFilePath);

	UIText uiText;
	uiText.SetText("This is a test of the ui text rendering. RAAAAAAAAAAHHHH!!!!?!?!?!");
	uiText.SetScreenPosition(50, 50);
	uiText.SetScale(4.0f, 4.0f);

	renderManager.RegisterUIElement(&uiText);
	renderManager.SetUITextFont(uiText, "assets/bytesized-mono.bmp");

	std::string entityFilePath = "assets/test_entity_2.json";
	EntityId id = fileHandler.LoadEntityFromFile(entityFilePath, Vector2(0,0));
	if (id == -1)
	{
		std::cerr << "fileHandler Error" << std::endl;
		return 0;
	}	

	std::cout << "b4 start" << std::endl;
	scriptManager.Start(id);
	EntityRecord record = scriptManager.GetEntityRecord(id);
	
	audioManager.PlayAllSongs();

	std::cout << entityFilePath << " pos: " << record.rigidBody->GetPosition().GetX() << ", " << record.rigidBody->GetPosition().GetY() << std::endl;

	std::cout << entityFilePath << " rb mass: " << record.rigidBody->GetMass() << std::endl;
	std::cout << entityFilePath << " rb isStatic: " << record.rigidBody->IsStatic() << std::endl;
	std::cout << entityFilePath << " rb colShape offset: " << record.rigidBody->GetColShape()->GetOffset().GetX() << ", " << record.rigidBody->GetColShape()->GetOffset().GetY() << std::endl;
	std::cout << entityFilePath << " rb colShape isTrigger: " << record.rigidBody->GetColShape()->IsTrigger() << std::endl;
	std::cout << entityFilePath << " rb colShape colMask: " << record.rigidBody->GetColShape()->GetColMask() << std::endl;

	if (typeid(*record.rigidBody->GetColShape()) == typeid(CollisionCircle))
	{
		CollisionCircle* colCircle = (CollisionCircle*)record.rigidBody->GetColShape();

		std::cout << entityFilePath << " rb colCircle radius: " << colCircle->GetRadius() << std::endl;
	}
	else if (typeid(*record.rigidBody->GetColShape()) == typeid(CollisionRectangle))
	{
		CollisionRectangle* colRect = (CollisionRectangle*)record.rigidBody->GetColShape();
		
		std::cout << entityFilePath << " rb colRectangle width & height: " << colRect->GetWidth() << ", " << colRect->GetLength() << std::endl;
	}
	else
	{
		std::cerr << "invalid colShape type" << std::endl;
		return 0;
	}

	renderManager.RegisterActor(record.actor);
	
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

			if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP)
			{
				inputManager.HandleInputEvent(&event);
			}
		}

		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		scriptManager.Update(1.0f/60.0f, w, h);
		inputManager.UpdateInput();
		
		renderManager.RenderAll(w, h);

		std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
	}
}