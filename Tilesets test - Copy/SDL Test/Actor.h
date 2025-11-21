#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <iostream>
//mostly done, but I will need to return to this to add in gamecoordinate conversion
class Actor {
public:
    Actor()
        : texture(nullptr)
    //assigns area derived from spritesheet
    {
        destRect = { 0, 0, 32, 32 };
        srcRect = { 0, 0, 32, 32 };
    }

    ~Actor() {
        if (texture)
            SDL_DestroyTexture(texture);
    }

    bool Load(SDL_Renderer* renderer,
        const std::string& spritesheetPath,
        const SDL_Rect& src)
    {
        srcRect.x = (float)src.x;
        srcRect.y = (float)src.y;
        srcRect.w = (float)src.w;
        srcRect.h = (float)src.h;

        SDL_Surface* surf = SDL_LoadBMP(spritesheetPath.c_str());
        if (!surf) {
            std::cerr << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
            return false;
        }

        texture = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_DestroySurface(surf);

        if (!texture) {
            std::cerr << "SDL_CreateTextureFromSurface Error: "
                << SDL_GetError() << std::endl;
            return false;
        }
        destRect.w = srcRect.w;
        destRect.h = srcRect.h;

        return true;
    }
    //combined setx and sety into one function
    void SetPosition(float x, float y) {
        destRect.x = x;
        destRect.y = y;
    }
    //this is not necessary unless we want to do additional enemy types like bosses. Still easy to have around and allows modularity
    void SetSize(float w, float h) {
        destRect.w = w;
        destRect.h = h;
    }

    void SetSourceRect(const SDL_Rect& src) {
        srcRect.x = (float)src.x;
        srcRect.y = (float)src.y;
        srcRect.w = (float)src.w;
        srcRect.h = (float)src.h;
    }

    void Render(SDL_Renderer* renderer) {
        if (!texture) return;

        SDL_RenderTexture(renderer, texture, &srcRect, &destRect);
    }

private:
    SDL_Texture* texture;
    SDL_FRect destRect;
    SDL_FRect srcRect;
};
