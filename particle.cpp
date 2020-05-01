#include "particle.h"

#include <iostream>
#include <algorithm>

#include <glm/gtx/vector_angle.hpp>

Particle::Particle(const glm::vec3& position, const glm::vec3& velocity,
                   const glm::vec4& color, GLfloat fLife, GLfloat fScale)
    : m_position(position),
      m_velocity(-velocity),
      m_acceleration(glm::normalize(m_velocity) * 0.02f),
      m_color(color),
      m_fLife(fLife),
      m_fScale(fScale),
      m_fInitialLife(m_fLife),
      m_fInitialScale(m_fScale)
{
}

const glm::vec3& Particle::GetPosition() const
{
    return m_position;
}

const glm::vec4& Particle::GetColor() const
{
    return m_color;
}

GLfloat Particle::GetScale() const
{
    return m_fScale;
}

bool Particle::IsAlive()
{
    return m_fLife > 0.0f;
};

void Particle::UpdateColor()
{
    m_color.a = m_fLife / m_fInitialLife;
}

void Particle::UpdateScale()
{
    m_fScale = m_fInitialScale * (m_fLife / m_fInitialLife);
}

void Particle::UpdatePosition(GLfloat dt)
{
    m_position += m_velocity * dt;
}

void Particle::UpdateVelocity(const std::vector<glm::vec3>& pressurePoints)
{
    const auto& iter = std::upper_bound(
        pressurePoints.begin(), pressurePoints.end(), m_position,
        [](const auto& a, const auto& b) {
        return (glm::length(a) < glm::length(b)) && (a.y < b.y); });

    if (iter != pressurePoints.end()) {
        m_velocity =
            glm::length(m_velocity) * glm::normalize(*iter - m_position) +
            glm::length(m_acceleration);
    }
    else {
        m_velocity += m_acceleration;
    }
}

bool Particle::Update(GLfloat dt, const std::vector<glm::vec3>& pressurePoints)
{
    // can be the cause of underflow
    m_fLife -= dt;
    if (IsAlive()) {
        // particle is alive, thus update
        UpdateColor();
        UpdateScale();
        UpdatePosition(dt);
        UpdateVelocity(pressurePoints);
        return true;
    }
    else {
        return false;
    }
}
