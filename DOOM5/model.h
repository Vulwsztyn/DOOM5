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
	std::vector < glm::vec4 > vertices;
	std::vector < glm::vec3 > uvs;
	std::vector < glm::vec4 > normals;
	GLuint vao;
	GLuint bufVertices;
	GLuint bufColors;
	GLuint bufNormals;
public:
	Model();
	~Model();
	bool loader(const char * path);
	std::vector < glm::vec4 > getVertices() { return vertices; }
	std::vector < glm::vec3 > getUvs() { return uvs; }
	std::vector < glm::vec4 > getNormals() { return normals; }
	GLuint makeBuffer(void *data, int vertexCount, int vertexSize);
	void assignVBOtoAttribute(ShaderProgram *shaderProgram, const char* attributeName, GLuint bufVBO, int vertexSize);
	void prepareObject(ShaderProgram *shaderProgram);
	GLuint* getVao();
};

