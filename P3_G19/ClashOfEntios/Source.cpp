#include <iostream>
#include <fstream>
#include "Input.inl.hh"
#include "Renderer.hh"
#include <vector>
#include <stack>
#include <algorithm>
const int SizeJ = 74;
const int SizeI = 36;
enum class direction
{
	_Up, _Down, _Left, _Right
};
#include "Entios.h"

#define ARRIBA enti::InputKey::w //este y todos los demas estaban asi w,W y abajo en la funcion MovimientoPlayer me daba error
#define ARRIBAM enti::InputKey::W
#define ABAJO enti::InputKey::s 
#define ABAJOM enti::InputKey::S
#define DERECHA enti::InputKey::d
#define DERECHAM enti::InputKey::D
#define IZQUIERDA enti::InputKey::a
#define IZQUIERDAM enti::InputKey::A
#define ATACAR enti::InputKey::SPACEBAR
#define REHACER enti::InputKey::z
#define REHACERM enti::InputKey::Z
#define CAMBIAR enti::InputKey::ENTER
#define ATRAS enti::InputKey::ESC

enti::InputKey tecla;
enum class Arma { SWORD, BOW };

/*
void Muerte(std::vector <MonigotesJuego> Equipo1, std::vector <MonigotesJuego> Equipo2); //forward declaration
void Ataque(int armusa, int &iterador, char &mapa, bool &controlador, int &ContadorAcciones, std::vector <MonigotesJuego> Equipo1, std::vector <MonigotesJuego> Equipo2); //forward declaration
*/

/*
void ComandoPlayer(char &mapa) //funcion sobre lo que puede hacer el player y como segun esta el mapa variar? lo pasamos por referencia
{								//esto no funcionar? bien porque debe llamarse dentro del gameloop, y en cada iteraci?n reseteamos todas estas variables
bool ControlTurnos = false;
int ContadorAcciones = 10;
int iterador = 0;
int arma=NULL;
tecla = enti::getInputKey();
bool adelante = true; //booleano para controlar que no cambie el turno del jugador si no se aprieta enter

if (enti::getInputKey() == CAMBIAR_ENTIO && ControlTurnos == false && adelante==true) //si es falso el jugador que controle los monigotes que son letras (Equipo1) hara sus diez acciones,  si es true, el otro las har?
{
if (Equipo1.size() == iterador)
{
iterador = -1; //le meto que sea igual a -1 porque como luego lo primeor que hacemos es augmentarlo, valdra 0, y 0 es una posicion v?lida
}
iterador++; //lo augmentamos antes porque el primero ya estaba en true
Equipo1[iterador - 1].esControlado = false;
Equipo1[iterador].esControlado = true;
ContadorAcciones++;

}
if (tecla!= CAMBIAR_ENTIO && ControlTurnos == false)
{
switch (tecla)
{

case ARRIBA:
Equipo1[iterador].getX() -= 1;
ContadorAcciones -= 1;
break;

case ABAJO:
Equipo1[iterador].getX() += 1;
ContadorAcciones -= 1;
break;

case DERECHA:
Equipo1[iterador].getY() += 1;
ContadorAcciones -= 1;
break;

case IZQUIERDA:
Equipo1[iterador].getY() -= 1;
ContadorAcciones -= 1;
break;

case REHACER:

ContadorAcciones -= 1;
break;

case ATACAR:
std::cout << "Which weapon do you want to attack with? \n0:Sword \n1:Bow ";
std::cin >> arma;
Ataque(arma, iterador, mapa, ControlTurnos, ContadorAcciones, Equipo1, Equipo2);//se llama a la funcion ataque

case (ATRAS):
break;


}
}

if (ContadorAcciones == 0)
{

ContadorAcciones = 10;
std::cout << "Press enter to continue"<<std::endl;
ControlTurnos = true;
adelante = false;

if (tecla == CAMBIAR_ENTIO)
{
ContadorAcciones = 10;
adelante = true;
}
else
{
std::cout << "Wrong key";
}






}


}
*/

//Los setters son mejores que tener X e Y publicos porque as? nos aseguramos de que el PJ no pueda salir del mapa

MonigotesJuego::MonigotesJuego(GameManager &boss) : manager(boss) {
	vida = 10;
	flechas = 10;
	dead = false;
	esControlado = false;
	fatiga = 0;
}

