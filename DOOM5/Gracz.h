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
#include "debugFunctions.h"
#include "Model.h"

using namespace glm;

class Gracz
{
private:
	//te 2 oznaczaj? aktualny stan
	//angle.x to gdzie patrzy na plaszczyznie rownleglej do ziemi
	//angle.y to k?t g?owy w gór? (albo dó?, zale?y jak wejdzie)
	vec2 angle;
	vec3 position;
	//te 2 oznaczaja chec zmiany polozenia i k?ta
	vec3 movement;
	vec3 normalisedMovement;
	vec2 rotation;
	vec2 normalisedRotation;
	//TODO: Skakanie
	//speed ma wartosc od 0 do 1
	vec3 speed;
	vec2 rotationSpeed;

public:
	Gracz();
	~Gracz();
	void rusz(Model &map,double czas);
	void skocz();

	vec3 getPosition() { return position; }
	vec2 getAngle() { return angle; }
	vec3 getSpeed() { return speed; }

	void addMovementX(float value) { movement.x += value; }
	void addMovementZ(float value) { movement.z += value; }

	void setMovementX(float value) { movement.x = value; }
	void setMovementZ(float value) { movement.z = value; }


	void addRotationX(float value) {angle.x += value;	}
	void addRotationY(float value) {
		angle.y += value;
		if (angle.y > maksymalnyKatWychyleniaGoraDol) angle.y = maksymalnyKatWychyleniaGoraDol;
		if (angle.y < -maksymalnyKatWychyleniaGoraDol) angle.y = -maksymalnyKatWychyleniaGoraDol;
	}

	void setRotationX(float value) {angle.x = value;}
	void setRotationY(float value) {
		angle.y = value;
		if (angle.y > maksymalnyKatWychyleniaGoraDol) angle.y = maksymalnyKatWychyleniaGoraDol;
		if (angle.y < -maksymalnyKatWychyleniaGoraDol) angle.y = -maksymalnyKatWychyleniaGoraDol;
	}

	bool detectTerrainColision(Model &map, bool debug = false);
	float sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);

	bool PointInTriangle(glm::vec2 position, glm::vec2 triangle[3]);
};

