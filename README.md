# DOORka engine 
v 0.1E-9

A pseudo-engine utilized to implenent DOOR game.

to compile engine use:
**g++ -Wall -Wextra -lGLEW -lglfw -lGL -lpthread -o render_test.exe ./main.cpp ./render/render.cpp ./camera/camera.cpp ./level/level_common.cpp ./level/level_engine.cpp ./camera_example/sphere.cpp**

to compile editor use qmake or qtCreator

for windows don't use mingw, linking with glew and glfw is somehow broken, use cmake and MSVC >=142 (visual studio 2019):
**mkdir build**
**cd build**
**cmake ../**
**$MSBUILD.EXE engine.sln**

Now engine.exe for win32 is in build/Debug directory

