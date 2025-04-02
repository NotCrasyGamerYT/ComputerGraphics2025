#pragma once

#include "Canis/Debug.hpp"
#include "Canis/Shader.hpp"
#include "Canis/Window.hpp"
#include "Canis/InputManager.hpp"
#include "Canis/Data/GLTexture.hpp"

class World;

class Entity {
public:
    std::string         name;
    glm::vec3           position;
    glm::vec3           scale;
    Canis::Shader       shader;
    Canis::GLTexture    texture;
    glm::vec4           color;

    World *world = nullptr;
    Canis::Window *window = nullptr;
    Canis::InputManager *inputManager = nullptr;

    virtual void Start() {}
    virtual void Update(float _dt) {}
    virtual void Draw() {}
    virtual void OnDestroy() {}
private:
};

static bool EntityOverlap2D(const Entity& a, const Entity& b) {
    float aHalfWidth = a.scale.x * 0.5f;
    float aHalfHeight = a.scale.y * 0.5f;
    float bHalfWidth = b.scale.x * 0.5f;
    float bHalfHeight = b.scale.y * 0.5f;

    bool overlapX = std::abs(a.position.x - b.position.x) <= (aHalfWidth + bHalfWidth);
    bool overlapY = std::abs(a.position.y - b.position.y) <= (aHalfHeight + bHalfHeight);

    return overlapX && overlapY;
}