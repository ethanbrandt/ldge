#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class TileRenderer {
private:
    //this helps split the tileset up and does not need to be accessed by anyone but me.
    SDL_Surface* CropSurface(SDL_Surface* source, SDL_Rect cropRect)
	{
        if (!source) {
            std::cerr << "CropSurface Error: source is null" << std::endl;
            return nullptr;
        }

        SDL_Surface* cropped = SDL_CreateSurface(cropRect.w, cropRect.h, source->format);
        if (!cropped) {
            std::cerr << "SDL_CreateSurface Error: " << SDL_GetError() << std::endl;
            return nullptr;
        }

        if (SDL_BlitSurface(source, &cropRect, cropped, nullptr) < 0) {
            std::cerr << "SDL_BlitSurface Error: " << SDL_GetError() << std::endl;
            SDL_DestroySurface(cropped);
            return nullptr;
        }

        return cropped;
    }

    std::vector<SDL_Texture*> tiles;
    std::vector<std::vector<int>> tileMap;

public:
    TileRenderer()
    {

    }

    ~TileRenderer()
	{
        for (auto tex : tiles)
            SDL_DestroyTexture(tex);

        tiles.clear();
        tileMap.clear();
    }

    //this splits up the images put through into many tiles
    void CreateTileTextures(SDL_Renderer* _renderer, const std::string& _tileMapPath, int _tileSize)
	{
		SDL_Surface* source = SDL_LoadBMP(_tileMapPath.c_str());
		
		if (!source)
		{
			std::cerr << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
			SDL_DestroySurface(source);
			SDL_Quit();
		}

        tiles.clear();

        if (!_renderer || _tileSize <= 0)
			return;

        for (int y = 0; y < source->h; y += _tileSize)
		{
            for (int x = 0; x < source->w; x += _tileSize)
			{
                SDL_Rect rect = { x, y, _tileSize, _tileSize };
                SDL_Surface* tile = CropSurface(source, rect);

                if (!tile)
					continue;

                SDL_Texture* tex = SDL_CreateTextureFromSurface(_renderer, tile);
				SDL_SetTextureScaleMode(tex, SDL_ScaleMode::SDL_SCALEMODE_NEAREST);
                SDL_DestroySurface(tile);

                if (tex)
                    tiles.push_back(tex);
                else
                    std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
            }
        }

    	SDL_DestroySurface(source);
    }

    void LoadTileMap(const std::string& _tileMapFilePath)
	{
        std::ifstream file(_tileMapFilePath);
        tileMap.clear();
        std::string line;

        if (!file.is_open())
		{
            std::cerr << "Failed to open tilemap file: " << _tileMapFilePath << std::endl;
            return;
        }

        while (std::getline(file, line))
		{
            if (line.empty() || line[0] == '#')
				continue;

            std::istringstream iss(line);
            std::vector<int> row;
            int tile;

            while (iss >> tile)
                row.push_back(tile);

            if (!row.empty())
                tileMap.push_back(row);
        }
    }

    void Render(SDL_Renderer* ren, int windowW, int windowH)
	{
        if (tileMap.empty() || tiles.empty())
			return;

        int mapRows = (int)tileMap.size();
        int mapCols = (int)tileMap[0].size();

        int tileDisplayW = windowW / mapCols;
        int tileDisplayH = windowH / mapRows;

        for (int y = 0; y < mapRows; ++y)
		{
            for (int x = 0; x < mapCols; ++x)
			{
                int tileIndex = tileMap[y][x];
                if (tileIndex < 0 || tileIndex >= (int)tiles.size())
					continue;

                SDL_FRect dest = {
                    (float)(x * tileDisplayW),
                    (float)(y * tileDisplayH),
                    (float)tileDisplayW,
                    (float)tileDisplayH
                };

                SDL_RenderTexture(ren, tiles[tileIndex], nullptr, &dest);
            }
        }
    }
};

