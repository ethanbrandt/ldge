#include <string>
#include <filesystem>
#include <unordered_map>
#include <lua.hpp>

#include "../utilities/Vector2.h"
#include "../utilities/GTS.h"
#include "../utilities/FileHandler.h"
#include "../rendering/Actor.h"
#include "../rendering/RenderManager.h"
#include "../physics/Rigidbody.h"
#include "../physics/PhysicsManager.h"
#include "../input/InputManager.h"
#include "../audio/AudioManager.h"

#include "ScriptManager.h"

ScriptManager::ScriptManager()
{
	instance = this;
	nextEntityId = 0;
	L = luaL_newstate();
	luaL_openlibs(L);
	BindAPI();
}

ScriptManager::~ScriptManager()
{
	if (L)
		lua_close(L);
}

ScriptManager* ScriptManager::GetInstance()
{
	return instance;
}

void ScriptManager::Load(EntityRecord* _record)
{
	if (_record->script.onStartRef != LUA_NOREF)
	{
		luaL_unref(L, LUA_REGISTRYINDEX, _record->script.onStartRef);
		_record->script.onStartRef = LUA_NOREF;
	}
	if (_record->script.onUpdateRef != LUA_NOREF)
	{
		luaL_unref(L, LUA_REGISTRYINDEX, _record->script.onUpdateRef);
		_record->script.onUpdateRef = LUA_NOREF;
	}
	if (_record->script.onTriggerRef != LUA_NOREF)
	{
		luaL_unref(L, LUA_REGISTRYINDEX, _record->script.onTriggerRef);
		_record->script.onTriggerRef = LUA_NOREF;
	}
	if (_record->script.onCollisionRef != LUA_NOREF)
	{
		luaL_unref(L, LUA_REGISTRYINDEX, _record->script.onCollisionRef);
		_record->script.onCollisionRef = LUA_NOREF;
	}
	if (_record->script.onDestroyRef != LUA_NOREF)
	{
		luaL_unref(L, LUA_REGISTRYINDEX, _record->script.onDestroyRef);
		_record->script.onDestroyRef = LUA_NOREF;
	}

	if (luaL_dofile(L, _record->script.filePath.c_str()) != LUA_OK)
	{
		std::cerr << "Load & Run Error: " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
		return;
	}

	lua_getglobal(L, "start");
	if (lua_isfunction(L, -1))
		_record->script.onStartRef = luaL_ref(L, LUA_REGISTRYINDEX);
	else
		lua_pop(L, 1);

	lua_getglobal(L, "update");
	if (lua_isfunction(L, -1))
		_record->script.onUpdateRef = luaL_ref(L, LUA_REGISTRYINDEX);
	else
		lua_pop(L, 1);

	lua_getglobal(L, "on_trigger");
	if (lua_isfunction(L, -1))
		_record->script.onTriggerRef = luaL_ref(L, LUA_REGISTRYINDEX);
	else
		lua_pop(L, 1);

	lua_getglobal(L, "on_collision");
	if (lua_isfunction(L, -1))
		_record->script.onCollisionRef = luaL_ref(L, LUA_REGISTRYINDEX);
	else
		lua_pop(L, 1);

	lua_getglobal(L, "on_destroy");
	if (lua_isfunction(L, -1))
		_record->script.onDestroyRef = luaL_ref(L, LUA_REGISTRYINDEX);
	else
		lua_pop(L, 1);

	lua_getglobal(L, "on_message_received");
	if (lua_isfunction(L, -1))
		_record->script.onMessageReceivedRef = luaL_ref(L, LUA_REGISTRYINDEX);
	else
		lua_pop(L, 1);
}

