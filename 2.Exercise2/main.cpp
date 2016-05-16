
#include <QtCore/QCoreApplication>
/* OpenGL 3th Libraries */
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/* Standard C++ Libraries */
#include <iostream>
using namespace std;

/* Call Back Functions */
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

/* Global Variables*/
/* Input data */
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
const int HEIGHT = 600;
const int WIDTH = 800;
const GLint LOGLEN = 512;

/* Shaders */
const GLchar* vertexShaderSource =
//"#version 450 core\n"
//"layout (location = 0) in vec3 position;\n"
//"void main() {\n"
//"gl_Position = vec4(position.x, position.y, position.z, 1.0f);\n"
//"}\n\0";
"#version 450 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";
const GLchar* fragmentShaderSource =
"#version 450 core\n"
"out vec4 color;\n"
"void main() {\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	/* GLFW */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "TWO SAME TRIANGLES", NULL, NULL);
	if (!window){
		cout << "ERROR::GLFW::FAILED. " << endl;
		system("pause");
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);


	/* GLEW */
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK){
		cout << "ERROR::GLEW::FAILED." << endl;
		system("pause");
		glfwTerminate();
		return EXIT_FAILURE;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	/* callback */
	glfwSetKeyCallback(window, key_callback);

	/* compile and link shaders */
	GLint success;
	GLchar infoLog[LOGLEN];

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);	
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(vertexShader, LOGLEN, NULL, infoLog);
		cout << "ERROR::VERTEX_COMPILE: " << infoLog << endl;
		
	}
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(fragmentShader, LOGLEN, NULL, infoLog);
		cout << "ERROR::FRAGMENT_COMPILE: " << infoLog << endl;
	}
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success){
		glGetProgramInfoLog(shaderProgram, LOGLEN, NULL, infoLog);
		cout << "ERROR::PROGRAM_LINK: " << infoLog << endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/* VAOs, VBOs */
	GLuint VAOs[2], VBOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	/* First triangle setup */
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	/* Second triangle setup */
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)){
		glfwPollEvents();

		/* Renders */
		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(0);

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
