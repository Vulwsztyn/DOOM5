#pragma once

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

using namespace glm;

class Gracz
{
private:
	vec3 angle;
	vec3 position;
	//movement: 1 do przodu,-1 do ty³u,0 nie
	int movement;
	//rotation: 1 w prawo,-1 w lewo,0 nie
	int rotation;
	
public:
	Gracz();
	~Gracz();

	void rusz(double czas);

	vec3 getPosition() { return position; }
	vec3 getAngle() { return angle; }
	void setMovement(int value) { movement = value; }
	void setRotation(int value) {rotation = value;	}

};

