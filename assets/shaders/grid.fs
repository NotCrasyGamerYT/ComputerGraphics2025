#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform float time;

void main()
{
    vec2 gridSize = vec2(10.0);  // Grid cell size
    vec2 movingGrid = TexCoord * gridSize;

    // Move the grid diagonally over time
    movingGrid += vec2(time * 1.0, time * 1.0); 

    vec2 grid = fract(movingGrid);

    float lineThickness = 0.1;
    float gridLines = step(grid.x, lineThickness) + step(grid.y, lineThickness);

    vec4 gridColor = vec4(0.3, 0.3, 0.3, 1.0); // Dark gray grid
    FragColor = mix(vec4(0.0, 0.0, 0.0, 1.0), gridColor, gridLines); // Blend with black background
}
