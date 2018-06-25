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

float sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)  
// po kt�rej stronie prostej p2 do p3 jest punkt p1
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p1.y - p3.y) * (p2.x - p3.x);
}

bool PointInTriangle(glm::vec2 position, glm::vec2 triangle[3]) 
//funkcja wykrywaj�ca czy punkt jest w obr�bie tr�jk�ta spatrzona z neta, wi�c chyba dobra <3
{
	//position x,z gracza
	// traingle x1,z1,x2,z2,x,3,z3 wsp�rz�dne wierzcho�k�w tr�jk�ta w kt�rym potencjalnie si� znajdujemy
	bool b1, b2, b3;

	b1 = sign(position, triangle[0], triangle[1]) < 0.0f;
	b2 = sign(position, triangle[1], triangle[2]) < 0.0f;
	b3 = sign(position, triangle[2], triangle[0]) < 0.0f;

	return ((b1 == b2) && (b2 == b3));
}

bool detectTerrainColision(Model &map, glm::vec3 position,int w, bool debug) {
	// detekcja kolizji z terenem narazie daje 1 jak jest w obr�bie jakiego� tr�jk�ta i 0 jak nie jest patrz�c tylko na x i z
	int w1 = (w + 1) % 3;
	int w0 = (w + 2) % 3;
	glm::vec3 triangle[3];
	for (int i = 0; i < map.getVertices().size(); i = i + 12) {
		//huh, no to tak, te Vertices s� zapisane po kolei jako x,y,z i ten mno�nik co jest zawsze jeden na razie
		//wi�c zczytujemy tylko x,y,z 3 punkt�w tworz�cych tr�jk�t
		//dopisuj� komentarz, �eby zrozumie� kod
		//co 12 bo 3 wierzcho�ki po 4 wsp�rz�dne

		for (int j = 0; j < 3; j++) {
			//dla ka�dego wierzho�ka w pliku jest x,y,z,w wi�c:
			triangle[j][w0] = map.getVertices()[i + 4 * j];
			triangle[j][w] = map.getVertices()[i + 4 * j + 1];
			triangle[j][w1] = map.getVertices()[i + 4 * j + 2];
		}

		//to jest ca�y trujk�t z�o�ony z 3 wierzcho�k�w, y olewamy
		glm::vec2 planeTriangle[3] = {
			glm::vec2(triangle[0][w0],triangle[0][w1]),
			glm::vec2(triangle[1][w0],triangle[1][w1]) ,
			glm::vec2(triangle[2][w0],triangle[2][w1]) };

		//po wczytaniu takiego tr�jk�ta sprawdzam czy pozycja gracza jest w jego wn�trzu
		bool warunek = PointInTriangle(glm::vec2(position[w0], position[w1]), planeTriangle);

		if (warunek) {
			if (debug == true) {
				// wypisuje x i z tego tr�jk�ta w kt�rym rzekomo jest gracz je�eli "tryb debugu"
				std::cout << position[w0] << " " << position[w1] << std::endl;
				std::cout << triangle[0][w0] << " " << triangle[0][w1] << " " << triangle[1][w0] << " " << triangle[1][w1] << " " << triangle[2][w0] << " " << triangle[2][w1] << std::endl;
			}
			if (triangle[0][w] > position[w] || triangle[1][w] > position[w] || triangle[2][w] > position[w]) {
				return true;
			}
		}
	}
	return false;
}

bool areFloatsEqual(float a, float b) {
	float e = 0.00001;
	float r = b - a;
	if (r < 0)r = -r;
	//cout << "roznica: " << r << " eps: " << e << endl; //I'm retarded 1/1000 to 0 bo to inty
	return r < e;
}

bool isFloatBetween(float a, float p1, float p2) {
	if (a > p1&&a < p2) return true;
	if (a < p1&&a > p2) return true;
	return false;
}
bool isFloatBetweenOrEqual(float a, float p1, float p2) {
	if (a >= p1&&a <= p2) return true;
	if (a <= p1&&a >= p2) return true;
	return false;
}

