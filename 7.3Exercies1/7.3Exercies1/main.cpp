/* See if you can transform the camera class in such a way that it becomes a true fps camera
   where you cannot fly; you can only look around while staying on the xz plane: solution.*/
#include <QtCore/QCoreApplication>
#define GLEW_STATIC			// Forget
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <soil\SOIL.h>

#include "Shader.h"
#include "Camera.h"

/* Properites */
GLuint screenWidth = 800, screenHeight = 600;

/* Function prototypes */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

/* Camera */
Camera camera(glm::vec3(0.f, 0.f, 3.f));
bool keys[1024];
GLfloat lastX = screenWidth / 2, lastY = screenHeight / 2;
bool firstMouse = true;

GLfloat deltaTime = 0.f;
GLfloat lastFrame = 0.f;

int main(int argc, char* argv[]){

	QCoreApplication app(argc, argv);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(screenHeight, screenHeight, "Execries 1", NULL, NULL);
	glfwMakeContextCurrent(window);

	/* set the key call back */
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	/* Options */
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	/* GLEW */
	glewExperimental = GL_TRUE;
	glewInit();

	/* define the viewport demension */
	glViewport(0, 0, screenWidth, screenHeight);

	/* set up Opengl option */
	glEnable(GL_DEPTH_TEST);

	/* set up and compile shader */
	Shader myShader ("./Resources/default.vs", "./Resources/default.frag");

	/* setup vertex data ....*/
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glCreateBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glNamedBufferStorage(VBO, sizeof(vertices), vertices, GL_MAP_WRITE_BIT);

	/* position attribute */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	/* TexCoord attribute */
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);



	/* load and creat texutre */
	GLuint texture[2];
	glGenTextures(2, texture);
	int width, height;	// forget
	unsigned char* image = nullptr;	//forget 
	std::string images[2] = { "./Resources/container.jpg", "./Resources/awesomeface.png" };
	for (int i = 0; i < 2; ++i){
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		image = SOIL_load_image(images[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);	// forget
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);	// forget
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Game loop
	while (!glfwWindowShouldClose(window)){
		
		/* set frame time */
		GLfloat currentFrame = glfwGetTime();	// forget
		deltaTime = currentFrame - lastFrame;	// forget
		lastFrame = currentFrame;		// forget
		
		/* check and call events */
		glfwPollEvents();
		Do_Movement();

		glClearColor(0.f, 0.f, 0.f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Draw fist triange ?*/
		myShader.Use();

		/* bind texture using texture units*/
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glUniform1i(glGetUniformLocation(myShader.Program, "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glUniform1i(glGetUniformLocation(myShader.Program, "ourTexture2"), 1);


		/* create camera transformations */
		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(camera.Zoom, (float)screenWidth / screenHeight, 0.1f, 100.f);
		/* get the uniform locations */
		GLint viewLoc = glGetUniformLocation(myShader.Program, "view");
		GLint modelLoc = glGetUniformLocation(myShader.Program, "model");
		GLint projLoc = glGetUniformLocation(myShader.Program, "projection");
		/* pass the matrices to the shader */
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		for (int i = 0; i < 10; ++i){
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 20.f * i;
			model = glm::rotate(model, angle, glm::vec3(0.3f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteTextures(2, texture);
	glfwTerminate();

	return app.exec();
}

/* *************************************************** */
void Do_Movement(){
	// camera controls 
	if (keys[GLFW_KEY_W]){
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S]){
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A]){
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D]){
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (keys[GLFW_KEY_HOME]){
		camera.ProcessKeyboard(UP, deltaTime);

	}
	if (keys[GLFW_KEY_END]){
		camera.ProcessKeyboard(DOWN, deltaTime);
	}
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key <= 1024){
		if (action == GLFW_PRESS){
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE){
			keys[key] = false;
		}
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	if (firstMouse){
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	camera.ProcessMouseScroll(yoffset);
}
