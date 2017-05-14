#include <iostream>
#include <string>
#include <conio.h>
#include <fstream>
#include "Input.inl.hh"
#include "Renderer.hh"
#include <vector>


#define ARRIBA enti::InputKey::w //este y todos los demas estaban asi w,W y abajo en la funcion MovimientoPlayer me daba error
#define ABAJO enti::InputKey::s
#define DERECHA enti::InputKey::d
#define IZQUIERDA enti::InputKey::a
#define ATACAR enti::InputKey::SPACEBAR
#define REHACER enti::InputKey::z
#define CAMBIAR_ENTIO enti::InputKey::ENTER
#define ATRAS enti::InputKey::ESC

const int SizeI = 36;
const int SizeJ = 74;
const int DosArmas = 2;


enti::InputKey tecla;
std::vector<MonigotesJuego> Equipo1; //undeclared 
std::vector<MonigotesJuego> Equipo2;

enum class Arma{SWORD,BOW};

void Muerte(std::vector <MonigotesJuego> Equipo1, std::vector <MonigotesJuego> Equipo2); //forward declaration
void Ataque(int armusa, int &iterador, char &mapa, bool &controlador, int &ContadorAcciones, std::vector <MonigotesJuego> Equipo1, std::vector <MonigotesJuego> Equipo2); //forward declaration

class MonigotesJuego
{
public:

	int vida;                             //todos los atributos que tendran los monigotes que se mueven en el juego
	Arma Weapon[DosArmas];  //diria que nos podemos ahorrar esto, ya que los jugadores siempre llevarán sus armas y no pueden perderlas/ cambiarlas
	int Flechas;
	int Damage;				//creo que nos lo podemos ahorrar ya que el damage depende del tipo de ataque, no del PJ
	char SimboloMonigote;
	int CoordenadasX;
	int CoordenadasY;
	bool dead;				//si tenemos la vida nos podemos ahorrar esto, aunque está bien tenerlo para hacer las cosas más legibles
	bool esControlado;		
	char TerrenoAnterior; //tomara el valor del char que corresponde al terreno donde esta el jugador, para cuando este se mueva vuelva a pintar el char del terreno donde estaba
							//diria que es más conveniente que sea el mapa quien actualiza lo que se dibuja en cada hueco del mismo. De modo que el mapa
							//sepa donde están los players y pinte el contenido del array y "encima" de esta, el simbolo de los PJ.

	MonigotesJuego() //constructor de los monigotes
	{
		vida = 10;
		Weapon[0] = Arma::SWORD;  Weapon[1]= Arma::BOW; //
		dead = false; 
		esControlado = false;
		Equipo1.front().esControlado = true; //todos seran false menos el primero del equipo 1 que sera true, sera el primero qeu empezara a controlarse
											//esto se llama al instanciar cada monigote, así que mejor lo hacemos al instanciar el mapa o en el main.
		Flechas = 10;
	}

	~MonigotesJuego() {};

	void SeleccionarEntio()
	{

	}

	void ComandoPlayer(char &mapa) //funcion sobre lo que puede hacer el player y como segun esta el mapa variará lo pasamos por referencia
	{
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

};

class Map
{
public:

	char mapa[SizeJ][SizeI];

	Map(std::vector<MonigotesJuego> &Team1, std::vector<MonigotesJuego> &Team2) //no se si el recurso se tiene que leer como archivo asi que dejo esto asi por si al final se hace asi leerlo caracter a caracter
	{
		std::ifstream mapOverlay("default.cfg");
		char linia[SizeJ];

		for (int i = 0; mapOverlay.getline(linia, SizeJ); ++i) { //lee el archivo default.cfg y lo guarda bien ordenadito en el array mapa.
			for (int j = 0; j < SizeJ; ++j) {					//si se encuentra con los players (1, 2, 3, 4, 5, a, b, c, d, e o f), escribe '.' en el mapa
				switch (linia[i]) {								//y les asigna la posición en la que nos los hemos encontrado.
				case 'A': Team1.at(0).CoordenadasX, Team1.at(0).CoordenadasY = i, j;
					mapa[i][j] = '.';
					break;
				case 'B':Team1.at(1).CoordenadasX, Team1.at(1).CoordenadasY = i, j;
					mapa[i][j] = '.';
					break;
				case 'C':Team1.at(2).CoordenadasX, Team1.at(2).CoordenadasY = i, j;
					mapa[i][j] = '.';
					break;
				case 'D':Team1.at(3).CoordenadasX, Team1.at(3).CoordenadasY = i, j;
					mapa[i][j] = '.';
					break;
				case 'E':Team1.at(4).CoordenadasX, Team1.at(4).CoordenadasY = i, j;
					mapa[i][j] = '.';
					break;
				case 'F':Team1.at(5).CoordenadasX, Team1.at(5).CoordenadasY = i, j;
					mapa[i][j] = '.';
					break;
				case '1':Team2.at(0).CoordenadasX, Team2.at(0).CoordenadasY = i, j;
					mapa[i][j] = '.';
					break;
				case '2':Team2.at(1).CoordenadasX, Team2.at(1).CoordenadasY = i, j;
					mapa[i][j] = '.';
					break;
				case '3':Team2.at(2).CoordenadasX, Team2.at(2).CoordenadasY = i, j;
					mapa[i][j] = '.';
					break;
				case '4':Team2.at(3).CoordenadasX, Team2.at(3).CoordenadasY = i, j;
					mapa[i][j] = '.';
					break;
				case '5':Team2.at(4).CoordenadasX, Team2.at(4).CoordenadasY = i, j;
					mapa[i][j] = '.';
					break;
				case '6':Team2.at(5).CoordenadasX, Team2.at(5).CoordenadasY = i, j;
					mapa[i][j] = '.';
					break;
				default:
					mapa[i][j] = linia[j];
					std::cout << linia[j];
					break;
				}
			}
			std::cout << std::endl;
		}

	}

};
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


void main()
{
	Map* mapa = new Map();
	int x = 0;
}