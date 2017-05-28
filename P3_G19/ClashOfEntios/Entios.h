#pragma once

class MonigotesJuego;

class GameManager { //el gamemanager controlará, básicamente, todo. A quien le toca jugar, donde está cada entio, 
					//cuando alguien puede moverse, las acciones restantes...
private:
	std::vector<MonigotesJuego> Equipo1;
	std::vector<MonigotesJuego> Equipo2;
	int actions;

public:
	bool Team1active;
	bool Team2active;
	//std::stack<GameManager> historial;
	char(*layOut)[SizeJ];
	std::vector<MonigotesJuego>& ActiveTeam();
	std::vector<MonigotesJuego>& UnactiveTeam();
	MonigotesJuego& nowMoves();
	MonigotesJuego& setAndFindStress();
	bool ActiveTeamIsDone();
	void Equipo1SetState(bool a);
	void Equipo2SetState(bool a);
	void submitMove(direction vector);
	void ComandoPJ(enti::InputKey pulsado);
	void GameStatus();
	void CambiarEntio();

	ATQStatus estado;
	enti::InputKey WeaponSel;
	enti::InputKey ATQKey;
	void Ataque();
	GameManager();
};

class MonigotesJuego {
private:
	int CoordenadasX;
	int CoordenadasY;
	int vida;

public:
	int flechas;
	char SimboloMonigote;
	bool esControlado;
	int movesAct;
	int turnsPlayed;
	bool hasPlayed;
	char lastChar;
	GameManager& manager;
	MonigotesJuego(GameManager &boss);//constructor de los monigotes


	void plusX();
	void minusX();
	void plusY();
	void minusY();
	void setX(int a);
	void setY(int a);
	int getX();
	int getY();
	void setHP(int a);
	void kill();
	int getHP();
	void harm(int a);
	void goToSleep();
};





class Map {
public:

	char mapa[SizeI][SizeJ];
	Map(std::vector<MonigotesJuego> &Team1, std::vector<MonigotesJuego> &Team2);
}; 
