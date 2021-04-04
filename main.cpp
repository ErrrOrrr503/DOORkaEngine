#include "level/level_engine.h"
#include "render/render.h"
#include <GLFW/glfw3.h>
#include <iostream>

int main (int argc, char **argv)
{
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " <filename>" << std::endl;
    }
    Level level;
    Render render(level);
    while (render.status () != should_close) {
        render.draw ();
        glfwPollEvents ();
    }
}