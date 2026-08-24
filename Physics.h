#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <deque>
#include <cmath>

struct Planet
{
    glm::vec3 pos;
    glm::vec3 vel;
    float mass;
    float radius;
    glm::vec3 color;
    float rotationSpeed;
    float currentAngle;
    std::vector<glm::vec3> trail;
};

inline const float G = 1.0f;
inline const float TIME_STEP = 0.0005f;
inline std::vector<Planet> planets;

inline void addPlanet(glm::vec3 pos, glm::vec3 vel, float mass, float radius, glm::vec3 color, float rotationSpeed)
{
    planets.push_back({pos, vel, mass, radius, color, rotationSpeed, 0.0f, {}}); 
}

inline void updatePhysics()
{
    int n = planets.size();
    std::vector<glm::vec3> forces(n, glm::vec3(0.0f));

    for (int i = 0; i < n; ++i)
    {
        planets[i].currentAngle += planets[i].rotationSpeed * TIME_STEP;

        for (int j = 0; j < n; ++j)
        {
            if (i == j)
                continue;
            glm::vec3 dir = planets[j].pos - planets[i].pos;
            float distSq = glm::dot(dir, dir);
            if (distSq < 0.1f)
                continue;

            float forceMagnitude = (G * planets[i].mass * planets[j].mass) / distSq;
            forces[i] += forceMagnitude * glm::normalize(dir);
        }
    }

    for (int i = 1; i < n; ++i)
    {
        glm::vec3 acceleration = forces[i] / planets[i].mass;
        planets[i].vel += acceleration * TIME_STEP;
        planets[i].pos += planets[i].vel * TIME_STEP;
    }

 static int trailStepCounter = 0;
    trailStepCounter++;
    if (trailStepCounter % 15 == 0) 
    {
        for (size_t i = 1; i < planets.size(); ++i)
        {
            planets[i].trail.push_back(planets[i].pos);
            if (planets[i].trail.size() > 400) 
            {
                planets[i].trail.erase(planets[i].trail.begin()); 
            }
        }
    }
}
inline void initSolarSystem() {
    float sunMass = 10000.0f;
    addPlanet(glm::vec3(0.0f), glm::vec3(0.0f), sunMass, 4.0f, glm::vec3(1.0f, 0.9f, 0.0f), 0.5f);
    auto calcVelocity = [sunMass](float distance) {
        return std::sqrt((G * sunMass) / distance);
    };

    addPlanet(glm::vec3(7.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, calcVelocity(7.0f)), 0.1f, 0.4f, glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);
    addPlanet(glm::vec3(11.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, calcVelocity(11.0f)), 0.8f, 0.6f, glm::vec3(0.9f, 0.8f, 0.4f), -0.8f); 
    addPlanet(glm::vec3(16.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, calcVelocity(16.0f)), 1.0f, 0.7f, glm::vec3(0.2f, 0.5f, 1.0f), 2.0f);

    float moonDist = 1.2f; 
    float earthMass = 1.0f;
    float moonRelativeVel = std::sqrt((G * earthMass) / moonDist);
    
    addPlanet(
        glm::vec3(16.0f + moonDist, 0.0f, 0.0f),                      
        glm::vec3(0.0f, 0.0f, calcVelocity(16.0f) + moonRelativeVel),  
        0.05f, 0.25f, glm::vec3(0.7f, 0.7f, 0.7f), 1.5f                                                                      
    );

    addPlanet(glm::vec3(22.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, calcVelocity(22.0f)), 0.5f, 0.5f, glm::vec3(0.9f, 0.3f, 0.1f), 1.9f);
    addPlanet(glm::vec3(35.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, calcVelocity(35.0f)), 10.0f, 1.8f, glm::vec3(0.8f, 0.5f, 0.3f), 4.5f);
    addPlanet(glm::vec3(50.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, calcVelocity(50.0f)), 8.0f, 1.5f, glm::vec3(0.9f, 0.9f, 0.5f), 4.0f);
    addPlanet(glm::vec3(65.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, calcVelocity(65.0f)), 4.0f, 1.0f, glm::vec3(0.5f, 0.8f, 0.9f), 3.0f);
    addPlanet(glm::vec3(80.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, calcVelocity(80.0f)), 4.0f, 0.9f, glm::vec3(0.1f, 0.2f, 0.9f), 3.2f);
}