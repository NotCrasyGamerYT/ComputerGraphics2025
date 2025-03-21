#include "Paddle.hpp"

#include "World.hpp"

#include "Ball.hpp"

using namespace glm;

void Paddle::Start()
{
    scale = vec3(20.0f, 100.0f, 0.0f);
}

void Paddle::Update(float _dt)
{
    vec2 dir;

    if (name == "LeftPaddle")
    {

        dir.y += inputManager->GetKey(SDL_SCANCODE_W);
        dir.y += inputManager->GetKey(SDL_SCANCODE_S) * -1;
        Ball *ball = world->FindByName<Ball>("Ball");
        if (EntityOverlap2D(*this, *ball))
        {
            if (!overlap_l)
            {
                overlap_l = true;
            }
        }
        if (overlap_l)
        {
            position.x -= 5.0f;
            if (position.x < -10.0f)
            {
                overlap_l = false;
                position.x = 5.0f;
            }
        }
    }
    else if (name == "RightPaddle")
    {
        dir.y += inputManager->GetKey(SDL_SCANCODE_UP);
        dir.y += inputManager->GetKey(SDL_SCANCODE_DOWN) * -1;
        Ball *ball = world->FindByName<Ball>("Ball");
        if (EntityOverlap2D(*this, *ball))
        {
            if (!overlap_r)
            {
                overlap_r = true;
            }
        }
        if (overlap_r)
        {
            position.x += 5.0f;
            if (position.x > 645.0f)
            {
                overlap_r = false;
                position.x = 635.0f;
            }
        }
    }

    position.y += dir.y * speed * _dt;

    if (position.y > window->GetScreenHeight() - (scale.y * 0.5f))
        position.y = window->GetScreenHeight() - (scale.y * 0.5f);
    if (position.y < scale.y * 0.5f)
        position.y = scale.y * 0.5f;
}

void Paddle::Draw()
{
    mat4 transform = mat4(1.0f);
    transform = translate(transform, position);
    transform = glm::scale(transform, scale);

    // set shader variables
    shader.SetVec4("COLOR", color);
    shader.SetMat4("TRANSFORM", transform);
}

void Paddle::OnDestroy()
{
}