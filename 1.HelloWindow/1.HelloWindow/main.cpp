
#include <QtCore/QCoreApplication>

/* OpenGL 3th party libraries */
#define GLEW_STATIC
/* Be sure to include GLEW before GLFW*/
#include <GL/glew.h>
#include <GLFW/glfw3.h>
/* Standard c++ libraries */
#include <iostream>

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	glfwInit();
	/* GLFW_CONTEXT_VERSION_MAJOR and GLFW_CONTEXT_VERSION_MINOR specify the client API version
	that the created context must be compatible with. The exact behavior of these hints depend 
	on the requested client API*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	/* GLFW_OPENGL_PROFILE specifies which OpenGL profile to create the context for. 
	Possible values are one of GLFW_OPENGL_CORE_PROFILE or GLFW_OPENGL_COMPAT_PROFILE, 
	or GLFW_OPENGL_ANY_PROFILE to not request a specific profile*/
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* GLFW_RESIZABLE indicates whether the specified window is resizable by the user.*/
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (!window){
		std::cout << "Failed to create GLFW window" << std::endl;
		system("pause");
		glfwTerminate();
		return EXIT_FAILURE;
	}
	/* Successful creation does not change which context is current . */
	glfwMakeContextCurrent(window);


	/* For GLFW, GLEW manages function pointers for OpenGL */
	/* glewExperimental variable to GL_TRUE before initializing GLEW. 
	Setting glewExperimental to true ensures GLEW uses more modern techniques
	for managing OpenGL functionality. Leaving it to its default value of GL_FALSE
	might give issues when using the core profile of OpenGL. */
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK){
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	/* We have to tell OpenGL the size of the rendering window so OpenGL knows how 
	we want to display the data and coordinates with respect to the window. 
	We can set those dimensions via the glViewport function*/
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	/*  register the function with the proper callback via GLFW.
	We register the callback functions after we've created the window and before the game loop is initiated. */
	glfwSetKeyCallback(window, key_callback);

	/*keeps on running until we tell GLFW to stop.*/
	while (!glfwWindowShouldClose(window)){

		/* Check and call events*/
		glfwPollEvents();

		/* Rendering commands here... 
		place all the rendering commands in the game loop,
		since we want to execute all the rendering commands each iteration of the loop*/
		/* At the start of each render iteration we always want to clear the screen 
		otherwise we would still see the results from the previous iteration*/
		/* glClearColor set a color value that OpenGL use to reset color buffer,
		as soon as glClear or glClearbuffer is called, it use the value to reset its color value */
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		GLfloat resetColor[] = { 0.2f, 0.2f, 0.3f, 1.0f };
		glClear(GL_COLOR_BUFFER_BIT, resetColor); // ERROR, NO OVERLOADED FUNCTION LIKE THIS;
		//glClear(GL_COLOR_BUFFER_BIT);
		// glClearColor is a state-setting function, and glClear is a state-using function.


		/* Swap the buffers*/
		glfwSwapBuffers(window);
	}

	
	glfwTerminate();
	

	return a.exec();
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	/*When a user presses the escape key, we set the WindowShouldClose property to ture,
	closing the application.*/
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}
