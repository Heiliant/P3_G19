#include <iostream>
#include <string>
#include <conio.h>
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
#define ABAJO enti::InputKey::s // nos faltará meter las letras en mayus. El define no creo que le guste mucho a la tona, ya que es de c xD.
#define DERECHA enti::InputKey::d
#define IZQUIERDA enti::InputKey::a
#define ATACAR enti::InputKey::SPACEBAR
#define REHACER enti::InputKey::z
#define REREHACER enti::InputKey::x
#define CAMBIAR enti::InputKey::ENTER
#define ATRAS enti::InputKey::ESC

//const int DosArmas = 2;


enti::InputKey tecla;
enum class Arma{SWORD,BOW};

/*
void Muerte(std::vector <MonigotesJuego> Equipo1, std::vector <MonigotesJuego> Equipo2); //forward declaration
void Ataque(int armusa, int &iterador, char &mapa, bool &controlador, int &ContadorAcciones, std::vector <MonigotesJuego> Equipo1, std::vector <MonigotesJuego> Equipo2); //forward declaration
*/

/*
	void ComandoPlayer(char &mapa) //funcion sobre lo que puede hacer el player y como segun esta el mapa variará lo pasamos por referencia
	{								//esto no funcionará bien porque debe llamarse dentro del gameloop, y en cada iteración reseteamos todas estas variables
		bool ControlTurnos = false;
		int ContadorAcciones = 10;
		int iterador = 0;
		int arma=NULL;
		tecla = enti::getInputKey();
		bool adelante = true; //booleano para controlar que no cambie el turno del jugador si no se aprieta enter

		if (enti::getInputKey() == CAMBIAR_ENTIO && ControlTurnos == false && adelante==true) //si es falso el jugador que controle los monigotes que son letras (Equipo1) hara sus diez acciones,  si es true, el otro las hará
		{
			if (Equipo1.size() == iterador)
			{
				iterador = -1; //le meto que sea igual a -1 porque como luego lo primeor que hacemos es augmentarlo, valdra 0, y 0 es una posicion válida
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
				Equipo1[iterador].CoordenadasX -= 1;
				ContadorAcciones -= 1;
				break;

			case ABAJO:
				Equipo1[iterador].CoordenadasX += 1;
				ContadorAcciones -= 1;
				break;

			case DERECHA:
				Equipo1[iterador].CoordenadasY += 1;
				ContadorAcciones -= 1;
				break;

			case IZQUIERDA:
				Equipo1[iterador].CoordenadasY -= 1;
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

//Los setters son mejores que tener X e Y publicos porque así nos aseguramos de que el PJ no pueda salir del mapa

MonigotesJuego::MonigotesJuego(GameManager &boss) : manager(boss) {
	vida = 10;
	flechas = 10;
	dead = false;
	esControlado = false;
	fatiga = 0;
}

//Con los setters nos aseguramos de que no se sale del mapa. Después en el gameManager haremos que no puedas meterte en la posición de otro entio.
void MonigotesJuego::plusX() {
	if (CoordenadasX < SizeJ - 1)
		++CoordenadasX;
}

void MonigotesJuego::plusY() {
	if (CoordenadasY < SizeI - 1)
		++CoordenadasY;
}

void MonigotesJuego::minusX() {
	if (CoordenadasX > 0)
		this->CoordenadasX--;
}

void MonigotesJuego::minusY() {
	if (CoordenadasY > 0)
		--CoordenadasY;
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

		if (!ActiveTeamIsDone()) { //si el equipo que está jugando aún tiene acciones
			switch (pulsado) {
			case CAMBIAR: CambiarEntio();
				break;
			case ARRIBA: submitMove(direction::_Up);
				break;
			case ABAJO: submitMove(direction::_Down);
				break;
			case DERECHA: submitMove(direction::_Right);
				break;
			case IZQUIERDA: submitMove(direction::_Left);
				break;
			case REHACER:
				break;
			case REREHACER:
				break;
			case ATRAS:
				break;
			case ATACAR:
				break;
			}
		}
		else{

		}
	}

void GameManager::CambiarEntio() //recorre el team activo. Setea el esControlado 
	{															//de todos los muñecos a falso y busca al menos fatigado y lo pone en true
		int ansposition=0;
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
				switch (linia[j]) {								//y les asigna la posición en la que nos los hemos encontrado.
				case 'A': Team1.at(0).setX(j); Team1.at(0).setY(i); Team1.at(0).SimboloMonigote = linia[j];
					mapa[i][j] = '.';
					break;
				case 'B':Team1.at(1).setX(j);  Team1.at(1).setY(i); Team1.at(1).SimboloMonigote = linia[j];
					mapa[i][j] = '.';
					break;
				case 'C':Team1.at(2).setX(j); Team1.at(2).setY(i); Team1.at(2).SimboloMonigote = linia[j];
					mapa[i][j] = '.';
					break;
				case 'D':Team1.at(3).setX(j); Team1.at(3).setY(i); Team1.at(3).SimboloMonigote = linia[j];
					mapa[i][j] = '.';
					break;
				case 'E':Team1.at(4).setX(j); Team1.at(4).setY(i); Team1.at(4).SimboloMonigote = linia[j];
					mapa[i][j] = '.';
					break;
				case 'F':Team1.at(5).setX(j); Team1.at(5).setY(i); Team1.at(5).SimboloMonigote = linia[j];
					mapa[i][j] = '.';
					break;
				case '1':Team2.at(0).setX(j); Team2.at(0).setY(i); Team2.at(0).SimboloMonigote = linia[j];
					mapa[i][j] = '.';
					break;
				case '2':Team2.at(1).setX(j); Team2.at(1).setY(i); Team2.at(1).SimboloMonigote = linia[j];
					mapa[i][j] = '.';
					break;
				case '3':Team2.at(2).setX(j); Team2.at(2).setY(i); Team2.at(2).SimboloMonigote = linia[j];
					mapa[i][j] = '.';
					break;
				case '4':Team2.at(3).setX(j); Team2.at(3).setY(i); Team2.at(3).SimboloMonigote = linia[j];
					mapa[i][j] = '.';
					break;
				case '5':Team2.at(4).setX(j); Team2.at(4).setY(i); Team2.at(4).SimboloMonigote = linia[j];
					mapa[i][j] = '.';
					break;
				case '6':Team2.at(5).setX(j); Team2.at(5).setY(i); Team2.at(5).SimboloMonigote = linia[j];
					mapa[i][j] = '.';
					break;
				default:
					mapa[i][j] = linia[j];
					break;
				}
			}
		}

	}


std::vector<MonigotesJuego> GameManager::ActiveTeam() {
		if (Team1active) 
			return Equipo1;
		else 
			return Equipo2;
	}

bool GameManager::ActiveTeamIsDone() {
		return actions <= 0;
	}

void GameManager::Equipo1SetState(bool a) {

	}

void GameManager::Equipo2SetState(bool a) {

	}

void GameManager::submitMove(direction vector) {
		for (unsigned int i = 0; i < ActiveTeam().size(); ++i) {
			if (ActiveTeam().at(i).esControlado) {
				switch (vector) {
				case direction::_Down: ActiveTeam().at(i).minusY();
					break;
				case direction::_Left: ActiveTeam().at(i).minusX();
					break;
				case direction::_Right: ActiveTeam().at(i).plusX();
					break;
				case direction::_Up: ActiveTeam().at(i).plusY();
					break;
				}
			}
			system("cls");
			//SE EJECUTA UNA VEZ POR JUGADOR, ARREGLAR
			for (int i = 0; i < SizeI; ++i) {
				for (int j = 0; j < SizeJ; ++j) {

					std::cout << layOut[i][j]; //Primer [] corresponde a las Y, el segundo [] a las X.
				}
				std::cout << std::endl;
			}
			//limpiar pantalla y volver a pintar el mapa
		}
	}

GameManager::GameManager() {
	for (int i = 0; i < 6; ++i) {
		Equipo1.push_back(MonigotesJuego(*this)); //Que diferencia hay?. Se supone que el bueno es el de equipo1
	}
	for (int i = 0; i < 6; ++i) {
		Equipo2.push_back(*this);
	}
	Equipo1.at(0).esControlado = true;
	Map* mapa = new Map(Equipo1, Equipo2);
	layOut = mapa->mapa;
	for (int i = 0; i < Equipo1.size(); ++i) {
		layOut[Equipo1.at(i).getY()][Equipo1.at(i).getX()] = Equipo1.at(i).SimboloMonigote;
	}
	for (int i = 0; i < Equipo2.size(); ++i) {
		layOut[Equipo2.at(i).getY()][Equipo2.at(i).getX()] = Equipo2.at(i).SimboloMonigote;
	}
	Team1active = true;
	Team2active = false;
	actions = 10;
	}

/*
void Ataque(int armusa, int &iterador, char &mapa, bool &controlador, int &ContadorAcciones, std::vector <MonigotesJuego> Equipo1, std::vector <MonigotesJuego> Equipo2)
{
	bool CasillaOcupada = false; //se pondra true si hay un enemigo del actual equipo a rango
	int i = 0;

	if (armusa = 1)
	{
		if (controlador = false) //significa que esta jugando el Equipo1 (el de letras)
		{
			for (int jota = Equipo1[iterador].CoordenadasX; jota < Equipo1[iterador].CoordenadasX + 10; jota++) //recorro diez posiciones desde su x hasta su x+10
			{
				if (mapa[i][Equipo1[iterador].CoordenadasX + (jota / jota)] != 'A' && mapa[i][Equipo1[iterador].CoordenadasX  +(jota / jota)] != 'B' && mapa[i][Equipo1[iterador].CoordenadasX +(jota / jota)] != 'C' //comprobaciones de si hay un enemigo
					&& mapa[i][Equipo1[iterador].CoordenadasX + (jota / jota)] != 'D' && mapa[i][Equipo1[iterador].CoordenadasX +(jota / jota)] != 'E' && mapa[i][Equipo1[iterador].CoordenadasX  +(jota / jota)] != 'F' &&
					mapa[i][Equipo1[iterador].CoordenadasX + (jota / jota)] != 'X' && mapa[i][Equipo1[iterador].CoordenadasX + (jota / jota)] != 'O' && mapa[i][Equipo1[iterador].CoordenadasX  +(jota / jota)] != ':' &&
					mapa[i][Equipo1[iterador].CoordenadasX + (jota / jota)] != '.')
				{
					for (auto &x : Equipo2) //recorro Equipo2 comparando los simbolos de su contenido para saber a quien le resto vida
					{
						if (x.SimboloMonigote == mapa[i][Equipo1[iterador].CoordenadasX + (jota / jota)])
							x.vida -= ;
						Equipo1[iterador].Flechas--;
					}
				}

			}

			for (int jota = Equipo1[iterador].CoordenadasX; jota < Equipo1[iterador].CoordenadasX - 10; jota++)//recorro diez posiciones desde su x hasta su x-10
			{
				if (mapa[i][Equipo1[iterador].CoordenadasX - (jota / jota)] != 'A' && mapa[i][Equipo1[iterador].CoordenadasX - (jota / jota)] != 'B' && mapa[i][Equipo1[iterador].CoordenadasX - (jota / jota)] != 'C' //comprobaciones de si hay un enemigo
					&& mapa[i][Equipo1[iterador].CoordenadasX + (jota / jota)] != 'D' && mapa[i][Equipo1[iterador].CoordenadasX - (jota / jota)] != 'E' && mapa[i][Equipo1[iterador].CoordenadasX - (jota / jota)] != 'F' &&
					mapa[i][Equipo1[iterador].CoordenadasX - (jota / jota)] != 'X' && mapa[i][Equipo1[iterador].CoordenadasX - (jota / jota)] != 'O' && mapa[i][Equipo1[iterador].CoordenadasX - (jota / jota)] != ':' &&
					mapa[i][Equipo1[iterador].CoordenadasX - (jota / jota)] != '.')
				{
					for (auto &x : Equipo2)
					{
						if (x.SimboloMonigote == mapa[i][Equipo1[iterador].CoordenadasX - (jota / jota)])
							x.vida -= rand() % 4; ContadorAcciones -= 1;
						Equipo1[iterador].Flechas--;
					}
				}

			}

			for (int jota = Equipo1[iterador].CoordenadasY; jota > Equipo1[iterador].CoordenadasY + 10; jota++) //lo mismo para las y
			{
				if (mapa[i][Equipo1[iterador].CoordenadasY + (jota / jota)] != 'A' && mapa[i][Equipo1[iterador].CoordenadasY + (jota / jota)] != 'B' && mapa[i][Equipo1[iterador].CoordenadasY + (jota / jota)] != 'C' //comprobaciones de si hay un enemigo
					&& mapa[i][Equipo1[iterador].CoordenadasY + (jota / jota)] != 'D' && mapa[i][Equipo1[iterador].CoordenadasY + (jota / jota)] != 'E' && mapa[i][Equipo1[iterador].CoordenadasY + (jota / jota)] != 'F' &&
					mapa[i][Equipo1[iterador].CoordenadasY + (jota / jota)] != 'X' && mapa[i][Equipo1[iterador].CoordenadasY + (jota / jota)] != 'O' && mapa[i][Equipo1[iterador].CoordenadasY + (jota / jota)] != ':' &&
					mapa[i][Equipo1[iterador].CoordenadasY + (jota / jota)] != '.')
				{
					for (auto &x : Equipo2)
					{
						if (x.SimboloMonigote == mapa[i][Equipo1[iterador].CoordenadasY + (jota / jota)])
							x.vida -= rand() % 4; ContadorAcciones -= 1;
						Equipo1[iterador].Flechas--;
					}
				}

			}

			for (int jota = Equipo1[iterador].CoordenadasY; jota < Equipo1[iterador].CoordenadasX - 10; jota++)
			{
				if (mapa[i][Equipo1[iterador].CoordenadasY + (jota / jota)] != 'A' && mapa[i][Equipo1[iterador].CoordenadasY + (jota / jota)] != 'B' && mapa[i][Equipo1[iterador].CoordenadasY + (jota / jota)] != 'C' //comprobaciones de si hay un enemigo
					&& mapa[i][Equipo1[iterador].CoordenadasY + (jota / jota)] != 'D' && mapa[i][Equipo1[iterador].CoordenadasY + (jota / jota)] != 'E' && mapa[i][Equipo1[iterador].CoordenadasY + (jota / jota)] != 'F' &&
					mapa[i][Equipo1[iterador].CoordenadasY + (jota / jota)] != 'X' && mapa[i][Equipo1[iterador].CoordenadasY + (jota / jota)] != 'O' && mapa[i][Equipo1[iterador].CoordenadasY + (jota / jota)] != ':' &&
					mapa[i][Equipo1[iterador].CoordenadasY + (jota / jota)] != '.')
				{
					for (auto &x : Equipo2)
					{
						if (x.SimboloMonigote == mapa[i][Equipo1[iterador].CoordenadasX + (jota / jota)])
							x.vida -= rand() % 4; ContadorAcciones -= 1;
						Equipo1[iterador].Flechas--;
					}
				}

			}
		}
		else //lo mismo de antes pero con el boolean al reves, por lo tanto cambiando de equipo
		{
			for (int jota = Equipo2[iterador].CoordenadasX; jota < Equipo2[iterador].CoordenadasX + 10; jota++)
			{
				if (mapa[i][Equipo2[iterador].CoordenadasX + (jota / jota)] != 'A' && mapa[i][Equipo2[iterador].CoordenadasX + (jota / jota)] != 'B' && mapa[i][Equipo2[iterador].CoordenadasX + (jota / jota)] != 'C' //comprobaciones de si hay un enemigo
					&& mapa[i][Equipo2[iterador].CoordenadasX + (jota / jota)] != 'D' && mapa[i][Equipo2[iterador].CoordenadasX + (jota / jota)] != 'E' && mapa[i][Equipo2[iterador].CoordenadasX + (jota / jota)] != 'F' &&
					mapa[i][Equipo2[iterador].CoordenadasX + (jota / jota)] != 'X' && mapa[i][Equipo2[iterador].CoordenadasX + (jota / jota)] != 'O' && mapa[i][Equipo2[iterador].CoordenadasX + (jota / jota)] != ':' &&
					mapa[i][Equipo2[iterador].CoordenadasX + (jota / jota)] != '.')
				{
					for (auto &x : Equipo1) //recorro Equipo2 comparando los simbolos de su contenido para saber a quien le resto vida
					{
						if (x.SimboloMonigote == mapa[i][Equipo2[iterador].CoordenadasX + (jota / jota)])
							x.vida -= rand() % 4; ContadorAcciones -= 1;
						Equipo2[iterador].Flechas--;
					}
				}

			}

			for (int jota = Equipo2[iterador].CoordenadasX; jota < Equipo2[iterador].CoordenadasX - 10; jota++)
			{
				if (mapa[i][Equipo2[iterador].CoordenadasX - (jota / jota)] != 'A' && mapa[i][Equipo2[iterador].CoordenadasX - (jota / jota)] != 'B' && mapa[i][Equipo2[iterador].CoordenadasX - (jota / jota)] != 'C' //comprobaciones de si hay un enemigo
					&& mapa[i][Equipo2[iterador].CoordenadasX + (jota / jota)] != 'D' && mapa[i][Equipo2[iterador].CoordenadasX - (jota / jota)] != 'E' && mapa[i][Equipo2[iterador].CoordenadasX - (jota / jota)] != 'F' &&
					mapa[i][Equipo2[iterador].CoordenadasX - (jota / jota)] != 'X' && mapa[i][Equipo2[iterador].CoordenadasX - (jota / jota)] != 'O' && mapa[i][Equipo2[iterador].CoordenadasX - (jota / jota)] != ':' &&
					mapa[i][Equipo2[iterador].CoordenadasX - (jota / jota)] != '.')
				{
					for (auto &x : Equipo1)
					{
						if (x.SimboloMonigote == mapa[i][Equipo2[iterador].CoordenadasX - (jota / jota)])
							x.vida -= rand() % 4; ContadorAcciones -= 1;
						Equipo2[iterador].Flechas--;
					}
				}

			}

			for (int jota = Equipo2[iterador].CoordenadasY; jota > Equipo2[iterador].CoordenadasY + 10; jota++)
			{
				if (mapa[i][Equipo2[iterador].CoordenadasY + (jota / jota)] != 'A' && mapa[i][Equipo2[iterador].CoordenadasY + (jota / jota)] != 'B' && mapa[i][Equipo2[iterador].CoordenadasY + (jota / jota)] != 'C' //comprobaciones de si hay un enemigo
					&& mapa[i][Equipo2[iterador].CoordenadasY + (jota / jota)] != 'D' && mapa[i][Equipo2[iterador].CoordenadasY + (jota / jota)] != 'E' && mapa[i][Equipo2[iterador].CoordenadasY + (jota / jota)] != 'F' &&
					mapa[i][Equipo2[iterador].CoordenadasY + (jota / jota)] != 'X' && mapa[i][Equipo2[iterador].CoordenadasY + (jota / jota)] != 'O' && mapa[i][Equipo2[iterador].CoordenadasY + (jota / jota)] != ':' &&
					mapa[i][Equipo2[iterador].CoordenadasY + (jota / jota)] != '.')
				{
					for (auto &x : Equipo1)
					{
						if (x.SimboloMonigote == mapa[i][Equipo2[iterador].CoordenadasY + (jota / jota)])
							x.vida -= rand() % 4; ContadorAcciones -= 1;
						Equipo2[iterador].Flechas--;
					}
				}

			}

			for (int jota = Equipo2[iterador].CoordenadasY; jota < Equipo2[iterador].CoordenadasX - 10; jota++)
			{
				if (mapa[i][Equipo2[iterador].CoordenadasY + (jota / jota)] != 'A' && mapa[i][Equipo2[iterador].CoordenadasY + (jota / jota)] != 'B' && mapa[i][Equipo2[iterador].CoordenadasY + (jota / jota)] != 'C' //comprobaciones de si hay un enemigo
					&& mapa[i][Equipo2[iterador].CoordenadasY + (jota / jota)] != 'D' && mapa[i][Equipo2[iterador].CoordenadasY + (jota / jota)] != 'E' && mapa[i][Equipo2[iterador].CoordenadasY + (jota / jota)] != 'F' &&
					mapa[i][Equipo2[iterador].CoordenadasY + (jota / jota)] != 'X' && mapa[i][Equipo2[iterador].CoordenadasY + (jota / jota)] != 'O' && mapa[i][Equipo2[iterador].CoordenadasY + (jota / jota)] != ':' &&
					mapa[i][Equipo2[iterador].CoordenadasY + (jota / jota)] != '.')
				{
					for (auto &x : Equipo1)
					{
						if (x.SimboloMonigote == mapa[i][Equipo2[iterador].CoordenadasX + (jota / jota)])
							x.vida -= rand() % 4; ContadorAcciones -= 1;
						Equipo2[iterador].Flechas--;
					}
				}

			}
		}
	}

		else if (armusa == 0)
		{

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
		}
		else; //El enti::systemPause se come el inputKey que me interesa. Hacer esto es lo mismo pero sin que se coma el input.
	} while (true);
}


void main()
{
	Play();
}