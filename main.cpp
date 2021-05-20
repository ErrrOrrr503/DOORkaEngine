#include "level/level_engine.h"
#include "render/render.h"
#include <GLFW/glfw3.h>
#include <iostream>

int main (int argc, char **argv)
{
    Level level;
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " [filename]" << std::endl;
        return 1;
    }
    if (level.load_level (argv[1])) {
        if (level.load_level ("testlvl.lvl"))
            return 1;
    }
    //now level is loaded
    Render render (level);
    if (render.status () != ok) {
        std::cerr << "Problem with render, exiting" << std::endl;
        return -1;
    }
    while (render.status () != should_close) {
        render.draw ();
        glfwPollEvents ();
    }
    glfwTerminate ();
    return 0;
}