#version 330 core
out vec4 frag_col;

in vec2 tex_coord;

uniform sampler2D texture0;

void main() {
    frag_col = texture(texture0, tex_coord);
}
