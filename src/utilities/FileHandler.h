#pragma once

#include <iostream>
#include <string>

#include "../rendering/RenderManager.h"
#include "../audio/AudioManager.h"

struct AudioFile
{
	std::string filePath;
	float startVolume;
	bool isLooping;
};

struct FontFile
{
	std::string filePath;
	std::string fontAtlasOrder;
	int charWidth;
	int charHeight;
	int horizontalPadding;
};

class FileHandler
{
private:
	RenderManager* renderManager;
	AudioManager* audioManager;

public:
	FileHandler(RenderManager* _renderManager, AudioManager* _audioManager);

	void LoadAudioFromFile(std::string _filePath);
	void LoadFontFromFile(std::string _filePath);
	
	void LoadUILayoutFromFile(std::string _filePath); // TODO: WILL BE IMPLEMENTED WHEN FULL SYSTEM IS FURTHER IMPLEMENTED
	void LoadSceneFromFile(std::string _filePath); // TODO: WILL BE IMPLEMENTED WHEN FULL SYSTEM IS FURTHER IMPLEMENTED
	void LoadGameObjectFromFile(std::string _filePath); // TODO: WILL BE IMPLEMENTED WHEN FULL SYSTEM IS FURTHER IMPLEMENTED
};