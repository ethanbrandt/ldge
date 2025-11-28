#pragma once
#include <string>
#include <filesystem>
#include <unordered_map>
#include <lua.hpp>

#include "../utilities/Vector2.h"
#include "../rendering/Actor.h"
#include "../physics/Rigidbody.h"

using EntityId = uint32_t;

struct Script
{
	std::string filePath;
	std::filesystem::file_time_type lastWrite;
	int onStartRef = LUA_NOREF;
	int onUpdateRef = LUA_NOREF;
};

struct EntityRecord
{
	Vector2 pos;
	Script script;
	Actor* actor;
	RigidBody* rigidBody;
};

class ScriptManager
{
private:
	lua_State* L;
	std::unordered_map<EntityId, EntityRecord> records;
	EntityId nextEntityId;

public:
	ScriptManager();
	~ScriptManager();

	EntityId CreateEntityRecord(Vector2 _pos, std::string _scriptFilePath, Actor* _actor, RigidBody* _rigidBody);
	EntityRecord GetEntityRecord(EntityId _entityId);

	void Start();
	void Update(float _deltaTime);
};