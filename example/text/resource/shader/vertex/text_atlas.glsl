#version 330 core

in vec2 position;
in vec2 texture_coordinates;

out vec2 fragment_texture_coordinates;

uniform mat4 projection_matrix;

void main() {
    gl_Position = projection_matrix * vec4(position, 0.0, 1.0);
    fragment_texture_coordinates = texture_coordinates;
}