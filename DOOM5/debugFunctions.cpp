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

void wypiszvec3(vec3 a) {
	cout << a[0] << ' ' << a[1] << ' ' << a[2] << ' ' << std::endl;
}
void wypiszvec2(vec2 a) {
	cout << a.x << ' ' << a.y << ' ' << std::endl;
}
void wypiszGuzikiPada() {
	int count = 14;
	const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
	for (int i = 0;i < count; i++) {
		cout << buttons[i] << ' ';
	}
	cout << endl;
	//GUZIKI
	//A,B,X,Y,LB,RB,BACK,START,LEWY GRZYBEK,PRAWY GRZYBEK,GORA,PRAWO,DOL,LEWO
}
void wypiszOsiePada() {
	int count = 2;
	const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
	for (int i = 0; i < 4; i++) {
		cout << axes[i] << ' ';
	}
	cout << endl;

	//OSIE
	//X LEWEGO,YLEWEGO,XPRAWEGO,YPRAWEGO,LEWY TRIGGER,PRAWY TRIGGER
}

