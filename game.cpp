/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game.h"
#include "resource_manager.h"

#include <iostream>

#define N_PARTICLES 500

Game::Game(GLuint width, GLuint height)
    : m_state(GameState::active),
      m_mouseXOffset(0.0f),
      m_mouseYOffset(0.0f),
      m_width(width),
      m_height(height),
      m_camera(glm::vec3(0.0f, 0.0f, 3.0f))
{
}

Game::~Game()
{
}

void Game::SetMouseMovement(GLfloat xoffset, GLfloat yoffset)
{
    m_mouseXOffset = xoffset;
    m_mouseYOffset = yoffset;
}

void Game::SetMouseScroll(GLfloat xoffset, GLfloat yoffset)
{
    m_scrollXOffset = xoffset;
    m_scrollYOffset = yoffset;
}

void Game::Init()
{
    // Load shaders
    ResourceManager::LoadShader("shaders/particle.vs", "shaders/particle.fs", nullptr, "particle");

    // Load textures
    ResourceManager::GetShader("particle").Use().SetInteger("particle", 0);
    ResourceManager::LoadTexture("textures/fire.png", GL_TRUE, "particle");

    m_ptrParticles.reset(
        new ParticleGenerator(ResourceManager::GetShader("particle"),
                              ResourceManager::GetTexture("particle"),
                              glm::vec3(50, 0, 0), glm::vec3(0, -5, 0), N_PARTICLES));
}

void Game::Update(GLfloat dt)
{
    // Update particles
    if (m_ptrParticles) {
        m_ptrParticles->Update(dt, 20);
    }
}

void Game::ProcessInput(GLfloat dt)
{
    for (size_t key = 0; key < N_KEYS; ++key) {
        if ((key == GLFW_KEY_W) && m_keys[key]) {
            m_camera.ProcessKeyboard(CameraMovement::forward, dt);
            std::cout << "FORWARD" << std::endl;
        }
        if ((key == GLFW_KEY_S) && m_keys[key]) {
            m_camera.ProcessKeyboard(CameraMovement::backward, dt);
            std::cout << "BACKFORWARD" << std::endl;
        }
        if ((key == GLFW_KEY_A) && m_keys[key]) {
            m_camera.ProcessKeyboard(CameraMovement::left, dt);
            std::cout << "LEFT" << std::endl;
        }
        if ((key == GLFW_KEY_D) && m_keys[key]) {
            m_camera.ProcessKeyboard(CameraMovement::right, dt);
            std::cout << "RIGHT" << std::endl;
        }
    }

    m_camera.ProcessMouseMovement(m_mouseXOffset, m_mouseYOffset);
    m_camera.ProcessMouseScroll(m_scrollYOffset);

    // we need to reset offsets manually
    m_mouseXOffset = 0.0f;
    m_mouseYOffset = 0.0f;

    m_scrollXOffset = 0.0f;
    m_scrollYOffset = 0.0f;
}

void Game::Render()
{
    if (m_state == GameState::active)
    {
        // Update projection and view matrices
        const glm::mat4 projection = glm::perspective(
            m_camera.GetZoom(),
            static_cast<GLfloat>(m_width) / static_cast<GLfloat>(m_height), 0.1f,
            100.0f);
        const glm::mat4 view = m_camera.GetViewMatrix();
        ResourceManager::GetShader("particle").Use().SetMatrix4("projection", projection);
        ResourceManager::GetShader("particle").SetMatrix4("view", view);

        // Draw particles
        if (m_ptrParticles) {
            m_ptrParticles->Draw();
        }
    }
}
