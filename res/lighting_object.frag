#version 330 core
out vec4 frag_col;

in vec3 pos;
in vec3 normal;
in vec2 tex_coord;

uniform sampler2D texture0;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;

void main() {
	vec4 sample = texture(texture0, tex_coord);
	vec3 obj_color = sample.xyz;
	vec3 ambient = light_color * 0.2;
	vec3 norm = normalize(normal);
	vec3 light_dir = normalize(light_pos - pos);
	vec3 diffuse = max(dot(norm, light_dir), 0.0) * light_color;
	vec3 view_dir = normalize(view_pos - pos);
	vec3 reflect_dir = reflect(-light_dir, norm);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
	vec3 specular = 0.5 * spec * light_color;
	vec3 result = (ambient + diffuse + specular) * obj_color;
	frag_col = vec4(result, 1.0);
}
