/* Specify a horizontal offset via a uniform and move the triangle to the right side of the screen in the vertex shader using this offset value */

#include <QtCore/QCoreApplication>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include "../include/Shader.h"
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "3.Exercise2", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glViewport(0, 0, 800, 600);

	glewExperimental = GL_TRUE;
	glewInit();

	GLfloat vertices[] = {
		// Positions         // Colors
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // Bottom Right
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // Bottom Left
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // Top 
	};

	Shader myshader("./Resources/default.vs", "./Resources/default.frag");

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	/* position attribute */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	/* position attribute */
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)){
		glfwPollEvents();

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		myshader.Use();
		
		GLuint offset = 1;
		glUniform1f(glGetUniformLocation(myshader.Program, "xOffset"), offset);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);


		glfwSwapBuffers(window);

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return a.exec();
}
