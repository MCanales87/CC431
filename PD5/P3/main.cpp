// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//~ #include <SOIL2/soil2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Cone.h"
#include "Utils.h"

using namespace std;

float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

#define numVAOs 2
#define numVBOs 4
#define numVBOsc 3

float cameraX, cameraY, cameraZ;
float torLocX, torLocY, torLocZ;
float conLocX, conLocY, conLocZ;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

GLuint vaoc[numVAOs];
GLuint vboc[numVBOsc];

Cone myCone(100, 2);

glm::vec3 lightLoc = glm::vec3(0.8f, 0.4f, 2.0f);
float amt = 0.0f;
float conScale;

// variable allocation for display
GLuint mvLoc, projLoc, nLoc;
GLuint diffLoc, specLoc, mdiffLoc, mspecLoc, mshiLoc;
GLuint cutLoc, expLoc, locLoc, dirLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat, invTrMat, rMat;
glm::vec3 currentLightPos, transformed;


void setupVerticesCone(void) {
	std::vector<int> ind = myCone.getIndices();
	std::vector<glm::vec3> vert = myCone.getVertices();
	std::vector<glm::vec3> norm = myCone.getNormals();

	std::vector<float> pvalues;

	for (int i = 0; i < myCone.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
	}
	glGenVertexArrays(1, vaoc);
	glBindVertexArray(vaoc[0]);
	glGenBuffers(numVBOsc, vboc);

	glBindBuffer(GL_ARRAY_BUFFER, vboc[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	//~ glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboc[2]);
	//~ glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("src/vshader1.glsl", "src/fshader1.glsl");
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 1.5f;
	conLocX = 0.0f; conLocY = 0.3f; conLocZ = 0.0f;
	
	conScale = 0.3;
	
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

	setupVerticesCone();
}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

	/*cone*/
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(conLocX, conLocY, conLocZ));
	//mMat *= glm::rotate(mMat, toRadians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mMat *= glm::scale(glm::mat4(1.0f), glm::vec3(conScale));
	mvMat = vMat * mMat;

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	glBindBuffer(GL_ARRAY_BUFFER, vboc[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	//~ glEnable(GL_DEPTH_TEST);
	//~ glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, myCone.getNumVertices() );
}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(800, 800, "Prob4", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	glfwSetWindowSizeCallback(window, window_size_callback);

	init(window);

	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
