#pragma once
#include "Vector2.h"
#include <cmath>

Vector2 GameToScreenPosition(int _windowWidth, int _windowHeight, Vector2 _gamePosition)
{
    Vector2 screenToCenter(floorf(_windowWidth/2), floorf(_windowHeight/2));

    Vector2 flippedGameToScreen(_gamePosition.GetX(), (-1 * _gamePosition.GetY()));

    Vector2 gameToScreen = screenToCenter + flippedGameToScreen;

    return gameToScreen;
}