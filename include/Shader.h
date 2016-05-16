#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

/* Include glew to get all of the required OpenGL headers */
#include <GL/glew.h>

class Shader{
public:
	/* Program ID */
	GLuint Program;
	/* Consturctor --- read and build the shader */
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	/* Use the program */
	void Use();
};

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath){
	/* a. Retrieve the vertex and fragment shader source code from their path respectively */
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;
	/* ensure ifstream objects can throw exceptions: */
	vertexShaderFile.exceptions(std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::badbit);
	try {
		/* Open files */
		vertexShaderFile.open(vertexPath);
		fragmentShaderFile.open(fragmentPath);
		std::stringstream vertexShaderStream, fragmentShaderStream;
		/* Read file's buffer contents into streams */
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderStream.rdbuf();
		/* close files */
		vertexShaderFile.close();
		fragmentShaderFile.close();
		/* convert stream into GLchar array */
		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();
	}
	catch(std::ifstream::failure e) {
		std::cerr << "ERROR::SHADER::FILE_NOT_SCCESFULLY_READ! " << std::endl;
	}
	const GLchar* vertexShaderCode = vertexCode.c_str();
	const GLchar* fragmentShaderCode = fragmentCode.c_str();

	/* b. Compile shaders */
	GLuint vertexShader, fragmentShader;
	GLint success;
	GLchar infoLog[LOGLEN];

	/* vertex shader */
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);
	/* check vertex compile error */
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(vertexShader, LOGLEN, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED: " << infoLog << std::endl;
	}
	/* fragment shader */
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);
	/* check fragment compile error */
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(fragmentShader, LOGLEN, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: " << infoLog << std::endl;
	}

	/* c. Build shaders and link them to program */
	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertexShader);
	glAttachShader(this->Program, fragmentShader);
	glLinkProgram(this->Program);
	/* check link errors */
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(this->Program, LOGLEN, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINK_FAILED: " << infoLog << std::endl;
	}

	/* d. delete the shaders */
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Use(){
	glUseProgram(this->Program);
}