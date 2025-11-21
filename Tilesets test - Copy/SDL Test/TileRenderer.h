#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class TileRenderer {
public:
    //splits the window into 16x14, although we will not be utilizing the top 3 tiles to leave room for the ui
    TileRenderer()
        : cols(16), rows(14) {
    }

    ~TileRenderer() {
        Cleanup();
    }

    //this splits up the images put through into many tiles
    void CreateTileTextures(SDL_Renderer* ren, SDL_Surface* source) {
        tiles.clear();

        if (!source || !ren) return;

        int tileW = source->w / cols;
        int tileH = source->h / rows;

        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                SDL_Rect rect = { x * tileW, y * tileH, tileW, tileH };
                SDL_Surface* tile = CropSurface(source, rect);
                if (!tile) continue;

                SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, tile);
                SDL_DestroySurface(tile);

                if (tex)
                    tiles.push_back(tex);
                else
                    std::cerr << "SDL_CreateTextureFromSurface Error: "
                    << SDL_GetError() << std::endl;
            }
        }
    }
    //this opens the tilemap txt file
    void LoadTileMap(const std::string& filename) {
        std::ifstream file(filename);
        tileMap.clear();
        std::string line;

        if (!file.is_open()) {
            std::cerr << "Failed to open tilemap file: " << filename << std::endl;
            return;
        }

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;

            std::istringstream iss(line);
            std::vector<int> row;
            int tile;

            while (iss >> tile)
                row.push_back(tile);

            if (!row.empty())
                tileMap.push_back(row);
        }
    }
// this directly renders the tiles
    void Render(SDL_Renderer* ren, int windowW, int windowH) {
        if (tileMap.empty() || tiles.empty()) return;

        int mapRows = (int)tileMap.size();
        int mapCols = (int)tileMap[0].size();

        int tileDisplayW = windowW / mapCols;
        int tileDisplayH = windowH / mapRows;

        for (int y = 0; y < mapRows; ++y) {
            for (int x = 0; x < mapCols; ++x) {
                int tileIndex = tileMap[y][x];
                if (tileIndex < 0 || tileIndex >= (int)tiles.size()) continue;

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
    //this gets rid of the tile texture, which is necessary for ending the program.
    void Cleanup() {
        for (auto tex : tiles)
            SDL_DestroyTexture(tex);

        tiles.clear();
        tileMap.clear();
    }

private:
    //this helps split the tileset up and does not need to be accessed by anyone but me.
    SDL_Surface* CropSurface(SDL_Surface* source, SDL_Rect cropRect) {
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
    int cols;
    int rows;
};

