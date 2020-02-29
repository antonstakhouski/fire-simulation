#include "particle.h"

Particle::Particle(const glm::vec3& position, const glm::vec3& velocity,
                   const glm::vec4& color, GLfloat fLife)
    : m_position(position),
      m_velocity(velocity),
      m_color(color),
      m_fLife(fLife),
      m_fInitialLife(m_fLife)
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

bool Particle::IsAlive()
{
    return m_fLife > 0.0f;
};

void Particle::UpdateColor()
{
    m_color.a = m_fLife / m_fInitialLife;
}

void Particle::UpdatePosition(GLfloat dt)
{
    m_position -= m_velocity * dt;
}

bool Particle::Update(GLfloat dt)
{
    // can be the cause of underflow
    m_fLife -= dt;
    if (IsAlive()) {
        // particle is alive, thus update
        UpdateColor();
        UpdatePosition(dt);
        return true;
    }
    else {
        return false;
    }
}
