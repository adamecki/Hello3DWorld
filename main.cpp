#include <iostream>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

// window settings
const unsigned int WINDOW_W = 1024;
const unsigned int WINDOW_H = 768;

// [DONE] Objective: Adjust the vertex shader so that the triangle is upside down.
// [DONE] Objective: Specify a horizontal offset via a uniform and move the triangle
//              to the right side of the screen in the vertex shader using
//              using this offset value.
// [DONE] Objective: Output the vertex position to the fragment shader using the out
//              keyword and set the fragment's color equal to this vertex
//              position.
// Question: Why is the bottom left (top left) side of the triangle black?
// Answer:   That's probably happening because negative color values are treated as 0.

// "Hello Triangle" chapter - triangle vertices
float vertices_and_their_colors[] = {
    // coordinates      // colors
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left:  red
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right: green
     0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // top:          blue
};

// functions from other files
void h3w_linkShaderProgram(unsigned int &shaderProgram);

// callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// input function
void processInput(GLFWwindow* window) {
    // 0 key exits the program
    if(glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For macOS

    // Create GLFW window
    GLFWwindow* window = glfwCreateWindow(WINDOW_W, WINDOW_H, "Hello, 3D World!", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // "Shader" chapter - use the shader header file to process vertex and fragment shaders
    h3w_shader ourShader("./shaders/shader.vert", "./shaders/shader.frag");

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // ditto - pass triangle vertex data to VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_and_their_colors), vertices_and_their_colors, GL_STATIC_DRAW);

    // ditto - specify how OpenGL should interpret the vertex buffer data: position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // render (update?) loop
    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Objective 2: moving offset
        float timeValue = glfwGetTime();
        float offsetValue = (sin(timeValue) / 10.0f) + 0.1f;
        
        // "Hello Triangle" rendering
        ourShader.use();
        ourShader.setFloat("offset", offsetValue);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean exit
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    ourShader.clean_delete();
    
    glfwTerminate();
    return 0;
}
