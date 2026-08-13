#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <cmath>

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
const float PI = 3.1415927f;

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    uniform mat4 MVP;
    void main() {
        gl_Position = MVP * vec4(aPos, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    uniform vec3 color;
    void main() {
        FragColor = vec4(color, 1.0);
    }
)";

class Sphere {
public:
    unsigned int VAO, indexCount;
    
    Sphere(int sectorCount, int stackCount, float radius) {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        for (int i = 0; i <= stackCount; ++i) {
            float stackAngle = PI / 2 - i * PI / stackCount;
            float xy = radius * cosf(stackAngle);
            float z = radius * sinf(stackAngle);

            for (int j = 0; j <= sectorCount; ++j) {
                float sectorAngle = j * 2 * PI / sectorCount;
                float x = xy * cosf(sectorAngle);
                float y = xy * sinf(sectorAngle);
                
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
        }
        for (int i = 0; i < stackCount; ++i) {
            int k1 = i * (sectorCount + 1);
            int k2 = k1 + sectorCount + 1;

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
                if (i != 0) {
                    indices.push_back(k1); indices.push_back(k2); indices.push_back(k1 + 1);
                }
                if (i != (stackCount - 1)) {
                    indices.push_back(k1 + 1); indices.push_back(k2); indices.push_back(k2 + 1);
                }
            }
        }
        indexCount = indices.size();

        unsigned int VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    void draw() {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Solar System Simulator", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    unsigned int mvpLoc = glGetUniformLocation(shaderProgram, "MVP");
    unsigned int colorLoc = glGetUniformLocation(shaderProgram, "color");

    Sphere sun(36, 18, 2.0f);

    while (!glfwWindowShouldClose(window)) {
    Sphere sun(36, 18, 2.0f);
    Sphere earth(36, 18, 0.5f);

    while (!glfwWindowShouldClose(window)) {
    Sphere sun(36, 18, 2.0f);
    Sphere earth(36, 18, 0.5f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        float time = glfwGetTime();

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 15.0f, 15.0f), 
            glm::vec3(0.0f, 0.0f, 0.0f),   
            glm::vec3(0.0f, 1.0f, 0.0f)   
        ); 

        float sunSpinAngle = time * 0.5f; 
        glm::mat4 sunModel = glm::mat4(1.0f); 
        sunModel = glm::rotate(sunModel, sunSpinAngle, glm::vec3(0.0f, 1.0f, 0.0f)); 
        
        glm::mat4 sunMVP = projection * view * sunModel;
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(sunMVP));
        glUniform3f(colorLoc, 1.0f, 0.9f, 0.0f); 
        sun.draw();

        float earthOrbitAngle = time * 1.0f; 
        float earthSpinAngle = time * 3.0f;  

        glm::mat4 earthModel = glm::mat4(1.0f);
        earthModel = glm::rotate(earthModel, earthOrbitAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        earthModel = glm::translate(earthModel, glm::vec3(6.0f, 0.0f, 0.0f)); 
        earthModel = glm::rotate(earthModel, earthSpinAngle, glm::vec3(0.0f, 1.0f, 0.0f)); 
        
        glm::mat4 earthMVP = projection * view * earthModel;
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(earthMVP));
        glUniform3f(colorLoc, 0.2f, 0.5f, 1.0f); 
        earth.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    }
    }
    glfwTerminate();
    return 0;
}