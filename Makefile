project-lua-test: src/main.cpp
	g++ -o lua-test.exe src/main.cpp -Iinclude -Llib -ISDL3-3.2.22/x86_64-w64-mingw32/include -LSDL3-3.2.22/x86_64-w64-mingw32/lib -llua -lSDL3