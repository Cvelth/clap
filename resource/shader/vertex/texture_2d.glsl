#version 330 core

in vec3 position;
in vec2 texture_coordinates;

out vec2 fragment_texture_coordinates;

void main() {
    gl_Position = vec4(position, 1.0);
    fragment_texture_coordinates = texture_coordinates;
}