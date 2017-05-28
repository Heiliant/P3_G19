#pragma once

class MonigotesJuego;



class GameManager { //El gameManager es, como su nombre indica, quien gestiona la partida. Cuando alguien se mueve, ataca, muere, pasa el turno, etc...
					//quien interpreta y actualiza la informaci�n es el gameManager.
private:
	std::vector<MonigotesJuego> Equipo1; //aqu� almacenamos los dos equipos que jugar�n. 
	std::vector<MonigotesJuego> Equipo2;
	int actions;
	
public:
	//Todos estos m�todos sirven para facilitar el c�digo al resto de funciones. GameManager es quien gestiona que equipo y entio estan jugando y 
	//cuando deben ceder su control a otro entio/jugador, de manera que el resto de classes y funciones no deben preocuparse por esas cosas
	bool Team1active;
	bool Team2active;
	std::vector<MonigotesJuego>& ActiveTeam();
	std::vector<MonigotesJuego>& UnactiveTeam();
	MonigotesJuego& nowMoves();
	void setAndFindStress();
	bool ActiveTeamIsDone();
	void Equipo1SetState(bool a);
	void Equipo2SetState(bool a);

	char(*layOut)[SizeJ]; //layOut es una referencia al map que permite al GameManager actualizarlo cuando sea preciso.

	//estas funciones son las que realizan las acciones como tales
	void ComandoPJ(enti::InputKey pulsado); //comandoPJ es quien recibe el Input del player y, en funci�n de este, llama al resto de funciones.
	void submitMove(direction vector);//submitMove mueve al entio activo realizando las comprobaciones necesarias, como que el jugador a�n tenga acciones
										//o que el terreno hacia d�nde se dirige el personaje es navegable.
	void GameStatus();					//GameStatus imprime el map, actualizado anteriormente por el resto de funciones.
	void CambiarEntio();				//CambiarEntio comprueba las acciones restantes del jugador y, o bien cambia al entio activo, o cede el turno
										//al otro Equipo.

	std::stack<std::pair<std::vector<MonigotesJuego>, std::vector<MonigotesJuego>>> historial; //Usamo este stack "historial" para almacenar el estado
	void Undo();				//de ambos equipos siempre que se realiza una acci�n, para as� poder deshacerla con la funci�n Undo.
	
	ATQStatus estado;		//esta variable de enum class sirve para controlar en que punto se encuentra el comando de ataque: nulo, seleccion de armas,
							//selecci�n de direcci�n o informe del ataque
	enti::InputKey WeaponSel;
	enti::InputKey ATQKey;	//Estos inputkeys son auxiliares para la funci�n de ataque
	void Ataque();

	bool exit;  //permite el jugador cerrar la aplicaci�n 

	GameManager(); //El constructor del GameManager se encarga de instanciar el resto de objetos de cada clase
};

class MonigotesJuego {
private:
	int CoordenadasX;		//Coordenadas del entio en cuesti�n. Aqu� es especialmente interesante hacerlas accesibles a trav�s de Setters para impedir as�
	int CoordenadasY;		//que el entio se salga del mapa. (Aunque en este caso est� rodeado de monta�a estos setters acaben resultando poco �tiles)
	int vida;

public:
	int flechas;
	char SimboloMonigote;	//El char con el que es pintado el entio en el mapa
	bool esControlado;

	int movesAct;		//miembros usados para calcular el par�metro fatiga
	int turnsPlayed;
	bool hasPlayed;

	char lastChar;		//este char almacena el caracter de terreno que est� pisando el entio para as� poder volver a pintarlo una vez se desplace
	GameManager& manager; //referencia al GameManager
	MonigotesJuego(GameManager &boss);//constructor de los monigotes


	  //m�todos accesores a las variables de
	//posici�n
	void plusX();		
	void minusX();
	void plusY();
	void minusY();
	void setX(int a);
	void setY(int a);
	int getX();
	int getY();
	//vida
	void setHP(int a);
	void kill();
	int getHP();
	void harm(int a);
	void goToSleep();

	//necesario para poder usar el stack de vectores de Monigotes para el historial
	void operator =(MonigotesJuego &a);
};

bool operator ==(MonigotesJuego &a, MonigotesJuego &b) {
	return(a.getHP() == b.getHP());
}



class Map {
public:
	//Solo se encarga de almacenar los chars que el gaemManager pintar� por pantalla.
	char mapa[SizeI][SizeJ];
	//El constructor del Mapa se encarga de abrir el fichero del dise�o del terreno, leerlo, almacenarlo y cerrarlo.
	Map(std::vector<MonigotesJuego> &Team1, std::vector<MonigotesJuego> &Team2);
}; 
