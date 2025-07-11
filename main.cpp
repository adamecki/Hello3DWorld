#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // 0 key exits the program
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
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
    GLFWwindow* window = glfwCreateWindow(1024, 768, "Hello, 3D World!", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set OpenGL viewport and callbacks (window resize)
    glViewport(0, 0, 1024, 768);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // render (update?) loop
    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        // clear screen
        glClearColor(0.2f, 0.4f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // ...

        glfwPollEvents();
        glfwSwapBuffers(window);
    }


    // Clean exit
    glfwTerminate();
    return 0;
}
