#version 330 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_tex_coord;

out vec3 pos;
out vec3 normal;
out vec2 tex_coord;

uniform mat4 model;
uniform mat4 model_inv_trans;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(in_pos, 1.0);
	pos = vec3(model * vec4(in_pos, 1.0));
	normal = mat3(model_inv_trans) * in_normal;
	tex_coord = in_tex_coord;
}
