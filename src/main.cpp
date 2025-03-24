#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>

#include <SDL.h>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "Canis/Canis.hpp"
#include "Canis/IOManager.hpp"
#include "Canis/FrameRateManager.hpp"

#include "Entity.hpp"
#include "Ball.hpp"
#include "Paddle.hpp"

// git restore .
// git fetch
// git pull

// move out to external class
unsigned int vertexShader;
unsigned int VBO, VAO, EBO;
unsigned int gridVBO, gridVAO;

void InitModel();
void InitGrid();

#ifdef _WIN32
#define main SDL_main
extern "C" int main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    Canis::Init();

    Canis::Window window;
    window.Create("Computer Graphics 2025", 640, 640, 0);

    Canis::InputManager inputManager;
    Canis::FrameRateManager frameRateManager;
    frameRateManager.Init(60.0f);
    float deltaTime = 0.0f;
    float fps = 0.0f;

    Canis::Shader gridshader;
    gridshader.Compile("assets/shaders/grid.vs", "assets/shaders/grid.fs");
    gridshader.Link();

    Canis::Shader spriteShader;
    spriteShader.Compile("assets/shaders/sprite.vs", "assets/shaders/sprite.fs");
    spriteShader.AddAttribute("aPos");
    spriteShader.AddAttribute("aUV");
    spriteShader.Link();

    InitModel();
    InitGrid();

    Canis::GLTexture texture = Canis::LoadImageGL("assets/textures/ForcePush.png", true);

    int textureSlots = 0;

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureSlots);

    Canis::Log(std::to_string(textureSlots));

    spriteShader.SetInt("texture1", 0);

  //  glActiveTexture(GL_TEXTURE0 + 0);
  //  glBindTexture(GL_TEXTURE_2D, texture.id);

    World world;
    world.VAO = VAO;
    world.window = &window;
    world.inputManager = &inputManager;

    Ball *ball = world.Instantiate<Ball>();
    ball->shader = spriteShader;
    ball->texture = texture;
    ball->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    {
        Paddle *paddle = world.Instantiate<Paddle>();
        paddle->shader = spriteShader;
        paddle->texture = texture;
        paddle->name = "RightPaddle";
        paddle->color = glm::vec4(01.0f, 0.0f, 0.0f, 1.0f); //red
        paddle->position = glm::vec3(window.GetScreenWidth() - (10.0f*0.5f), window.GetScreenHeight() * 0.5f, 0.0f);
    }

    {
        Paddle *paddle = world.Instantiate<Paddle>();
        paddle->shader = spriteShader;
        paddle->texture = texture;
        paddle->name = "LeftPaddle";
        paddle->color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); //blue
        paddle->position = glm::vec3(10.0f*0.5f, window.GetScreenHeight() * 0.5f, 0.0f);
    }

    float time = 0.0f;

    while (inputManager.Update(window.GetScreenWidth(), window.GetScreenHeight()))
    {
        deltaTime = frameRateManager.StartFrame();
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f);

        time += deltaTime;

        glClear(GL_COLOR_BUFFER_BIT);

        using namespace glm;

        mat4 projection = ortho(0.0f, (float)window.GetScreenWidth(), 0.0f, (float)window.GetScreenHeight(), 0.001f, 100.0f);
        
        mat4 view = mat4(1.0f);
        view = translate(view, vec3(0.0f, 0.0f, 0.5f));
        view = inverse(view);

        gridshader.Use();
        gridshader.SetFloat("time", time);

        glBindVertexArray(gridVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        world.Update(view, projection, deltaTime);

        window.SwapBuffer();

        fps = frameRateManager.EndFrame();
    }

    return 0;
}

void InitModel()
{
    float vertices[] = {
        // position         // uv
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // top left
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void InitGrid()
{
    float quadVertices[] = {
        // positions    // texCoords
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,  // Top-left
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,  // Bottom-left
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,  // Bottom-right
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f   // Top-right
    };
    
    unsigned int indices[] = {
        0, 1, 2, // First triangle
        0, 2, 3  // Second triangle
    };

    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);
    
    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}