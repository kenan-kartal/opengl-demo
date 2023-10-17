#version 330 core
out vec4 frag_col;

in vec3 col;
in vec2 tex_coord;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main() {
    frag_col = mix(texture(texture0, tex_coord), texture(texture1, tex_coord), tex_coord.x);
}
