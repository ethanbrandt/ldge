#include <SDL3/SDL.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "utilities/FileHandler.h"
#include "input/InputManager.h"
#include "rendering/RenderManager.h"
#include "rendering/UIElement.h"
#include "physics/RigidBody.h"
#include "physics/PhysicsManager.h"
#include "audio/AudioManager.h"
#include "gorm/ScriptManager.h"

InputManager* InputManager::instance = nullptr;
RenderManager* RenderManager::instance = nullptr;
FileHandler* FileHandler::instance = nullptr;
ScriptManager* ScriptManager::instance = nullptr;
AudioManager* AudioManager::instance = nullptr;
PhysicsManager* PhysicsManager::instance = nullptr;

const int FRAMES_PER_SECOND = 60;
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

int main()
{
    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL_CreateWindowAndRenderer("LDGE", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer);  
	SDL_SetWindowResizable(window, false);

	AudioManager audioManager;
	RenderManager renderManager(renderer);
	ScriptManager scriptManager;
	InputManager inputManager;
	FileHandler fileHandler;
	PhysicsManager physicsManager;

	std::string entityFilePath = "assets/test_entity.json";
	EntityId id = fileHandler.LoadEntityFromFile(entityFilePath);
	if (id == -1)
	{
		std::cerr << "fileHandler Error" << std::endl;
		return 0;
	}

	std::string entityFilePath = "assets/test_entity_2.json";
	EntityId id = fileHandler.LoadEntityFromFile(entityFilePath);
	if (id == -1)
	{
		std::cerr << "fileHandler Error" << std::endl;
		return 0;
	}

	physicsManager.RegisterRigidBody(scriptManager.GetEntityRecord(id).rigidBody, id);

	scriptManager.Start(id);
	renderManager.RegisterActor(scriptManager.GetEntityRecord(id).actor);

	const auto frameTime = std::chrono::microseconds((int)std::ceil(1000000.0 / FRAMES_PER_SECOND));
	auto nextTick = std::chrono::steady_clock::now();
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

		float dt = 1.0f / FRAMES_PER_SECOND;
		physicsManager.UpdatePhysics(dt);
		scriptManager.Update(dt, WINDOW_WIDTH, WINDOW_HEIGHT);
		inputManager.UpdateInput();
		
		renderManager.RenderAll(WINDOW_WIDTH, WINDOW_HEIGHT);

		nextTick += frameTime;
		auto now = std::chrono::steady_clock::now();
		if (nextTick > now)
			std::this_thread::sleep_for(nextTick - now);
		else
			nextTick = now;
	}
	return 0;
}