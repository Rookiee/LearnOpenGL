
#include <QtCore/QCoreApplication>
/* OpenGL 3th party libraries */
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/* Standard C++ libraries */
#include <iostream>

/* Call Back Functions */
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
	if (key == 27 && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

/* Shaders */
const GLchar* vertexShaderSource =
"#version 450 core\n"
"layout (Location = 0) in vec3 position;\n"
"void main() {\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0f);\n"
"}\n\0";

const GLchar* fragmentShaderSource =
"#version 450 core\n"
"out vec4 color;\n"
"void main() {\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

/* Input data */
static GLfloat vertices[] = {
	//// First triangle
	//0.5f, 0.5f, 0.0f,  // Top Right
	//0.5f, -0.5f, 0.0f,  // Bottom Right
	//-0.5f, 0.5f, 0.0f,  // Top Left 
	//// Second triangle
	//0.5f, -0.5f, 0.0f,  // Bottom Right
	//-0.5f, -0.5f, 0.0f,  // Bottom Left
	//-0.5f, 0.5f, 0.0f   // Top Left
	// First triangle
	-0.9f, -0.5f, 0.0f,  // Left 
	-0.0f, -0.5f, 0.0f,  // Right
	-0.45f, 0.5f, 0.0f,  // Top 
	// Second triangle
	0.0f, -0.5f, 0.0f,  // Left
	0.9f, -0.5f, 0.0f,  // Right
	0.45f, 0.5f, 0.0f   // Top

};


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	/* Initialize GLFW*/
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Exercise_1", NULL, NULL);
	if (!window){
		std::cout << "Failed to initialize GLFW." << std::endl;
		system("pause");
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);

	

	/* Initialize GLEW*/
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK){
		std::cout << "Failed to initialize GLEW." << std::endl;
		system("pause");
		glfwTerminate();
		return EXIT_FAILURE;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);



	/* Compile and Link shaders */
	GLint success;
	GLchar infoLog[512];

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Error::Compile::Vertex: " << infoLog << std::endl;
		system("pause");
	}
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Error::Compile::Fragment: " << infoLog << std::endl;
		system("pause");
	}
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success){
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Error::Link: " << infoLog << std::endl;
		system("pause");
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	/* VAO, VBO */
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	glfwSetKeyCallback(window, key_callback);


	while (!glfwWindowShouldClose(window)){

		glfwPollEvents();

		/* Render */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		
		glfwSwapBuffers(window);
	}

	//glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);	

	glfwTerminate();

	return a.exec();
}
