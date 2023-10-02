#version 330 core
out vec4 frag_col;

in vec4 vert_col;

void main() {
    frag_col = vert_col;
}
