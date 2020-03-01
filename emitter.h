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
#include <memory>

#include "particle.h"
#include "shader.h"
#include "texture.h"

// Emitter acts as a container for rendering a large number of
// particles by repeatedly spawning and updating particles and killing
// them after a given amount of time.
class Emitter {
public:
    // Constructor
    Emitter(const Shader& shader,
            const Texture2D& texture,
            const glm::vec3& position,
            const glm::vec3& direction,
            GLfloat radius,
            GLfloat energy,
            GLfloat velocity,
            GLuint amount);

    // TODO: pass Object that is on fire here
    // Update all particles
    void Update(GLfloat dt, GLuint nNewParticles,
                const glm::vec3& offset = glm::vec3(0.0f));
    // Render all particles
    void Draw();
    bool IsAlive() const;

private:
    // Initializes buffer and vertex attributes
    void Init();
    // Returns the first Particle index that's currently unused e.g. Life <=
    // 0.0f or 0 if no particle is currently inactive
    int64_t FirstUnusedParticle();
    Particle GenerateParticle(const glm::vec3& offset);

    // Render state
    Shader m_shader;
    Texture2D m_texture;
    GLuint m_VAO;

    // State
    std::vector<Particle> m_particles;
    std::vector<size_t> m_deadIndexes;
    const size_t m_amount;

    const glm::vec3 m_position;
    const glm::vec3 m_direction;
    const GLfloat m_radius;
    GLfloat m_energy;
    const GLfloat m_velocity;

    // Stores the index of the last particle used (for quick access to next dead particle)
    GLuint m_lastUsedParticle;

    GLuint m_offsetVBO;
    GLuint m_colorVBO;
    GLuint m_scaleVBO;

    std::default_random_engine m_rndGenerator;
};
