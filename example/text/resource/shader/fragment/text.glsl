#version 330 core

in vec2 fragment_texture_coordinates;

out vec4 color_output;

uniform sampler2D texture;

void main() {
    color_output = texture(texture, fragment_texture_coordinates);
}