#include "Common.h"
#include "Shader.h"
#include "Sphere.h"
#include "orbit.h"
#include "Physics.h"
#include "Starfield.h"
#include <glm/gtc/type_ptr.hpp>
#include <string>

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Solar System Simulation | Movement: W, A, S, D | Exit: ESC", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, keyCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    unsigned int shaderProgram = createShaderProgram();
    unsigned int mvpLoc = glGetUniformLocation(shaderProgram, "MVP");
    unsigned int colorLoc = glGetUniformLocation(shaderProgram, "color");

    Sphere unitSphere(36, 18, 1.0f);
    Orbit orbitRing(100);
    Starfield stars(5000);
    initSolarSystem();

    float simulatedTime = 0.0f;
    const float EARTH_YEAR_DURATION = 4.0212f;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.02f, 0.02f, 0.02f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        for (int i = 0; i < 10; i++)
        {
            updatePhysics();
            simulatedTime += TIME_STEP;
        }

        int currentYear = static_cast<int>(simulatedTime / EARTH_YEAR_DURATION);

        std::string title = "Solar System Simulation | Year: " + std::to_string(currentYear) +
                            " | Movement: W, A, S, D | Exit: ESC";

        glfwSetWindowTitle(window, title.c_str());

        int currentWidth, currentHeight;
        glfwGetFramebufferSize(window, &currentWidth, &currentHeight);
        if (currentHeight == 0)
            currentHeight = 1;
        float aspectRatio = (float)currentWidth / (float)currentHeight;

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::vec3 sunPos = planets[0].pos;

        glm::mat4 starModel = glm::mat4(1.0f);
        glm::mat4 starMVP = projection * view * starModel;
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(starMVP));
        

        glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f); 
        stars.draw();

        for (size_t i = 1; i < planets.size(); ++i)
        {
            const auto &planet = planets[i];
            float distance = glm::distance(planet.pos, sunPos);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, sunPos);
            model = glm::scale(model, glm::vec3(distance));

            glm::mat4 mvp = projection * view * model;
            glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

            glUniform3f(colorLoc, 0.2f, 0.2f, 0.2f);

            orbitRing.draw();
        }

        for (const auto &planet : planets)
        {
            glm::mat4 model = glm::mat4(1.0f);

            model = glm::translate(model, planet.pos);
            model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::rotate(model, planet.currentAngle, glm::vec3(0.0f, 1.0f, 0.0f));
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