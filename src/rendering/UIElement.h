#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <iostream>
#include <unordered_map>

class UIElement
{
protected:
    int x, y;
	float xScale, yScale;

public:
    UIElement(int _x = 0, int _y = 0)
	{
		x = _x; 
		y = _y;
		xScale = 1.0f;
		yScale = 1.0f;
	}

    virtual ~UIElement() {}

	void SetScreenPosition(int _x, int _y)
	{
		x = _x; 
		y = _y;
	}

	void SetScale(float _xScale, float _yScale)
	{
		xScale = _xScale;
		yScale = _yScale;
	}

    virtual void Render(SDL_Renderer* ren) = 0;
};

class UIImage : public UIElement
{
private:
    SDL_Texture* texture;
    int width, height;
    std::string spritePath;

public:
    UIImage()
	{
		texture = nullptr;
		width = 0;
		height = 0;
	}

    ~UIImage()
	{
		if (texture)
			SDL_DestroyTexture(texture);
	}

    void LoadUIImage(SDL_Renderer* ren, const std::string& filePath)
	{
        if (texture)
			SDL_DestroyTexture(texture);

        SDL_Surface* surface = SDL_LoadBMP(filePath.c_str());
        if (!surface) 
            std::cerr << "LoadImage Error: " << SDL_GetError() << std::endl;

        texture = SDL_CreateTextureFromSurface(ren, surface);
		SDL_SetTextureScaleMode(texture, SDL_ScaleMode::SDL_SCALEMODE_NEAREST);
        width = surface->w;
        height = surface->h;
        SDL_DestroySurface(surface);
        if (!texture)
            std::cerr << "CreateTexture Error: " << SDL_GetError() << std::endl;
    }

    void Render(SDL_Renderer* ren) override
	{
        if (!texture)
			return;
		
        SDL_FRect dest = {
            (float)x,
            (float)y,
            (float)(width * xScale),
            (float)(height * yScale)
        };

        if (!SDL_RenderTexture(ren, texture, nullptr, &dest))
            std::cerr << "SDL_RenderTexture Error: " << SDL_GetError() << std::endl;
    }
};

class UIText : public UIElement
{
private:
    SDL_Texture* texture;
    std::string text;
    int charWidth, charHeight;
    std::unordered_map<char, SDL_Rect> charMap;
    
public:
    void SetText(const std::string& _text)
	{
		text = _text;
	}

    void SetFont(SDL_Texture* _sheet, const std::unordered_map<char, SDL_Rect>& _map, int _charWidth, int _charHeight)
	{
		charWidth = _charWidth;
		charHeight = _charHeight;
		texture = _sheet;
        charMap = _map;
    }

    void Render(SDL_Renderer* ren) override
	{
        if (!texture)
			return;

        int cursorX = x;
        int cursorY = y;

        for (char c : text)
		{
			if (c == ' ')
			{
				cursorX += charWidth;
				continue;
			}

            auto it = charMap.find(c);
            if (it == charMap.end())
			{
				std::cerr << "Char not in font atlas: " << c << std::endl;
                cursorX += charWidth;
                continue;
            }

            SDL_FRect src = {
                (float)it->second.x,
                (float)it->second.y,
                (float)it->second.w,
                (float)it->second.h
            };

            SDL_FRect dest = {
                (float)cursorX,
                (float)cursorY,
                (float)charWidth * xScale,
                (float)charHeight * yScale
            };

            if (!SDL_RenderTexture(ren, texture, &src, &dest))
                std::cerr << "SDL_RenderTexture Error: " << SDL_GetError() << std::endl;
            
			cursorX += int((float)charWidth * xScale);
        }
    }
};
