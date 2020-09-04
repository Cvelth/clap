#version 330 core

layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;

in vec2 geometry_texture_coordinates[];

out vec2 fragment_texture_coordinates;

void main() {
    vec2 top_left = vec2(gl_in[0].gl_Position);
    vec2 bottom_right = vec2(gl_in[1].gl_Position);

    gl_Position = vec4(top_left.x, top_left.y, 0, 1);
    fragment_texture_coordinates = vec2(geometry_texture_coordinates[0].x, 
                                        geometry_texture_coordinates[0].y);
    EmitVertex();

    gl_Position = vec4(top_left.x, bottom_right.y, 0, 1);
    fragment_texture_coordinates = vec2(geometry_texture_coordinates[0].x, 
                                        geometry_texture_coordinates[1].y);
    EmitVertex();

    gl_Position = vec4(bottom_right.x, top_left.y, 0, 1);
    fragment_texture_coordinates = vec2(geometry_texture_coordinates[1].x, 
                                        geometry_texture_coordinates[0].y);
    EmitVertex();

    gl_Position = vec4(bottom_right.x, bottom_right.y, 0, 1);
    fragment_texture_coordinates = vec2(geometry_texture_coordinates[1].x, 
                                        geometry_texture_coordinates[1].y);
    EmitVertex();

    EndPrimitive();
}