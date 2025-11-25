#include <SDL3/SDL.h>
#include <vector>
#include <algorithm>
#include <string>

#include "Actor.h"
#include "TileRenderer.h"

class RenderManager
{
private:
	SDL_Renderer* renderer;
	std::vector<Actor*> actors;
	TileRenderer* tileRenderer;
	// insert a vector for ui elements

public:
	RenderManager(SDL_Renderer* _renderer)
	{
		renderer = _renderer;
		tileRenderer = new TileRenderer();
	}

	~RenderManager()
	{
		delete tileRenderer;

		for (auto actor : actors)
			delete actor;
		actors.clear();
	}

	void RegisterActor(Actor* _actor)
	{
		actors.push_back(_actor);
	}

	void UnregisterActor(Actor* _actor)
	{
		actors.erase(std::remove(actors.begin(), actors.end(), _actor), actors.end());
	}

	void RegisterUIElement(/*UIElement* uiElement*/)
	{
		// this will add a ui element to the vector
	}

	void UnregisterUIElement(/*UIElement* uiElement*/)
	{
		// this will remove a ui element from the vector
	}

	void LoadTileSet(const std::string& _tileMapPath, int _tileSize)
	{
		tileRenderer->CreateTileTextures(renderer, _tileMapPath, _tileSize);
	}

	void LoadTileMap(const std::string& _tileMapFilePath)
	{
		tileRenderer->LoadTileMap(_tileMapFilePath);
	}

	void RenderAll(int _windowWidth, int _windowHeight)
	{
		SDL_RenderClear(renderer);

		tileRenderer->Render(renderer, _windowWidth, _windowHeight);

		for (auto actor : actors)
			actor->Render(renderer);
		
		// render ui elements

		SDL_RenderPresent(renderer);
	}
};