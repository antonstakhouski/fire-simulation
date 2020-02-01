#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

static const GLfloat YAW = -90.0f;
static const GLfloat PITCH = 0.0f;

enum class CameraMovement {
    forward,
    backward,
    left,
    right
};

class Camera {
    public:
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
               glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
               GLfloat yaw = YAW, GLfloat pitch = PITCH);

        Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
               GLfloat upX, GLfloat upY, GLfloat upZ,
               GLfloat yaw, GLfloat pitch);

        glm::mat4 GetViewMatrix();
        void ProcessKeyboard(CameraMovement direction, GLfloat deltaTime);
        void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset,
                                  GLboolean constrainPitch = true);
        void ProcessMouseScroll(GLfloat yoffset);

        GLfloat GetZoom() const { return m_zoom; }

    private:
        void updateCameraVectors();

        glm::vec3 m_position;
        glm::vec3 m_front;
        glm::vec3 m_up;
        glm::vec3 m_right;
        glm::vec3 m_worldUp;

        GLfloat m_yaw;
        GLfloat m_pitch;

        GLfloat m_movementSpeed;
        GLfloat m_mouseSensitivity;

        GLfloat m_zoom;
};