//Con los setters nos aseguramos de que no se sale del mapa. Despu?s en el gameManager haremos que no puedas meterte en la posici?n de otro entio.
void MonigotesJuego::plusX() {
	if (getX() < SizeJ - 1)
		++CoordenadasX;
}

void MonigotesJuego::plusY() {
	if (CoordenadasY>0)
		--CoordenadasY;
}

void MonigotesJuego::minusX() {
	if (getX() > 0)
		--CoordenadasX;
}

void MonigotesJuego::minusY() {
	if (CoordenadasY < SizeI-1)
		++CoordenadasY;
}

void MonigotesJuego::setX(int a) {
	if (a >= 0 && a < SizeJ)
		CoordenadasX = a;
}

void MonigotesJuego::setY(int a) {
	if (a >= 0 && a < SizeI)
		CoordenadasY = a;
}

int MonigotesJuego::getX() {
	return CoordenadasX;
}

int MonigotesJuego::getY() {
	return CoordenadasY;
}

void GameManager::ComandoPJ(enti::InputKey pulsado) {

	if (!ActiveTeamIsDone()) { //si el equipo que est? jugando a?n tiene acciones
		switch (pulsado) {
		case CAMBIAR: CambiarEntio();
			break;
		case ARRIBAM:
		case ARRIBA: submitMove(direction::_Up);
			break;
		case ABAJOM:
		case ABAJO: submitMove(direction::_Down);
			break;
		case DERECHAM:
		case DERECHA: submitMove(direction::_Right);
			break;
		case IZQUIERDAM:
		case IZQUIERDA: submitMove(direction::_Left);
			break;
		case REHACER:
			break;
		case ATRAS:
			break;
		case ATACAR:
			break;
		}
	}
	else {

	}
}

void GameManager::CambiarEntio() //recorre el team activo. Setea el esControlado 
{															//de todos los mu?ecos a falso y busca al menos fatigado y lo pone en true
	int ansposition = 0;
	int minimalStress = ActiveTeam().at(0).fatiga;

	for (unsigned int i = 1; i < ActiveTeam().size(); ++i) {
		if (ActiveTeam().at(i).fatiga < minimalStress) {
			ansposition = i;
			minimalStress = ActiveTeam().at(i).fatiga;
			ActiveTeam().at(i).esControlado = false;
		}
	}

	ActiveTeam().at(ansposition).esControlado = true;;
}


