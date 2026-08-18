#pragma once
#include <GL/glew.h>
#include <vector>
#include <cmath>
#include "Common.h"

class Orbit
{
public:
    unsigned int VAO, VBO;
    int vertexCount;

    Orbit(int segmentCount = 100)
    {
        vertexCount = segmentCount;
        std::vector<float> vertices;

        for (int i = 0; i < segmentCount; ++i)
        {
            float angle = 2.0f * PI * float(i) / float(segmentCount);
            vertices.push_back(cosf(angle));
            vertices.push_back(0.0f);
            vertices.push_back(sinf(angle));
        }

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    void draw() const
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
        glBindVertexArray(0);
    }
};