cmake_minimum_required (VERSION 3.0)
project (engine)

set (CMAKE_VERBOSE_MAKEFILE TRUE)

add_compile_definitions (GLFW_STATIC GLEW_STATIC)

set (CMAKE_CXX_FLAGS  "-Wall -Wextra -Wnarrowing -Wfloat-equal -Wundef -Wshadow -Wconversion -Wunreachable-code -Wpointer-arith -Wcast-align -Wwrite-strings -Wswitch-default -Wswitch-enum -Winit-self -Wcast-qual -Wlogical-op")
set (CMAKE_EXE_LINKER_FLAGS_LINUX "-lGL -lGLEW -lglfw -lpthread")

set (SOURCE_EXE main.cpp render/render.cpp camera/camera.cpp level/level_common.cpp level/level_engine.cpp)

include_directories (${CMAKE_SOURCE_DIR}/libraries/glm/include)

if (WIN32)
	message ("Generating for win32")
	set (CMAKE_GENERATOR_PLATFORM win32)
	include_directories (${CMAKE_SOURCE_DIR}/libraries/win32/include)
	add_compile_definitions (_USE_MATH_DEFINES)
	target_link_libraries (engine opengl32 ${CMAKE_SOURCE_DIR}/libraries/win32/lib/glew32s.lib ${CMAKE_SOURCE_DIR}/libraries/win32/lib/glfw3.lib)
elseif (${CMAKE_SYSTEM} MATCHES "Linux")
	message ("generating for Linux")
	#find_package (glfw3 REQUIRED)  cmake knows how to search for glfw3 but it fails as package is called glfw. Cmake can't search it and I'm lazy to write finding script myself
	#if (NOT GLFW3_FOUND)
	#	message (FATAL_ERROR "GLFW not found, use 'pacman -S glfw-x11 glfw-wayland' to install it (and ubuntu users should now forget about copypasting and find package name thireselves ;)")
	#endif ()
	find_package (GLEW REQUIRED)
	if (NOT GLEW_FOUND)
		message (FATAL_ERROR "GLEW not found, use 'pacman -S glew lib32-glew' to install it (and ubuntu users should now forget about copypasting and find package name thireselves ;)")
	endif ()
	set (CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS_LINUX})
else ()
	message (FATAL_ERROR "Unknown os: go patch KDE2 for your FREEbsdm or pay 9999$ to install normal os on your aplle machine")
endif ()


add_executable(engine ${SOURCE_EXE})


