#pragma once
#ifndef DEBUG_FUNCTONS_H
#define DEBUG_FUNCTONS_H
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
#include "debugFunctions.h"

using namespace std;
using namespace glm;

void wypiszvec3(vec3 a);
void wypiszvec2(vec2 a);
void wypiszGuzikiPada();
void wypiszOsiePada();

#endif