cmake_minimum_required (VERSION 3.0)

set (CMAKE_VERBOSE_MAKEFILE TRUE)

add_compile_definitions (GLFW_STATIC GLEW_STATIC)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS_LINUX  "-Wall -Wextra -Wnarrowing -Wfloat-equal -Wundef -Wshadow -Wconversion -Wunreachable-code -Wpointer-arith -Wcast-align -Wwrite-strings -Wswitch-default -Wswitch-enum -Winit-self -Wcast-qual -Wlogical-op")
set (CMAKE_EXE_LINKER_FLAGS_LINUX "-lGL -lGLEW -lglfw -lpthread -lSOIL")
set (CMAKE_EXE_LINKER_FLAGS_WIN32 "opengl32.lib glew32s.lib glfw3.lib")

set (SOURCE_EXE main.cpp BSP/BSP.cpp render/render.cpp camera/camera.cpp camera/position.cpp level/level_common.cpp level/level_engine.cpp level/compress.cpp)

include_directories (${CMAKE_SOURCE_DIR}/libraries/glm/include)

if (WIN32)
	message ("Generating for win32")
	set (CMAKE_GENERATOR_PLATFORM win32)
	project (engine)
	include_directories (${CMAKE_SOURCE_DIR}/libraries/win32/include)
	link_directories (${CMAKE_SOURCE_DIR}/libraries/win32/lib)
	add_compile_definitions (_USE_MATH_DEFINES)
	set (CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS_WIN32})
elseif (UNIX)
	project (engine)
	if (NOT ${CMAKE_SYSTEM} MATCHES "Linux")
		message (FATAL_ERROR "Unknown UNIX: go patch KDE2 for your FREEbsdm or pay 9999$ to install normal os on your aplle machine")	
	endif ()
	message ("generating for Linux")
	#find_package (glfw3 REQUIRED)  cmake knows how to search for glfw3 but it fails as package is called glfw. Cmake can't search it and I'm too lazy to write finding script myself
	#if (NOT GLFW3_FOUND)
	#	message (FATAL_ERROR "GLFW not found, use 'pacman -S glfw-x11 glfw-wayland' to install it (and ubuntu users should now forget about copypasting and find package name themselves ;)")
	#endif ()
	find_package (GLEW REQUIRED)
	if (NOT GLEW_FOUND)
		message (FATAL_ERROR "GLEW not found, use 'pacman -S glew lib32-glew' to install it (and ubuntu users should now forget about copypasting and find package name themselves ;)")
	endif()
	#find_package (SOIL REQUIRED) cmake does not know SOIL lib
	#if (NOT SOIL_FOUND)
	#	message (FATAL_ERROR "SOIL not found, use 'pacman -S soil' to install it (and ubuntu users should now forget about copypasting and find package name themselves ;)")
	#endif ()
	set (CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS_LINUX})
	set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_LINUX})
else ()
	message (FATAL_ERROR "Unknown os: наши полномочия все... закончились")
endif ()


add_executable(engine ${SOURCE_EXE})


