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
//este enum class lo usamos para pasar un parámetro a la función de movimiento y facilitar la lectura del código

enum class ATQStatus { _NULL, _CHOOSE, _ACT, _REPORT, _MAX };
//y este otro para gestionar el estado en que se encuentra el ataque de un entio

#include "Entios.h"

#define ARRIBA enti::InputKey::w  //defines para simplificar el código
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
#define SALIR enti::InputKey::ESC

//Métodos de MonigotesJuego

MonigotesJuego::MonigotesJuego(GameManager &boss) : manager(boss) {
	vida = 10;
	flechas = 10;
	esControlado = false;
	turnsPlayed=0;
	movesAct = 0;
	hasPlayed = false;
}

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

void MonigotesJuego::goToSleep() { //este método lo hemos usado para "deshabilitar" a los entios caídos en combate. No hemos hecho delete de los entios
									//ya que la implementación del destructor nos causaba ciertos problemas.
	SimboloMonigote = lastChar;
	manager.layOut[CoordenadasY][CoordenadasX] = lastChar;
	for (int i = 0; i < manager.UnactiveTeam().size(); ++i) { //hacemos que el etio en cuestión sea parte del escenario para que así el jugador no lo vea
		if (manager.UnactiveTeam().at(i).SimboloMonigote == SimboloMonigote) //y los otros entios lo puedan atravesar.
			manager.UnactiveTeam().at(i).turnsPlayed = std::numeric_limits<int>::max()/2; //y seteamos el factor que prevalece entre turnos, de los dos
	}//que determinan la fatiga a la mitad del valor máximo de un int (Si lo poniamos al máximo la variable hacia overflow y adquiria un valor negativo considerable
	
}

void MonigotesJuego::operator =(MonigotesJuego &a) {
	CoordenadasX = a.CoordenadasX;
	CoordenadasY = a.CoordenadasY;
	vida = a.vida;
	flechas = a.flechas;
	SimboloMonigote = a.SimboloMonigote;
	esControlado = a.esControlado;
	movesAct = a.movesAct;
	turnsPlayed = a.turnsPlayed;
	hasPlayed = a.hasPlayed;
	lastChar = a.lastChar;
	manager = a.manager;
}

//Constructor de map

Map::Map(std::vector<MonigotesJuego> &Team1, std::vector<MonigotesJuego> &Team2)
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

//Métodos de GameManager

void GameManager::ComandoPJ(enti::InputKey pulsado) {
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
		case REHACER: if(actions!=0) Undo(); 
			break;
		case SALIR: exit = true;
			break;
		case ATACAR: if (actions>0) estado = ATQStatus::_CHOOSE;
			break;
		}
}

void GameManager::setAndFindStress() { //esta función busca al entio menos fatigado y, a la vez que los pone a todos en estado inactivo, pone a 
	int ansposition = 0;							// a este ene stado activo.
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
}

void GameManager::CambiarEntio() {

	if (actions > 0) {//si al equipo activo le quedan acciones, busca al entio menos fatigado y le da el control, a la vez que se lo quita al entio activo.
		//es posible que el entio al que se le da el control sea el entio que estaba activo en un principio.
		historial.push(std::pair<std::vector<MonigotesJuego>, std::vector<MonigotesJuego>>{Equipo1, Equipo2});
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
		historial.push(std::pair<std::vector<MonigotesJuego>, std::vector<MonigotesJuego>>{Equipo1, Equipo2});
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
		if (actions!=10)
			actions++;
		historial.pop();
	}
	for (int i = 0; i < ActiveTeam().size(); ++i) {
		layOut[ActiveTeam().at(i).getY()][ActiveTeam().at(i).getX()] = ActiveTeam().at(i).SimboloMonigote;
	}
}

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
		historial.push(std::pair<std::vector<MonigotesJuego>, std::vector<MonigotesJuego>>{Equipo1, Equipo2});
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
			if (layOut[nowMoves().getY() + i*(localDir.first)][nowMoves().getX() + i*(localDir.second)] != 'X' &&
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
			else if (layOut[nowMoves().getY() + i*(localDir.first)][nowMoves().getX() + i*(localDir.second)] == 'X')
				break;
		}
		if (localCheck) {
			enti::cout << enti::endl << enti::Color::WHITE << "You inflicted " << localDmg << " to entio " << localPos << enti::endl;
			if (UnactiveTeam().at(localPos).getHP()>0)
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
	exit = false;
}

bool Play() { //recopilamos toda la aplicación en la función Play.
	bool byPass = false; //el booleano byPass lo usamos para dar un "falso positivo" en de Input de teclado cuando nos es conveniente
	bool GAMEOVER = true;
	GameManager* boss = new GameManager();
	int x = 0;

	enti::cout << enti::Color::YELLOW << "CLASH OF ENTIOS" << enti::endl;
	enti::cout << enti::endl << enti::Color::WHITE << "-Each player has 6 Entios" << enti::endl;
	enti::cout << "- Use WASD to move an entio." << enti::endl;
	enti::cout << "- Use Z to undo a movement." << enti::endl;
	enti::cout << "- Each entio has 2 weapones: 1 sword and  1 bow." << enti::endl;
	enti::cout << "- Press SPACEBAR to use a weapon." << enti::endl;
	enti::cout << "- The sword can instatly kill an enemy if it is placed in a cell next door." << enti::endl;
	enti::cout << "- The bow can damage an enemy within a maximum distance of 10 cells." << enti::endl;	
	enti::cout << "- Each character has only 10 arrows." << enti::endl;
	enti::cout << "- Each turn the less fatigated entio will play first." << enti::endl;
	enti::cout << "- To play with the next entio press ENTER." << enti::endl;
	enti::cout << "- Each player can do 10 of these actions per turn." << enti::endl;
	enti::cout << enti::endl << enti::Color::LIGHTMAGENTA << "Press any key to begin the game!" << enti::endl;
	enti::cout << enti::cend;


	//en lugar del enti::SystemPause usamos bucles do while que comprueban todo el rato la tecla pulsada porque sino perdemos el Input de teclado
	//del usuario
	do {
		if (enti::getInputKey() != enti::InputKey::NONE) {
			byPass = true;
			break;
		}
	} while (true);

	do {
		enti::InputKey localChar = enti::getInputKey();
		if (boss->exit)
			break;
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
						byPass = true;
						break;
					}
				} while (true);
			}
		}
		else; 
	} while (true);
	if (GAMEOVER && !exit) {
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
	return exit;
}


void main()
{
	bool check;
	replay:
	check=Play();
	if (!check) {
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
}