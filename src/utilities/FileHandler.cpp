#include <string>
#include <fstream>
#include <vector>

#include "../../include/json.hpp" // nlohmann/json library
using json = nlohmann::json;

#include "../rendering/RenderManager.h"
#include "../audio/AudioManager.h"
#include "../gorm/ScriptManager.h"
#include "../physics/RigidBody.h"
#include "../physics/CollisionShape.h"
#include "../physics/CollisionCircle.h"
#include "../physics/CollisionRectangle.h"
#include "Vector2.h"

#include "FileHandler.h"

FileHandler::FileHandler(RenderManager* _renderManager, AudioManager* _audioManager, ScriptManager* _scriptManager)
{
	renderManager = _renderManager;
	audioManager = _audioManager;
	scriptManager = _scriptManager;
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


EntityId FileHandler::LoadEntityFromFile(std::string _filePath)
{
	std::ifstream f(_filePath);
	if (!f)
	{
		std::cerr << "Error Loading File: " << _filePath << std::endl;
		return -1;
	}

	json entityData = json::parse(f);

	std::string scriptFilePath;

	bool errorFlag = false;

	std::string currentKey = "scriptFilePath";
	if (entityData.contains(currentKey))
		scriptFilePath = entityData[currentKey].get<std::string>();
	else
	{
		std::cerr << _filePath << " does not contain key: " << currentKey << std::endl;
		errorFlag = true;
	}

	RigidBody* rb = LoadRigidbody(entityData, _filePath, errorFlag);
	Actor* actor = LoadActor(entityData, _filePath, errorFlag);

	if (errorFlag)
		return -1;

	return scriptManager->CreateEntityRecord(Vector2(0,0), scriptFilePath, actor, rb);
}

RigidBody* FileHandler::LoadRigidbody(json entityData, std::string& filePath, bool& errorFlag)
{
	std::string currentKey = "rigidBody";
	float mass = 1;
	CollisionShape* colShape;	
	
	if (entityData.contains(currentKey))
	{
		if (entityData[currentKey].contains("mass"))
			mass = entityData[currentKey]["mass"].get<float>();

		Vector2 offset(0, 0);
		bool isTrigger = false;
		uint16_t colMask = 0;

		if (entityData[currentKey].contains("collisionCircle"))
		{
			float radius;
			if (entityData[currentKey]["collisionCircle"].contains("radius"))
				radius = entityData[currentKey]["collisionCircle"]["radius"].get<float>();
			else
			{
				std::cerr << filePath << " collisionCircle must have radius" << std::endl;
				errorFlag = true;
			}

			std::vector<float> offsetVector;
			if (entityData[currentKey]["collisionCircle"].contains("offset"))
			{
				offsetVector = entityData[currentKey]["collisionCircle"]["offset"].get<std::vector<float>>();
				if (offsetVector.size() >= 2)
					offset = Vector2(offsetVector[0], offsetVector[1]);
			}

			if (entityData[currentKey]["collisionCircle"].contains("isTrigger"))
				isTrigger = entityData[currentKey]["collisionCircle"]["isTrigger"].get<bool>();

			if (entityData[currentKey]["collisionCircle"].contains("colMask"))
				colMask = entityData[currentKey]["collisionCircle"]["colMask"].get<int>();

			if (!errorFlag)
				colShape = new CollisionCircle(radius, offset, isTrigger, colMask);	
		}
		else if (entityData[currentKey].contains("collisionRectangle"))
		{
			float width, height;
			if (entityData[currentKey]["collisionRectangle"].contains("width"))
				width = entityData[currentKey]["collisionRectangle"]["width"].get<float>();
			else
			{
				std::cerr << filePath << " collisionRectangle must have width" << std::endl;
				errorFlag = true;
			}

			if (entityData[currentKey]["collisionRectangle"].contains("height"))
				width = entityData[currentKey]["collisionRectangle"]["height"].get<float>();
			else
			{
				std::cerr << filePath << " collisionRectangle must have height" << std::endl;
				errorFlag = true;
			}

			std::vector<float> offsetVector;
			if (entityData[currentKey]["collisionRectangle"].contains("offset"))
			{
				offsetVector = entityData[currentKey]["collisionRectangle"]["offset"].get<std::vector<float>>();
				if (offsetVector.size() >= 2)
					offset = Vector2(offsetVector[0], offsetVector[1]);
			}

			if (entityData[currentKey]["collisionRectangle"].contains("isTrigger"))
				isTrigger = entityData[currentKey]["collisionRectangle"]["isTrigger"].get<bool>();

			if (entityData[currentKey]["collisionRectangle"].contains("colMask"))
				colMask = entityData[currentKey]["collisionRectangle"]["colMask"].get<int>();

			if (!errorFlag)
				colShape = new CollisionRectangle(width, height, offset, isTrigger, colMask);	
		}
		else
		{
			std::cerr << filePath << " does not contain key: " << currentKey << " : collisionCircle or collisionRectangle" << std::endl;
			errorFlag = true;
		}
	}
	else
	{
		std::cerr << filePath << " does not contain key: " << currentKey << std::endl;
		errorFlag = true;
	}

	return new RigidBody(Vector2(0,0), Vector2(0,0), mass, colShape);
}

Actor* FileHandler::LoadActor(json entityData, std::string& filePath, bool& errorFlag)
{
	Actor* actor = new Actor();

	if (entityData.contains("sprite"))
	{
		std::string spriteFilePath;
		if (entityData["sprite"].contains("spriteFilePath"))
			spriteFilePath = entityData["sprite"]["spriteFilePath"].get<std::string>();
		else
		{
			std::cerr << filePath << " sprite must have spriteFilePath" << std::endl;
			errorFlag = true;
		}

		std::vector<float> spriteScale = {1.0f, 1.0f};
		if (entityData["sprite"].contains("scale"))
		{
			std::vector<float> temp = entityData["sprite"]["scale"].get<std::vector<float>>();
			if (temp.size() >= 2)
				spriteScale = temp;
		}

		if (!errorFlag)
		{
			renderManager->SetActorSprite(actor, spriteFilePath);
			actor->SetScale(spriteScale[0], spriteScale[1]);
		}
	}	
	else if (entityData.contains("spriteSheet"))
	{
		std::string spriteFilePath;
		if (entityData["spriteSheet"].contains("spriteFilePath"))
			spriteFilePath = entityData["spriteSheet"]["spriteFilePath"].get<std::string>();
		else
		{
			std::cerr << filePath << " spriteSheet must have spriteFilePath" << std::endl;
			errorFlag = true;
		}

		float spriteWidth, spriteHeight;
		if (entityData["spriteSheet"].contains("spriteWidth"))
			spriteWidth = entityData["spriteSheet"]["spriteWidth"].get<float>();
		else
		{
			std::cerr << filePath << " spriteSheet must have spriteWidth" << std::endl;
			errorFlag = true;
		}

		if (entityData["spriteSheet"].contains("spriteHeight"))
			spriteHeight = entityData["spriteSheet"]["spriteHeight"].get<float>();
		else
		{
			std::cerr << filePath << " spriteSheet must have spriteHeight" << std::endl;
			errorFlag = true;
		}

		std::vector<float> spriteScale = {1.0f, 1.0f};
		if (entityData["spriteSheet"].contains("scale"))
		{
			std::vector<float> temp = entityData["spriteSheet"]["scale"].get<std::vector<float>>();
			if (temp.size() >= 2)
				spriteScale = temp;
		}

		if (!errorFlag)
		{
			renderManager->SetActorSpriteSheet(actor, spriteFilePath, spriteWidth, spriteHeight);
			actor->SetScale(spriteScale[0], spriteScale[1]);
		}
	}
	else
	{
		std::cerr << filePath << " does not contain key: sprite or spritesheet" << std::endl;
		errorFlag = true;
	}

	return actor;
}