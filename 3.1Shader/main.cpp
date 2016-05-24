
#include <QtCore/QCoreApplication>
/* OpenGL 3th party libraries */
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/* Standard C++ Libraries */
#include <iostream>

/* Shaders header */
#include "Shader.h"

/* Callback */
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

/* Gloabl variables */
const GLuint WIDTH = 800, HEIGHT = 600;

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	return a.exec();
}
