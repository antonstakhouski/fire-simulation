/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "emitter.h"

#include <iostream>

#define LIFE_MEAN 3.0f
#define LIFE_DEVATION 1.0f

Emitter::Emitter(const Shader& shader,
                 const Texture2D& texture,
                 const glm::vec3& position,
                 const glm::vec3& direction,
                 GLfloat radius,
                 GLfloat energy,
                 GLfloat velocity,
                 GLuint amount)
    : m_shader(shader),
      m_texture(texture),
      m_amount(amount),
      m_position(position),
      m_direction(glm::normalize(direction)),
      m_radius(radius),
      m_energy(energy),
      // emit in direction inverse to movement
      m_velocity(-velocity),
      m_lastUsedParticle(0)
{
    Init();
}

bool Emitter::IsAlive() const
{
    return m_energy > 0.0f;
}

void Emitter::Update(GLfloat dt, GLuint nNewParticles, const glm::vec3& offset)
{
    m_energy -= dt;

    if (IsAlive()) {
        // Add new particles
        for (GLuint i = 0; i < nNewParticles; ++i)
        {
            const int64_t res = FirstUnusedParticle();

            size_t unusedParticle = 0;
            if (res < 0) {
                std::uniform_int_distribution<> distribution(0, m_amount);
                unusedParticle = distribution(m_rndGenerator);
            } else {
                unusedParticle = static_cast<size_t>(res);
            }

            m_particles[unusedParticle] = GenerateParticle(offset);
        }
    }

    m_deadIndexes.clear();

    // TODO:
    // Update all particles
    for (size_t i = 0; i < m_amount; ++i)
    {
        Particle& p = m_particles[i];
        if (!p.Update(dt)) {
            m_deadIndexes.push_back(i);
        }
    }
}

// Render all particles
void Emitter::Draw()
{
    // Use additive blending to give it a 'glow' effect
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    m_shader.Use();

    glm::vec3* ptrOffset = static_cast<glm::vec3*>(glMapNamedBuffer(m_offsetVBO, GL_WRITE_ONLY));
    glm::vec4* ptrColors = static_cast<glm::vec4*>(glMapNamedBuffer(m_colorVBO, GL_WRITE_ONLY));

    for (size_t i = 0; i < m_amount; ++i) {
        if (m_particles[i].IsAlive()) {
            ptrOffset[i] = m_particles[i].GetPosition();
            ptrColors[i] = m_particles[i].GetColor();
        }
    }

    glUnmapNamedBuffer(m_offsetVBO);
    glUnmapNamedBuffer(m_colorVBO);

    glBindVertexArray(m_VAO);

    m_texture.Bind();
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, m_amount);
    glBindVertexArray(0);
    // Don't forget to reset to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Emitter::Init()
{
    // Set up mesh and attribute properties
    GLuint VBO;
    GLfloat particle_cube[] = {
        // positions          // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(m_VAO);
    // Fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_cube), particle_cube, GL_STATIC_DRAW);
    // Set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

    glBindVertexArray(0);

    // memory consuming but fast and reliable
    m_particles.reserve(m_amount);
    m_deadIndexes.reserve(m_amount);

    // Create this->amount default particle instances
    for (GLuint i = 0; i < m_amount; ++i) {
        m_particles.push_back(Particle());
    }

    glm::mat4 model(1.0f);
    model = glm::translate(model, m_position);
    m_shader.SetMatrix4("model", model);

    // setUp VBOs
    glGenBuffers(1, &m_offsetVBO);
    glGenBuffers(1, &m_colorVBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_offsetVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_amount, nullptr, GL_MAP_WRITE_BIT | GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * m_amount, nullptr, GL_MAP_WRITE_BIT | GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // setUp VAO
    glBindVertexArray(m_VAO);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, m_offsetVBO);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(3, 1);
}

int64_t Emitter::FirstUnusedParticle()
{
    if (!m_deadIndexes.empty()) {
        int64_t res = m_deadIndexes.back();
        m_deadIndexes.pop_back();
        return res;
    } else {
        return -1;
    }
}

// can't be const, ca'z modifies m_rndGenerator
Particle Emitter::GenerateParticle(const glm::vec3& offset)
{
    // we set stddev as R / 4
    std::normal_distribution<> posDistribution(0.0f, m_radius / 4);

    const glm::vec3 random = {posDistribution(m_rndGenerator), 0.0f,
                              posDistribution(m_rndGenerator)};

    const glm::vec3 position = random + offset;
    const glm::vec3 velocity = m_direction * m_velocity;

    const GLfloat fColor = 0.5 + ((rand() % 100) / 100.0f);
    const glm::vec4 color(fColor, fColor, fColor, 1.0f);

    std::normal_distribution<> lifeDistriburion(LIFE_MEAN, LIFE_DEVATION);
    const GLfloat fLife = lifeDistriburion(m_rndGenerator);

    return Particle(position, velocity, color, fLife);
}
