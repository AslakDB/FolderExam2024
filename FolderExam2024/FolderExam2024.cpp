#include "Core/render.h"
#include "Core/Setup.h"
SetUp set_up;
Render render;

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f;

int main(int argc, char* argv[])
{
    GLFWwindow* window ;
    /// Press I for å få en ball til å bevege på seg
    set_up.setup("Window", window, SCR_WIDTH,SCR_HEIGHT);
    glfwSetCursorPosCallback(window, mouse_callback);

    unsigned int shadersProgram = shaders.GetProgram();
    glUseProgram(shadersProgram);




    render.render(window , shadersProgram, deltaTime, lastFrame);

    glfwTerminate();
    
}
