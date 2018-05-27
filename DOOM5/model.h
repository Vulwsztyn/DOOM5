#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
class Model
{
private:
	std::vector < float > vertices;
	std::vector < float > uvs;
	std::vector < float > normals;
public:
	Model();
	~Model();
	bool loader(const char * path);
	std::vector < float > getVertices() { return vertices; }
	std::vector < float > getUvs() { return uvs; }
	std::vector < float > getNormals() { return normals; }
	float* getConvertedVertices();
	float* getConvertedUvs();
	float* getConvertedNormals();
};

