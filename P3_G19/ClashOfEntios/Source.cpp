#include <iostream>
#include <fstream>
#include "Input.inl.hh"
#include "Renderer.hh"
#include <vector>
#include <stack>
#include <algorithm>
#include <queue>
#include <limits>
const int SizeJ = 74;
const int SizeI = 36;
enum class direction
{
	_Up, _Down, _Left, _Right
};

enum class ATQStatus { _NULL, _CHOOSE, _ACT, _REPORT, _MAX };

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


void GameManager::Ataque()
{
	switch (estado) {
	case ATQStatus::_NULL:
		break;
	case ATQStatus::_CHOOSE: 
		enti::cout << enti::Color::WHITE << "Enter the weapon you want to choose:" << enti::endl;
		enti::cout << enti::Color::YELLOW << "1 - SWORD" << enti::endl;
		enti::cout << enti::Color::YELLOW << "2 - BOW" << enti::endl;
		break;
	case ATQStatus::_ACT:
		enti::cout << enti::Color::WHITE << "Enter the direction to attack:" << enti::endl;
		enti::cout << enti::Color::YELLOW << "1 - UP" << enti::endl;
		enti::cout << enti::Color::YELLOW << "2 - LEFT" << enti::endl;
		enti::cout << enti::Color::YELLOW << "3 - DOWN" << enti::endl;
		enti::cout << enti::Color::YELLOW << "4 - RIGHT" << enti::endl;
		break;
	case ATQStatus::_REPORT:
		std::pair<int, int> localDir;//el primero son las Y y el segundo las X
		switch (ATQKey) {
		case enti::InputKey::NUM1: //up	
			localDir.first = -1;
			localDir.second = 0;
			break;
		case enti::InputKey::NUM2: //left
			localDir.first = 0;
			localDir.second = -1;
			break;
		case enti::InputKey::NUM3: //down
			localDir.first = 1;
			localDir.second = 0;
			break;
		case enti::InputKey::NUM4: //right
			localDir.first = 0;
			localDir.second = 1;
			break;
		}

		int localMinRange;
		int localMaxRange;
		bool localCheck = false;
		int localPos;
		int localDmg;
		actions--;
		if (WeaponSel == enti::InputKey::NUM1) {
			localMinRange = 1;
			localMaxRange = 2;
		}
		else {
			localMinRange = 3;
			localMaxRange = 11;
			nowMoves().flechas--;
		}


		for (int i = localMinRange; i < localMaxRange; ++i) {
			if (layOut[nowMoves().getY() + i*(localDir.first)][nowMoves().getX()+i*(localDir.second)] != 'X' && 
				layOut[nowMoves().getY() + i*(localDir.first)][nowMoves().getX() + i*(localDir.second)] != '.' &&
				layOut[nowMoves().getY() + i*(localDir.first)][nowMoves().getX() + i*(localDir.second)] != 'O' && 
				layOut[nowMoves().getY() + i*(localDir.first)][nowMoves().getX() + i*(localDir.second)] != ':') {
						for (int j = 0; j < ActiveTeam().size(); ++j) {
							if (UnactiveTeam().at(j).SimboloMonigote == layOut[nowMoves().getY() + i*(localDir.first)][nowMoves().getX() + i*(localDir.second)]) {
								localDmg = 11 - i;
								UnactiveTeam().at(j).harm(localDmg);
								localCheck = true;
								localPos = j;
								break;
							}
						}					
			}
		}
		if (localCheck) {
			enti::cout << enti::endl << enti::Color::WHITE << "You inflicted " << localDmg << " to entio " << localPos << enti::endl;
			if(UnactiveTeam().at(localPos).getHP()>0)
				enti::cout << enti::Color::LIGHTMAGENTA << "Entio " << localPos << " life " << UnactiveTeam().at(localPos).getHP() << enti::endl;
			else {
				enti::cout << enti::Color::LIGHTMAGENTA << "Entio killed! " << enti::endl;
				UnactiveTeam().at(localPos).goToSleep();
			}
		}
		else {
			enti::cout << enti::endl << enti::Color::WHITE << "You missed!" << enti::endl;
		}
		break;
	}

}

