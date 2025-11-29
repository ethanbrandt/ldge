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
	int onTriggerRef = LUA_NOREF;
	int onCollisionRef = LUA_NOREF;
	int onDestroyRef = LUA_NOREF;
};

struct EntityRecord
{
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

	static ScriptManager* instance;

	void BindAPI();
	void Load(EntityRecord* _record);

	// Log Interfaces
	static int l_Log(lua_State* _L);
	static int l_LogError(lua_State* _L);

	// FileHandler Interfaces
	static int l_LoadAudioFromFile(lua_State* _L);
	static int l_LoadFontFromFile(lua_State* _L);
	static int l_LoadEntityFromFile(lua_State* _L);

	// RigidBody Interfaces	
	static int l_SetPosition(lua_State* _L);
	static int l_GetPosition(lua_State* _L);
	static int l_SetVelocity(lua_State* _L);
	static int l_GetVelocity(lua_State* _L);
	static int l_IsTrigger(lua_State* _L); //TODO
	static int l_IsStatic(lua_State* _L); //TODO

	// Actor Interfaces
	static int l_SetSprite(lua_State* _L);
	static int l_SetSpriteSheet(lua_State* _L);
	static int l_SetSpriteIndex(lua_State* _L);
	static int l_SetScale(lua_State* _L);

	// Audio Interfaces
	static int l_PlaySound(lua_State* _L);
	static int l_SetVolume(lua_State* _L);
	static int l_PauseAllAudio(lua_State* _L);
	static int l_ResumeAllAudio(lua_State* _L);

	// Input Interfaces
	static int l_IsPressed(lua_State* _L);
	static int l_WasPressedDown(lua_State* _L);
	static int l_WasReleased(lua_State* _L);

public:
	ScriptManager();
	~ScriptManager();

	static ScriptManager* GetInstance();

	EntityId CreateEntityRecord(std::string _scriptFilePath, Actor* _actor, RigidBody* _rigidBody);
	EntityRecord GetEntityRecord(EntityId _e);
	void DestroyEntityRecord(EntityId _e);

	void Start(EntityId _e);
	void Update(float _deltaTime, int _screenWidth, int _screenHeight);
	void OnTrigger(EntityId _e, RigidBody& _other);
	void OnCollision(EntityId _e, RigidBody& _other);
	void OnDestroy(EntityId _e);
};