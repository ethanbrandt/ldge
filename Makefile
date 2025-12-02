prod: src/main.cpp
	g++ .\src\main.cpp .\src\utilities\FileHandler.cpp .\src\physics\RigidBody.cpp .\src\physics\CollisionShape.cpp .\src\physics\CollisionCircle.cpp .\src\physics\CollisionRectangle.cpp .\src\utilities\Vector2.cpp .\src\gorm\ScriptManager.cpp -Iinclude -Llib -ISDL3-3.2.22\x86_64-w64-mingw32\include -LSDL3-3.2.22\x86_64-w64-mingw32\lib -lSDL3 -llua -o FileTest.exe

project-rb-test: src/main.cpp
	g++ .\src\physics\RigidBodyTest.cpp .\src\physics\RigidBody.cpp .\src\physics\CollisionShape.cpp .\src\physics\CollisionCircle.cpp .\src\physics\CollisionRectangle.cpp .\src\utilities\Vector2.cpp -Iinclude -Llib -ISDL3-3.2.22\x86_64-w64-mingw32\include -LSDL3-3.2.22\x86_64-w64-mingw32\lib -lSDL3 -llua -o RB.exe