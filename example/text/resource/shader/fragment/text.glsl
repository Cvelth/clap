#version 330 core

in vec2 fragment_texture_coordinates;

out vec4 color_output;

uniform sampler2D font_atlas;

void main() {
    color_output = vec4(1, 1, 1, 
        texture(font_atlas, fragment_texture_coordinates).r
    );
}