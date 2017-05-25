#pragma once

class MonigotesJuego;

class GameManager { //el gamemanager controlará, básicamente, todo. A quien le toca jugar, donde está cada entio, 
					//cuando alguien puede moverse, las acciones restantes
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
	bool ActiveTeamIsDone();
	void Equipo1SetState(bool a);
	void Equipo2SetState(bool a);
	void submitMove(direction vector);
	void ComandoPJ(enti::InputKey pulsado);
	void CambiarEntio();
	GameManager();
};

class MonigotesJuego {
private:
	int CoordenadasX;
	int CoordenadasY;

public:
	int vida;                             //todos los atributos que tendran los monigotes que se mueven en el juego
	//Arma Weapon[DosArmas];  //diria que nos podemos ahorrar esto, ya que los jugadores siempre llevarán sus armas y no pueden perderlas/ cambiarlas
	int flechas;
	//int Damage;				//creo que nos lo podemos ahorrar ya que el damage depende del tipo de ataque, no del PJ
	char SimboloMonigote;
	bool dead;				//si tenemos la vida nos podemos ahorrar esto, aunque está bien tenerlo para hacer las cosas más legibles
	bool esControlado;
	//char TerrenoAnterior; //tomara el valor del char que corresponde al terreno donde esta el jugador, para cuando este se mueva vuelva a pintar el char del terreno donde estaba
	//diria que es más conveniente que sea el mapa quien actualiza lo que se dibuja en cada hueco del mismo. De modo que el mapa
	//sepa donde están los players y pinte el contenido del array y "encima" de esta, el simbolo de los PJ.
	int fatiga;
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
};





class Map {
public:

	char mapa[SizeI][SizeJ];
	Map(std::vector<MonigotesJuego> &Team1, std::vector<MonigotesJuego> &Team2);
};