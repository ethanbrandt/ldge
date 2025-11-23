#pragma once
#include "InputManager.h"

#include <SDL3/SDL.h>
#include <map>
#include <string>
#include <iostream>
enum ReturnEventType
{
	NONE,
	QUIT
};

class EventHandler
{
	private:
		InputManager* inputManager;

	public:
		EventHandler(InputManager* _inputManager);
		ReturnEventType UpdateEventHandler();
};