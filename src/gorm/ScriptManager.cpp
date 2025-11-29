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

void ScriptManager::OnTrigger(EntityId _e, RigidBody& _other)
{

}

void ScriptManager::OnCollision(EntityId _e, RigidBody& _other)
{

}

void ScriptManager::OnDestroy(EntityId _e)
{

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
	EntityId id = FileHandler::GetInstance()->LoadEntityFromFile(s);
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

	std::cout << "Created Entity Record" << std::endl;

	return entity;
}

EntityRecord ScriptManager::GetEntityRecord(EntityId _e)
{
	return records[_e];
}

void ScriptManager::DestroyEntityRecord(EntityId _e)
{
	// TODO: IMPLEMENT
}