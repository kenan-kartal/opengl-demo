#version 330 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_col;
layout (location = 2) in vec2 in_tex_coord;

out vec3 col;
out vec2 tex_coord;

void main() {
    gl_Position = vec4(in_pos.x, in_pos.y, in_pos.z, 1.0);
    col = in_col;
    tex_coord = in_tex_coord;
}
