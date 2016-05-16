
#include <QtCore/QCoreApplication>
/* OpenGL 3th praty libraries */
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/* Standard C++ libraries */
#include <iostream>
#include <vector>


/* declaration of callback funcs */
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

/* global variables */
const GLuint WIDTH = 800;
const GLuint HEIGHT = 600;
const GLuint LOGLEN = 512;
GLint success;
GLchar infoLog[LOGLEN];
const GLfloat firstTriangle[] = {
	-0.9f, -0.5f, 0.0f,  // Left 
	-0.0f, -0.5f, 0.0f,  // Right
	-0.45f, 0.5f, 0.0f,  // Top
};
const GLfloat secondTriangle[] = {
	0.0f, -0.5f, 0.0f,  // Left
	0.9f, -0.5f, 0.0f,  // Right
	0.45f, 0.5f, 0.0f   // Top
};




/* Vertex and Fragment Shaders */
const GLchar* vertexShaderSource =
"#version 450 core\n"
"layout (location = 0) in vec3 position;\n"
"void main() {\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.f);\n"
"}\n\0";
const GLchar* fragmentShader1Source =
"#version 450 core\n"
"out vec4 color;\n"
"void main() {\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";
const GLchar* fragmentShader2Source =
"#version 450 core\n"
"out vec4 color;\n"
"void main() {\n"
"color = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	/* GLFW */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "EXERCISE 3", NULL, NULL);
	if (!window){
		std::cout << "ERROR::GLFW: FAILED." << std::endl;
		system("pause");
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	

	/* GLEW */
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK){
		std::cout << "ERROR::GLEW: FAILED." << std::endl;
		system("pause");
		glfwTerminate();
		return EXIT_FAILURE;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	/* compile and link shaders */
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(vertexShader, LOGLEN, NULL, infoLog);
		std::cout << "ERROR::VERTEX_COMPILE: " << infoLog << std::endl;
	}

	GLuint fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader1, 1, &fragmentShader1Source, NULL);
	glCompileShader(fragmentShader1);
	glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(fragmentShader1, LOGLEN, NULL, infoLog);
		std::cout << "ERROR::FRAGMENT_1_COMPILE: " << infoLog << std::endl;
	}
	GLuint fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &fragmentShader2Source, NULL);
	glCompileShader(fragmentShader2);
	glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(fragmentShader2, LOGLEN, NULL, infoLog);
		std::cout << "ERROR::FRAGMENT_2_COMPILE: " << infoLog << std::endl;
	}

	GLuint shaderProgram1 = glCreateProgram();
	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragmentShader1);
	glLinkProgram(shaderProgram1);
	glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
	if (!success){
		glGetProgramInfoLog(shaderProgram1, LOGLEN, NULL, infoLog);
		std::cout << "ERROR::PROGRAM_1_LINK: " << infoLog << std::endl;
	}

	GLuint shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);
	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
	if (!success){
		glGetProgramInfoLog(shaderProgram2, LOGLEN, NULL, infoLog);
		std::cout << "ERROR::PROGRAM_2_LINK: " << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader1);
	glDeleteShader(fragmentShader2);

	/* VAOs, VBOs */
	GLuint VAOs[2], VBOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	/* First triangle */
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBindVertexArray(VAOs[0]);
	/* Second triangle */
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBindVertexArray(VAOs[1]);

	glfwSetKeyCallback(window, key_callback);

	while (!glfwWindowShouldClose(window)){
		glfwPollEvents();

		/* render */
		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram1);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glUseProgram(shaderProgram2);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteBuffers(2, VBOs);
	glDeleteVertexArrays(2, VAOs);
	glfwTerminate();

	return a.exec();
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}