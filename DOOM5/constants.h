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

#ifndef CONSTANTS_H
#define CONSTANTS_H

const float PI = 3.141592653589793f;
const float playerMovementSpeed = 10;
const float playerRotationSpeed = 10;
const float playerJumpSpeed = 20;
const float gravitationalConstant = 9.81*3;
const float czasPrzyspieszania = 0.1;
const int windowDimension = 1000;
const int pixelsPerRotation = 100;
const int stalaDoDzieleniaObrotuZPada = 100;
const float maksymalnyKatWychyleniaGoraDol = PI-0.5;
const float wysokoscGracza = 3;
const float szerokoscGracza = 2;

#endif
