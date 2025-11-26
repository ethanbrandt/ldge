#include"GTS.h"
#include<cmath>

Vector2 GTS(int screenX, int screenY, int tileSizeX, int tileSizeY, Vector2 gameCoords)
{
    Vector2 screenToCenter(floorf(screenX/2), floorf(screenY/2));

    Vector2 flippedGameToScreen(tileSizeX * gameCoords.getX(), (tileSizeY * -1 * gameCoords.getY()));

    Vector2 GameToScreen = screenToCenter + flippedGameToScreen;

    return GameToScreen;
}