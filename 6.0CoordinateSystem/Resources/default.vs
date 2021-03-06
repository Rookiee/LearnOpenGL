#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 ourColor;
out vec2 ourTexCoord;

// 变换矩阵
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	// 使用变换矩阵乘以原向量
	gl_Position = projection * view * model *  vec4(position, 1.0f);
	ourColor = color;
	// ourTexCoord = texCoord;
	ourTexCoord = vec2(texCoord.x, 1.0-texCoord.y);
}