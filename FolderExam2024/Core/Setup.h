//
// Created by Aslak on 4/1/2024.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"


#ifndef MAIN_CPP_SETUP_H
#define MAIN_CPP_SETUP_H


Shaders shaders;

std::string vertexShaderSourceString = shaders.readFile("Shaders/Shader.vs");
std::string fragmentShaderSourceString = shaders.readFile("Shaders/Shader.fs");

const char *vertexShaderSource = vertexShaderSourceString.c_str();
const char *fragmentShaderSource = fragmentShaderSourceString.c_str();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
struct SetUp
{


    unsigned shaderProgram;
    int value1;

    void setup(const char* title, GLFWwindow*& window, int SCR_WIDTH, int SCR_HEIGHT) {
        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            value1 = -1;
            return;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            value1 = -1;
            return;
        }

        shaders.CreateFragmentShader(fragmentShaderSource);
        shaders.CreateVertexShader(vertexShaderSource);
        shaders.LinkProgram();

        glEnable(GL_DEPTH_TEST);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
};
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
#endif //MAIN_CPP_SETUP_H
