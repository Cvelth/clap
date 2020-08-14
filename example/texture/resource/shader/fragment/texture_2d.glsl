#version 330 core

in vec2 fragment_texture_coordinates;

out vec4 color_output;

uniform sampler2D texture_sampler;

void main() {
    color_output = texture(texture_sampler, fragment_texture_coordinates);
} 