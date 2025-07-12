#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// window settings
const unsigned int WINDOW_W = 1024;
const unsigned int WINDOW_H = 768;

// "Hello Triangle" chapter - polygon vertices
float vertices[] = {
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f
};
unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
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

    // Move the polygon's vertex around (yeah, I'm distracting myself from the actual graphics programming)
    // X axis
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        vertices[6] += 0.01f;
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        vertices[6] -= 0.01f;
    }
    // Y axis
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        vertices[7] += 0.01f;
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        vertices[7] -= 0.01f;
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
    unsigned int shaderProgram;
    h3w_linkShaderProgram(shaderProgram);

    // ditto - Vertex Array Object, Element Buffer Object, Vertex Buffer Object
    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    // specify how OpenGL should interpret the vertex buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // render (update?) loop
    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        // clear screen
        glClearColor(0.2f, 0.4f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // "Hello Triangle" rendering
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // Update top vertice location
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        // Draw polygon
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // Clean exit
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    
    glfwTerminate();
    return 0;
}
