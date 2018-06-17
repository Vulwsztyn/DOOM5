#include "Model.h"



Model::Model()
{
}


Model::~Model()
{
	glDeleteVertexArrays(1, &vao); //Usuniêcie vao
	glDeleteBuffers(1, &bufVertices); //Usuniêcie VBO z wierzcho³kami
	glDeleteBuffers(1, &bufColors); //Usuniêcie VBO z kolorami
	glDeleteBuffers(1, &bufNormals); //Usuniêcie VBO z wektorami normalnymi
	glDeleteBuffers(1, &bufC1);
	glDeleteBuffers(1, &bufC2);
	glDeleteBuffers(1, &bufC3);

	//Wykasuj tekstury
	glDeleteTextures(1, &diffTex);
	glDeleteTextures(1, &normalTex);
	glDeleteTextures(1, &heightTex);
}


bool Model::loader(const char * path) {
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;


	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	while (1) {
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		//std::cout << lineHeader[0] << " " << test << std::endl;
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

				   // else : parse lineHeader
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0],&uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			//std::cout << vertexIndex[0] <<" "<<vertexIndex[1]<<" "<<vertexIndex[2]<< std::endl;
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);

			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);

			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
	// For each vertex of each triangle
	for (int i = 0; i < vertexIndices.size(); i++) {
		int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		vertices.push_back(vertex.x);
		vertices.push_back(vertex.y);
		vertices.push_back(vertex.z);
		vertices.push_back(1);
	}

	for (int i = 0; i < uvIndices.size(); i++) {
		int uvIndex = uvIndices[i];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		uvs.push_back(uv.x);
		uvs.push_back(uv.y);
		//std::cout << uv.x << " " << uv.y << std::endl;
	}


	for (int i = 0; i < normalIndices.size(); i++) {
		int normalIndex = normalIndices[i];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		vertexNormals.push_back(normal.x);
		vertexNormals.push_back(normal.y);
		vertexNormals.push_back(normal.z);
		vertexNormals.push_back(0);
	}

}

void Model::computeTangentBasis() {
	int j = 0;
	for (int i = 0; i < vertices.size(); i += 12) {
		// Shortcuts for vertices
		glm::vec3 v0 = glm::vec3(vertices[i+0], vertices[i+1], vertices[i+2]);
		glm::vec3 v1 = glm::vec3(vertices[i + 4], vertices[i + 5], vertices[i + 6]);
		glm::vec3 v2 = glm::vec3(vertices[i + 8], vertices[i + 9], vertices[i + 10]);


		// Shortcuts for UVs
		glm::vec2 uv0 = glm::vec2(uvs[j+0], uvs[j + 1]);
		glm::vec2 uv1 = glm::vec2(uvs[j + 2] , uvs[j + 3]);
		glm::vec2 uv2 = glm::vec2(uvs[j + 4] , uvs[j + 5]);
		//std::cout << uv0.x << " " << uv0.y << " " << uv1.x << " " << uv1.y << " " << uv2.x << " " << uv2.y << " " << std::endl;
		// Edges of the triangle : position delta
		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;

		// UV delta
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;
		//std::cout << deltaUV1.x<<" "<<deltaUV1.y<<" " << deltaUV2.x << " " << deltaUV2.y << " " << std::endl;
		float r = 1 / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		//std::cout << r << std::endl;

		glm::vec3 normal = normalize(cross(deltaPos1, deltaPos2));
		std::cout << normal.x << " " << normal.y << " " << normal.z << std::endl;
		glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

		//std::cout << tangent.x << " " << tangent.y << " " << tangent.z << " " << bitangent.x << " " << bitangent.y << " " << bitangent.z << " " << std::endl;

		// Set the same tangent for all three vertices of the triangle.
		// They will be merged later, in vboindexer.cpp
		tangents.push_back(tangent.x);
		tangents.push_back(tangent.y);
		tangents.push_back(tangent.z);
		tangents.push_back(0);
		tangents.push_back(tangent.x);
		tangents.push_back(tangent.y);
		tangents.push_back(tangent.z);
		tangents.push_back(0);
		tangents.push_back(tangent.x);
		tangents.push_back(tangent.y);
		tangents.push_back(tangent.z);
		tangents.push_back(0);

		// Same thing for bitangents
		bitangents.push_back(bitangent.x);
		bitangents.push_back(bitangent.y);
		bitangents.push_back(bitangent.z);
		bitangents.push_back(0);
		bitangents.push_back(bitangent.x);
		bitangents.push_back(bitangent.y);
		bitangents.push_back(bitangent.z);
		bitangents.push_back(0);
		bitangents.push_back(bitangent.x);
		bitangents.push_back(bitangent.y);
		bitangents.push_back(bitangent.z);
		bitangents.push_back(0);

		normals.push_back(normal.x);
		normals.push_back(normal.y);
		normals.push_back(normal.z);
		normals.push_back(0);
		normals.push_back(normal.x);
		normals.push_back(normal.y);
		normals.push_back(normal.z);
		normals.push_back(0);
		normals.push_back(normal.x);
		normals.push_back(normal.y);
		normals.push_back(normal.z);
		normals.push_back(0);
		j += 6;
	}
}

