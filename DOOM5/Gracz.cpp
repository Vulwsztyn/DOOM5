#include "Gracz.h"
#include "debugFunctions.h"
#include "naszaMatematyka.h"

Gracz::Gracz()
{
	angle = vec2(4.6f, 0.0f);
	position = vec3(5.0f, 0.0f, 0.0f);
	rotation = vec2(0.0f, 0.0f);
	normalisedRotation = vec2(0, 0);
	movement = vec3(0, 0, 0);
	normalisedMovement = vec3(0, 0, 0);
	speed = vec3(0, 0, 0);
	rotationSpeed = vec2(0.0f, 0.0f);
}


Gracz::~Gracz()
{
	//nie umiem robi? destruktorow
}

void Gracz::skocz() {
	speed.y = playerJumpSpeed;
}



void Gracz::rusz(Model &map, double czas)
{
	if (position.y <= 0.05) {
		normalisedMovement = normalize(vec3(movement.x*sin(angle.x + PI / 2) + movement.z*sin(angle.x), 0.0f, movement.z*cos(angle.x) + movement.x*cos(angle.x + PI / 2)));
		if (normalisedMovement.x != normalisedMovement.x || normalisedMovement.y != normalisedMovement.y || normalisedMovement.z != normalisedMovement.z) {
			normalisedMovement = vec3(0, 0, 0);
		}
		speed += vec3((normalisedMovement.x - speed.x)*czas / czasPrzyspieszania, 0, (normalisedMovement.z - speed.z)*czas / czasPrzyspieszania);
	}

	if (position.y != 0 || speed.y != 0) {
		if (position.y < 0) {
			position.y = 0;
			speed.y = 0;
		}
		position.y += speed.y*czas;
		speed.y -= czas * gravitationalConstant;
	}

	if (detectTerrainColision(map,position) != 1) {
		position += vec3(czas*playerMovementSpeed*speed.x, 0.0f, czas*playerMovementSpeed*speed.z);
	}

}

