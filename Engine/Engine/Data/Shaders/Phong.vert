#version 330 core 

layout(location = 0) in vec3 in_position; 
layout(location = 1) in vec3 in_color; 
layout(location = 2) in vec2 in_texCords;
layout(location = 3) in vec3 in_normal;

out vec4 v2f_color; 
out vec2 v2f_texCords;
out vec3 v2f_normal;
out vec3 v2f_fragPos;

uniform mat4 model;
uniform mat4 projection;	
uniform mat4 view;			


void main() 
{ 
	mat4 mvp = projection * view * model;
	gl_Position = mvp * vec4(in_position, 1); 
	
	v2f_color = vec4(in_color, 1);
	v2f_texCords = in_texCords;

	mat3 normalMatrix = mat3(mvp);
	normalMatrix = inverse(normalMatrix);
	normalMatrix = transpose(normalMatrix);

	v2f_normal = normalize(in_normal * normalMatrix);
	v2f_fragPos = vec3(model * vec4(in_position, 1));
}