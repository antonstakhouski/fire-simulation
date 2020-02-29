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
#include <GLFW/glfw3.h>

#include <memory>
#include <random>

#include "camera.h"
#include "emitter.h"

#define N_KEYS 1024

class FPSMeter {
public:
    FPSMeter();
    void Count(GLfloat dt);

private:
    GLfloat m_time;
    size_t m_nFrames;
};

// Represents the current state of the game
enum class GameState { active, menu, win };

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game {
public:
    // Constructor/Destructor
    Game(GLuint width, GLuint height);
    ~Game();
    // Initialize game state (load all shaders/textures/levels)
    void Init();
    // GameLoop
    void ProcessInput(GLfloat dt);
    void Update(GLfloat dt);
    void Render();

    void SetState(GameState state) { m_state = state; }
    void SetKey(size_t key, GLboolean value) { m_keys[key] = value; }

    void SetWidth(GLuint width) { m_width = width; }
    void SetHeight(GLuint height) { m_height = height; }

    void SetMouseMovement(GLfloat xoffset, GLfloat yoffset);
    void SetMouseScroll(GLfloat xoffset, GLfloat yoffset);

private:
    // Game state
    GameState m_state;
    GLboolean m_keys[N_KEYS] = {GL_FALSE};

    GLfloat m_mouseXOffset;
    GLfloat m_mouseYOffset;

    GLfloat m_scrollXOffset;
    GLfloat m_scrollYOffset;

    GLuint m_width;
    GLuint m_height;

    Camera m_camera;

    // Game-related State data
    std::unique_ptr<Emitter> m_ptrParticles;
    std::default_random_engine m_rndGenerator;

    FPSMeter m_fpsMeter;
};
