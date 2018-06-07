#include "Gracz.h"



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
void Gracz::rusz(double czas)
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

	position += vec3(czas*speed.x, 0.0f, czas*speed.z);

}

float Gracz::sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool Gracz::PointInTriangle(glm::vec2 position, glm::vec2 triangle[3])
{
	bool b1, b2, b3;

	b1 = sign(position, triangle[0], triangle[1]) < 0.0f;
	b2 = sign(position, triangle[1], triangle[2]) < 0.0f;
	b3 = sign(position, triangle[2], triangle[0]) < 0.0f;

	return ((b1 == b2) && (b2 == b3));
}

bool Gracz::detectTerrainColision(Model &map) {
	glm::vec3 triangle[3];
	for(int i = 0; i < map.getVertices().size(); i = i + 12) {
		int j = 0;
		triangle[j].x = map.getVertices()[i];
		triangle[j].y = map.getVertices()[i+1];
		triangle[j].z = map.getVertices()[i+2];
		j++;
		triangle[j].x = map.getVertices()[i+4];
		triangle[j].y = map.getVertices()[i+4 + 1];
		triangle[j].z = map.getVertices()[i+4 + 2];
		j++;
		triangle[j].x = map.getVertices()[i+8];
		triangle[j].y = map.getVertices()[i+8 + 1];
		triangle[j].z = map.getVertices()[i+8 + 2];
		j++;
		glm::vec2 planeTriangle[3] = { glm::vec2(triangle[j - 2].x,triangle[j - 2].z),glm::vec2(triangle[j - 1].x,triangle[j - 1].z) ,glm::vec2(triangle[j].x,triangle[j].z) };
		if (PointInTriangle(glm::vec2(position.x, position.z), planeTriangle)) {
			return true;
			glm::vec2 planeTriangle[3] = { glm::vec2(triangle[j - 2].x,triangle[j - 2].y),glm::vec2(triangle[j - 1].x,triangle[j - 1].y) ,glm::vec2(triangle[j].x,triangle[j].y) };
			if (PointInTriangle(glm::vec2(position.x, position.y), planeTriangle)) {
				//std::cout << triangle[0].x << " " << triangle[0].z << " " << triangle[0].y << " " << std::endl;
				//std::cout << triangle[1].x << " " << triangle[1].z << " " << triangle[1].y << " " << std::endl;
				//std::cout << triangle[2].x << " " << triangle[2].z << " " << triangle[2].y << " " << std::endl;
				return true;
			}
		}
	}
	return false;
}