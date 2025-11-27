#include <iostream>
#include <thread>
#include <chrono>
#include <lua.hpp>


const float FRAMES_PER_SECOND = 60;

int main()
{
	/*int testFrames = 4000;
	const int frameDuration = int(1000 / FRAMES_PER_SECOND);

	for (int i = 0; i < testFrames; ++i)
	{
		std::cout << "Frame " << i + 1 << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(frameDuration));
	}*/

	std::cout << "test" << std::endl;

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	if (luaL_loadfile(L, "scripts/test_script.lua") == 0)
		lua_pcall(L, 0, 0, 0);
	else
	{
		std::cout << "Error: " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
	}

	std::cout << "Lua version is " << lua_version(L) << std::endl;
	lua_close(L);

	return 0;
}