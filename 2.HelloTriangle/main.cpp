
#include <QtCore/QCoreApplication>

/* OpenGL 3th party libraries */
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/* Standard C++ libraries */
#include <iostream>


/* Key Callback Function.*/
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

/* Shaders */
/* Vertex Shader Source Code in C String */
const GLchar* vertexShaderSource = "# version 450 core\n"
		"layout(location = 0) in vec3 position;\n"
		"void main(){\n"
		"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
		"}\n\0";

/* Fragment Shader Source Code in C String */
const GLchar * fragmentShaderSource = "# version 440 core \n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";




int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Triangle", NULL, NULL);
	if (!window){
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK){
		std::cout << "Failed to create GLEW." << std::endl;
		return EXIT_FAILURE;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	/* Build and compile our shader program. */
	/* ---------------------------Vertex shader----------------------------- */
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	/* attach the shader source code to the shader object and compile the shader */
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	/* Check for compile-time errors */
	/*  define an integer to indicate success */
	GLint success;
	/* a storage container for the error messages*/
	GLchar infoLog[512];
	/* heck if compilation was successful with glGetShaderiv. */
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	/*  If compilation failed, we should retrieve the error message with glGetShaderInfoLog and print the error message.*/
	if (!success){
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPLILATION_FAILED\n" << infoLog << std::endl;
	}

	/* ---------------------------Fragment shader---------------------------*/
	/* fragment shader is all about calculating the color output of your pixels.*/
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPLILATION_FAILED\n" << infoLog << std::endl;
	}

	/* ---------------------------Link---------------------------*/
	/* link both shader objects into a shader program that we can use for rendering. */
	/* Create a link program object */
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();

	/* attach the previously compiled shaders to the program object and then link them with glLinkProgram. */
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	/* Check if the link is failed */
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success){
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINK::COMPLILATION_FAILED\n" << infoLog << std::endl;
	}
	/* delete the shader objects once we've linked them into the program object; we no longer need them anymore*/
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/* Set up vertex data[] */
	const GLfloat vertices[] = {
		-0.5f, -0.5f, 0.f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.f,
		-0.5f, -0.5f, 0.f,
	};

	/* --------------------------------VBO, VAO---------------------------------*/
	GLuint VBO, VAO;
	//glGenBuffers(1, &VBO);
	glCreateBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	

	/* Bind the VAO first, 
	then bind and set vertex buffer(s) and attribute prointer(s) */
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/* explain the structure of input data(vertex data)*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	glfwSetKeyCallback(window, key_callback);

	while (!glfwWindowShouldClose(window)){

		/* Check if any events have been activated (key pressed, mouse moved etc.
		and call corresponding response functions.*/
		glfwPollEvents();


		/* Render */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/* Draw triangle */
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_LINE_STRIP, 0, 4);
		glBindVertexArray(0);


		/* Swap the screen buffers */
		glfwSwapBuffers(window);

	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	
	glfwTerminate();

	return a.exec();
}


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
}