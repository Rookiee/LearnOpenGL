
#include <QtCore/QCoreApplication>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <soil/SOIL.h>

#include "../include/Shader.h"

/* 加入list头文件 (用来玩）*/
#include <list>

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
const GLuint HEIGHT = 600, WIDTH = 800;

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "4.1Texture", NULL, NULL);
	if (!window){
		std::cerr << "GLFW FAILED." << std::endl;
		glfwTerminate();
		system("pause");
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	int height, width;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK){
		std::cerr << "GLEW FAILED." << std::endl;
		glfwTerminate();
		system("pause");
		exit(EXIT_FAILURE);
	}

	Shader myShader("./Resources/default.vs", "./Resources/default.frag");

	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		1.5f, 1.5f, 0.0f,    1.0f, 0.0f, 0.0f,      1.0f, 1.0f, // Top Right
		1.5f, -1.5f, 0.0f,   0.0f, 1.0f, 0.0f,      1.0f, 0.0f, // Bottom Right
		-1.5f, -1.5f, 0.0f,  0.0f, 0.0f, 1.0f,      0.0f, 0.0f, // Bottom Left
		-1.5f, 1.5f, 0.0f,   1.0f, 1.0f, 0.0f,      0.0f, 1.0f  // Top Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};

	///* ***************************************************** * /
	///* 玩玩上面的数组 */

	///* 显示一下 */
	//for (int i = 0; i < 32; ++i){
	//	std::cout << vertices[i] << "\t";
	//	if ((i + 1) % 8 == 0){
	//		std::cout << std::endl;
	//	}
	//}
	///* 放入一个list */
	//std::list<GLfloat> myList;
	//for (size_t i = 0; i < sizeof(vertices) / sizeof(GLfloat); ++i){
	//	myList.push_back(vertices[i]);
	//}
	//for (std::list<GLfloat>::const_iterator iter = myList.begin(); iter != myList.end(); ++iter){
	//	std::cout << *iter << std::endl;
	//}


	///* ***************************************************** */



	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/* position attribute */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	/* color attribute */
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	/* texture attribute */
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	/* load and create texture */
	GLuint texture1, texture2;

	/* Texture1 */
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);	// all coming GL_TEXTURE_2D operations now have effect on texture1 object
	/* set texture1 wrap parameters */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	/* set texture1 filtering parameters */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	
	unsigned char* image = SOIL_load_image("./Resources/container.jpg", &width, &height, 0,
		SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	/* Texture2 */
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	image = SOIL_load_image("./Resources/awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	while (!glfwWindowShouldClose(window)){

		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		myShader.Use();

		/* bind textures using texture units */
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(myShader.Program, "ourTexture1"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(myShader.Program, "ourTexture2"), 1);

		/* draw */
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();

	return a.exec();
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}