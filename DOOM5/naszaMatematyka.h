
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "Gracz.h"

using namespace std;
using namespace glm;

//£ukaszowe
float sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3); 
bool PointInTriangle(glm::vec2 position, glm::vec2 triangle[3]); 
bool detectTerrainColision(Model &map, glm::vec3 position, int w = 1, bool debug = false);

//Arturowe
bool areFloatsEqual(float a, float b);
bool isFloatBetween(float a, float p1, float p2);
float trzeciWymiarTrojkata(glm::vec3 position, glm::vec3 triangle[3], int w=1);
bool triangleSegmentIntersection(glm::vec3 triangle[3], glm::vec3 mins, vec3 maxs,bool debug=0);
bool terrainCollision(Model &map, glm::vec3 position,bool debug=0);

