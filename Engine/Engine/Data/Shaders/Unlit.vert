#version 330 core 

layout(location = 0) in vec3 in_position; 
layout(location = 1) in vec3 in_color; 
layout(location = 2) in vec2 in_texCords;

out vec4 v2f_color; 
out vec2 v2f_texCords;

uniform mat4 model;
uniform mat4 projection;	
uniform mat4 view;			


void main() 
{ 
	gl_Position = projection * view * model * vec4(in_position, 1); 
	v2f_color = vec4(in_color, 1);
	v2f_texCords = in_texCords;
}