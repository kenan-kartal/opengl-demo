#version 330 core
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

out vec4 frag_col;

in vec3 pos;
in vec3 normal;

uniform Material material;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;

void main() {
	vec3 ambient = light_color * material.ambient;
	vec3 norm = normalize(normal);
	vec3 light_dir = normalize(light_pos - pos);
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = diff * light_color * material.diffuse;
	vec3 view_dir = normalize(view_pos - pos);
	vec3 reflect_dir = reflect(-light_dir, norm);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
	vec3 specular = spec * light_color * material.specular;
	vec3 result = ambient + diffuse + specular;
	frag_col = vec4(result, 1.0);
}
