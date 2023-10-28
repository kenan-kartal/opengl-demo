#version 330 core
out vec4 frag_col;

in vec2 tex_coord;

uniform sampler2D texture0;
uniform vec3 light_color;

void main() {
	vec4 sample = texture(texture0, tex_coord);
	vec3 obj_color = sample.xyz;
	vec3 ambient = light_color * 0.2;
	vec3 result = ambient * obj_color;
	frag_col = vec4(result, 1.0);
}
