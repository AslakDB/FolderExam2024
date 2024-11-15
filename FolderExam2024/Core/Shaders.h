//

#ifndef SHADERS_H
#define SHADERS_H
#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shaders {

private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int shaderProgram;
public:

    Shaders();
    unsigned int ID;

    std::string readFile(const std::string& path);

    void CreateVertexShader(const char* vertexShaderSource);
    void CreateFragmentShader(const char* fragmentShaderSource);
    void LinkProgram();
    unsigned int GetProgram();

    void setInt(const std::string &name, int value);
    
};



#endif //SHADERS_H
