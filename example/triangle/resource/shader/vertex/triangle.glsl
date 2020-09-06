#version 330 core

in vec2 position;
in vec3 color;
in vec2 texture_coordinates;

out vec3 fragment_color;
out vec2 fragment_texture_coordinates;

void main() {
    gl_Position = vec4(position, 0.0, 1.0);
    fragment_color = color;
    fragment_texture_coordinates = texture_coordinates;
}