#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib> 

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
const float PI = 3.1415927f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

const char *vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    uniform mat4 MVP;
    void main() {
        gl_Position = MVP * vec4(aPos, 1.0);
    }
)";

const char *fragmentShaderSource = R"(
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
                vertices.push_back(xy * cosf(sectorAngle));
                vertices.push_back(xy * sinf(sectorAngle));
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }
    void draw() {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

struct Planet {
    glm::vec3 pos;
    glm::vec3 vel;
    float mass;
    float radius;
    glm::vec3 color;
};

const float G = 1.0f;
const float TIME_STEP = 0.0005f; 
std::vector<Planet> planets;

void addPlanet(glm::vec3 pos, glm::vec3 vel, float mass, float radius, glm::vec3 color) {
    planets.push_back({pos, vel, mass, radius, color});
}

void updatePhysics() {
    int n = planets.size();
    std::vector<glm::vec3> forces(n, glm::vec3(0.0f));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            glm::vec3 dir = planets[j].pos - planets[i].pos;
            float distSq = glm::dot(dir, dir);
            if (distSq < 0.1f) continue; 
            
            float forceMagnitude = (G * planets[i].mass * planets[j].mass) / distSq;
            forces[i] += forceMagnitude * glm::normalize(dir);
        }
    }

    for (int i = 0; i < n; ++i) {
        glm::vec3 acceleration = forces[i] / planets[i].mass;
        planets[i].vel += acceleration * TIME_STEP;
        planets[i].pos += planets[i].vel * TIME_STEP;
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        float randomSpeed = 10.0f + (rand() % 10);
        addPlanet(
            glm::vec3(45.0f, 0.0f, 0.0f),       
            glm::vec3(0.0f, 0.0f, randomSpeed), 
            5.0f,                               
            1.0f,                               
            glm::vec3(0.0f, 1.0f, 0.5f)         
        );
        std::cout << "Custom Planet Added! Total Bodies: " << planets.size() << "\n";
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Gravity Solar System", NULL, NULL);
    glfwMakeContextCurrent(window);
    

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, keyCallback);
    
    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

    Sphere unitSphere(36, 18, 1.0f);


    float sunMass = 10000.0f;
    addPlanet(glm::vec3(0.0f), glm::vec3(0.0f), sunMass, 4.0f, glm::vec3(1.0f, 0.9f, 0.0f));

    auto calcVelocity = [sunMass](float distance) {
        return std::sqrt((G * sunMass) / distance);
    };

 
    addPlanet(glm::vec3(7.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, calcVelocity(7.0f)), 0.1f, 0.4f, glm::vec3(0.5f, 0.5f, 0.5f));

    addPlanet(glm::vec3(11.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, calcVelocity(11.0f)), 0.8f, 0.6f, glm::vec3(0.9f, 0.8f, 0.4f));

    addPlanet(glm::vec3(16.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, calcVelocity(16.0f)), 1.0f, 0.7f, glm::vec3(0.2f, 0.5f, 1.0f));

    addPlanet(glm::vec3(22.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, calcVelocity(22.0f)), 0.5f, 0.5f, glm::vec3(0.9f, 0.3f, 0.1f));
    
   
    addPlanet(glm::vec3(35.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, calcVelocity(35.0f)), 10.0f, 1.8f, glm::vec3(0.8f, 0.5f, 0.3f));

    addPlanet(glm::vec3(50.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, calcVelocity(50.0f)), 8.0f, 1.5f, glm::vec3(0.9f, 0.9f, 0.5f));
    

    addPlanet(glm::vec3(65.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, calcVelocity(65.0f)), 4.0f, 1.0f, glm::vec3(0.5f, 0.8f, 0.9f));

    addPlanet(glm::vec3(80.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, calcVelocity(80.0f)), 4.0f, 0.9f, glm::vec3(0.1f, 0.2f, 0.9f));


  
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.02f, 0.02f, 0.02f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        for(int i = 0; i < 10; i++) {
            updatePhysics();
        }

        int currentWidth, currentHeight;
        glfwGetFramebufferSize(window, &currentWidth, &currentHeight);
        
        if (currentHeight == 0) currentHeight = 1;
        float aspectRatio = (float)currentWidth / (float)currentHeight;


        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 70.0f, 100.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 

        for (const auto& planet : planets) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, planet.pos);
            model = glm::scale(model, glm::vec3(planet.radius));
            
            glm::mat4 mvp = projection * view * model;
            glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
            glUniform3fv(colorLoc, 1, glm::value_ptr(planet.color)); 
            
            unitSphere.draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}