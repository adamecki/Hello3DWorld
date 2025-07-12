#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// [DONE] Objective 1: Draw 2 triangles next to each other using glDrawArrays by adding more vertices to your data.
// [DONE] Objective 2: Create the same 2 triangles using two different VAOs and VBOs for their data.
// [DONE] Objective 3: Create two shader programs where the second program uses a different fragment shader
//              that outputs the color yellow; draw both triangles again where one outputs the color yellow.

// window settings
const unsigned int WINDOW_W = 1024;
const unsigned int WINDOW_H = 768;

// "Hello Triangle" chapter - triangle vertices
float vertices_1[] = {
    // First triangle
    -0.75f, -0.25f, 0.0f,
    -0.25f, -0.25f, 0.0f,
    -0.5f, 0.25f, 0.0f
};

float vertices_2[] = {
    // Second triangle
    0.25f, -0.25f, 0.0f,
    0.75f, -0.25f, 0.0f,
    0.5f, 0.25f, 0.0f
};

// functions from other files
void h3w_linkShaderProgram(unsigned int &shaderProgram, bool yellow_shader);

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

    //
    // "Hello Triangle" chapter - compile Vertex and Fragment shaders, link shader program
    //
    unsigned int shaderProgram_w, shaderProgram_y;
    h3w_linkShaderProgram(shaderProgram_w, false);
    h3w_linkShaderProgram(shaderProgram_y, true);

    // ditto - Vertex Array Object, Vertex Buffer Object
    GLuint vao[2], vbo[2];

    glGenVertexArrays(2, vao);
    glGenBuffers(2, vbo);

    // Triangle 1
    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW);

    // specify how OpenGL should interpret the vertex buffer data [for every vbo!]
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Triangle 2
    glBindVertexArray(vao[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_2), vertices_2, GL_STATIC_DRAW);

    // specify how OpenGL should interpret the vertex buffer data [for every vbo!]
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // render (update?) loop
    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        // clear screen
        glClearColor(0.2f, 0.4f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // "Hello Triangle" rendering
        // Draw triangle 1
        glUseProgram(shaderProgram_w);
        glBindVertexArray(vao[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // Draw triangle 2
        glUseProgram(shaderProgram_y);
        glBindVertexArray(vao[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // Clean exit
    glDeleteVertexArrays(2, vao);
    glDeleteBuffers(2, vbo);

    glDeleteProgram(shaderProgram_w);
    glDeleteProgram(shaderProgram_y);
    
    glfwTerminate();
    return 0;
}