MonigotesJuego::MonigotesJuego(GameManager &boss) : manager(boss) {
	vida = 10;
	flechas = 10;
	esControlado = false;
	turnsPlayed=0;
	movesAct = 0;
	hasPlayed = false;
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

void MonigotesJuego::setHP(int a) {
	if (a > 0)
		vida = a;
}

void MonigotesJuego::harm(int a) {
	vida -= a;
}

void MonigotesJuego::kill() {
	vida = 0;
}

int MonigotesJuego::getHP() {
	return vida;
}


void MonigotesJuego::goToSleep() {
	SimboloMonigote = lastChar;
	manager.layOut[CoordenadasY][CoordenadasX] = lastChar;
	for (int i = 0; i < manager.UnactiveTeam().size(); ++i) {
		if (manager.UnactiveTeam().at(i).SimboloMonigote == SimboloMonigote)
			manager.UnactiveTeam().at(i).turnsPlayed = std::numeric_limits<int>::max()/2;
	}
	
}

void GameManager::ComandoPJ(enti::InputKey pulsado) {
	bool localCheck=false;
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
		case REHACER: Undo(); localCheck = true;
			break;
		case ATRAS:
			break;
		case ATACAR: if (actions>0) estado = ATQStatus::_CHOOSE;
			break;
		}
		if(!localCheck)
			historial.push(std::pair<std::vector<MonigotesJuego>, std::vector<MonigotesJuego>>{Equipo1, Equipo2});
}

MonigotesJuego& GameManager::setAndFindStress() {
	int ansposition = 0;
	int minimalStress = ActiveTeam().at(0).turnsPlayed + ActiveTeam().at(0).movesAct;

	for (unsigned int i = 0; i < ActiveTeam().size(); ++i) {
		if ((ActiveTeam().at(i).turnsPlayed + ActiveTeam().at(i).movesAct) < minimalStress) {
			ansposition = i;
			minimalStress = (ActiveTeam().at(i).turnsPlayed + ActiveTeam().at(i).movesAct);
		}
		if (ActiveTeam().at(i).esControlado)
			ActiveTeam().at(i).esControlado = false;
	}

	ActiveTeam().at(ansposition).esControlado = true;
	return ActiveTeam().at(ansposition);
}

void GameManager::CambiarEntio() {

	if (actions > 0) {//si al equipo activo le quedan acciones, busca al entio menos fatigado y le da el control, a la vez que se lo quita al entio activo.
		//es posible que el entio al que se le da el control sea el entio que estaba activo en un principio.
		setAndFindStress();
		actions--;
	}
	else {
		for (unsigned int i = 0; i < ActiveTeam().size(); ++i) {
			ActiveTeam().at(i).movesAct = 0;
			if (ActiveTeam().at(i).hasPlayed) {
				ActiveTeam().at(i).turnsPlayed++;
				ActiveTeam().at(i).hasPlayed = false;
			}

		}
			Equipo1SetState(Team2active);
			setAndFindStress();
			actions = 10;
			while (historial.size() > 0)
				historial.pop();
	}
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

MonigotesJuego& GameManager::nowMoves() {
	for (unsigned int i = 0; i < ActiveTeam().size(); ++i) {
		if (ActiveTeam().at(i).esControlado)
			return ActiveTeam().at(i);
	}
}

bool GameManager::ActiveTeamIsDone() {
	return actions < 0;
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
	if (actions!=0) {
		for (unsigned int i = 0; i < ActiveTeam().size(); ++i) {
			if (ActiveTeam().at(i).esControlado) {
				switch (vector) {
				case direction::_Down:
					if (layOut[ActiveTeam().at(i).getY() + 1][ActiveTeam().at(i).getX()] == '.' || layOut[ActiveTeam().at(i).getY() + 1][ActiveTeam().at(i).getX()] == ':') {
						layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX()] = ActiveTeam().at(i).lastChar;
						ActiveTeam().at(i).lastChar = layOut[ActiveTeam().at(i).getY() + 1][ActiveTeam().at(i).getX()];
						ActiveTeam().at(i).minusY();
						actions--;
					}
					break;
				case direction::_Left:
					if (layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX() - 1] == '.' || layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX() - 1] == ':') {
						layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX()] = ActiveTeam().at(i).lastChar;
						ActiveTeam().at(i).lastChar = layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX() - 1];
						ActiveTeam().at(i).minusX();
						actions--;
					}
					break;
				case direction::_Right:
					if (layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX() + 1] == '.' || layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX() + 1] == ':') {
						layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX()] = ActiveTeam().at(i).lastChar;
						ActiveTeam().at(i).lastChar = layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX() + 1];
						ActiveTeam().at(i).plusX();
						actions--;
					}
					break;
				case direction::_Up:
					if (layOut[ActiveTeam().at(i).getY() - 1][ActiveTeam().at(i).getX()] == '.' || layOut[ActiveTeam().at(i).getY() - 1][ActiveTeam().at(i).getX()] == ':') {
						layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX()] = ActiveTeam().at(i).lastChar;
						ActiveTeam().at(i).lastChar = layOut[ActiveTeam().at(i).getY() - 1][ActiveTeam().at(i).getX()];
						ActiveTeam().at(i).plusY();
						actions--;
					}
					break;
				}				
				layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX()] = ActiveTeam().at(i).SimboloMonigote;
				ActiveTeam().at(i).movesAct++;
				ActiveTeam().at(i).hasPlayed = true;
				break;
			}
		}		
	}
}

