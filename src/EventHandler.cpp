#include "EventHandler.h"
#include "InputManager.h"

#include <SDL3/SDL.h>
#include <iostream>

EventHandler::EventHandler(InputManager* _inputManager)
{
	inputManager = _inputManager;
}

ReturnEventType EventHandler::UpdateEventHandler()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
		{
			return QUIT;
		}

		if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP)
		{
			std::cout << "Key event detected: " << event.key.key << std::endl;
			inputManager->HandleInputEvent(&event);
		}
	}
	return NONE;
}