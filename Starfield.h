#pragma once
#include <GL/glew.h>
#include <vector>
#include <cstdlib>
#include <cmath>

class Starfield {
public:
    unsigned int VAO, VBO;
    int starCount;

    Starfield(int count = 5000) {
        starCount = count;
        std::vector<float> stars;
        srand(42); 

        for(int i = 0; i < count; ++i) {
            float u = ((float)rand() / RAND_MAX);
            float v = ((float)rand() / RAND_MAX);
            float theta = u * 2.0f * 3.14159265359f;
            float phi = acos(2.0f * v - 1.0f);
            
            float radius = 400.0f + ((float)rand() / RAND_MAX) * 400.0f; 

            float x = radius * sin(phi) * cos(theta);
            float y = radius * sin(phi) * sin(theta);
            float z = radius * cos(phi);

            stars.push_back(x);
            stars.push_back(y);
            stars.push_back(z);
        }

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, stars.size() * sizeof(float), stars.data(), GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    void draw() const {
        glBindVertexArray(VAO);
        glPointSize(1.5f); 
        glDrawArrays(GL_POINTS, 0, starCount);
        glBindVertexArray(0);
    }
};