float trzeciWymiarTrojkata(glm::vec3 position, glm::vec3 triangle[3],int w) {
	//wymiar-w: x-0 lewo prawo, y-1 gora dol (czyli wysokosc punktu),z-2 przod tyl
	int w1 = (w + 1) % 3;
	int w0 = (w + 2) % 3;

	//komentarze pisane jak do wysoko�ci
	//y olewam
	std::cout << "zaczynam" << std::endl;
	float x = position[w0];
	float z = position[w1];
	/*
	DEBUG DO WYPISU
	wypiszvec3(position);
	for (int i = 0; i<3; i++) {
	wypiszvec3(triangle[i]);
	cout << areFloatsEqual(x, triangle[i].x) << endl;
	}
	*/
	//je�eli jest wierzcholkiem
	for (int i = 0; i < 3; i++) {
		if (areFloatsEqual(x, triangle[i][w0]) && areFloatsEqual(z, triangle[i][w1])) {
			std::cout << "jest wierzcholkiem" << std::endl;
			return triangle[i][w];
		}
	}

	//znajduje 2 punkty na obwodzie trunkata, kt�re maj� x r�wne x punktu
	glm::vec3 p[2];

	//najpeirw szukam czy x nie jest r�wny x jednego z wierzcholkow
	bool czyXwierzcholka = false;
	for (int i = 0; i < 3; i++) {
		//powstaje pytanie po co w ogole si� tym przejmuj�???
		//ot� gdy x jest x wierzcholka to ka�dy z bok�w tr�jk�ta zawiera punkt o wspolzeniej x
		//i jak widac w nastenym bloku kodu if (!czyXwierzcholka) {
		//ca�kowicie olewam tam wszystko i by�by problem ze stworzeniem odcinka p[1], bo gdyby dla dw�ch pierwszych iteracji p�tli znalazz�o ten punkt kt�ry ma x wierzcholka to oba p[1] by�by takie same
		//ale rozprawka tej
		if (areFloatsEqual(x, triangle[i][w0])) {
			std::cout << "ma x wierzcholka" << std::endl;
			czyXwierzcholka = true;
			p[0] = triangle[i];
			//drugi punkt musi by� na przeciwleglym boku,
			//potrzbeuje pozostale punkty w postaci zmiennych bo umre
			glm::vec3 r1 = triangle[(i + 1) % 3];
			glm::vec3 r2 = triangle[(i + 2) % 3];
			//p[1].x=x;//chyba niepotrzebne jakby przeszakdza�o to usu� jak ju� ise oka�e, �e dzia�a
			// ulamek 0-1 odleglosc od r1.x do r2.x gdzie ulamek=0 gdy x=r1.x, a ulamek =1 gdy x=r2.x;
			//zak�adam, �e r1.x<r2.x, ale matematyka dzia�a w nadal
			float ulamek = ((x - r1[w0]) / (r2[w0] - r1[w0]));
			p[1][w] = r1[w] + ulamek * (r2[w] - r1[w]);
			p[1][w1] = r1[w1] + ulamek * (r2[w1] - r1[w1]);
		}
	}
	//zauwazmy ze jezli x pozycji to x jednego z wierzcholkow to mamy do czynienia z sytuacj�, gdy x jest wspolrzedn� punkt�w na wszystkich trzech bokach
	//wi�c jak nie jest to jest wsp�rz�dn� punkt�w na tylko dw�ch bokach
	if (!czyXwierzcholka) {
		std::cout << "nie ma x wierzcholka" << std::endl;
		int indeksPdoWypelnienia = 0;
		for (int i = 0; i < 3; i++) {
			int j = (i + 1) % 3; //indeks nastepnego wierzcholka
								 //gdy i=0 to j=1,i=1 to j=2, i=2 to j=0, czyli sprawdzamy wszystkie pary wierzcholkow
			if (isFloatBetween(x, triangle[i][w0], triangle[j][w0])) {
				//p[indeksPdoWypelnienia].x = x; //chyba niepotrzebne jakby przeszakdza�o to usu� jak ju� ise oka�e, �e dzia�a
				glm::vec3 r1 = triangle[i];
				glm::vec3 r2 = triangle[j];
				// ulamek 0-1 odleglosc od r1.x do r2.x gdzie ulamek=0 gdy x=r1.x, a ulamek =1 gdy x=r2.x;
				//zak�adam, �e r1.x<r2.x, ale matematyka dzia�a w nadal
				float ulamek = ((x - r1[w0]) / (r2[w0] - r1[w0]));
				p[indeksPdoWypelnienia][w] = r1[w] + ulamek * (r2[w] - r1[w]);
				p[indeksPdoWypelnienia][w1] = r1[w1] + ulamek * (r2[w1] - r1[w1]);
				indeksPdoWypelnienia++;
			}
		}
	}
	//w tym momnecie mamy ju� p[2] kt�re tworz� odcinek do ktroego nalezy punkt ktory nas interesuje
	//x mo�emy w sumie ola�
	//zak�adam, �e p[0].z<p[1].z, ale matematyka dzia�a w nadal
	std::cout << "koniec" << std::endl;
	float ulamek = ((z - p[0][w1]) / (p[1][w1] - p[0][w1]));
	float wynik = p[0][w] + ulamek * (p[1][w] - p[0][w]);;
	return wynik;
}

