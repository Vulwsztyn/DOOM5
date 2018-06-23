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
#include "lodepng.h"
#include "Lights.h"
class Model
{
private:
	std::vector <float> vertices;
	std::vector <float> uvs;
	std::vector <float> vertexNormals;
	std::vector <float> normals;
	std::vector<float> tangents;
	std::vector<float> bitangents;

	glm::vec4 ambient;
	float shininess;
	float roughness;
	GLuint diffTex; //Zmienna reprezentujaca teksturê
	GLuint normalTex; //Zmienna reprezentujaca teksturê
	GLuint heightTex; //Zmienna reprezentuj¹ca teksturê
	GLuint specTex;
	GLuint vao;
	GLuint bufVertices;
	GLuint bufColors;
	GLuint bufNormals;
<<<<<<< HEAD
	GLuint bufTexCoords;
	GLuint bufC1;
	GLuint bufC2;
	GLuint bufC3;
=======
	glm::vec3 pozycja;
>>>>>>> A-Kolizja
public:
	Model();
	~Model();
	bool loader(const char * path);
	std::vector <float> getVertices() { return vertices; }
	std::vector <float> getUvs() { return uvs; }
	std::vector <float> getvertexNormals() { return vertexNormals; }
	std::vector <float> getNormals() { return normals; }
	std::vector <float> getTangents() { return tangents; }
	std::vector <float> getBitangents() { return bitangents; }
	GLuint makeBuffer(void *data, int vertexCount, int vertexSize);
	void assignVBOtoAttribute(ShaderProgram *shaderProgram, const char* attributeName, GLuint bufVBO, int vertexSize);
	void prepareObject(ShaderProgram *shaderProgram, char* diff, char* normal, char* height, char* spec, glm::vec4 mambient, float mshininess, float mroughness);
	GLuint getVao() { return vao; }
<<<<<<< HEAD
	void Model::drawObject(ShaderProgram *shaderProgram, glm::mat4 mP, glm::mat4 mV, glm::mat4 mM, glm::vec3 playerPosition,Light &lights);
	GLuint Model::readTexture(char* filename);
	void computeTangentBasis();
=======
	void Model::drawObject(ShaderProgram *shaderProgram, glm::mat4 mP, glm::mat4 mV, glm::mat4 mM);
	void setPozycja(glm::vec3 a) { pozycja = a; }
>>>>>>> A-Kolizja
};

