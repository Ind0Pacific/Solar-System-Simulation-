#pragma once
#include <GL/glew.h>
#include <vector>
#include <cmath>
#include "Common.h"

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

    void draw() const {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};