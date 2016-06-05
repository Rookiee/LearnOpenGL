
/* 设置uniform全局变量 */
#include <QtCore/QCoreApplication>


/* Standart C++ Libraries */
#include <iostream>

/* 3th Party Libraries */
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/* key call back function */
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

/* shaders */
const GLchar* vertexShaderSource = "#version 450 core \n"
"layout (location = 0) in vec3 position; \n"
"layout (location = 1) in vec3 color;\n"
"out vec3 ourColor;\n"
"void main () {\n"
"gl_Position = vec4(position, 1.0);\n"
"ourColor = color;\n"
"}\0";

const GLchar* fragmentShaderSource = "#version 450 core\n"
"out vec4 color;\n"
"uniform vec4 ourColor;\n"
"void main() {\n"
"color = ourColor;\n"
"}\n\0";


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	
	/* Init GLFW */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Changing Color", NULL, NULL);
	if (!window){
		std::cerr << "GLFW Failed." << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	/* GLEW */
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK){
		std::cerr << "GLEW Failed." << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	/* Viewport */
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	/* build and compile shaders */
	/* vertex shader */
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	/* check for compile time errors */
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED: " << infoLog << std::endl;
	}
	
	/* fragment shader */
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	/* check for compile time errors */
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED: " << infoLog << std::endl;
	}

	/* Link shaders */
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	/* check for link errors */
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success){
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED: " << infoLog << std::endl;
	}

	/* delet shaders */
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	/* setup vertex data (and buffer(s)) and attribute pointers */
	GLfloat vertices[] = {
		// Positions        
		0.5f, -0.5f, 0.0f,  // Bottom Right
		-0.5f, -0.5f, 0.0f,  // Bottom Left
		0.0f, 0.5f, 0.0f   // Top 
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	/* bind the vertex array object (VAO) first, then bind and set vertex bufffer(s) and attribute pointer(s) */
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/* positioni attribute */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	/* unbind VAO */
	glBindVertexArray(0);

	/* set the required callback functions */
	glfwSetKeyCallback(window, key_callback);
	

	/* Game loop */
	int index = 0;
	while (!glfwWindowShouldClose(window)){
		
		/* check if any events have been activated */
		glfwPollEvents();

		/* render */
		/* clear the color buffer */
		glClearColor(0.2f, 0.3f, 0.3f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		/* be sure to active the shader */
		glUseProgram(shaderProgram);

		/* update the uniform shader */
		GLfloat timeValue = glfwGetTime();
		GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
		GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		std::cout << "i: " << vertexColorLocation << std::endl;
		++index;

		/* draw the triangle */
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		/*swap the screen buffers */
		glfwSwapBuffers(window);


	}

	/* properly de-allocate all resources once they've outlived their purpose */
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return a.exec();
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
