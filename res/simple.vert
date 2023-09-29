#version 330 core
layout (location = 0) in vec3 pos;

out vec4 vert_col;

void main() {
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
    vert_col = (gl_Position + 1.0) / 2.0;
}
