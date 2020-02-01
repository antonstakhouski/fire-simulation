/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <random>
#include <vector>

#include "shader.h"
#include "texture.h"

// Represents a single particle and its state
struct Particle {
    glm::vec3 Position, Velocity;
    glm::vec4 Color;
    GLfloat Life;
    GLfloat InitialLife;

    Particle()
        : Position(0.0f),
          Velocity(0.0f),
          Color(1.0f),
          Life(0.0f),
          InitialLife(0.0f)
    {
    }
};

// ParticleGenerator acts as a container for rendering a large number of
// particles by repeatedly spawning and updating particles and killing
// them after a given amount of time.
class ParticleGenerator {
public:
    // Constructor
    ParticleGenerator(const Shader& shader, const Texture2D& texture,
                      const glm::vec3& position, const glm::vec3& velocity,
                      GLuint amount);

    // TODO: pass Object that is on fire here
    // Update all particles
    void Update(GLfloat dt, GLuint newParticles,
                const glm::vec3& offset = glm::vec3(0.0f));
    // Render all particles
    void Draw();

private:
    // Initializes buffer and vertex attributes
    void Init();
    // Returns the first Particle index that's currently unused e.g. Life <=
    // 0.0f or 0 if no particle is currently inactive
    GLuint FirstUnusedParticle();
    // Respawns particle
    void RespawnParticle(Particle& particle,
                         const glm::vec3& offset = glm::vec3(0.0f));
    // Render state
    Shader m_shader;
    Texture2D m_texture;
    GLuint m_vao;

    // State
    std::vector<Particle> m_particles;
    const GLuint m_amount;

    const glm::vec3 m_position;
    const glm::vec3 m_velocity;

    std::default_random_engine m_rndGenerator;

    // Stores the index of the last particle used (for quick access to next dead particle)
    GLuint m_lastUsedParticle;
};
