#version 330 core

in vec4 fragment_color;

out vec4 color_output;

void main() {
    color_output = fragment_color;
} 