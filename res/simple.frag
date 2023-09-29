#version 330 core
out vec4 frag_col;

in vec4 vert_col;

uniform vec4 my_col;

void main() {
    frag_col = my_col;
}
