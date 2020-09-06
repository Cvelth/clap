#version 330 core

in vec3 fragment_color;
in vec2 fragment_texture_coordinates;

uniform uint mode;
uniform sampler2D texture_sampler;

out vec4 color_output;

void main() {
    vec4 texture_color = texture(texture_sampler, 
                                 fragment_texture_coordinates);
    if (mode == 1u)
        color_output = texture_color;
    else if (mode == 2u)
        color_output = (texture_color + vec4(fragment_color, 1.f)) / 2.f;
    else
        color_output = vec4(fragment_color, 1.0);
} 