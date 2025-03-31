#include "Ball.hpp"

#include "Paddle.hpp"


using namespace glm;

void Ball::Start() {
    name = "Ball";
    position = vec3(window->GetScreenWidth() * 0.5f, window->GetScreenHeight() * 0.5f, 0.0f);
    scale = vec3(100.0f, 100.0f, 0.0f);
}

void Ball::Update(float _dt) {


    if (dir == vec2(0.0f))
    {
        if (inputManager->GetKey(SDL_SCANCODE_SPACE))
        {
            vec2 directions[] = {vec2(1.0f, 1.0f), vec2(1.0f, -1.0f), vec2(-1.0f, 1.0f), vec2(-1.0f, -1.0f)};
            dir = directions[rand()%4];
        }
    }
    
    if (position.y > window->GetScreenHeight() - (scale.y * 0.5f)) {
        position.y = window->GetScreenHeight() - (scale.y * 0.5f);
        dir.y = abs(dir.y) * -1.0f;
    }
    if (position.y < scale.y * 0.5f) {
        position.y = scale.y * 0.5f;
        dir.y = abs(dir.y);
    }

    // detect score
    if (position.x > window->GetScreenWidth() - (scale.x * 0.5f)) {
        position = vec3(window->GetScreenWidth()*0.5f, window->GetScreenHeight()*0.5f, 0.0f);
        dir = vec2(0.0f);
        score_l++;
    }
    if (position.x < scale.x * 0.5f) {
        position = vec3(window->GetScreenWidth()*0.5f, window->GetScreenHeight()*0.5f, 0.0f);
        dir = vec2(0.0f);
        score_r++;
    }

    if (score_l != 5 && score_r != 5)
    {
        window->SetWindowName("Pong" + std::string(" || Blue Score: ") + std::to_string(score_l) + std::string(" || Red Score: ") + std::to_string(score_r));  
    }

    if (score_l == 5)
    {
        window->SetWindowName("Blue wins!");
        position = vec3(window->GetScreenWidth()*0.5f, window->GetScreenHeight()*0.5f, 0.0f);
        dir = vec2(0.0f);
    }

    if (score_r == 5)
    {
        window->SetWindowName("Red wins!");
        position = vec3(window->GetScreenWidth()*0.5f, window->GetScreenHeight()*0.5f, 0.0f);
        dir = vec2(0.0f);
    }


    // detect if ball hits left paddle
    Paddle* leftPaddle = world->FindByName<Paddle>("LeftPaddle"); 
    if (leftPaddle && EntityOverlap2D(*this, *leftPaddle)) { // Check if leftPaddle is not null
        dir.x = abs(dir.x);
        color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // Change color to blue
    }

    // detect if ball hits right paddle
    Paddle* rightPaddle = world->FindByName<Paddle>("RightPaddle"); 
    if (rightPaddle && EntityOverlap2D(*this, *rightPaddle)) { // Check if rightPaddle is not null
        dir.x = abs(dir.x) * -1.0f;
        color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // Change color to red
    }

    if (dir != vec2(0.0f))
        position += vec3(dir.x, dir.y, 0.0f) * speed * _dt;
}

void Ball::Draw() {
    mat4 transform = mat4(1.0f);
    transform = translate(transform, position);
    transform = glm::scale(transform, scale);

    // Set shader variables
    shader.SetVec4("COLOR", color);
    shader.SetMat4("TRANSFORM", transform);

    // Draw the ball
    renderer->Draw(transform, shader);

    // Draw the trail
    DrawTrail();
}

void Ball::OnDestroy() {

}

void Ball::DrawTrail() {
    static std::vector<vec3> trailPositions;
    static const size_t maxTrailSize = 100;

    // Add the current position to the trail
    trailPositions.push_back(position);
    if (trailPositions.size() > maxTrailSize) {
        trailPositions.erase(trailPositions.begin());
    }

    // Render the trail
    for (const auto& trailPos : trailPositions) {
        mat4 trailTransform = mat4(1.0f);
        trailTransform = translate(trailTransform, trailPos);
        trailTransform = glm::scale(trailTransform, vec3(10.0f, 10.0f, 0.0f));

        shader.SetVec4("COLOR", glm::vec4(1.0f, 1.0f, 1.0f, 0.5f)); // Semi-transparent white
        shader.SetMat4("TRANSFORM", trailTransform);

        if (renderer) { // Ensure renderer is not null
            renderer->Draw(trailTransform, shader);
        }
    }
}