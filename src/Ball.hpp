#pragma once

#include "Entity.hpp"

#include "World.hpp"

#include <deque>

class Ball : public Entity {
public:
    void Start();
    void Update(float _dt);
    void Draw();
    void OnDestroy();

    int score_l = 0;
    int score_r = 0;

    bool isRainingBall = false;
    bool isGameOver = false;  
    float gameOverTime = 0.0f; 

    float speed = 80.0f;
    glm::vec2 dir = glm::vec2(0.0f, 0.0f);

    std::deque<glm::vec3> trailPositions; // Store past positions
    int trailLength = 10; // Number of trail points
};