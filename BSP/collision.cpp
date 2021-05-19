#include <GLFW/glfw3.h>
#include <iostream>
#include "BSP.cpp"
#include <unistd.h>
int a = 0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    int fixed_number = 0;
    wall fixed_wall;
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        change_gamer_position(0.0, 0.01); 
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        change_gamer_position(0.0, -0.01);  
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        change_gamer_position(-0.01, 0.0); 
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        change_gamer_position(0.01, 0.0);
 
}

void draw_bsp (bsp_tree_wall* bsp_node)
{
    glBegin(GL_LINES);
    glVertex2f(bsp_node -> node_wall.x1, bsp_node -> node_wall.y1);
    glVertex2f(bsp_node -> node_wall.x2, bsp_node -> node_wall.y2);
    glEnd();
    if (bsp_node -> front_bsp_tree != NULL)
        draw_bsp(bsp_node->front_bsp_tree);
    if (bsp_node -> back_bsp_tree != NULL)
        draw_bsp(bsp_node->back_bsp_tree);
}

void write_bsp (bsp_tree_wall* bsp_node){
    std::cout << "x1" <<"  "<<bsp_node->node_wall.x1 << std::endl;
    std::cout << "y1" <<"  "<<bsp_node->node_wall.y1 << std::endl;
    std::cout << "x2" <<"  "<<bsp_node->node_wall.x2 << std::endl;
    std::cout << "y2" <<"  "<<bsp_node->node_wall.y2 << std::endl;
    if (bsp_node -> front_bsp_tree != NULL)
        write_bsp(bsp_node->front_bsp_tree);
    if (bsp_node -> back_bsp_tree != NULL)
        write_bsp(bsp_node->back_bsp_tree);
}
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 800, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    vector <wall> level;
    wall wall1(-0.3, 0.0, 0.0, 0.5);
    wall wall2(0.3, 0.0, 0.0, 0.5);
    wall wall3(0.0, -0.5, 0.3, 0.0);
    wall wall4(-0.3, 0.0, 0.0, -0.5);
    level.push_back(wall4);
    level.push_back(wall3);
    level.push_back(wall2);
    level.push_back(wall1);
    glfwSetKeyCallback(window, key_callback);
    bsp_tree_wall bsp_node(level);
    bsp_main_node = new bsp_tree_wall(level);
    //write_bsp(&bsp_node); 


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 1.0, 1.0);
        /* Swap front and back buffers */   
        draw_bsp(&bsp_node);
        glPointSize(5);
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_POINTS);
        glVertex2f(pos_x, pos_y);
        glEnd();
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}