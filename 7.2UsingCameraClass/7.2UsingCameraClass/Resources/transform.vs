#version 440 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texPosition;

out vec2 ourTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position = projection * view * model * vec4(position, 1.f);
	ourTexCoord = vec2(texPosition.x, 1.0 - texPosition.y);
}