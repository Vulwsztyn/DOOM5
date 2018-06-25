#include "Gracz.h"
#include "debugFunctions.h"
#include "naszaMatematyka.h"

Gracz::Gracz()
{
	angle = vec2(4.6f, 0.0f);
	position = vec3(0.0f, 5.0f, 0.0f);
	rotation = vec2(3.2f, 0.0f);
	normalisedRotation = vec2(0, 0);
	movement = vec3(0, 0, 0);
	normalisedMovement = vec3(0, 0, 0);
	speed = vec3(0, 0, 0);
	rotationSpeed = vec2(0.0f, 0.0f);
}


Gracz::~Gracz()
{
	//nie umiem robic destruktorow
}

void Gracz::skocz() {
	speed.y = playerJumpSpeed;
}


bool Gracz::setTrojkat(std::vector<float> a) {
	for (int i = 0; i < a.size(); i = i + 12) {
		vec2 triangle[3];
		for (int j = 0; j < 3; j++) {
			triangle[j].x = a[i + 4 * j];
			triangle[j].y = a[i + 4 * j + 2];
		}
		if (PointInTriangle(vec2(position.x, position.z), triangle)) {
			cout << i << endl;
			for (int j = 0; j < 3; j++) {
				trojkat[j] = triangle[j];
			}
			minWysokosc = maxZvec3(vec3(a[i + 1], a[i + 5], a[i + 9]));
			return true;
		}
	}
	return false;
}

void Gracz::rusz(Model map[2], double czas)
{
	
	//je¿eli nie leci
	if (1||!speed.y) {
		normalisedMovement = normalize(vec3(movement.x*sin(angle.x + PI / 2) + movement.z*sin(angle.x), 0.0f, movement.z*cos(angle.x) + movement.x*cos(angle.x + PI / 2)));
		if (normalisedMovement.x != normalisedMovement.x || normalisedMovement.y != normalisedMovement.y || normalisedMovement.z != normalisedMovement.z) {
			normalisedMovement = vec3(0, 0, 0);
		}
		speed += vec3((normalisedMovement.x - speed.x)*czas / czasPrzyspieszania, 0, (normalisedMovement.z - speed.z)*czas / czasPrzyspieszania);
	}

	speed.y -= czas * gravitationalConstant;
	if (position.y+sign(speed.y)*1.5>minWysokosc) {
		position.y += speed.y*czas;
	}
	else {
		speed.y = 0;
	}

	vec3 move = vec3(czas*playerMovementSpeed*speed.x, 0.0f, czas*playerMovementSpeed*speed.z);
	
	
	//opuszczamTrojkat(map[1], );
	if (PointInTriangle(vec2(position.x + sign(speed.x)/3, position.z + sign(speed.z)/3), trojkat)) {
		position += move;
	}
	else if (setTrojkat(map[1].getVertices()) ){
		position += move;
	}
}


	
