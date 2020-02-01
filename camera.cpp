#include "camera.h"

static const GLfloat SPEED = 2.5f;
static const GLfloat SENSITIVITY = 0.1f;
static const GLfloat ZOOM = 45.0f;

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
    : m_position(position),
      m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
      m_worldUp(up),
      m_yaw(yaw),
      m_pitch(pitch),
      m_movementSpeed(SPEED),
      m_mouseSensitivity(SENSITIVITY),
      m_zoom(ZOOM)
{
    updateCameraVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX,
               GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch)
    : m_position(glm::vec3(posX, posY, posZ)),
      m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
      m_worldUp(glm::vec3(upX, upY, upZ)),
      m_yaw(yaw),
      m_pitch(pitch),
      m_movementSpeed(SPEED),
      m_mouseSensitivity(SENSITIVITY),
      m_zoom(ZOOM)
{
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::ProcessKeyboard(CameraMovement direction, GLfloat deltaTime)
{
    const GLfloat velocity = m_movementSpeed * deltaTime;
    switch (direction) {
        case CameraMovement::forward:
            m_position += glm::vec3(m_front.x, m_front.y, m_front.z) * velocity;
            break;
        case CameraMovement::backward:
            m_position -= glm::vec3(m_front.x, m_front.y, m_front.z) * velocity;
            break;
        case CameraMovement::left:
            m_position -= glm::vec3(m_right.x, m_right.y, m_right.z) * velocity;
            break;
        case CameraMovement::right:
            m_position += glm::vec3(m_right.x, m_right.y, m_right.z) * velocity;
    }
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    m_pitch = constrainPitch ? glm::clamp(m_pitch, -89.0f, 89.0f) : m_pitch;

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat yoffset)
{
    m_zoom = glm::clamp(m_zoom - yoffset, 1.0f, 45.0f);
}

void Camera::updateCameraVectors()
{
    const glm::vec3 front(
        cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),
        sin(glm::radians(m_pitch)),
        sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
