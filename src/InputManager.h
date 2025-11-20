#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <map>
#include <string>

class InputManager
{
	private:
		struct keyState {
			bool isPressed;
			bool wasPressedDown;
			bool wasReleased;
		};
		std::map<SDL_Keycode, keyState> keyboard;

		enum KeyToKeyCode {
			A = SDLK_A,
			B = SDLK_B,
			C = SDLK_C,
			D = SDLK_D,
			E = SDLK_E,
			F = SDLK_F,
			G = SDLK_G,
			H = SDLK_H,
			I = SDLK_I,
			J = SDLK_J,
			K = SDLK_K,
			L = SDLK_L,
			M = SDLK_M,
			N = SDLK_N,
			O = SDLK_O,
			P = SDLK_P,
			Q = SDLK_Q,
			R = SDLK_R,
			S = SDLK_S,
			T = SDLK_T,
			U = SDLK_U,
			V = SDLK_V,
			W = SDLK_W,
			X = SDLK_X,
			Y = SDLK_Y,
			Z = SDLK_Z,
			a = SDLK_A,
			b = SDLK_B,
			c = SDLK_C,
			d = SDLK_D,
			e = SDLK_E,
			f = SDLK_F,
			g = SDLK_G,
			h = SDLK_H,
			i = SDLK_I,
			j = SDLK_J,
			k = SDLK_K,
			l = SDLK_L,
			m = SDLK_M,
			n = SDLK_N,
			o = SDLK_O,
			p = SDLK_P,
			q = SDLK_Q,
			r = SDLK_R,
			s = SDLK_S,
			t = SDLK_T,
			u = SDLK_U,
			v = SDLK_V,
			w = SDLK_W,
			x = SDLK_X,
			y = SDLK_Y,
			z = SDLK_Z
		};

	public:

		bool IsPressed(char letter)
		{
			/*
			* Determine if keycode is in map,
			* if not return false
			* if in map, return most recent state of IsPressed bool
			*/

			SDL_Keycode key = KeyToKeyCode(letter);

			if (keyboard.find(key) == keyboard.end())
			{
				return false;
			} 

			return keyboard[key].isPressed;
		}

		bool WasPressedDown(char letter)
		{
			/*
			* Determine if keycode is in map,
			* if not return false
			* if in map, return most recent state of WasPressedDown bool
			*/
			SDL_Keycode key = KeyToKeyCode(letter);

			if (keyboard.find(key) == keyboard.end())
			{
				return false;
			}
			return keyboard[key].wasPressedDown;
		}
		
		bool WasReleased(char letter)
		{
			/*
			* Determine if keycode is in map,
			* if not return false
			* if in map, return most recent state of WasReleased bool
			*/
			SDL_Keycode key = KeyToKeyCode(letter);

			if (keyboard.find(key) == keyboard.end())
			{
				return false;
			}
			return keyboard[key].wasReleased;
		}

		void UpdateInput()
		{
			/*Uses PollEvent to listen to the event. IDs the key from the event.
			* Determines if that key is in the map, if not put the key in the map, 
			* then initialize everything to false.
			* Then checks through each case of IsPressed and changes the three
			* bools depending on what action was taken.
			*/
			SDL_Event event;

			//resets the wasReleased value of every key to false
			//because we don't get repeated key-up events if there is no activity.
			for (auto it = keyboard.begin(); it != keyboard.end(); ++it)
			{
				keyboard[it->first].wasReleased = false;
			}

			while (SDL_PollEvent(&event))
			{
				if (keyboard.find(event.key.key) == keyboard.end())
				{
					keyboard[event.key.key].isPressed = false;
					keyboard[event.key.key].wasPressedDown = false;
					keyboard[event.key.key].wasReleased = false;
				}

				//this is the first frame the key is being pressed down
				if (keyboard[event.key.key].isPressed == false && event.key.type == SDL_EVENT_KEY_DOWN)
				{
					keyboard[event.key.key].wasPressedDown = true;
					keyboard[event.key.key].isPressed = true;
					keyboard[event.key.key].wasReleased = false;
				}
				//this is the case if the key was up and is still up (not pressed down)
				else if (keyboard[event.key.key].isPressed == false && event.key.type == SDL_EVENT_KEY_UP)
				{
					keyboard[event.key.key].wasPressedDown = false;
					keyboard[event.key.key].isPressed = false;
					keyboard[event.key.key].wasReleased = false;
				}
				//key is pressed down, and is still down
				else if (keyboard[event.key.key].isPressed == true && event.key.type == SDL_EVENT_KEY_DOWN)
				{
					keyboard[event.key.key].wasPressedDown = false;
					keyboard[event.key.key].isPressed = true;
					keyboard[event.key.key].wasReleased = false;
				}
				//the key was pressed down, but is now up
				else if (keyboard[event.key.key].isPressed == true && event.key.type == SDL_EVENT_KEY_UP)
				{
					keyboard[event.key.key].wasPressedDown = false;
					keyboard[event.key.key].isPressed = false;
					keyboard[event.key.key].wasReleased = true;
				}

			}

		}
		
};