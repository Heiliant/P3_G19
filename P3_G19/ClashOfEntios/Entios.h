#pragma once

class MonigotesJuego;

class GameManager { //el gamemanager controlar�, b�sicamente, todo. A quien le toca jugar, donde est� cada entio, 
					//cuando alguien puede moverse, las acciones restantes...
private:
	std::vector<MonigotesJuego> Equipo1;
	std::vector<MonigotesJuego> Equipo2;
	bool Team1active;
	bool Team2active;
	int actions;

public:
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
	//int Damage;				//creo que nos lo podemos ahorrar ya que el damage depende del tipo de ataque, no del PJ
	char SimboloMonigote;
	bool dead;				//si tenemos la vida nos podemos ahorrar esto, aunque est� bien tenerlo para hacer las cosas m�s legibles
	bool esControlado;
	//char TerrenoAnterior; //tomara el valor del char que corresponde al terreno donde esta el jugador, para cuando este se mueva vuelva a pintar el char del terreno donde estaba
	//diria que es m�s conveniente que sea el mapa quien actualiza lo que se dibuja en cada hueco del mismo. De modo que el mapa
	//sepa donde est�n los players y pinte el contenido del array y "encima" de esta, el simbolo de los PJ.
	int movesAct;
	int turnsPlayed;
	bool hasPlayed;
	char lastChar;
	GameManager& manager;
	MonigotesJuego(GameManager &boss);//constructor de los monigotes
	~MonigotesJuego();

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
};





class Map {
public:

	char mapa[SizeI][SizeJ];
	Map(std::vector<MonigotesJuego> &Team1, std::vector<MonigotesJuego> &Team2);
}; 