bool areBothFloatsOutside(float a, float b, float c1, float c2) {
	return (a < c1&&a < c2&&b < c1&&b < c2) || (a > c1&&a > c2&&b > c1&&b > c2);
}

bool triangleSegmentIntersection(glm::vec3 triangle[3], glm::vec3 mins,vec3 maxs,bool debug) {
	vec3 mint=triangle[0],maxt= triangle[0];
	for (int i = 1; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (mint[j] > triangle[i][j]) mint[j] = triangle[i][j];
			if (maxt[j] < triangle[i][j]) maxt[j] = triangle[i][j];
		}
	}

	if (debug) {
		cout << "trojkat: " << endl;
		for (int i = 0; i < 3; i++) {
			wypiszvec3(triangle[i]);
		}
		cout << "odcinek: " << endl;
		cout << "mint: ";
		wypiszvec3(mint);
		cout << "maxt: ";
		wypiszvec3(maxt);
		cout << "mins: ";
		wypiszvec3(mins);
		cout << "maxs: ";
		wypiszvec3(maxs);

	}
	
	for (int i = 0; i < 3; i++) {
		if (areBothFloatsOutside(mins[i], maxs[i], mint[i], maxt[i])) return false;
	}
	return true;
}


bool terrainCollision(Model &map, glm::vec3 position,bool debug) {
	vec3 seg[2];
	int wysokosc = wysokoscGracza;
	int szerokosc = szerokoscGracza;
	seg[0] = vec3(position.x - szerokosc, position.y - wysokosc, position.z - szerokosc);
	seg[1] = seg[0] + vec3(szerokosc * 2, wysokosc * 2, szerokosc * 2);
	vec3 mins=seg[0];
	vec3 maxs=seg[1];

	for (int i = 0; i < map.getVertices().size(); i = i + 12) {
		vec3 triangle[3];
		for (int j = 0; j < 3; j++) {
			triangle[j][0] = map.getVertices()[i + 4 * j];
			triangle[j][1] = map.getVertices()[i + 4 * j + 1];
			triangle[j][2] = map.getVertices()[i + 4 * j + 2];
		}
		if (triangleSegmentIntersection(triangle, mins,maxs,debug) ){
			//cout << i << endl;
			return true;
		}
	}
	return false;
}
float minZvec3(vec3 a) {
	float w = a.x;
	if (a.y < w) w = a.y;
	if (a.z < w) return a.z;
	return w;
}
float maxZvec3(vec3 a) {
	float w = a.x;
	if (a.y > w) w = a.y;
	if (a.z > w) return a.z;
	return w;
}
int sign(float a) {
	if (a > 0) return 1;
	if (a < 0) return -1;
	return 0;
}