Map::Map(std::vector<MonigotesJuego> &Team1, std::vector<MonigotesJuego> &Team2) //no se si el recurso se tiene que leer como archivo asi que dejo esto asi por si al final se hace asi leerlo caracter a caracter
{
	std::ifstream mapOverlay("default.cfg");
	char linia[SizeJ];

	for (int i = 0; mapOverlay.getline(linia, SizeJ); ++i) { //lee el archivo default.cfg y lo guarda bien ordenadito en el array mapa.
		for (int j = 0; j < SizeJ; ++j) {
			//si se encuentra con los players (1, 2, 3, 4, 5, a, b, c, d, e o f), escribe '.' en el mapa
			switch (linia[j]) {								//y les asigna la posici?n en la que nos los hemos encontrado.
			case 'A': Team1.at(0).setX(j); Team1.at(0).setY(i); Team1.at(0).SimboloMonigote = linia[j]; Team1.at(0).lastChar = '.';
				mapa[i][j] = Team1.at(0).SimboloMonigote;
				break;
			case 'B':Team1.at(1).setX(j);  Team1.at(1).setY(i); Team1.at(1).SimboloMonigote = linia[j]; Team1.at(1).lastChar = '.';
				mapa[i][j] = Team1.at(1).SimboloMonigote;
				break;
			case 'C':Team1.at(2).setX(j); Team1.at(2).setY(i); Team1.at(2).SimboloMonigote = linia[j]; Team1.at(2).lastChar = '.';
				mapa[i][j] = Team1.at(2).SimboloMonigote;
				break;
			case 'D':Team1.at(3).setX(j); Team1.at(3).setY(i); Team1.at(3).SimboloMonigote = linia[j]; Team1.at(3).lastChar = '.';
				mapa[i][j] = Team1.at(3).SimboloMonigote;
				break;
			case 'E':Team1.at(4).setX(j); Team1.at(4).setY(i); Team1.at(4).SimboloMonigote = linia[j]; Team1.at(4).lastChar = '.';
				mapa[i][j] = Team1.at(4).SimboloMonigote;
				break;
			case 'F':Team1.at(5).setX(j); Team1.at(5).setY(i); Team1.at(5).SimboloMonigote = linia[j]; Team1.at(5).lastChar = '.';
				mapa[i][j] = Team1.at(5).SimboloMonigote;
				break;
			case '1':Team2.at(0).setX(j); Team2.at(0).setY(i); Team2.at(0).SimboloMonigote = linia[j]; Team2.at(0).lastChar = '.';
				mapa[i][j] = Team2.at(0).SimboloMonigote;
				break;
			case '2':Team2.at(1).setX(j); Team2.at(1).setY(i); Team2.at(1).SimboloMonigote = linia[j]; Team2.at(1).lastChar = '.';
				mapa[i][j] = Team2.at(1).SimboloMonigote;
				break;
			case '3':Team2.at(2).setX(j); Team2.at(2).setY(i); Team2.at(2).SimboloMonigote = linia[j]; Team2.at(2).lastChar = '.';
				mapa[i][j] = Team2.at(2).SimboloMonigote;
				break;
			case '4':Team2.at(3).setX(j); Team2.at(3).setY(i); Team2.at(3).SimboloMonigote = linia[j]; Team2.at(3).lastChar = '.';
				mapa[i][j] = Team2.at(3).SimboloMonigote;
				break;
			case '5':Team2.at(4).setX(j); Team2.at(4).setY(i); Team2.at(4).SimboloMonigote = linia[j]; Team2.at(4).lastChar = '.';
				mapa[i][j] = Team2.at(4).SimboloMonigote;
				break;
			case '6':Team2.at(5).setX(j); Team2.at(5).setY(i); Team2.at(5).SimboloMonigote = linia[j]; Team2.at(5).lastChar = '.';
				mapa[i][j] = Team2.at(5).SimboloMonigote;
				break;
			default:
				mapa[i][j] = linia[j];
				break;
			}
		}
	}

}


std::vector<MonigotesJuego>& GameManager::ActiveTeam() {
	if (Team1active)
		return Equipo1;
	else
		return Equipo2;
}

std::vector<MonigotesJuego>& GameManager::UnactiveTeam() {
	if (Team1active)
		return Equipo2;
	else
		return Equipo1;
}

char GameManager::nowMoves() {
	for (int i = 0; i < ActiveTeam().size(); ++i) {
		if (ActiveTeam().at(i).esControlado)
			return ActiveTeam().at(i).SimboloMonigote;
	}
}

bool GameManager::ActiveTeamIsDone() {
	return actions <= 0;
}

void GameManager::Equipo1SetState(bool a) {
	Team2active = !a;
	Team1active = a;
}

void GameManager::Equipo2SetState(bool a) {
	Team2active = a;
	Team1active = !a;
}

