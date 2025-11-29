#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <unordered_map>

#include "Actor.h"
#include "TileRenderer.h"
#include "UIElement.h"

struct FontAsset
{
	std::unordered_map<char, SDL_Rect> fontMap;
	int charWidth;
	int charHeight;
};


class RenderManager
{
private:
	SDL_Renderer* renderer;
	std::vector<Actor*> actors;
	std::vector<UIElement*> uiElements;	
	TileRenderer* tileRenderer;
	std::unordered_map<std::string, FontAsset> fontAssets;

	static RenderManager* instance;

public:
	RenderManager(SDL_Renderer* _renderer)
	{
		instance = this;
		renderer = _renderer;
		tileRenderer = new TileRenderer();
	}

	~RenderManager()
	{
		delete tileRenderer;

		for (auto actor : actors)
			delete actor;
		actors.clear();

		for (auto uiElement : uiElements)
			delete uiElement;
		uiElements.clear();
	}

	static RenderManager* GetInstance()
	{
		return instance;
	}

	void RegisterActor(Actor* _actor)
	{
		actors.push_back(_actor);
	}

	void UnregisterActor(Actor* _actor)
	{
		actors.erase(std::remove(actors.begin(), actors.end(), _actor), actors.end());
	}

	void RegisterUIElement(UIElement* _uiElement)
	{
		uiElements.push_back(_uiElement);
	}

	void UnregisterUIElement(UIElement* _uiElement)
	{
		uiElements.erase(std::remove(uiElements.begin(), uiElements.end(), _uiElement), uiElements.end());
	}

	void SetUITextFont(UIText& _uiText, const std::string& _fontAtlasFilePath)
	{
		if (fontAssets.find(_fontAtlasFilePath) == fontAssets.end())
		{
			std::cerr << "Font not found in loaded font atlases" << std::endl;
			return;
		}

		FontAsset fontAsset = fontAssets[_fontAtlasFilePath];

		if (fontAsset.fontMap.size() == 0)
		{
			std::cerr << "0 characters loaded into font map" << std::endl;
			return;
		}
		
		SDL_Surface* surf = SDL_LoadBMP(_fontAtlasFilePath.c_str());
		SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
		SDL_SetTextureScaleMode(tex, SDL_ScaleMode::SDL_SCALEMODE_NEAREST);

		SDL_DestroySurface(surf);

		_uiText.SetFont(tex, fontAsset.fontMap, fontAsset.charWidth, fontAsset.charHeight);
	}

	// All font atlases must be monospaced with each character in a single horizontal line
	void LoadFontAtlas(const std::string& _fontAtlasFilePath, const std::string& _fontAtlasOrder, int _charWidth, int _charHeight, int _horizontalPadding)
	{
		int currX = 0;
		std::unordered_map<char, SDL_Rect> fontMap;

		SDL_Surface* surf = SDL_LoadBMP(_fontAtlasFilePath.c_str()); 

		for (char c : _fontAtlasOrder)
		{
			if (currX >= surf->w)
			{
				std::cerr << "Incorrect Font Atlas Formatting" << std::endl;
				return;
			}

			SDL_Rect rect = {currX, 0, _charWidth, _charHeight};
			fontMap[c] = rect;
			currX += _charWidth + _horizontalPadding;
		}

		FontAsset fontAsset;
		fontAsset.charWidth = _charWidth;
		fontAsset.charHeight = _charHeight;
		fontAsset.fontMap = fontMap;
		fontAssets[_fontAtlasFilePath] = fontAsset;
	}

	void SetActorSprite(Actor* _actor, std::string& _spriteFilePath)
	{
		_actor->LoadSprite(renderer, _spriteFilePath);
	}

	void SetActorSpriteSheet(Actor* _actor, std::string& _spriteFilePath, float _spriteWidth, float _spriteHeight)
	{
		_actor->LoadSpriteSheet(renderer, _spriteFilePath, _spriteWidth, _spriteHeight);
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

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		for (auto actor : actors)
			actor->Render(renderer);

		for (auto uiElement : uiElements)
			uiElement->Render(renderer);

		SDL_RenderPresent(renderer);
	}
};