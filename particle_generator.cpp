/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "particle_generator.h"

#include <iostream>

ParticleGenerator::ParticleGenerator(const Shader& shader,
                                     const Texture2D& texture,
                                     const glm::vec3& position,
                                     const glm::vec3& velocity, GLuint amount)
    : m_shader(shader),
      m_texture(texture),
      m_amount(amount),
      m_position(position),
      m_velocity(velocity),
      m_lastUsedParticle(0)
{
    Init();
}

void ParticleGenerator::Update(GLfloat dt, GLuint newParticles,
                               const glm::vec3& offset)
{
    // Add new particles
    for (GLuint i = 0; i < newParticles; ++i)
    {
        int unusedParticle = FirstUnusedParticle();
        RespawnParticle(m_particles[unusedParticle], offset);
    }

    // Update all particles
    for (GLuint i = 0; i < m_amount; ++i)
    {
        Particle& p = m_particles[i];
        p.Life -= dt; // reduce life
        if (p.Life > 0.0f)
        {	// particle is alive, thus update
            p.Position -= p.Velocity * dt;
            p.Color.a = p.Life / p.InitialLife;
        }
    }
}

// Render all particles
void ParticleGenerator::Draw()
{
    // Use additive blending to give it a 'glow' effect
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    m_shader.Use();

    glm::vec3* ptrOffset = static_cast<glm::vec3*>(glMapNamedBuffer(m_offsetVBO, GL_WRITE_ONLY));
    glm::vec4* ptrColors = static_cast<glm::vec4*>(glMapNamedBuffer(m_colorVBO, GL_WRITE_ONLY));

    for (size_t i = 0; i < m_amount; ++i) {
        if (m_particles[i].Life > 0.0f) {
            ptrOffset[i] = m_particles[i].Position;
            ptrColors[i] = m_particles[i].Color;
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

void ParticleGenerator::Init()
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

GLuint ParticleGenerator::FirstUnusedParticle()
{
    // First search from last used particle, this will usually return almost instantly
    for (GLuint i = m_lastUsedParticle; i < m_amount; ++i) {
        if (m_particles[i].Life <= 0.0f) {
            m_lastUsedParticle = i;
            return i;
        }
    }
    // Otherwise, do a linear search
    for (GLuint i = 0; i < m_lastUsedParticle; ++i) {
        if (m_particles[i].Life <= 0.0f) {
            m_lastUsedParticle = i;
            return i;
        }
    }
    // All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    m_lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::RespawnParticle(Particle &particle, const glm::vec3& offset)
{
    std::normal_distribution<double> xPosDistriburion(0, 0.5f);
    std::normal_distribution<double> zPosDistriburion(0, 0.5f);
    const glm::vec3 random = {xPosDistriburion(m_rndGenerator), 0.0f,
                              zPosDistriburion(m_rndGenerator)};

    GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
    particle.Position = random + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);

    // std::normal_distribution<double> xPosDistriburion(0, 50);
    std::normal_distribution<double> lifeDistriburion(3.0, 1.0);
    particle.Life = lifeDistriburion(m_rndGenerator);
    particle.InitialLife = particle.Life;

    particle.Velocity = m_velocity;
}