void GameManager::submitMove(direction vector) {
	for (unsigned int i = 0; i < ActiveTeam().size(); ++i) {
		if (ActiveTeam().at(i).esControlado) {
			switch (vector) {
			case direction::_Down:
				if (layOut[ActiveTeam().at(i).getY() + 1][ActiveTeam().at(i).getX()] == '.' || layOut[ActiveTeam().at(i).getY() + 1][ActiveTeam().at(i).getX()] == ':') {
					layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX()] = ActiveTeam().at(i).lastChar;
					ActiveTeam().at(i).lastChar = layOut[ActiveTeam().at(i).getY() + 1][ActiveTeam().at(i).getX()];
					ActiveTeam().at(i).minusY();
				}
				break;
			case direction::_Left:
				if (layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX() - 1] == '.' || layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX() - 1] == ':') {
					layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX()] = ActiveTeam().at(i).lastChar;
					ActiveTeam().at(i).lastChar = layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX() - 1];
					ActiveTeam().at(i).minusX();
				}
				break;
			case direction::_Right:
				if (layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX() + 1] == '.' || layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX() + 1] == ':') {
					layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX()] = ActiveTeam().at(i).lastChar;
					ActiveTeam().at(i).lastChar = layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX() + 1];
					ActiveTeam().at(i).plusX();
				}
				break;
			case direction::_Up:
				if (layOut[ActiveTeam().at(i).getY() - 1][ActiveTeam().at(i).getX()] == '.' || layOut[ActiveTeam().at(i).getY() - 1][ActiveTeam().at(i).getX()] == ':') {
					layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX()] = ActiveTeam().at(i).lastChar;
					ActiveTeam().at(i).lastChar = layOut[ActiveTeam().at(i).getY() - 1][ActiveTeam().at(i).getX()];
					ActiveTeam().at(i).plusY();
				}
				break;
			}
			layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX()] = ActiveTeam().at(i).SimboloMonigote;
			break;
		}
	}
	system("cls");

	for (int i = 0; i < SizeI; ++i) {
		for (int j = 0; j < SizeJ; ++j) {
			switch (layOut[i][j]) {
			case 'X': enti::cout << enti::Color::LIGHTRED << layOut[i][j];
				break;
			case '.': enti::cout << enti::Color::WHITE << layOut[i][j];
				break;
			case':': enti::cout << enti::Color::LIGHTGREEN << layOut[i][j];
				break;
			case'O': enti::cout << enti::Color::LIGHTCYAN << layOut[i][j];
				break;
			default: enti::cout << enti::Color::YELLOW << layOut[i][j];
				break;
			}
			enti::cout << ' ';
		}
		enti::cout << enti::endl;
	}
	//enti::cout << enti::cend;
}

void GameManager::GameStatus() {
	enti::cout << enti::Color::YELLOW << "Remaining Movements: " << enti::Color::LIGHTCYAN << actions << enti::endl;
	enti::cout << enti::Color::YELLOW << "Now moves: " << enti::Color::LIGHTCYAN << nowMoves() << enti ::cend;
}

GameManager::GameManager() {
	for (int i = 0; i < 6; ++i) {
		Equipo1.push_back(MonigotesJuego(*this));
	}
	for (int i = 0; i < 6; ++i) {
		Equipo2.push_back(MonigotesJuego(*this));
	}
	Equipo1.at(0).esControlado = true;
	Map* terreno = new Map(Equipo1, Equipo2);
	layOut = terreno->mapa;
	Team1active = true;
	Team2active = false;
	actions = 10;
}
/*

void GameManager::Ataque()
{
if (ActiveTeam()==Equipo1)
{
for (int i = 0; i < Equipo1.size(); i++)
{
if (Equipo1.at(i).esControlado == true)
{
for (int casillas = Equipo1.at(i).getX(); casillas < Equipo1.at(i).getX() + 10; casillas++)
{
if (layOut[Equipo1.at(i).getX() + (casillas / casillas)][Equipo1.at(i).getY()]=='A' || layOut[Equipo1.at(i).getX() + (casillas / casillas)][Equipo1.at(i).getY()] == 'B' ||
layOut[Equipo1.at(i).getX() + (casillas / casillas)][Equipo1.at(i).getY()] == 'C' || layOut[Equipo1.at(i).getX() + (casillas / casillas)][Equipo1.at(i).getY()] == 'D'
|| layOut[Equipo1.at(i).getX() + (casillas / casillas)][Equipo1.at(i).getY()] == 'E' || layOut[Equipo1.at(i).getX() + (casillas / casillas)][Equipo1.at(i).getY()] == 'F') //casillas/casillas para ir aumentando de uno en uno
{
for (int x = 0; x < Equipo2.size(); x++)
{
if (Equipo2.at(x).getX()== Equipo1.at(i).getX() + (casillas / casillas))
Equipo2.at(x).vida-=
}
}
}
}
}
}

}

*/

void Play() {

	GameManager* boss = new GameManager();
	int x = 0;

	for (int i = 0; i < SizeI; ++i) {
		for (int j = 0; j < SizeJ; ++j) {

			std::cout << boss->layOut[i][j]; //Primer [] corresponde a las Y, el segundo [] a las X.
		}
		std::cout << std::endl;
	}

	do {
		enti::InputKey localChar = enti::getInputKey();
		if (localChar != enti::InputKey::NONE) {
			boss->ComandoPJ(localChar);
			boss->GameStatus();
		}
		else; //El enti::systemPause se come el inputKey que me interesa. Hacer esto es lo mismo pero sin que se coma el input.
	} while (true);
}


void main()
{
	Play();
}