#include <string>
#include <fstream>

#include "../../include/json.hpp" // nlohmann/json library
using json = nlohmann::json;

#include "../rendering/RenderManager.h"
#include "../audio/AudioManager.h"
#include "FileHandler.h"

FileHandler::FileHandler(RenderManager* _renderManager, AudioManager* _audioManager)
{
	renderManager = _renderManager;
	audioManager = _audioManager;
}

// Load Audio File into AudioManager
void FileHandler::LoadAudioFromFile(std::string _filePath)
{
	std::ifstream f(_filePath);
	if (!f)
	{
		std::cerr << "Error Loading File: " << _filePath << std::endl;
		return;
	}

	json audioData = json::parse(f);

	std::string audioFilePath;
	float startVolume;
	bool isLooping;

	bool errorFlag = false;

	if (audioData.contains("audioFilePath"))
		audioFilePath = audioData["audioFilePath"].get<std::string>();
	else
	{
		std::cerr << _filePath << " does not contain key: audioFilePath" << std::endl;
		errorFlag = true;
	}

	if (audioData.contains("startVolume"))
		startVolume = audioData["startVolume"].get<float>();
	else
	{
		std::cerr << _filePath << " does not contain key: startVolume" << std::endl;
		errorFlag = true;
	}

	if (audioData.contains("isLooping"))
		bool isLooping = audioData["isLooping"].get<bool>();
	else
	{
		std::cerr << _filePath << " does not contain key: isLooping" << std::endl;
		errorFlag = true;
	}

	if (errorFlag)
		return;

	if (isLooping)
		audioManager->AddSong(audioFilePath, startVolume);
	else
		audioManager->AddSound(audioFilePath, startVolume);
}

void FileHandler::LoadFontFromFile(std::string _filePath)
{
	std::ifstream f(_filePath);
	if (!f)
	{
		std::cerr << "Error Loading File: " << _filePath << std::endl;
		return;
	}

	json fontData = json::parse(f);

	std::string fontAtlasFilePath;
	std::string fontAtlasOrder;
	int charWidth;
	int charHeight;
	int horizontalPadding;
	
	bool errorFlag = false;

	if (fontData.contains("fontAtlasFilePath"))
		fontAtlasFilePath = fontData["fontAtlasFilePath"].get<std::string>();
	else
	{
		std::cerr << _filePath << " does not contain key: fontAtlasFilePath" << std::endl;
		errorFlag = true;
	}

	if (fontData.contains("fontAtlasOrder"))
		fontAtlasOrder = fontData["fontAtlasOrder"].get<std::string>();
	else
	{
		std::cerr << _filePath << " does not contain key: fontAtlasOrder" << std::endl;
		errorFlag = true;
	}

	if (fontData.contains("charWidth"))
		charWidth = fontData["charWidth"].get<int>();
	else
	{
		std::cerr << _filePath << " does not contain key: charWidth" << std::endl;
		errorFlag = true;
	}

	if (fontData.contains("charHeight"))
		charHeight = fontData["charHeight"].get<int>();
	else
	{
		std::cerr << _filePath << " does not contain key: charHeight" << std::endl;
		errorFlag = true;
	}

	if (fontData.contains("horizontalPadding"))
		horizontalPadding = fontData["horizontalPadding"].get<int>();
	else
	{
		std::cerr << _filePath << " does not contain key: horizontalPadding" << std::endl;
		errorFlag = true;
	}

	if (errorFlag)
		return;
	
	renderManager->LoadFontAtlas(fontAtlasFilePath, fontAtlasOrder, charWidth, charHeight, horizontalPadding);
}