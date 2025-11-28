#include <string>
#include <filesystem>
#include <unordered_map>
#include <lua.hpp>

#include "../utilities/Vector2.h"
#include "../rendering/Actor.h"
#include "../physics/Rigidbody.h"

#include "ScriptManager.h"

ScriptManager::ScriptManager()
{
	nextEntityId = 0;
}

ScriptManager::~ScriptManager()
{

}

EntityId ScriptManager::CreateEntityRecord(Vector2 _pos, std::string _scriptFilePath, Actor* _actor, RigidBody* _rigidBody)
{
	EntityId entity = nextEntityId++;

	EntityRecord record;

	record.pos = _pos;

	Script script;
	script.filePath = _scriptFilePath;
	record.script = script;

	record.actor = _actor;

	record.rigidBody = _rigidBody;

	records[entity] = record;

	std::cout << "Created Entity Record" << std::endl;

	return entity;
}

EntityRecord ScriptManager::GetEntityRecord(EntityId _entityId)
{
	return records[_entityId];
}