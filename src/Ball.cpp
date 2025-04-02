#include "Ball.hpp"

#include "Paddle.hpp"


using namespace glm;

void Ball::Start() {
    name = "Ball";
    position = vec3(window->GetScreenWidth() * 0.5f, window->GetScreenHeight() * 0.5f, 0.0f);
    scale = vec3(100.0f, 100.0f, 0.0f);
    window->SetWindowName("Pong || Blue Score: 0 || Red Score: 0");
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

if (score_l == 5 || score_r == 5) {
    if (!isGameOver) {
        isGameOver = true;
        gameOverTime = SDL_GetTicks() / 1000.0f; // Start the 5-second timer
    }

    window->SetWindowName(score_l == 5 ? "Blue wins!" : "Red wins!");
    position = vec3(window->GetScreenWidth() * 0.5f, window->GetScreenHeight() * 0.5f, 0.0f);
    dir = vec2(0.0f);

    glm::vec4 winnerColor = (score_l == 5) ? glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) : glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

    static float lastSpawnTime = 0.0f;
    if ((SDL_GetTicks() / 1000.0f) - lastSpawnTime > 0.2f) { 
        lastSpawnTime = SDL_GetTicks() / 1000.0f;

        Ball* newBall = world->Instantiate<Ball>();
        if (newBall) {
            newBall->isRainingBall = true;
            newBall->position = vec3(rand() % window->GetScreenWidth(), window->GetScreenHeight(), 0.0f);
            newBall->dir = glm::vec2((rand() % 10 - 5) * 0.1f, -1.0f);
            newBall->speed = 200.0f;
            newBall->color = winnerColor;
            newBall->shader = shader;
            newBall->texture = texture;
        }
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
if (EntityOverlap2D(*this, *leftPaddle)) {
    dir.x = abs(dir.x);
    color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // Change color to blue
}

    // detect if ball hits right paddle
    Paddle* rightPaddle = world->FindByName<Paddle>("RightPaddle"); 
    if (EntityOverlap2D(*this ,*rightPaddle)) {
        dir.x = abs(dir.x) * -1.0f;
        color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // Change color to red
    }
    
    for (Entity* b : toDestroy) {
        world->Destroy(b);
    }
}


    // detect score (ignore raining balls)
    if (!isRainingBall) {
        if (position.x > window->GetScreenWidth() - (scale.x * 0.5f)) {
            position = vec3(window->GetScreenWidth() * 0.5f, window->GetScreenHeight() * 0.5f, 0.0f);
            dir = vec2(0.0f);
            score_l++;
            window->SetWindowName("Pong || Blue Score: " + std::to_string(score_l) + " || Red Score: " + std::to_string(score_r));
        }
        if (position.x < scale.x * 0.5f) {
            position = vec3(window->GetScreenWidth() * 0.5f, window->GetScreenHeight() * 0.5f, 0.0f);
            dir = vec2(0.0f);
            score_r++;
            window->SetWindowName("Pong || Blue Score: " + std::to_string(score_l) + " || Red Score: " + std::to_string(score_r));
        }
    }

// Prevent raining balls from changing color
if (!isRainingBall) {
    Paddle* leftPaddle = world->FindByName<Paddle>("LeftPaddle"); 
    if (EntityOverlap2D(*this, *leftPaddle)) {
        dir.x = abs(dir.x);
        color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    }

    Paddle* rightPaddle = world->FindByName<Paddle>("RightPaddle"); 
    if (EntityOverlap2D(*this, *rightPaddle)) {
        dir.x = abs(dir.x) * -1.0f;
        color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    }
}

    if (dir != vec2(0.0f))
        position += vec3(dir.x, dir.y, 0.0f) * speed * _dt;
} 

void Ball::Draw() {mat4 transform = mat4(1.0f);
    transform = translate(transform, position);
    transform = glm::scale(transform, scale);

    // set shader variables
    shader.SetVec4("COLOR", color);
    shader.SetMat4("TRANSFORM", transform);
}

void Ball::OnDestroy() {
    
}