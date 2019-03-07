/*--------------------------------------------------------*/
/* ----------------   Práctica 5 -------------------------*/
/*-----------------    2019-2   --------------------------*/
/*- Computación gráfica e interacción humano computadora -*/
/*-------------------- Grupo 04 --------------------------*/
/*------------- Hernández García Israel ------------------*/
#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>

#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

void myData(void);
void display(void);
void getResolution(void);

//For Keyboard
float	movHombro = 0.0f,
movCodo = 0.0f,
movMunieca = 0.0f,
movPulgar = 0.0f,
movDedos = 0.0f,
movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rotX = 0.0f,
rotY = 0.0f;





void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{	
		GLfloat vertices[] = {
		//Position				//Color
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V0 - Frontal
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V1
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V5
		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V4

		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	//V3
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V7
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V4 - Izq
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	//V7
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	//V3
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V0

		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V5 - Der
		0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V1
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V2
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V4 - Sup
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V6
		-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V7

		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V0 - Inf
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	//V3
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	//V2
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V1
	};

	unsigned int indices[] =	//I am not using index for this session
	{
		0
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void display(void)
{
	//Shader myShader("shaders/shader.vs", "shaders/shader.fs");
	Shader projectionShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");

	projectionShader.use();

	// create transformations and Projection
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 modelTemp = glm::mat4(1.0f);
	glm::mat4 modelTemp2 = glm::mat4(1.0f);
	glm::mat4 modelTemp3 = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	
	//Use "projection" in order to change how we see the information
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	//Use "view" in order to affect all models
	view = glm::translate(view, glm::vec3(movX, movY, movZ));
	view = glm::rotate(view, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::rotate(view,glm::radians(rotY),glm::vec3(0.0f,1.0f,0.0f));
	// pass them to the shaders
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);

	///////////////////////////////
	glBindVertexArray(VAO);
	model = glm::scale(model, glm::vec3(4.0f, 5.0f, 1.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.8f));
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::scale(model, glm::vec3(1.0f, 0.4f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::scale(model, glm::vec3(0.25f, 1.5f, 1.0f));
	model = glm::translate(model, glm::vec3(-1.5f, -0.833f, 0.0f));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::scale(model, glm::vec3(4.0f, 0.33f, 1.0f));
	model = glm::translate(model, glm::vec3(0.375f, -2.0f, 0.0f));
	projectionShader.setVec3("aColor", glm::vec3(0.55f, 0.26f, 0.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models

	model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 5.5f, 0.0f));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::scale(model, glm::vec3(1.5f, 5.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.6f, 0.0f));
	projectionShader.setVec3("aColor", glm::vec3(0.9f, 0.0f, 0.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::mat4(1.0f);
	modelTemp3 = model;


	//Brazo Derecho
	model = glm::translate(model, glm::vec3(2.5f, 2.0f, 0.0f));
	model = glm::rotate(model, glm::radians(movHombro), glm::vec3(0, 0, 1));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor",glm::vec3(1,1,0));
	glDrawArrays(GL_QUADS,0,24);

	modelTemp = model;
	
	//
	model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0, 0, 1));
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;

	//antebrazo
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(movCodo), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));

	modelTemp = model;

	model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0, 0));
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;
	//muñeca
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(movMunieca), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));

	modelTemp = model;

	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.5f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0, 1, 0));
	glDrawArrays(GL_QUADS, 0, 24);

	/////////////////////
	model = modelTemp;
	modelTemp2 = model; 

	//Pulgar - Falange 1
	model = glm::translate(model, glm::vec3(-0.35, 0.5, 0));
	model = glm::rotate(model, glm::radians(movPulgar), glm::vec3(1, 0, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;

	//Pulgar - Falange 2
	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	model = glm::rotate(model, glm::radians(movPulgar*0.7f), glm::vec3(1, 0, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.5f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp2;

	//Indice - Falange 1
	model = glm::translate(model, glm::vec3(0.5, 0.38, 0));
	model = glm::rotate(model, glm::radians(movDedos), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.2, 0.2));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;

	//Indice - Falange 2
	model = glm::translate(model, glm::vec3(0.05, 0.0, 0));
	model = glm::rotate(model, glm::radians(movDedos), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(0.25, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.2, 0.2));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;

	//Indice - Falange 3
	model = glm::translate(model, glm::vec3(0.05, 0.0, 0));
	model = glm::rotate(model, glm::radians(movDedos), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(0.25, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.2, 0.2));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp2;

	//Medio - Falange 1
	model = glm::translate(model, glm::vec3(0.5, 0.13, 0));
	model = glm::rotate(model, glm::radians(movDedos), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(0.18, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.35, 0.2, 0.2));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;

	//Medio - Falange 2
	model = glm::translate(model, glm::vec3(0.05, 0.0, 0));
	model = glm::rotate(model, glm::radians(movDedos), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(0.3, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.35, 0.2, 0.2));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;

	//Medio - Falange 3
	model = glm::translate(model, glm::vec3(0.05, 0.0, 0));
	model = glm::rotate(model, glm::radians(movDedos), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(0.3, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.35, 0.2, 0.2));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp2;

	//Anular Falange 1
	model = glm::translate(model, glm::vec3(0.5, -0.13, 0));
	model = glm::rotate(model, glm::radians(movDedos), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.2, 0.2));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;

	//Anular - Falange 2
	model = glm::translate(model, glm::vec3(0.05, 0.0, 0));
	model = glm::rotate(model, glm::radians(movDedos), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(0.25, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.2, 0.2));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;

	//Anular - Falange 3
	model = glm::translate(model, glm::vec3(0.05, 0.0, 0));
	model = glm::rotate(model, glm::radians(movDedos), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(0.25, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.2, 0.2));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp2;

	//Menor - Falange 1
	model = glm::translate(model, glm::vec3(0.5, -0.38, 0));
	model = glm::rotate(model, glm::radians(movDedos), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(0.1, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;

	//Menor - Falange 2
	model = glm::translate(model, glm::vec3(0.05, 0.0, 0));
	model = glm::rotate(model, glm::radians(movDedos), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;

	//Menor - Falange 3
	model = glm::translate(model, glm::vec3(0.05, 0.0, 0));
	model = glm::rotate(model, glm::radians(movDedos), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	
	model = modelTemp3;

	//Brazo Izquierdo

	glBindVertexArray(VAO);
	model = glm::translate(model, glm::vec3(-2.5f, 2.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-movHombro), glm::vec3(0, 0, 1));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 1, 0));
	glDrawArrays(GL_QUADS, 0, 24);

	modelTemp = model;

	//
	model = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.0f));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(-2.0f, 1.0f, 1.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0, 0, 1));
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;

	//antebrazo
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-movCodo), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.0f));

	modelTemp = model;

	model = glm::scale(model, glm::vec3(-3.0f, 1.0f, 1.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0, 0));
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;
	//muñeca
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-movMunieca), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));

	modelTemp = model;

	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.5f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0, 1, 0));
	glDrawArrays(GL_QUADS, 0, 24);

	/////////////////////
	model = modelTemp;
	modelTemp2 = model;

	//Pulgar - Falange 1
	model = glm::translate(model, glm::vec3(0.35, 0.5, 0));
	model = glm::rotate(model, glm::radians(movPulgar), glm::vec3(1, 0, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;

	//Pulgar - Falange 2
	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	model = glm::rotate(model, glm::radians(movPulgar*0.7f), glm::vec3(1, 0, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.5f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp2;

	//Indice - Falange 1
	model = glm::translate(model, glm::vec3(-0.5, 0.38, 0));
	model = glm::rotate(model, glm::radians(-movDedos), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.2, 0.2));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;

	//Indice - Falange 2
	model = glm::translate(model, glm::vec3(-0.05, 0.0, 0));
	model = glm::rotate(model, glm::radians(-movDedos), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(-0.25, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.2, 0.2));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;

	//Indice - Falange 3
	model = glm::translate(model, glm::vec3(-0.05, 0.0, 0));
	model = glm::rotate(model, glm::radians(-movDedos), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(-0.25, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.2, 0.2));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp2;

	//Medio - Falange 1
	model = glm::translate(model, glm::vec3(-0.5, 0.13, 0));
	model = glm::rotate(model, glm::radians(-movDedos), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(-0.18, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.35, 0.2, 0.2));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;

	//Medio - Falange 2
	model = glm::translate(model, glm::vec3(-0.05, 0.0, 0));
	model = glm::rotate(model, glm::radians(-movDedos), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(-0.3, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.35, 0.2, 0.2));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;

	//Medio - Falange 3
	model = glm::translate(model, glm::vec3(-0.05, 0.0, 0));
	model = glm::rotate(model, glm::radians(-movDedos), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(-0.3, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.35, 0.2, 0.2));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp2;

	//Anular Falange 1
	model = glm::translate(model, glm::vec3(-0.5, -0.13, 0));
	model = glm::rotate(model, glm::radians(-movDedos), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.2, 0.2));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;

	//Anular - Falange 2
	model = glm::translate(model, glm::vec3(-0.05, 0.0, 0));
	model = glm::rotate(model, glm::radians(-movDedos), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(-0.25, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.2, 0.2));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;

	//Anular - Falange 3
	model = glm::translate(model, glm::vec3(-0.05, 0.0, 0));
	model = glm::rotate(model, glm::radians(-movDedos), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(-0.25, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.2, 0.2));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp2;

	//Menor - Falange 1
	model = glm::translate(model, glm::vec3(-0.5, -0.38, 0));
	model = glm::rotate(model, glm::radians(-movDedos), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(-0.1, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;

	//Menor - Falange 2
	model = glm::translate(model, glm::vec3(-0.05, 0.0, 0));
	model = glm::rotate(model, glm::radians(-movDedos), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;

	//Menor - Falange 3
	model = glm::translate(model, glm::vec3(-0.05, 0.0, 0));
	model = glm::rotate(model, glm::radians(-movDedos), glm::vec3(0, 1, 0));
	projectionShader.setMat4("model", model);
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);


	glBindVertexArray(0);



}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 5", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	myData();
	glEnable(GL_DEPTH_TEST);

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);

        // render
        // Backgound color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi función de dibujo
		display();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.008f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.008f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		movZ += 0.008f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		movZ -= 0.008f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movY += 0.008f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movY -= 0.008f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		rotX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		rotX -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rotY -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rotY += 0.08f;

	if (glfwGetKey(window, GLFW_KEY_Z))
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
			if (movHombro < 90)
				movHombro += 0.08f;

		}
		else {
			if (movHombro > -90)
				movHombro -= 0.08f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_X))
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
			if (movCodo < 0)
				movCodo += 0.08f;
		}
		else {
			if (movCodo > -140)
				movCodo -= 0.08f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_C))
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
			if (movMunieca < 40)
				movMunieca += 0.08f;
		}
		else {
			if (movMunieca > -60)
				movMunieca -= 0.08f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_V))
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
			if (movPulgar > 0)
				movPulgar -= 0.08f;
		}
		else {
			if (movPulgar < 60)
				movPulgar += 0.08f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_B))
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
			if (movDedos < 0)
				movDedos += 0.08f;
		}
		else {
			if (movDedos > -60)
				movDedos -= 0.08f;
		}
	}




}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}