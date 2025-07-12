#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// "Hello Triangle" chapter - vertex shader source
const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

// ditto - fragment shader source
const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
    "}\0";

unsigned int vertexShader, fragmentShader;

void h3w_compileVertexShader() {
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // check if vertex shader compilation was successful
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(success) {
        std::cout << "I: vertex shader compilation most likely successful" << std::endl;
    } else {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "E: vertex shader compilation failed\n" << infoLog << std::endl;
    }
}

void h3w_compileFragmentShader() {
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check if fragment shader compilation was successful
    int success;
    char infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(success) {
        std::cout << "I: fragment shader compilation most likely successful" << std::endl;
    } else {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "E: fragment shader compilation failed\n" << infoLog << std::endl;
    }
}

void h3w_linkShaderProgram(unsigned int &shaderProgram) {
    // compile Vertex and Fragment shaders
    h3w_compileVertexShader();
    h3w_compileFragmentShader();

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check if shader program linking was successful
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(success) {
        std::cout << "I: shader program linking most likely successful" << std::endl;
    } else {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "E: shader program linking failed\n" << infoLog << std::endl;
    }

    // delete remaining shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