void ScriptManager::BindAPI()
{
	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &ScriptManager::l_Log, 1);
	lua_setglobal(L, "Log");

	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &ScriptManager::l_LogError, 1);
	lua_setglobal(L, "LogError");

	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &ScriptManager::l_GetPosition, 1);
	lua_setglobal(L, "GetPosition");

	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &ScriptManager::l_SetPosition, 1);
	lua_setglobal(L, "SetPosition");

	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &ScriptManager::l_GetVelocity, 1);
	lua_setglobal(L, "GetVelocity");

	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &ScriptManager::l_SetVelocity, 1);
	lua_setglobal(L, "SetVelocity");

	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &ScriptManager::l_IsTrigger, 1);
	lua_setglobal(L, "IsTrigger");

	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &ScriptManager::l_IsStatic, 1);
	lua_setglobal(L, "IsStatic");

	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &ScriptManager::l_SetSprite, 1);
	lua_setglobal(L, "SetSprite");

	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &ScriptManager::l_SetSpriteSheet, 1);
	lua_setglobal(L, "SetSpriteSheet");

	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &ScriptManager::l_SetSpriteIndex, 1);
	lua_setglobal(L, "SetSpriteIndex");

	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &ScriptManager::l_SetScale, 1);
	lua_setglobal(L, "SetScreenScale");

	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &ScriptManager::l_IsPressed, 1);
	lua_setglobal(L, "IsPressed");

	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &ScriptManager::l_WasPressedDown, 1);
	lua_setglobal(L, "WasPressedDown");

	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &ScriptManager::l_WasReleased, 1);
	lua_setglobal(L, "WasReleased");

	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &ScriptManager::l_PlaySound, 1);
	lua_setglobal(L, "PlaySound");

	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &ScriptManager::l_SetVolume, 1);
	lua_setglobal(L, "SetVolume");

	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &ScriptManager::l_PauseAllAudio, 1);
	lua_setglobal(L, "PauseAllAudio");

	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &ScriptManager::l_ResumeAllAudio, 1);
	lua_setglobal(L, "ResumeAllAudio");

	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &ScriptManager::l_LoadAudioFromFile, 1);
	lua_setglobal(L, "LoadAudioFromFile");

	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &ScriptManager::l_LoadFontFromFile, 1);
	lua_setglobal(L, "LoadFontFromFile");

	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &ScriptManager::l_LoadEntityFromFile, 1);
	lua_setglobal(L, "LoadEntityFromFile");

	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &ScriptManager::l_DestroyEntity, 1);
	lua_setglobal(L, "DestroyEntity");

	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &ScriptManager::l_SendMessage, 1);
	lua_setglobal(L, "SendMessage");
}

#pragma region TO_LUA_INTERFACES

void ScriptManager::Start(EntityId _e)
{
	EntityRecord* r = &records[_e];
	Load(r);
	if (r->script.onStartRef != LUA_NOREF)
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, r->script.onStartRef);
		lua_pushinteger(L, (lua_Integer)_e);
		if (lua_pcall(L, 1, 0, 0) != LUA_OK)
		{
			std::cerr << "Start Error: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
}

