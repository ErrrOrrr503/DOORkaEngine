# DOORka engine 
v 0.3E-9

A pseudo-engine utilized to implenent DOOR game.

Note: windows compilation is broken now due to SOIL lib not included for win32.

**Cmake** is used for building on both windows (win32) and linux (native x86 or x64). Just make a build folder and run cmake /path/to/the/source/. On linux you have to have glfw and glew installed. After cmake use **make** for linux and **MSbuild.exe engine.sln** for windows. For example from source folder run:

    mkdir build
    cd build  
    cmake ../

    then "MSbuild.exe engine.sln" for windows

    or "make" for linux

On linux **engine.exe** is right in *build* folder, On windows it is located in *build/Debug/folder*

Also to compile engine on linux you can use:  
**g++ -std=c++17 -Wall -Wextra -lGLEW -lglfw -lGL -lSOIL -lpthread -o render_test.exe ./main.cpp ./render/render.cpp ./camera/camera.cpp ./level/level_common.cpp ./level/compress.cpp ./level/level_engine.cpp ./camera/position.cpp**

to compile editor use *qmake* or **qtCreator**
