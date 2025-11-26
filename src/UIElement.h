#define UIELEMENT_H
#include <SDL3/SDL.h>
#include <string>
#include <iostream>
#include <unordered_map>


class UIElement {
protected:
    int x, y;

public:
    //generic variables that apply to both UIImage and UIText
    UIElement(int x = 0, int y = 0) : x(x), y(y) {}
    virtual ~UIElement() {}
    virtual void Render(SDL_Renderer* ren) = 0;
    void SetPosition(int newX, int newY) { x = newX; y = newY; }

};


class UIImage : public UIElement {
private:
    SDL_Texture* texture;
    int width, height;
    float scaleX, scaleY;
    string spritePath; //added this Ethan :)

public:
    UIImage() : texture(nullptr), width(0), height(0), scaleX(1.0f), scaleY(1.0f) {}
    ~UIImage() { if (texture) SDL_DestroyTexture(texture); }
   //gets image loaded.
    void LoadImage(SDL_Renderer* ren, const std::string& filePath) {
        if (texture) SDL_DestroyTexture(texture);

        SDL_Surface* surface = SDL_LoadBMP(filePath.c_str());
        if (!surface) 
            std::cerr << "LoadImage Error: " << SDL_GetError() << std::endl;
            

        texture = SDL_CreateTextureFromSurface(ren, surface);
        width = surface->w;
        height = surface->h;
        SDL_DestroySurface(surface);
        if (!texture)
            std::cerr << "CreateTexture Error: " << SDL_GetError() << std::endl;
            
        }

    //renders image. Goes into main while loop of the game.
    void Render(SDL_Renderer* ren) override {
        if (!texture) return;
        SDL_FRect dest = {
            (float)x,
            (float)y,
            (float)(width * scaleX),
            (float)(height * scaleY)
        };
        if (!SDL_RenderTexture(ren, texture, nullptr, &dest)) {
            std::cerr << "SDL_RenderTexture Error: " << SDL_GetError() << std::endl;
        }
    }
    //this allows us to change size. Originally was supposed to be 2 functions but is more efficient this way
    void SetScale(float sx, float sy) { scaleX = sx; scaleY = sy; }

};







class UIText : public UIElement {
private:
    SDL_Texture* texture;
    std::string text;
    int charWidth, charHeight;
    std::unordered_map<char, SDL_Rect> charMap;
    
public:
    UIText(SDL_Texture* sheet, int charW, int charH)
        : texture(sheet), charWidth(charW), charHeight(charH) {
    }

    void SetText(const std::string& t) { text = t; }
    //this setcharmap will probably be resolved in the manager since I have some test code I was using which we
    //can use for setting the font. Good for modularity, anyways.
    void SetCharMap(const std::unordered_map<char, SDL_Rect>& map) {
        charMap = map;
    }
    //renders. This would go into the main while loop for the game.
    void Render(SDL_Renderer* ren) override {
        if (!texture) return;

        int cursorX = x;
        int cursorY = y;
        //parses text to render, also it skips any invalid characters so that it can continue to move
        //in case anyone tries to type cyrillic (could work w/ font, but I didn't want to put in the effort).
        for (char c : text) {
            auto it = charMap.find(c);
            if (it == charMap.end()) {
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
                (float)charWidth,
                (float)charHeight
            };

            if (!SDL_RenderTexture(ren, texture, &src, &dest)) {
                std::cerr << "SDL_RenderTexture Error: " << SDL_GetError() << std::endl;
            }
            cursorX += charWidth;
        }
    }

};