void ScriptManager::Update(float _deltaTime, int _screenWidth, int _screenHeight)
{
	for (auto& [e, r] : records)
	{
		if (r.script.onUpdateRef == LUA_NOREF)
			continue;
		
		lua_rawgeti(L, LUA_REGISTRYINDEX, r.script.onUpdateRef);
		lua_pushinteger(L, (lua_Integer)e);
		lua_pushnumber(L, (lua_Number)_deltaTime);
		if (lua_pcall(L, 2, 0, 0) != LUA_OK)
		{
			std::cerr << "Update Error: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
		
		Vector2 screenPos = GameToScreenPosition(_screenWidth, _screenHeight, r.rigidBody->GetPosition());
		r.actor->SetScreenPosition(screenPos.GetX(), screenPos.GetY());
	}
}

void ScriptManager::OnTrigger(EntityId _e, EntityId _other)
{
	EntityRecord* r = &records[_e];
	if (r->script.onTriggerRef != LUA_NOREF)
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, r->script.onTriggerRef);
		lua_pushinteger(L, (lua_Integer)_e);
		lua_pushinteger(L, (lua_Integer)_other);
		if (lua_pcall(L, 2, 0, 0) != LUA_OK)
		{
			std::cerr << "OnTrigger Error: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
}

void ScriptManager::OnCollision(EntityId _e, EntityId _other)
{
	EntityRecord* r = &records[_e];
	if (r->script.onCollisionRef != LUA_NOREF)
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, r->script.onCollisionRef);
		lua_pushinteger(L, (lua_Integer)_e);
		lua_pushinteger(L, (lua_Integer)_other);
		if (lua_pcall(L, 2, 0, 0) != LUA_OK)
		{
			std::cerr << "OnCollision Error: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
}

void ScriptManager::OnMessageReceived(EntityId _e, EntityId _sender, const char* _message)
{
	EntityRecord* r = &records[_e];
	if (r->script.onMessageReceivedRef != LUA_NOREF)
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, r->script.onMessageReceivedRef);
		lua_pushinteger(L, (lua_Integer)_e);
		lua_pushinteger(L, (lua_Integer)_sender);
		lua_pushstring(L, _message);
		if (lua_pcall(L, 3, 0, 0) != LUA_OK)
		{
			std::cerr << "OnMessageReceived Error: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
}

void ScriptManager::OnDestroy(EntityId _e)
{
	EntityRecord* r = &records[_e];
	if (r->script.onDestroyRef != LUA_NOREF)
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, r->script.onDestroyRef);
		lua_pushinteger(L, (lua_Integer)_e);
		if (lua_pcall(L, 1, 0, 0) != LUA_OK)
		{
			std::cerr << "OnDestroy Error: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
}

#pragma endregion

#pragma region FROM_LUA_INTERFACES

// Log Interfaces
int ScriptManager::l_Log(lua_State* _L)
{
	const char* s = luaL_checkstring(_L, 1);
	std::cout << "[LUA LOG] " << s << std::endl;
	return 0;
}

int ScriptManager::l_LogError(lua_State* _L)
{
	const char* s = luaL_checkstring(_L, 1);
	std::cerr << "[LUA ERROR LOG] " << s << std::endl;
	return 0;
}

// FileHandler Interfaces
int ScriptManager::l_LoadAudioFromFile(lua_State* _L)
{
	const char* s = luaL_checkstring(_L, 1);
	FileHandler::GetInstance()->LoadAudioFromFile(s);
	return 0;
}

int ScriptManager::l_LoadFontFromFile(lua_State* _L)
{
	const char* s = luaL_checkstring(_L, 1);
	FileHandler::GetInstance()->LoadFontFromFile(s);
	return 0;
}

int ScriptManager::l_LoadEntityFromFile(lua_State* _L)
{
	const char* s = luaL_checkstring(_L, 1);
	float x = (float)luaL_checknumber(_L, 2);
	float y = (float)luaL_checknumber(_L, 3);
	EntityId id = FileHandler::GetInstance()->LoadEntityFromFile(s, Vector2(x, y));
	lua_pushinteger(_L, id);	
	return 1;
}

// RigidBody Interfaces	
int ScriptManager::l_SetPosition(lua_State* _L)
{
	EntityId id = (EntityId)luaL_checkinteger(_L, 1);
	float x = (float)luaL_checknumber(_L, 2);
	float y = (float)luaL_checknumber(_L, 3);
	
	auto i = instance->records.find(id);
	if (i != instance->records.end() && i->second.rigidBody)
	{
		i->second.rigidBody->SetPosition(Vector2(x, y));
	}
	return 0;
}

int ScriptManager::l_GetPosition(lua_State* _L)
{
	EntityId id = (EntityId)luaL_checkinteger(_L, 1);
	auto i = instance->records.find(id);
	if (i == instance->records.end())
	{
		lua_pushnil(_L);
		lua_pushnil(_L);
		return 2;
	}

	Vector2 pos = i->second.rigidBody->GetPosition();
	lua_pushnumber(_L, pos.GetX());
	lua_pushnumber(_L, pos.GetY());
	return 2;
}

int ScriptManager::l_SetVelocity(lua_State* _L)
{
	EntityId id = (EntityId)luaL_checkinteger(_L, 1);
	float x = (float)luaL_checknumber(_L, 2);
	float y = (float)luaL_checknumber(_L, 3);
	
	auto i = instance->records.find(id);
	if (i != instance->records.end() && i->second.rigidBody)
	{
		i->second.rigidBody->SetVelocity(Vector2(x, y));
	}
	return 0;
}

int ScriptManager::l_GetVelocity(lua_State* _L)
{
	EntityId id = (EntityId)luaL_checkinteger(_L, 1);
	auto i = instance->records.find(id);
	if (i == instance->records.end())
	{
		lua_pushnil(_L);
		lua_pushnil(_L);
		return 2;
	}

	Vector2 vel = i->second.rigidBody->GetVelocity();
	lua_pushnumber(_L, vel.GetX());
	lua_pushnumber(_L, vel.GetY());
	return 2;
}

int ScriptManager::l_IsTrigger(lua_State* _L)
{
	EntityId id = (EntityId)luaL_checkinteger(_L, 1);
	auto i = instance->records.find(id);
	if (i == instance->records.end())
	{
		lua_pushnil(_L);
		return 1;
	}

	bool isTrigger = i->second.rigidBody->GetColShape()->IsTrigger();
	lua_pushboolean(_L, isTrigger);
	return 1;
}

int ScriptManager::l_IsStatic(lua_State* _L)
{
	EntityId id = (EntityId)luaL_checkinteger(_L, 1);
	auto i = instance->records.find(id);
	if (i == instance->records.end())
	{
		lua_pushnil(_L);
		return 1;
	}

	bool isStatic = i->second.rigidBody->IsStatic();
	lua_pushboolean(_L, isStatic);
	return 1;
}

// Actor Interfaces
int ScriptManager::l_SetSprite(lua_State* _L)
{
	EntityId id = (EntityId)luaL_checkinteger(_L, 1);
	const char* spritePath = luaL_checkstring(_L, 2);
	
	auto i = instance->records.find(id);
	if (i != instance->records.end() && i->second.actor)
	{
		std::string s(spritePath);
		RenderManager::GetInstance()->SetActorSprite(i->second.actor, s);
	}
	return 0;
}

int ScriptManager::l_SetSpriteSheet(lua_State* _L)
{
	EntityId id = (EntityId)luaL_checkinteger(_L, 1);
	const char* spritePath = luaL_checkstring(_L, 2);
	float spriteWidth = (float)luaL_checknumber(_L, 3);
	float spriteHeight = (float)luaL_checknumber(_L, 4);
	
	auto i = instance->records.find(id);
	if (i != instance->records.end() && i->second.actor)
	{
		std::string s(spritePath);
		RenderManager::GetInstance()->SetActorSpriteSheet(i->second.actor, s, spriteWidth, spriteHeight);
	}
	return 0;
}

int ScriptManager::l_SetSpriteIndex(lua_State* _L)
{
	EntityId id = (EntityId)luaL_checkinteger(_L, 1);
	int x = luaL_checkinteger(_L, 2);
	int y = luaL_checkinteger(_L, 3);
	
	auto i = instance->records.find(id);
	if (i != instance->records.end() && i->second.actor)
	{
		i->second.actor->SetSpriteIndex(x, y);
	}
	return 0;
}

int ScriptManager::l_SetScale(lua_State* _L)
{
	EntityId id = (EntityId)luaL_checkinteger(_L, 1);
	float x = (float)luaL_checknumber(_L, 2);
	float y = (float)luaL_checknumber(_L, 3);
	
	auto i = instance->records.find(id);
	if (i != instance->records.end() && i->second.actor)
	{
		i->second.actor->SetScale(x, y);
	}
	return 0;
}

// Audio Interfaces
int ScriptManager::l_PlaySound(lua_State* _L)
{
	const char* audioPath = luaL_checkstring(_L, 1);
	AudioManager::GetInstance()->PlaySound(audioPath);
	return 0;
}

int ScriptManager::l_SetVolume(lua_State* _L)
{
	const char* audioPath = luaL_checkstring(_L, 1);
	float volume = (float)luaL_checknumber(_L, 2);
	AudioManager::GetInstance()->SetAudioVolume(audioPath, volume);
	return 0;
}

int ScriptManager::l_PauseAllAudio(lua_State* _L)
{
	AudioManager::GetInstance()->PauseAllAudio();
	return 0;
}

int ScriptManager::l_ResumeAllAudio(lua_State* _L)
{
	AudioManager::GetInstance()->ResumeAllAudio();
	return 0;
}

// Input Interfaces
int ScriptManager::l_IsPressed(lua_State* _L)
{
	const char* s = luaL_checkstring(_L, 1);
	bool isPressed = InputManager::GetInstance()->IsPressed(s[0]);
	lua_pushboolean(_L, isPressed);
	return 1;
}

int ScriptManager::l_WasPressedDown(lua_State* _L)
{
	const char* s = luaL_checkstring(_L, 1);
	bool isPressed = InputManager::GetInstance()->WasPressedDown(s[0]);
	lua_pushboolean(_L, isPressed);
	return 1;
}

int ScriptManager::l_WasReleased(lua_State* _L)
{
	const char* s = luaL_checkstring(_L, 1);
	bool isPressed = InputManager::GetInstance()->WasReleased(s[0]);
	lua_pushboolean(_L, isPressed);
	return 1;
}

// Control Interfaces
int ScriptManager::l_DestroyEntity(lua_State* _L)
{
	EntityId destroyId = luaL_checkinteger(_L, 1);
	auto i = instance->records.find(destroyId);
	if (i == instance->records.end())
		return 0;
	instance->DestroyEntityRecord(destroyId);
	return 0;
}

int ScriptManager::l_SendMessage(lua_State* _L)
{
	EntityId id = luaL_checkinteger(_L, 1);
	EntityId receiverId = luaL_checkinteger(_L, 2);
	const char* s = luaL_checkstring(_L, 3);
	auto i = instance->records.find(receiverId);
	if (i == instance->records.end())
		return 0;
	instance->OnMessageReceived(receiverId, id, s);
	return 0;
}

#pragma endregion

EntityId ScriptManager::CreateEntityRecord(std::string _scriptFilePath, Actor* _actor, RigidBody* _rigidBody)
{
	EntityId entity = nextEntityId++;

	EntityRecord record;

	Script script;
	script.filePath = _scriptFilePath;
	record.script = script;

	record.actor = _actor;

	record.rigidBody = _rigidBody;

	records[entity] = record;
	
	PhysicsManager::GetInstance()->RegisterRigidBody(_rigidBody, entity);
	RenderManager::GetInstance()->RegisterActor(_actor);
	
	Start(entity);
	
	std::cout << "Created Entity Record: " << entity << std::endl;
	
	return entity;
}

EntityRecord ScriptManager::GetEntityRecord(EntityId _e)
{
	return records[_e];
}

void ScriptManager::DestroyEntityRecord(EntityId _e)
{
	OnDestroy(_e);

	PhysicsManager::GetInstance()->UnregisterRigidBody(records[_e].rigidBody);
	RenderManager::GetInstance()->UnregisterActor(records[_e].actor);
	
	delete records[_e].rigidBody;
	delete records[_e].actor;

	records.erase(_e);

	std::cout << "Destroyed Entity Record: " << _e << std::endl;
}