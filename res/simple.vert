#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;

out vec4 vert_col;

void main() {
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
    vert_col = vec4(col, 1.0);
}
