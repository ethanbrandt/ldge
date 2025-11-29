#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <iostream>

class Actor
{
private:
    SDL_Texture* texture;
    SDL_FRect destRect;
    SDL_FRect srcRect;
	float xScale;
	float yScale;
	bool hasSpriteSheet = false;

	void ApplyScale()
	{
		destRect.w = srcRect.w * xScale;
		destRect.h = srcRect.h * yScale;
	}

public:
    Actor() : texture(nullptr)
    {
        destRect = { 0, 0, 32, 32 };
        srcRect = { 0, 0, 32, 32 };
    }

    ~Actor()
	{
        if (texture)
            SDL_DestroyTexture(texture);
    }

    void LoadSpriteSheet(SDL_Renderer* _renderer, const std::string& _spritesheetPath, float _spriteWidth, float _spriteHeight)
    {
        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = _spriteWidth;
        srcRect.h = _spriteHeight;

        SDL_Surface* surf = SDL_LoadBMP(_spritesheetPath.c_str());

        if (!surf)
            std::cerr << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;

        texture = SDL_CreateTextureFromSurface(_renderer, surf);
		SDL_SetTextureScaleMode(texture, SDL_ScaleMode::SDL_SCALEMODE_NEAREST);
        SDL_DestroySurface(surf);

        if (!texture)
            std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        
		destRect.w = srcRect.w;
        destRect.h = srcRect.h;
		hasSpriteSheet = true;
    }

	void LoadSprite(SDL_Renderer* _renderer, const std::string& _spritePath)
	{
		SDL_Surface* surf = SDL_LoadBMP(_spritePath.c_str());

		if (!surf)
			std::cerr << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;

		texture = SDL_CreateTextureFromSurface(_renderer, surf);
		SDL_SetTextureScaleMode(texture, SDL_ScaleMode::SDL_SCALEMODE_NEAREST);

		if (!texture)
			std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		
		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = (float)surf->w;
		srcRect.h = (float)surf->h;

		SDL_DestroySurface(surf);
		
		ApplyScale();

		hasSpriteSheet = false;
	}

    void SetScreenPosition(float x, float y)
	{
        destRect.x = x;
        destRect.y = y;
    }

    void SetScale(float _xScale, float _yScale)
	{
		xScale = _xScale;
		yScale = _yScale;

		ApplyScale();
	}

	void SetScreenSize(float _width, float _height)
	{
		destRect.w = _width;
		destRect.h = _height;
	}

    void SetSpriteIndex(int _indexX, int _indexY)
	{
		if (!hasSpriteSheet)
		{
			std::cerr << "Attempted to set sprite index on an Actor without a spritesheet" << std::endl;
			return;
		}
		
		if (_indexX < 0 || _indexY < 0)
		{
			std::cerr << "Sprite index cannot be negative" << std::endl;
			return;
		}

		if (srcRect.w * _indexX >= texture->w || srcRect.h * _indexY >= texture->h)
		{
			std::cerr << "Sprite index out of bounds" << std::endl;
			return;
		}

        srcRect.x = srcRect.w * _indexX;
        srcRect.y = srcRect.h * _indexY;
    }

    void Render(SDL_Renderer* _renderer)
	{
        if (!texture)
			return;

        SDL_RenderTexture(_renderer, texture, &srcRect, &destRect);
    }
};
