#version 330 core
out vec4 frag_col;

in vec2 tex_coord;

uniform sampler2D texture0;
uniform vec3 light_color;

void main() {
    frag_col = texture(texture0, tex_coord) * vec4(light_color, 1.0);
}
