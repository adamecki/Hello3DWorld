#include <iostream>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader/shader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

// window settings
const unsigned int WINDOW_W = 1024;
const unsigned int WINDOW_H = 768;

// [DONE] Objective 1: Make sure only the happy face looks in the other/reverse direction by changing the fragment shader.
// [DONE] Objective 2: See if you can display 4 smiley faces on a single container image clamped at its edge.
// [DONE] Objective 3: Try to display only the center pixels of the texture image on the rectangle in such a way that
//              the individual pixels are getting visible by changing the texture coordinates. Try to set the texture
//              filtering method to GL_NEAREST to see the pixels more clearly.
// [DONE] Objective 4: Use a uniform variable as the mix function's third parameter to vary the amount the two textures
//              are visible. Use the up and down arrow keys to change how much the container or the smiley face
//              is visible.

float vertices_colors_txtcoords[] = {
    // vertices         // colors           // texture coordinates
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   2.0f, 2.0f,     // top right
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   2.0f, 0.0f,     // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,     // bottom left
    -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f,   0.0f, 2.0f      // top left
};

unsigned int indices[] = {
    0, 1, 3,
    3, 2, 1
};

float texture_mix = 0.0;

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
    
    // change texture zoom mode
    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        vertices_colors_txtcoords[6] = 2.0f;
        vertices_colors_txtcoords[7] = 2.0f;

        vertices_colors_txtcoords[14] = 2.0f;
        vertices_colors_txtcoords[15] = 0.0f;

        vertices_colors_txtcoords[22] = 0.0f;
        vertices_colors_txtcoords[23] = 0.0f;

        vertices_colors_txtcoords[30] = 0.0f;
        vertices_colors_txtcoords[31] = 2.0f;
    }

    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        vertices_colors_txtcoords[6] = 0.55f;
        vertices_colors_txtcoords[7] = 0.55f;

        vertices_colors_txtcoords[14] = 0.55f;
        vertices_colors_txtcoords[15] = 0.45f;

        vertices_colors_txtcoords[22] = 0.45f;
        vertices_colors_txtcoords[23] = 0.45f;

        vertices_colors_txtcoords[30] = 0.45f;
        vertices_colors_txtcoords[31] = 0.55f;
    }

    // change mix values
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && texture_mix < 1.0) {
        texture_mix += 0.01;
    }

    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && texture_mix > 0.0) {
        texture_mix -= 0.01;
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

    // "Textures" chapter - create OpenGL texture
    unsigned int textures[2];
    glGenTextures(2, textures);

    for(unsigned int texture : textures) {
        glBindTexture(GL_TEXTURE_2D, texture);
        // set texture wrapping and filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    // ditto - load image via stb_image library
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char* data = stbi_load("./textures/container.jpg", &width, &height, &nrChannels, 0);
    if(data) {
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    } else {
        std::cout << "E: failed to load texture" << std::endl;
    }
    data = stbi_load("./textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if(data) {
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    } else {
        std::cout << "E: failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // "Shader" chapter - use the shader header file to process vertex and fragment shaders
    h3w_shader ourShader("./shaders/shader.vert", "./shaders/shader.frag");
    ourShader.use();
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture2"), 1);

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // ditto - pass vertices data to VBO and indices data to EBO
    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_colors_txtcoords), vertices_colors_txtcoords, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // specify how OpenGL should interpret the vertex buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // render (update?) loop
    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // rendering
        ourShader.use();
        ourShader.setFloat("texture_mix", texture_mix);
        // "Textures" chapter - bind texture
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_colors_txtcoords), vertices_colors_txtcoords, GL_DYNAMIC_DRAW);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean exit
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    ourShader.clean_delete();
    
    glfwTerminate();
    return 0;
}
