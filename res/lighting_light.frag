#version 330 core
out vec4 frag_col;

uniform vec3 light_color;

void main() {
    frag_col = vec4(light_color, 1.0);
}