GLuint Model::readTexture(char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamiêci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
							  //Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import do pamiêci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
									   //Wczytaj obrazek do pamiêci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

GLuint Model::makeBuffer(void *data, int vertexCount, int vertexSize) {
	GLuint handle;

	glGenBuffers(1, &handle);//Wygeneruj uchwyt na Vertex Buffer Object (VBO), który bêdzie zawiera³ tablicê danych
	glBindBuffer(GL_ARRAY_BUFFER, handle);  //Uaktywnij wygenerowany uchwyt VBO
	glBufferData(GL_ARRAY_BUFFER, vertexCount*vertexSize, data, GL_STATIC_DRAW);//Wgraj tablicê do VBO

	return handle;
}

void Model::assignVBOtoAttribute(ShaderProgram *shaderProgram, const char* attributeName, GLuint bufVBO, int vertexSize) {
	GLuint location = shaderProgram->getAttribLocation(attributeName); //Pobierz numer slotu dla atrybutu
	glBindBuffer(GL_ARRAY_BUFFER, bufVBO);  //Uaktywnij uchwyt VBO
	glEnableVertexAttribArray(location); //W³¹cz u¿ywanie atrybutu o numerze slotu zapisanym w zmiennej location
	glVertexAttribPointer(location, vertexSize, GL_FLOAT, GL_FALSE, 0, NULL); //Dane do slotu location maj¹ byæ brane z aktywnego VBO
}
void Model::prepareObject(ShaderProgram *shaderProgram) {
	//Zbuduj VBO z danymi obiektu do narysowania
	computeTangentBasis();
	bufVertices = makeBuffer(&getVertices()[0], getVertices().size() / 4, sizeof(float) * 4); //VBO ze wspó³rzêdnymi wierzcho³ków
	bufNormals = makeBuffer(&getvertexNormals()[0], getvertexNormals().size() / 4, sizeof(float) * 4);//VBO z wektorami normalnymi wierzcho³ków
	bufTexCoords = makeBuffer(&getUvs()[0], getUvs().size(), sizeof(float) * 2);//VBO ze wspó³rzêdnymi teksturowania
	bufC1 = makeBuffer(&getTangents()[0], getTangents().size() / 4, sizeof(float) * 4);//VBO z pierwsz¹ kolumn¹ macierzy TBN^-1 dla ka¿dego wierzcho³a
	bufC2 = makeBuffer(&getBitangents()[0], getBitangents().size() / 4, sizeof(float) * 4);//VBO z drug¹ kolumn¹ macierzy TBN^-1 dla ka¿dego wierzcho³a
	bufC3 = makeBuffer(&getNormals()[0], getNormals().size() / 4, sizeof(float) * 4);//VBO z trzeci¹ kolumn¹ macierzy TBN^-1 dla ka¿dego wierzcho³a
																								  //Zbuduj VAO wi¹¿¹cy atrybuty z konkretnymi VBO
	glGenVertexArrays(1, &vao); //Wygeneruj uchwyt na VAO i zapisz go do zmiennej globalnej

	glBindVertexArray(vao); //Uaktywnij nowo utworzony VAO

	assignVBOtoAttribute(shaderProgram, "vertex", bufVertices, 4); //"vertex" odnosi siê do deklaracji "in vec4 vertex;" w vertex shaderze
	assignVBOtoAttribute(shaderProgram, "normal", bufNormals, 4); //"normal" odnosi siê do deklaracji "in vec4 normal;" w vertex shaderze
	assignVBOtoAttribute(shaderProgram, "texCoord0", bufTexCoords, 2); //"texCoord0" odnosi siê do deklaracji "in vec2 texCoord0;" w vertex shaderze
	assignVBOtoAttribute(shaderProgram, "c1", bufC1, 4); //"c1" odnosi siê do deklaracji "in vec4 c1;" w vertex shaderze
	assignVBOtoAttribute(shaderProgram, "c2", bufC2, 4); //"c2" odnosi siê do deklaracji "in vec4 c2;" w vertex shaderze
	assignVBOtoAttribute(shaderProgram, "c3", bufC3, 4); //"c3" odnosi siê do deklaracji "in vec4 c3;" w vertex shaderze

	glBindVertexArray(0); //Dezaktywuj VAO

	diffTex = readTexture("bricks2_diffuse.png");
	normalTex = readTexture("bricks2_normal.png");
	heightTex = readTexture("bricks2_height.png");
}

void Model::drawObject(ShaderProgram *shaderProgram, glm::mat4 mP, glm::mat4 mV, glm::mat4 mM) {
	//W³¹czenie programu cieniuj¹cego, który ma zostaæ u¿yty do rysowania
	//W tym programie wystarczy³oby wywo³aæ to raz, w setupShaders, ale chodzi o pokazanie,
	//¿e mozna zmieniaæ program cieniuj¹cy podczas rysowania jednej sceny
	shaderProgram->use();

	//Przeka¿ do shadera macierze P,V i M.
	//W linijkach poni¿ej, polecenie:
	//  shaderProgram->getUniformLocation("P")
	//pobiera numer slotu odpowiadaj¹cego zmiennej jednorodnej o podanej nazwie
	//UWAGA! "P" w powy¿szym poleceniu odpowiada deklaracji "uniform mat4 P;" w vertex shaderze,
	//a mP w glm::value_ptr(mP) odpowiada argumentowi  "mat4 mP;" TYM pliku.
	//Ca³a poni¿sza linijka przekazuje do zmiennej jednorodnej P w vertex shaderze dane z argumentu mP niniejszej funkcji
	//Pozosta³e polecenia dzia³aj¹ podobnie.
	glUniformMatrix4fv(shaderProgram->getUniformLocation("P"), 1, false, glm::value_ptr(mP));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("V"), 1, false, glm::value_ptr(mV));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("M"), 1, false, glm::value_ptr(mM));

	//Powi¹¿ zmienne typu sampler2D z jednostkami teksturuj¹cymi
	glUniform1i(shaderProgram->getUniformLocation("diffuseMap"), 0);
	glUniform1i(shaderProgram->getUniformLocation("normalMap"), 1);
	glUniform1i(shaderProgram->getUniformLocation("heightMap"), 2);


	//Przypisz tekstury do jednostek teksturuj¹cych
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalTex);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, heightTex);

	//Uaktywnienie VAO i tym samym uaktywnienie predefiniowanych w tym VAO powi¹zañ slotów atrybutów z tablicami z danymi
	glBindVertexArray(vao);

	//Narysowanie obiektu
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 4);

	//Posprz¹tanie po sobie (niekonieczne w sumie je¿eli korzystamy z VAO dla ka¿dego rysowanego obiektu)
	glBindVertexArray(0);
}