#include <SDL3/SDL.h>
#include <iostream>
#include <vector>
#include <string>
#include "Actor.h"
#include "TileRenderer.h"
#include "UIElement.h"
using namespace std;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL_Init Error: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Tiles", 960, 700, 0);
    if (!window) {
        cerr << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    //load up tileset
    SDL_Surface* source = SDL_LoadBMP("lettuce.bmp");
    if (!source) {
        cerr << "SDL_LoadBMP Error: " << SDL_GetError() << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    //load up the images and map layout we will use.
    TileRenderer tileRenderer;
    tileRenderer.CreateTileTextures(renderer, source);
    tileRenderer.LoadTileMap("map.txt");
    Actor player;
    SDL_Rect frame = { 0, 0, 32, 32 };
    player.Load(renderer, "sshot0002.bmp", frame);
    
    UIImage uiImage;
    if (!uiImage.LoadImage(renderer, "lettuce.bmp")) {
        std::cerr << "failed to load lettuce.bmp" << std::endl;
    }
    uiImage.SetPosition(50, 50);
    uiImage.SetScale(2.0f, 2.0f);
    SDL_Surface* fontSurface = SDL_LoadBMP("kobold-gold-v1.bmp");
    if (!fontSurface) {
        std::cerr << "Failed to load kobold-gold-v1.bmp" << std::endl;
    }
    SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);

    UIText uiText(fontTexture, 16, 16);
    int charWidth = 8;
    int charHeight = 16;

    std::unordered_map<char, SDL_Rect> fontMap;

    for (int i = 0; i < 26; ++i) {
        fontMap['A' + i] = { i * charWidth, 0, charWidth, charHeight };
    }
    for (int i = 0; i < 26; ++i) {
        fontMap['a' + i] = { i * charWidth, charHeight, charWidth, charHeight };
    }
    for (int i = 0; i < 10; ++i) {
        fontMap['0' + i] = { i * charWidth, 2 * charHeight, charWidth, charHeight };
    }
    fontMap[' '] = { 25 * charWidth, 9 * charHeight, charWidth, charHeight };
    fontMap['.'] = { 11 * charWidth, 2 * charHeight, charWidth, charHeight };
    fontMap[','] = { 12 * charWidth, 2 * charHeight, charWidth, charHeight };
    fontMap[';'] = { 13 * charWidth, 2 * charHeight, charWidth, charHeight };
    fontMap[':'] = { 14 * charWidth, 2 * charHeight, charWidth, charHeight };
    fontMap['?'] = { 15 * charWidth, 2 * charHeight, charWidth, charHeight };
    fontMap['!'] = { 16 * charWidth, 2 * charHeight, charWidth, charHeight };
    fontMap['-'] = { 17 * charWidth, 2 * charHeight, charWidth, charHeight };
    fontMap['_'] = { 18 * charWidth, 2 * charHeight, charWidth, charHeight };
    fontMap['~'] = { 19 * charWidth, 2 * charHeight, charWidth, charHeight };
    fontMap['#'] = { 20 * charWidth, 2 * charHeight, charWidth, charHeight };
    fontMap['\''] = { 21 * charWidth, 2 * charHeight, charWidth, charHeight };
    fontMap['&'] = { 22 * charWidth, 2 * charHeight, charWidth, charHeight };
    fontMap['('] = { 23 * charWidth, 2 * charHeight, charWidth, charHeight };
    fontMap[')'] = { 24 * charWidth, 2 * charHeight, charWidth, charHeight };
    fontMap['['] = { 25 * charWidth, 2 * charHeight, charWidth, charHeight };
    fontMap[']'] = { 1 * charWidth, 3 * charHeight, charWidth, charHeight };
    fontMap['{'] = { 2 * charWidth, 3 * charHeight, charWidth, charHeight };
    fontMap['}'] = { 3 * charWidth, 3 * charHeight, charWidth, charHeight };
    fontMap['^'] = { 4 * charWidth, 3 * charHeight, charWidth, charHeight };
    fontMap['|'] = { 5 * charWidth, 3 * charHeight, charWidth, charHeight };
    fontMap['\''] = { 6 * charWidth, 3 * charHeight, charWidth, charHeight };
    fontMap['\\'] = { 7 * charWidth, 3 * charHeight, charWidth, charHeight };
    fontMap['/'] = { 8 * charWidth, 3 * charHeight, charWidth, charHeight };
    fontMap['@'] = { 9 * charWidth, 3 * charHeight, charWidth, charHeight };
    fontMap['*'] = { 10 * charWidth, 3 * charHeight, charWidth, charHeight };
    fontMap['+'] = { 11 * charWidth, 3 * charHeight, charWidth, charHeight };
    fontMap['='] = { 12 * charWidth, 3 * charHeight, charWidth, charHeight };
    fontMap['%'] = { 13 * charWidth, 3 * charHeight, charWidth, charHeight };
    fontMap['€'] = { 14 * charWidth, 3 * charHeight, charWidth, charHeight };
    fontMap['$'] = { 15 * charWidth, 3 * charHeight, charWidth, charHeight };
    fontMap['£'] = { 16 * charWidth, 3 * charHeight, charWidth, charHeight };
    fontMap['¢'] = { 17 * charWidth, 3 * charHeight, charWidth, charHeight };
    fontMap['<'] = { 18 * charWidth, 3 * charHeight, charWidth, charHeight };
    fontMap['>'] = { 19 * charWidth, 3 * charHeight, charWidth, charHeight };
    fontMap['©'] = { 20 * charWidth, 3 * charHeight, charWidth, charHeight };
    fontMap['®'] = { 21 * charWidth, 3 * charHeight, charWidth, charHeight };

    uiText.SetCharMap(fontMap);
    uiText.SetText("[asdfghjklQWERTYUIOP<><><><><><><>]][][][][][]..............$$$$$$$$");
    uiText.SetPosition(50, 100);
   

    float playerx = 100, playery = 150;
    player.SetPosition(playerx, playery);
    player.SetSize(64, 64);
   

    SDL_DestroySurface(source);
    //the following is my test game loop. We can change variables/move things around as necessary but
    //we need this to be sure it works.
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT)
                quit = true;
        }
        SDL_RenderClear(renderer);
        tileRenderer.Render(renderer, 960, 700);
        player.Render(renderer);
        uiImage.Render(renderer);
        uiText.Render(renderer);

        SDL_RenderPresent(renderer);
        player.SetPosition(playerx, playery);
        player.unRegister();
    }
    tileRenderer.Cleanup();
    SDL_RenderClear(renderer);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
