#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
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
};

