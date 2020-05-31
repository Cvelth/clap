#version 330 core

in vec3 in_position;
in vec4 in_color;

out vec4 fragment_color;

void main() {
    gl_Position = vec4(in_position, 1.0);
    fragment_color = in_color;
}