void GameManager::GameStatus() {
	if (!ActiveTeamIsDone()) {
		system("cls");
		char localActive = nowMoves().SimboloMonigote;
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
				default: 
					if (layOut[i][j] == nowMoves().SimboloMonigote)
						enti::cout << enti::Color::LIGHTMAGENTA << layOut[i][j];
					else
						enti::cout << enti::Color::YELLOW << layOut[i][j];
					break;
				}
				enti::cout << ' ';
			}
			enti::cout << enti::endl;
		}

		enti::cout << enti::Color::YELLOW << "Remaining Movements: " << enti::Color::LIGHTCYAN << actions << enti::endl;
		enti::cout << enti::Color::YELLOW << "Now moves: " << enti::Color::LIGHTCYAN << nowMoves().SimboloMonigote << enti::endl;
		if (actions==0)
			enti::cout << enti::endl << enti::Color::LIGHTMAGENTA << "Press ENTER to end your turn!" << enti::endl;
	}
}

void GameManager::Undo() {
	if (historial.size()>0) {
		for (int i = 0; i < ActiveTeam().size(); ++i) {
			layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX()] = ActiveTeam().at(i).lastChar;
		}
		Equipo1 = historial.top().first;
		Equipo2 = historial.top().second;
		if (actions != 0 && actions!=10)
			actions++;
		historial.pop();
	}
	for (int i = 0; i < ActiveTeam().size(); ++i) {
		layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX()] = ActiveTeam().at(i).SimboloMonigote;
	}
}

GameManager::GameManager(){
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
	estado = ATQStatus::_NULL;
}

void Play() {
	bool byPass = false;
	bool GAMEOVER = true;
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
		if (localChar != enti::InputKey::NONE || byPass) {
			GAMEOVER = true;
			byPass = false;
			for (int i = 0; i < boss->UnactiveTeam().size(); ++i) {
				if (boss->UnactiveTeam().at(i).getHP() > 0)
					GAMEOVER = false;
			}
			boss->ComandoPJ(localChar);
			boss->GameStatus();
			boss->Ataque();
			
			enti::cout << enti::cend;
			if (GAMEOVER)
				break;
			if(boss->estado != ATQStatus::_NULL) {
				do {
					enti::InputKey localChar2 = enti::getInputKey();
					if ((boss->estado == ATQStatus::_CHOOSE) && (localChar2 == enti::InputKey::NUM1 || localChar2 == enti::InputKey::NUM2)) {
						boss->WeaponSel = localChar2;
						boss->estado = ATQStatus::_ACT;
						byPass = true;
						break;
					}
					else if ((boss->estado == ATQStatus::_ACT) && (localChar2 == enti::InputKey::NUM1 || localChar2 == enti::InputKey::NUM2 ||
						localChar2 == enti::InputKey::NUM3 || localChar2 == enti::InputKey::NUM4)) {
						boss->estado = ATQStatus::_REPORT;
						boss->ATQKey = localChar2;
						byPass = true;
						break;
					}
					else if (localChar2 == enti::InputKey::NONE);
					else {
						boss->estado = ATQStatus::_NULL;
						break;
					}
				} while (true);
			}
		}
		else; //El enti::systemPause se come el inputKey que me interesa. Hacer esto es lo mismo pero sin que se coma el input.
	} while (true);
	if (GAMEOVER) {
		boss->GameStatus();
		int localWinner;
		if (boss->Team1active) {
			localWinner = 1;
		}
		else
			localWinner = 2;
		enti::cout << enti::Color::LIGHTGRAY << "CONGRATULATIONS! Team " << localWinner << " has won." << enti::endl;
		enti::cout << "Play again? (Y/N) " << enti::cend;
		delete boss;
		boss = nullptr;
	}
}


void main()
{
	replay:
	Play();
	do {
		enti::InputKey a = enti::getInputKey();
		if (a == enti::InputKey::Y || a == enti::InputKey::y) {
			system("cls");
			goto replay;
		}
		else if (a == enti::InputKey::N || a == enti::InputKey::n) {

			break;
		}
		else;
	} while (true);
}