#version 330 core

in vec2 position;
in vec2 texture_coordinates;

out vec2 geometry_texture_coordinates;

uniform mat4 projection_matrix;
uniform vec2 offset;

void main() {
    gl_Position = projection_matrix * vec4(position + offset, 0.0, 1.0);
    geometry_texture_coordinates = texture_coordinates;
}