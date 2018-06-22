#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "shaderprogram.h"
#include <iostream>
class Model
{
private:
	std::vector <float> vertices;
	std::vector <float> uvs;
	std::vector <float> normals;
	GLuint vao;
	GLuint bufVertices;
	GLuint bufColors;
	GLuint bufNormals;
	glm::vec3 pozycja;
public:
	Model();
	~Model();
	bool loader(const char * path);
	std::vector <float> getVertices() { return vertices; }
	std::vector <float> getUvs() { return uvs; }
	std::vector <float> getNormals() { return normals; }
	GLuint makeBuffer(void *data, int vertexCount, int vertexSize);
	void assignVBOtoAttribute(ShaderProgram *shaderProgram, const char* attributeName, GLuint bufVBO, int vertexSize);
	void prepareObject(ShaderProgram *shaderProgram);
	GLuint getVao() { return vao; }
	void Model::drawObject(ShaderProgram *shaderProgram, glm::mat4 mP, glm::mat4 mV, glm::mat4 mM);
	void setPozycja(glm::vec3 a) { pozycja = a; }
};

