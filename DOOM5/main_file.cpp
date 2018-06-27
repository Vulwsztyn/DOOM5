/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS

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
#include "Model.h"
#include "Lights.h"
#include "naszaMatematyka.h"

using namespace glm;
//using namespace std;

float aspect=1; //Stosunek szerokości do wysokości okna

//Uchwyty na shadery
ShaderProgram *shaderProgram; //Wskaźnik na obiekt reprezentujący program cieniujący.
ShaderProgram *lightShader;
GLuint vao;
GLuint bufVertices;
GLuint bufColors;
GLuint bufNormals;
Light lights[2] ={ vec4(5,2, 0,1),vec4 (0.1f, 0.1f, 0.1f,1),vec4 (1, 1, 1,1),vec4( 1.0f, 1.0f, 1.0f,1),vec4(1.0f, 1.0f, 1.0f, 1),vec4(22,2, 0,1),vec4(0.1f, 0.1f, 0.1f,1),vec4(1, 1, 1,1),vec4(1.0f, 1.0f, 1.0f,1),vec4(1.0f, 1.0f, 1.0f, 1) };
int numberOfLights = 2;

Model map[2];
Model lightsObj;

Gracz gracz = Gracz();

double oldMouseX;
double oldMouseY;
bool oldMouseInitiated = false;
bool joystickConnected = false;

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura obsługi klawiatury
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_A:
		case GLFW_KEY_LEFT:
			gracz.addMovementX(1);
			break;
		case GLFW_KEY_D:
		case GLFW_KEY_RIGHT:
			gracz.addMovementX(-1);
			break;
		case GLFW_KEY_W:
		case GLFW_KEY_UP:
			gracz.addMovementZ(1);
			break;
		case GLFW_KEY_S:
		case GLFW_KEY_DOWN:
			gracz.addMovementZ(-1);
			break;
		case GLFW_KEY_SPACE:
			gracz.skocz();
			break;
			
		case GLFW_KEY_O:
			gracz.setPositon(vec3(3.0f, 20.0f, 0.0f));
			break;
		
		case GLFW_KEY_P:
			wypiszvec3(gracz.getPosition());
			wypiszvec2(gracz.getAngle());
			break;
		}
	}

	if (action == GLFW_RELEASE) {
		switch (key) {
		case GLFW_KEY_A:
		case GLFW_KEY_LEFT:
			gracz.addMovementX(-1);
			break;
		case GLFW_KEY_D:
		case GLFW_KEY_RIGHT:
			gracz.addMovementX(1);
			break;
		case GLFW_KEY_W:
		case GLFW_KEY_UP:
			gracz.addMovementZ(-1);
			break;
		case GLFW_KEY_S:
		case GLFW_KEY_DOWN:
			gracz.addMovementZ(1);
			break;
		}
	}
}
void joystick_functions() {
	int count = 2;
	const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
	gracz.setMovementX(-1 * axes[0]);
	gracz.setMovementZ(axes[1]);
	gracz.addRotationX(-axes[2] / stalaDoDzieleniaObrotuZPada);
	gracz.addRotationY(2 * axes[3] / stalaDoDzieleniaObrotuZPada);
	count = 14;
	const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
	if (buttons[0] == GLFW_PRESS) gracz.skocz();
	//OSIE
	//X LEWEGO,YLEWEGO,XPRAWEGO,YPRAWEGO,LEWY TRIGGER,PRAWY TRIGGER
	//GUZIKI
	//A,B,X,Y,LB,RB,BACK,START,LEWY GRZYBEK,PRAWY GRZYBEK,GORA,PRAWO,DOL,LEWO
}

void joystick_callback(int joy, int eve) {
	joystickConnected = eve == GLFW_CONNECTED;
}
void cursor_enter_callback(GLFWwindow* window, int entered) {
	if (!entered) oldMouseInitiated = false;
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (oldMouseInitiated) {
		gracz.addRotationX((oldMouseX - xpos) / pixelsPerRotation);
		gracz.addRotationY((oldMouseY - ypos) / pixelsPerRotation);
	}
	oldMouseX = xpos;
	oldMouseY = ypos;
	oldMouseInitiated = true;
}


