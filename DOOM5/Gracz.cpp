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
	//nie umiem robi? destruktorow
}

void Gracz::skocz() {
	speed.y = playerJumpSpeed;
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
	if (!terrainCollision(map[1], vec3(position.x, position.y + speed.y*czas*2, position.z))) {
		position.y += speed.y*czas;
	}
	else {
		speed.y = 0;
	}

	vec3 move = vec3(czas*playerMovementSpeed*speed.x, 0.0f, czas*playerMovementSpeed*speed.z);
	if (!terrainCollision(map[0], position + move)) {
		position += move;
	}
}
	
