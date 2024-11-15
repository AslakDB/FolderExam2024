//
// Created by askel on 3/18/2024.
//

#include "Shaders.h"

Shaders::Shaders() {
}

std::string Shaders::readFile(const std::string &path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    if (!file) {
        std::cout << "Error reading file: " << path << std::endl;
    }
    return buffer.str();
}

void Shaders::CreateVertexShader(const char *vertexShaderSource) {
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

}

void Shaders::CreateFragmentShader(const char *fragmentShaderSource) {
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    int success;
    char infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void Shaders::LinkProgram() {
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

unsigned int Shaders::GetProgram() {
    return shaderProgram;
}

void Shaders::setInt(const std::string &name, int value) {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
