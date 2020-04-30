#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <vector>

// Represents a single particle and its state
class Particle {
public:
    Particle(const glm::vec3& position = glm::vec3(0.0f),
             const glm::vec3& velocity = glm::vec3(0.0f),
             const glm::vec4& color = glm::vec4(1.0f),
             GLfloat fLife = 0.0f,
             GLfloat fScale = 0.0f);

    bool Update(GLfloat dt, const std::vector<glm::vec3>& m_pressurePoints);
    const glm::vec3& GetPosition() const;
    const glm::vec4& GetColor() const;
    GLfloat GetScale() const;
    bool IsAlive();

private:
    void UpdateColor();
    void UpdateScale();
    void UpdatePosition(GLfloat dt);
    void UpdateVelocity(const std::vector<glm::vec3>& pressurePoints);

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
