#pragma once

#include "Entity.hpp"
#include "World.hpp"
#include "Renderer.hpp" // Include the renderer header

class Ball : public Entity {
public:
    Ball() : renderer(nullptr) {} // Initialize renderer to null
    void Start() override;
    void Update(float _dt) override;
    void Draw() override;
    void OnDestroy() override;
    void DrawTrail();

private:
    Renderer* renderer; // No need to explicitly set to nullptr here
};