//Procedura obługi zmiany rozmiaru bufora ramki
void windowResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
    if (height!=0) {
        aspect=(float)width/(float)height; //Stosunek szerokości do wysokości okna
    } else {
        aspect=1;
    }
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0, 0, 0, 1); //Czyść ekran na czarno
	glEnable(GL_DEPTH_TEST); //Włącz używanie Z-Bufora
	glfwSetKeyCallback(window, key_callback); //Zarejestruj procedurę obsługi klawiatury
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetCursorEnterCallback(window, cursor_enter_callback);
	glfwSetJoystickCallback(joystick_callback);
	joystickConnected = glfwJoystickPresent(0);
	glfwSetFramebufferSizeCallback(window, windowResize); //Zarejestruj procedurę obsługi zmiany rozmiaru bufora ramki
	shaderProgram = new ShaderProgram("vshader.glsl",NULL, "fshader.glsl"); //Wczytaj program cieniujący
	lightShader = new ShaderProgram("vshader.glsl", NULL, "lightfshader.glsl"); //Wczytaj program cieniujący

	map[0].loader("e1m1_walls.obj");
	map[1].loader("e1m1_floor.obj");
	lightsObj.loader("light.obj");
	map[0].prepareObject(shaderProgram,"Textures/CliffJagged004_COL_VAR1_1K.png","Textures/CliffJagged004_NRM_1K.png","Textures/CliffJagged004_DISP_VAR1_1K.png","Textures/CliffJagged004_GLOSS_1K.png",vec4(0.2, 0.2, 0.2, 1), 32, 0.1);
	map[1].prepareObject(shaderProgram, "Textures/GroundClay002_COL_VAR1_1K.png", "Textures/GroundClay002_NRM_1K.png", "Textures/GroundClay002_DISP_1K.png", "Textures/GroundClay002_GLOSS_1K.png",vec4(0.2, 0.2, 0.2, 1), 32, 0.02);
	lightsObj.prepareObject(lightShader, "light.png", "light.png", "light.png", "light.png",vec4(1,1,1,1),1,1);
}

//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram() {
	delete shaderProgram; //Usunięcie programu cieniującego
	}



//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window) {
	//************Tutaj umieszczaj kod rysujący obraz******************l

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wykonaj czyszczenie bufora kolorów

	glm::mat4 P = glm::perspective(50 * PI / 180, aspect, 1.0f, 50.0f); //Wylicz macierz rzutowania

	vec3 vec = vec3(
		gracz.getPosition().x + sin(gracz.getAngle().x),
		gracz.getPosition().y + sin(gracz.getAngle().y),
		gracz.getPosition().z + cos(gracz.getAngle().x)

	);

	glm::mat4 V = glm::lookAt(					//Wylicz macierz widoku
		glm::vec3(gracz.getPosition().x, gracz.getPosition().y, gracz.getPosition().z),					//kamera znajduje sie w
		vec,									//patrzy na
		glm::vec3(0, 1, 0)						//wektor nosa
	);

		

		//cout << gracz.getAngle().x << " " << gracz.getAngle().y << " " << gracz.getAngle().z << endl;


	//Wylicz macierz modelu rysowanego obiektu
	glm::mat4 M = glm::mat4(1.0f);
	//M = glm::rotate(M, gracz.getPosition().x, glm::vec3(1, 0, 0));
	//M = glm::rotate(M, gracz.getPosition().z, glm::vec3(0, 1, 0));
	//M = glm::translate(M,position);
	

	//Narysuj obiekt
	map[0].drawObject(shaderProgram,P,V,M, gracz.getPosition(),lights,numberOfLights);
	map[1].drawObject(shaderProgram, P, V, M, gracz.getPosition(), lights, numberOfLights);
	M = glm::translate(M, vec3(5, 2, 0));
	lightsObj.drawObject(lightShader, P, V, M, gracz.getPosition(), lights, numberOfLights);
	M = glm::mat4(1.0f);
	M = glm::translate(M, vec3(22, 2, 0));
	lightsObj.drawObject(lightShader, P, V, M, gracz.getPosition(), lights, numberOfLights);

	//Przerzuć tylny bufor na przedni

}


int main(void)
{
	
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów
	
	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}
	
	window = glfwCreateWindow(1000, 1000, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	
	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora
	
	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}
	
	initOpenGLProgram(window); //Operacje inicjujące

	glfwSetTime(0); //Wyzeruj licznik czasu
	
	//debuguj tutaj jeśli jednorazowo

	glm::vec3 a[3];
	glm::vec3 b[2];
	//a[0] = glm::vec3(0, 0, 0);
	//a[1] = glm::vec3(0, 1, 1);
	//a[2] = glm::vec3(1, 1, 0);
	//b[0] = glm::vec3(0.5, 0, 0.5);
	//b[1] = glm::vec3(0, 1, 0);
	a[0] = glm::vec3(0, 0, 2);
	a[1] = glm::vec3(2, 200, -6);
	a[2] = glm::vec3(3, 900, -10);
	b[0] = glm::vec3(-1, 5, 1);
	b[1] = glm::vec3(3, 5, 2);
	//cout << triangleSegmentIntersection(a, b) << endl;
	gracz.setTrojkat(map[1].getVertices());
	//Główna pętla
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		//angle += speed*vec3(glfwGetTime(), glfwGetTime(), glfwGetTime()); //nie mam pojecia czy tak jest lepiej
		if (60*glfwGetTime()>1) {
			//cout << glfwGetTime() << endl;
			gracz.rusz(map, glfwGetTime());
			glfwSetTime(0); //Wyzeruj licznik czasu
		}
		drawScene(window); //Wykonaj procedurę rysującą
		glfwSwapBuffers(window);
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram();

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
