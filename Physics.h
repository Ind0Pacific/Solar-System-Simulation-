#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <cmath>

struct Planet {
    glm::vec3 pos;
    glm::vec3 vel;
    float mass;
    float radius;
    glm::vec3 color;
};

inline const float G = 1.0f;
inline const float TIME_STEP = 0.0005f;
inline std::vector<Planet> planets;

inline void addPlanet(glm::vec3 pos, glm::vec3 vel, float mass, float radius, glm::vec3 color) {
    planets.push_back({pos, vel, mass, radius, color});
}

inline void updatePhysics() {
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

inline void initSolarSystem() {
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
}