#include "Gracz.h"
#include "debugFunctions.h"


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

float Gracz::sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)  // po której stronie prostej p2 do p3 jest punkt p1
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p1.y - p3.y) * (p2.x - p3.x);
}

bool Gracz::PointInTriangle(glm::vec2 position, glm::vec2 triangle[3]) //funkcja wykrywaj¹ca czy punkt jest w obrêbie trójk¹ta spatrzona z neta, wiêc chyba dobra
{
	//position x,z gracza
	// traingle x1,z1,x2,z2,x,3,z3 wspó³rzêdne wierzcho³ków trójk¹ta w którym potencjalnie siê znajdujemy
	bool b1, b2, b3;

	b1 = sign(position, triangle[0], triangle[1]) < 0.0f;
	b2 = sign(position, triangle[1], triangle[2]) < 0.0f;
	b3 = sign(position, triangle[2], triangle[0]) < 0.0f;

	return ((b1 == b2) && (b2 == b3));
}

bool Gracz::detectTerrainColision(Model &map,bool debug) { // detekcja kolizji z terenem narazie daje 1 jak jest w obrêbie jakiegoœ trójk¹ta i 0 jak nie jest patrz¹c tylko na x i z
	glm::vec3 triangle[3];
	for(int i = 0; i < map.getVertices().size(); i = i + 12) { //huh, no to tak, te Vertices s¹ zapisane po kolei jako x,y,z i ten mno¿nik co jest zawsze jeden na razie
															//wiêc zczytujemy tylko x,y,z 3 punktów tworz¹cych trójk¹t
		//dopisujê komentarz, ¿eby zrozumieæ kod
		//co 12 bo 3 wierzcho³ki po 4 wspó³rzêdne

		for (int j = 0; j < 3; j++) {
			//dla ka¿dego wierzho³ka w pliku jest x,y,z,w wiêc:
			triangle[j].x = map.getVertices()[i+4*j];
			triangle[j].y = map.getVertices()[i + 4 * j + 1];
			triangle[j].z = map.getVertices()[i + 4 * j + 2];
		}
		
		//to jest ca³y trujk¹t z³o¿ony z 3 wierzcho³ków, y olewamy
		glm::vec2 planeTriangle[3] = { 
			glm::vec2(triangle[0].x,triangle[0].z),
			glm::vec2(triangle[1].x,triangle[1].z) ,
			glm::vec2(triangle[2].x,triangle[2].z) };

		//po wczytaniu takiego trójk¹ta sprawdzam czy pozycja gracza jest w jego wnêtrzu
		bool warunek = PointInTriangle(glm::vec2(position.x, position.z), planeTriangle);

		if (warunek) {  
			if (debug == true) {                                                  
				// wypisuje x i z tego trójk¹ta w którym rzekomo jest gracz je¿eli "tryb debugu"
				std::cout << position.x << " " << position.z << std::endl;
				std::cout << triangle[0].x << " " << triangle[0].z << " " << triangle[1].x << " " << triangle[1].z << " " << triangle[2].x << " " << triangle[2].z << std::endl;
			}
			if (triangle[0].y > position.y || triangle[1].y > position.y || triangle[2].y > position.y) {
				return true;
			}
		}
	}
	return false;
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

	if (detectTerrainColision(map) != 1) {
		position += vec3(czas*playerMovementSpeed*speed.x, 0.0f, czas*playerMovementSpeed*speed.z);
	}

}

bool areFloatsEqual(float a, float b) {
	float e = 0.00001;
	float r = b - a;
	if (r < 0)r = -r;
	//cout << "roznica: " << r << " eps: " << e << endl; I'm retarded 1/1000 to 0 bo to inty
	return r < e;
}
bool isFloatBetween(float a, float p1,float p2) {
	if (a > p1&&a < p2) return true;
	if (a < p1&&a > p2) return true;
	return false;
}

