

#include <QtCore/QCoreApplication>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <soil/SOIL.h>

#include "Shader.h"

#include <string>
#include <string>
#include <fstream>
const GLuint WIDTH = 800, HEIGHT = 600;
glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 3.f);
glm::vec3 cameraFront = glm::vec3(0.f, 0.f, -1.f);
glm::vec3 cameraUp = glm::vec3(0.f, 1.f, 0.f);
/* 键盘操作及需要的全局变量*/
bool keys[1024];
GLfloat deltaTime = 0.f;
GLfloat lastFrame = 0.f;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void doMovement();
/* 增加鼠标操作及需要的全局变量 */
GLfloat yawAngle = -90.f;
GLfloat pitchAngle = 0.f;
GLfloat lastX = WIDTH / 2, lastY = HEIGHT / 2;
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
/* 增加缩放操作及需要的全局变量*/
GLfloat fov = 45.0f;
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);





int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	GLFWwindow * window = glfwCreateWindow(800, 600, "6.0 Camera", NULL, NULL);
	/* 这句忘了，引发异常：
	0x74F4CB49 处有未经处理的异常(在 7.0Camera.exe 中):  0xC0000005:  执行位置 0x00000000 时发生访问冲突 */
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	/* 增加鼠标操作 */
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	/* Optional */
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0, 0, WIDTH, HEIGHT);

	/* 开启深度测试 */
	glEnable(GL_DEPTH_TEST);

	Shader myShader("./Resources/default.vs", "./Resources/default.frag");

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
	/* VAO, VBO *************************************** */
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	/* positioin attribute */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	/* texture attribute */
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	/* Load and create texture ***********************************/
	GLuint texture1, texture2;
	/* Texture1*/
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	/* load, create texture and generate mipmaps */
	int width, height;
	unsigned char* image = SOIL_load_image("./Resources/container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	/* Texture2*/
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	/* load, create texture and generate mipmaps */
	image = SOIL_load_image("./Resources/awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	

	while (!glfwWindowShouldClose(window)){

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - deltaTime;

		glfwPollEvents();
		/* 配合最后的函数*/
		doMovement();

		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* bind textures using texture units */
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(myShader.Program, "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(myShader.Program, "ourTexture2"), 1);

		/* active shader */
		myShader.Use();

		/* camear/view transform */
		glm::mat4 view;
		GLfloat radius = 10.f;

		/*GLfloat camX = sin(glfwGetTime())*radius;
		GLfloat camZ = cos(glfwGetTime())*radius;
		view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.f, 1.f, 0.f));*/
		/* 以上三行是7.0Camera中的代码，此处用上面定义的glm::vec3 全局变量 */
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


		/* projection */
		glm::mat4 projection;
		projection = glm::perspective(fov, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.f);
		/* get the uniform location */
		GLint modeLoc = glGetUniformLocation(myShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(myShader.Program, "view");
		GLint proLoc = glGetUniformLocation(myShader.Program, "projection");
		/* pass the matrices to the shader */
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(proLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glBindVertexArray(VAO);
		for (GLuint i = 0; i < 10; ++i){
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 20.f * i;
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modeLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		glfwSwapBuffers(window);


	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();


	return app.exec();
}

//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
//{
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, GL_TRUE);
//
//}

/* 重新定义回调函数1 
 * 使用下面的函数1， 会有卡顿 */
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
//		glfwSetWindowShouldClose(window, GL_TRUE);
//	}
//	GLfloat cameraSpeed = 0.05f;
//	if (key == GLFW_KEY_W){
//		cameraPos += cameraSpeed * cameraFront;
//	}
//	if (key == GLFW_KEY_S){
//		cameraPos -= cameraSpeed * cameraFront;
//	}
//
//	if (key == GLFW_KEY_A){
//		cameraPos += glm::normalize(glm::cross(cameraUp, cameraFront))*cameraSpeed;
//	}
//
//	if (key == GLFW_KEY_D){
//		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp))*cameraSpeed;
//	}
//	if (key == GLFW_KEY_Q){
//		cameraUp += cameraSpeed*cameraUp;
//	}
//}

/* 函数3: 解决多键， 卡顿 */
/* 需要在game loop中定义状态 */
void doMovement(){
	// Camera controls
	GLfloat cameraSpeed = deltaTime * 0.05f;
	if (keys[GLFW_KEY_W]){
		cameraPos += cameraSpeed * cameraFront;
	}
	if (keys[GLFW_KEY_S]){
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (keys[GLFW_KEY_A]){
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp))*cameraSpeed;
	}
	if (keys[GLFW_KEY_D]){
		cameraPos += glm::normalize(glm::cross(cameraUp, cameraFront))*cameraSpeed;
	}
	if (keys[GLFW_KEY_Q]){
		cameraPos += cameraSpeed * cameraUp;

	}
	if (keys[GLFW_KEY_Z]){
		cameraPos -= cameraSpeed * cameraUp;
	}
	
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	

	if (key > 0 && key < 1024){
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
		
	}
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse){
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yawAngle += xoffset;
	pitchAngle += yoffset;

	/*if (pitchAngle > 89.0)
		pitchAngle = 89.f;
	if (pitchAngle < -89.f)
		pitchAngle = -89.f;*/

	glm::vec3 front;
	front.x = cos(glm::radians(yawAngle)) * cos(glm::radians(pitchAngle));
	front.y = sin(glm::radians(pitchAngle));
	front.z = sin(glm::radians(yawAngle)) * cos(glm::radians(pitchAngle));
	cameraFront = glm::normalize(front);


}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	GLfloat scaleFactor = 0.05;
	//if (fov >= 1.f && fov <= 45.0){
		fov -= yoffset*scaleFactor;
	//}
	/*if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;*/
	
}