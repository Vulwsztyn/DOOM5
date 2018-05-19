#include "Gracz.h"



Gracz::Gracz()
{
	angle = vec3(0.0f, 0.0f, 0.0f);
	position = vec3(0.0f, 0.0f, -5.0f);
	rotation = 0;
	movement = 0;
}


Gracz::~Gracz()
{
}

void Gracz::rusz(double czas)
{
	angle += rotation*playerRotationSpeed*vec3(czas,0,0);
	position += movement*playerMovementSpeed*vec3(czas*sin(angle.x), 0.0f,czas*cos(angle.x));
}

