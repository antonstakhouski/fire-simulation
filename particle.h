#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>

// Represents a single particle and its state
class Particle {
public:
    Particle(const glm::vec3& position = glm::vec3(0.0f),
             const glm::vec3& velocity = glm::vec3(0.0f),
             const glm::vec4& color = glm::vec4(1.0f),
             GLfloat fLife = 0.0f,
             GLfloat fScale = 0.0f);

    bool Update(GLfloat dt);
    const glm::vec3& GetPosition() const;
    const glm::vec4& GetColor() const;
    GLfloat GetScale() const;
    bool IsAlive();

private:
    void UpdateColor();
    void UpdateScale();
    void UpdatePosition(GLfloat dt);
    void UpdateVelocity();

    glm::vec3 m_position;
    glm::vec3 m_velocity;
    glm::vec3 m_acceleration;
    glm::vec4 m_color;
    GLfloat m_fLife;
    GLfloat m_fScale;

    // not-const to help compiler generate default constructor
    GLfloat m_fInitialLife;
    GLfloat m_fInitialScale;
};