float Gracz::wysokoscPunktuTrojkata(glm::vec3 position, glm::vec3 triangle[3]) {
	//y olewam
	
	std::cout << "zaczynam" << std::endl;
	float x = position.x;
	float z = position.z;
	/*
	DEBUG DO WYPISU
	wypiszvec3(position);
	for (int i = 0; i<3; i++) {
		wypiszvec3(triangle[i]);
		cout << areFloatsEqual(x, triangle[i].x) << endl;
	}
	*/
	//je¿eli jest wierzcholkiem
	for (int i = 0; i < 3; i++) {
		if (areFloatsEqual(x, triangle[i].x) && areFloatsEqual(z, triangle[i].z)) {
			std::cout << "jest wierzcholkiem" << std::endl;
			return triangle[i].y;
		}
	}
	
	//znajduje 2 punkty na obwodzie trunkata, które maj¹ x równe x punktu
	glm::vec3 p[2];

	//najpeirw szukam czy x nie jest równy x jednego z wierzcholkow
	bool czyXwierzcholka = false;
	for (int i = 0; i < 3; i++) {
		//powstaje pytanie po co w ogole siê tym przejmujê???
		//otó¿ gdy x jest x wierzcholka to ka¿dy z boków trójk¹ta zawiera punkt o wspolzeniej x
		//i jak widac w nastenym bloku kodu if (!czyXwierzcholka) {
		//ca³kowicie olewam tam wszystko i by³by problem ze stworzeniem odcinka p[1], bo gdyby dla dwóch pierwszych iteracji pêtli znalazz³o ten punkt który ma x wierzcholka to oba p[1] by³by takie same
		//ale rozprawka tej
		if (areFloatsEqual(x, triangle[i].x)) {
			std::cout << "ma x wierzcholka" << std::endl;
			czyXwierzcholka = true;
			p[0] = triangle[i];
			//drugi punkt musi byæ na przeciwleglym boku,
			//potrzbeuje pozostale punkty w postaci zmiennych bo umre
			glm::vec3 r1 = triangle[(i + 1) % 3];
			glm::vec3 r2 = triangle[(i + 2) % 3];
			//p[1].x=x;//chyba niepotrzebne jakby przeszakdza³o to usuñ jak ju¿ ise oka¿e, ¿e dzia³a
			// ulamek 0-1 odleglosc od r1.x do r2.x gdzie ulamek=0 gdy x=r1.x, a ulamek =1 gdy x=r2.x;
			//zak³adam, ¿e r1.x<r2.x, ale matematyka dzia³a w nadal
			float ulamek = ((x - r1.x) / (r2.x - r1.x));
			p[1].y = r1.y + ulamek * (r2.y - r1.y);
			p[1].z = r1.z + ulamek * (r2.z - r1.z);
		}
	}
	//zauwazmy ze jezli x pozycji to x jednego z wierzcholkow to mamy do czynienia z sytuacj¹, gdy x jest wspolrzedn¹ punktów na wszystkich trzech bokach
	//wiêc jak nie jest to jest wspó³rzêdn¹ punktów na tylko dwóch bokach
	if (!czyXwierzcholka) {
		std::cout << "nie ma x wierzcholka" << std::endl;
		int indeksPdoWypelnienia=0;
		for (int i = 0; i < 3; i++) {
			int j = (i + 1) % 3; //indeks nastepnego wierzcholka
			//gdy i=0 to j=1,i=1 to j=2, i=2 to j=0, czyli sprawdzamy wszystkie pary wierzcholkow
			if (isFloatBetween(x, triangle[i].x, triangle[j].x) ){
				//p[indeksPdoWypelnienia].x = x; //chyba niepotrzebne jakby przeszakdza³o to usuñ jak ju¿ ise oka¿e, ¿e dzia³a
				glm::vec3 r1 = triangle[i];
				glm::vec3 r2 = triangle[j];
				// ulamek 0-1 odleglosc od r1.x do r2.x gdzie ulamek=0 gdy x=r1.x, a ulamek =1 gdy x=r2.x;
				//zak³adam, ¿e r1.x<r2.x, ale matematyka dzia³a w nadal
				float ulamek = ((x - r1.x) / (r2.x - r1.x));
				p[indeksPdoWypelnienia].y = r1.y + ulamek * (r2.y - r1.y);
				p[indeksPdoWypelnienia].z = r1.z + ulamek * (r2.z - r1.z);
				indeksPdoWypelnienia++;
			}
		}
	}
	//w tym momnecie mamy ju¿ p[2] które tworz¹ odcinek do ktroego nalezy punkt ktory nas interesuje
	//x mo¿emy w sumie olaæ
	//zak³adam, ¿e p[0].z<p[1].z, ale matematyka dzia³a w nadal
	std::cout << "koniec" << std::endl;
	float ulamek = ((z - p[0].z) / (p[1].z - p[0].z));
	float wynik = p[0].y + ulamek * (p[1].y - p[0].y);;
	return wynik;
}