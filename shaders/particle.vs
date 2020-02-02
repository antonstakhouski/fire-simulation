#version 450 core

layout (location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aOffset;
layout(location = 3) in vec4 aColor;

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    const float scale = 0.05f;
    TexCoords = aTexCoords;
    ParticleColor = aColor;
    gl_Position = projection * view * model * vec4((aPos * scale) + aOffset, 1.0);
}
