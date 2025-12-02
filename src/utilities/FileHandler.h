#pragma once

#include <iostream>
#include <string>

#include "../../include/json.hpp" // nlohmann/json library
using json = nlohmann::json;

#include "../rendering/RenderManager.h"
#include "../audio/AudioManager.h"
#include "../gorm/ScriptManager.h"
#include "Vector2.h"

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
	static FileHandler* instance;

	RigidBody* LoadRigidbody(json _entityData, std::string& _filePath, bool& _errorFlag);
	Actor* LoadActor(json _entityData, std::string& _filePath, bool& _errorFlag);

public:
	FileHandler();

	void LoadAudioFromFile(std::string _filePath);
	void LoadFontFromFile(std::string _filePath);
	EntityId LoadEntityFromFile(std::string _filePath, Vector2 _startPos);
	void LoadSceneFromFile(std::string _filePath);

	void LoadUILayoutFromFile(std::string _filePath); // TODO: WILL BE IMPLEMENTED WHEN FULL SYSTEM IS FURTHER IMPLEMENTED

	static FileHandler* GetInstance();
};