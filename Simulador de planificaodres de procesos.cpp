//Cabecera

	/*
	Proyecto: 
	Simulador de planificadores de procesos
	Alumnos:
	Francisco de Jesus Mendez Lara			ID: 289539
	Sandra Olimpia Estrella Prieto Garcia	ID: 292930
	Gerardo Femat Delgado					ID: 244371
	Emmanuel Munoz Cerda					ID: 244280
	entrega 13/12/2021

	*/

//Librerias

	#include <iostream> 
	#include <fstream>
	#include <stdlib.h>
	#include <time.h>
	#include <locale.h> //Agarra acentos
	#include <windows.h>//Funcion Sleep
	#include <iomanip> //para usar setprecision
	#include <cmath> //para usar trunc
	#include<string.h>
	using namespace std;

//Constantes

	#define TAM 25
	#define TAMPART 32 /*tamano que mide cada espacio en RAM*/
	#define TAMSTRING 25 /*Tamaño para los char * que se usan*/

//Estructuras

	struct IngreUsu{//Usuario y contrasena
		std::string Usuario;
		std::string Contrasena;
	};

	struct Proceso{//Caracteristicas de un proceso
		int tam /*Tamaño que utiliza en RAM*/, cuanto /*lo que dura para ejec.*/, id, cuantoCiclo /*Cuanto se ejecuta por ciclo*/;
		int tiempo /*Tiempo que se tardo en atenderse*/, espacios;
	};

	struct Lista{//Lista de procesos listos
		Proceso listado /*Procesos en la lista*/;
		Lista* liga /*Liga al sig. proceso*/;
	};

	struct Memoria{//Caracteristicas de la memoria RAM
		int* cola /*Id de procesos ejecutandose*/;
		bool* ocupado /*Se llena de trues y false*/;
		int tam /*Tamano es el que asigna el usuario*/, espacioMemoria;// 1, 4, 8 MB (Tamano que asigno el usuario en GenerarMemoria())
	};

	struct Parametros{ //Parametros que el usuario ingresa
		int cuantosM /*Cuantos maximos*/, cuantosProceso /*Guarda cuantos CUANTOS necesita el proceso para terminar*/;
		int NumProceso /*Cantidad de procesos que el usuario quiere crear*/;
		int MemoMaxProc; /*Guardara el tamano exacto del proceso*/
		int MemoriaMax; /*Guarda la RAM que coloco el usuario*/
	};

	struct GuardarParametros{ //Guarda los parametros
		Parametros datos;
		char nombre[TAMSTRING];
	};

	struct Resultados{
		int ciclos /*Ciclos que tarda en finalizar los programas*/ ;
		float sumTiempo/*Suma de los tiempos que se tardo en atender un proceso*/;
		int contTermProceso /*Contador de procesos terminados*/, idM /*ID del nuevo proceso*/;
		char nombrePlan[TAMSTRING]; /*Nombre del planificador*/
	};

//Clases

	class Planificadores{
		public:
			Planificadores();//const
			~Planificadores();//des
			void Saltos(int); //Funcion para dar saltos
			//Procesos
			void GenerarProceso();
			int TamAleatorio();
			void AsignarMemoria();//Se asigna un espacio de memoria disponible para un proceso en la cola de listos (PRIMER AJUSTE)
			Lista* eliminaProceso();
			void listaProcesos(); //Muestra la lista de procesos en el orden que va
			Lista* eliminaProcesoNo();
			void AsignarCuanto(){};
			//Memoria
			void GenerarMemoria();//Generamos los espacios de memoria
			Lista* Crea();
			Lista* Inserta(Lista *);
			bool verificarEspacio();//verifica que haya espacio suf. en RAM para meter procesos
			void manejoMemoria(); //Saca a la pantalla como se esta manejando la memoria
			void mostrarMemoria();
			//Archivos
			void ReporteCiclos();
			void ReporteTiempo();
			void ReporteIniciados();
			void ReporteEspera();

			//Variables
				//Variables para los procesos
				Resultados res;
				Proceso actual/*El proceso que esta en ejecucion*/;
				Lista *procesosListos = NULL;//generar los procesos para ingresar a la cola
				//Variables para la memoria
				Memoria prin;// memoria ram para la cola
				Lista *procesosNoListos = NULL;
				Lista *TodosProcesos = NULL;
				int MemSelec,MemPart; // variables para generar la memoria principal
				//Datos parametrizables
				Parametros configuracion; //Datos configurados por el usuario
				friend ostream &operator << (ostream &, Planificadores &);
				friend istream &operator >> (istream &, Planificadores &);
			//fstream archResultados;
	};

	class RoundRobin: public Planificadores{//RoundRobin
		public:
			Lista* finalLista();
			void AsignarCuanto();
			void inicia();
			//Archivos
			void ReporteCiclos();
			void ReporteTiempo();
			void ReporteIniciados();
			void ReporteEspera();
			//Ejecucion del planificador
			RoundRobin();
			RoundRobin(const RoundRobin&);
			RoundRobin(int, int, int, int, int);
			~RoundRobin();
			friend ostream &operator << (ostream &, RoundRobin &);
			friend istream &operator >> (istream &, RoundRobin &);
	};

	class PEPS: public Planificadores{ //Primera entrada primera salida
		public:
			void AsignarCuanto();
			void inicia();
			//Archivos
			void ReporteCiclos();
			void ReporteTiempo();
			void ReporteIniciados();
			void ReporteEspera();
			//Ejecucion del planificador
			PEPS();
			PEPS(const PEPS&);
			PEPS(int, int, int, int, int);
			~PEPS();
			friend ostream &operator << (ostream &, PEPS &);
			friend istream &operator >> (istream &, PEPS &);
	};

	class SJF: public Planificadores{ //Trabajo más corto primero
		public:
			void AsignarCuanto();
			void inicia();
			Lista *masPeq(); //Obtiene de la lista el proceso m�s peque�o
			//Archivos
			void ReporteCiclos();
			void ReporteTiempo();
			void ReporteIniciados();
			void ReporteEspera();
			SJF();
			SJF(const SJF&);
			SJF(int, int, int, int, int);
			~SJF();
			friend ostream &operator << (ostream &, SJF &);
			friend istream &operator >> (istream &, SJF &);
	};

	class LTL: public Planificadores{ //Trabajo con menos tiempo primero
		public:
			void AsignarCuanto();
			void inicia();
			//Archivos
			void ReporteCiclos();
			void ReporteTiempo();
			void ReporteIniciados();
			void ReporteEspera();
			Lista *menosTiempo(); //Obtiene de la lista el proceso m�s peque�o
			LTL();
			LTL(const LTL&);
			LTL(int, int, int, int, int);
			~LTL();
	};

//Declaracion de funciones

	void GenerarArch();
	bool Verificar(IngreUsu);
	void Registrar(IngreUsu);
	void UsuariosReg();
	void IniciarInvitado();
	void VerificarInvitados();
	void MenuPlanificadores();
	void MenuLogin();
	void MenuBusquedas();
	void MenuOrdenaciones();
	void MenuBusquedaEspecifico(char []);
	void Ordenaciones(Planificadores,char []);
	void BusquedaCiclos(char [], int);
	void BusquedaTiempo(char [], int);
	void BusquedaTerminados(char [], int);
	void BusquedaGenerados(char [], int);
	void mostrarLomito();
	void Grinch();
	void Pentagrama();
//Funciones para excepciones

	void insertar(int &num){
		bool aux=true;
		do{
			try{
			std::cin >> num;
			aux=true;
			}
			catch ( ... ){
				cout << "\tAlgo ha pasado al leer!" << endl;
				cin.clear();
				fflush(stdin);
				cout << "\tError eliminado... " << endl;
				aux=false;
			}
		
		}while(!aux);
	}

//cin y cout programados para todas las clases

	ostream &operator << (ostream &out, PEPS &obj){
			//Estadisticas inician
		fstream archResultados, archResGen, archResPlan;
		char res;
		out << endl << endl;
		printf("\033[1;35m");
		printf("\t%c",201);
		for(int i=0;i<47;i++)printf("%c",205);
		printf("%c\n",187);
		printf("\t%c",186);
		out << "                                               ";
		printf("%c\n",186);
		printf("\t%c",186);
		printf("\033[0m");
		out << "        Estadisticas de la simulacion:         ";
		printf("\033[1;35m");
		printf("%c\n",186);
		printf("\t%c",186);
		out << "                                               ";
		printf("%c\n",186);
		printf("\t%c",186);
		printf("\033[0m");
		out << "    Ciclos promedio en finalizar: " << obj.res.ciclos << " ciclos";
		printf("\033[1;35m");
		printf("\t%c\n",186);
		printf("\t%c",186);
		printf("\033[0m");
		out << "    Cantidad de procesos iniciados: " << obj.res.idM << "\t\t";
		printf("\033[1;35m");
		printf("%c\n",186);
		printf("\t%c",186);
		printf("\033[0m");
		out << "    Procesos que esperaron para entrar: " << obj.res.contTermProceso << "\t";
		printf("\033[1;35m");
		printf("%c\n",186);
		printf("\t%c",186);
		printf("\033[0m");
		out << "    Tiempo promedio de atencion: " << setprecision(4) << obj.res.sumTiempo << "\t\t";
		printf("\033[1;35m");
		printf("%c\n",186);
		printf("\t%c",186);
		out << "                                               ";
		printf("%c\n",186);
		printf("\t%c",186);
		out << "                                               ";
		printf("%c\n",186);
		printf("\t%c",200);
		for(int i=0;i<47;i++)printf("%c",205);
		printf("%c\n",188);
		printf("\033[0m");
		cout << "\tQuiere guardar los resultados en un archivo? (y/n) ";//Falta agregar a los demas simuladores
		cin >> res;
		res = toupper(res);
		if(res=='Y'){
			archResultados.open("PEPSResultados.txt",ios::app|ios::out);
			archResultados << "\t*************************************************\n";
			archResultados << "\t*                                               *\n";
			archResultados << "\t*                    P E P S                    *\n";
			archResultados << "\t*         Estadisticas de la simulacion         *\n";
			archResultados << "\t*                                               *\n";
			archResultados << "\t*    Ciclos promedio en finalizar: " << obj.res.ciclos << " ciclos" << "\t*\n";
			archResultados << "\t*    Cantidad de procesos iniciados: " << obj.res.idM << "\t\t*\n";
			archResultados << "\t*    Procesos que esperaron para entrar: " << obj.res.contTermProceso << "\t*\n";
			archResultados << "\t*    Tiempo promedio de atencion: " << setprecision(4) << obj.res.sumTiempo << "\t\t*\n";
			archResultados << "\t*                                               *\n";
			archResultados << "\t*                                               *\n";
			archResultados << "\t*************************************************\n\n";
			archResultados.close();
		}
		strcpy(obj.res.nombrePlan,"PEPS");
		archResGen.open("GeneralResultados.dat",ios::binary|ios::app|ios::in);
		archResGen.write((char *)&obj.res,sizeof(Resultados));
		archResGen.close();
		archResPlan.open("PEPSbinResultados.dat",ios::binary|ios::app|ios::in);
		archResPlan.write((char *)&obj.res,sizeof(Resultados));
		archResPlan.close();
		return out;
	}

	ostream &operator << (ostream &out, RoundRobin &obj){
			//Estadisticas inician
		fstream archResultados, archResGen, archResPlan;
		char res;
		out << endl << endl;
		printf("\033[1;35m");
		printf("\t%c",201);
		for(int i=0;i<47;i++)printf("%c",205);
		printf("%c\n",187);
		printf("\t%c",186);
		out << "                                               ";
		printf("%c\n",186);
		printf("\t%c",186);
		printf("\033[0m");
		out << "        Estadisticas de la simulacion:         ";
		printf("\033[1;35m");
		printf("%c\n",186);
		printf("\t%c",186);
		out << "                                               ";
		printf("%c\n",186);
		printf("\t%c",186);
		printf("\033[0m");
		out << "    Ciclos promedio en finalizar: " << obj.res.ciclos << " ciclos";
		printf("\033[1;35m");
		printf("\t%c\n",186);
		printf("\t%c",186);
		printf("\033[0m");
		out << "    Cantidad de procesos iniciados: " << obj.res.idM << "\t\t";
		printf("\033[1;35m");
		printf("%c\n",186);
		printf("\t%c",186);
		printf("\033[0m");
		out << "    Procesos que esperaron para entrar: " << obj.res.contTermProceso << "\t";
		printf("\033[1;35m");
		printf("%c\n",186);
		printf("\t%c",186);
		printf("\033[0m");
		out << "    Tiempo promedio de atencion: " << setprecision(4) << obj.res.sumTiempo << "\t\t";
		printf("\033[1;35m");
		printf("%c\n",186);
		printf("\t%c",186);
		out << "                                               ";
		printf("%c\n",186);
		printf("\t%c",186);
		out << "                                               ";
		printf("%c\n",186);
		printf("\t%c",200);
		for(int i=0;i<47;i++)printf("%c",205);
		printf("%c\n",188);
		printf("\033[0m");
		cout << "\tQuiere guardar los resultados en un archivo? (y/n) ";//Falta agregar a los demas simuladores
		cin >> res;
		res = toupper(res);
		if(res=='Y'){
				archResultados.open("RoundRobinResultados.txt",ios::app|ios::out);
				archResultados << "\t*************************************************\n";
				archResultados << "\t*                                               *\n";
				archResultados << "\t*                 Round Robin                   *\n";
				archResultados << "\t*         Estadisticas de la simulacion         *\n";
				archResultados << "\t*                                               *\n";
				archResultados << "\t*    Ciclos promedio en finalizar: " << obj.res.ciclos << " ciclos" << "\t*\n";
				archResultados << "\t*    Cantidad de procesos iniciados: " << obj.res.idM << "\t\t*\n";
				archResultados << "\t*    Procesos que esperaron para entrar: " << obj.res.contTermProceso << "\t*\n";
				archResultados << "\t*    Tiempo promedio de atencion: " << setprecision(4) << obj.res.sumTiempo << "\t\t*\n";
				archResultados << "\t*                                               *\n";
				archResultados << "\t*                                               *\n";
				archResultados << "\t*************************************************\n\n";
				archResultados.close();
		}
		strcpy(obj.res.nombrePlan,"RoundRobin");
		archResGen.open("GeneralResultados.dat",ios::binary|ios::app|ios::in);
		archResGen.write((char *)&obj.res,sizeof(Resultados));
		archResGen.close();
		archResPlan.open("RoundRobinBinResultados.dat",ios::binary|ios::app|ios::in);
		archResPlan.write((char *)&obj.res,sizeof(Resultados));
		archResPlan.close();
		return out;
	}

	ostream &operator << (ostream &out, SJF &obj){
			//Estadisticas inician
		fstream archResultados, archResGen, archResPlan;
		char res;
		out << endl << endl;
		printf("\033[1;35m");
		printf("\t%c",201);
		for(int i=0;i<47;i++)printf("%c",205);
		printf("%c\n",187);
		printf("\t%c",186);
		out << "                                               ";
		printf("%c\n",186);
		printf("\t%c",186);
		printf("\033[0m");
		out << "        Estadisticas de la simulacion:         ";
		printf("\033[1;35m");
		printf("%c\n",186);
		printf("\t%c",186);
		out << "                                               ";
		printf("%c\n",186);
		printf("\t%c",186);
		printf("\033[0m");
		out << "    Ciclos promedio en finalizar: " << obj.res.ciclos << " ciclos";
		printf("\033[1;35m");
		printf("\t%c\n",186);
		printf("\t%c",186);
		printf("\033[0m");
		out << "    Cantidad de procesos iniciados: " << obj.res.idM << "\t\t";
		printf("\033[1;35m");
		printf("%c\n",186);
		printf("\t%c",186);
		printf("\033[0m");
		out << "    Procesos que esperaron para entrar: " << obj.res.contTermProceso << "\t";
		printf("\033[1;35m");
		printf("%c\n",186);
		printf("\t%c",186);
		printf("\033[0m");
		out << "    Tiempo promedio de atencion: " << setprecision(4) << obj.res.sumTiempo << "\t\t";
		printf("\033[1;35m");
		printf("%c\n",186);
		printf("\t%c",186);
		out << "                                               ";
		printf("%c\n",186);
		printf("\t%c",186);
		out << "                                               ";
		printf("%c\n",186);
		printf("\t%c",200);
		for(int i=0;i<47;i++)printf("%c",205);
		printf("%c\n",188);
		printf("\033[0m");
		cout << "\tQuiere guardar los resultados en un archivo? (y/n) ";//Falta agregar a los demas simuladores
		cin >> res;
		res = toupper(res);
		if(res=='Y'){
			archResultados.open("SJFResultados.txt",ios::app|ios::out);
			archResultados << "\t*************************************************\n";
			archResultados << "\t*                                               *\n";
			archResultados << "\t*                     S J F                     *\n";
			archResultados << "\t*         Estadisticas de la simulacion         *\n";
			archResultados << "\t*                                               *\n";
			archResultados << "\t*    Ciclos promedio en finalizar: " << obj.res.ciclos << " ciclos" << "\t*\n";
			archResultados << "\t*    Cantidad de procesos iniciados: " << obj.res.idM << "\t\t*\n";
			archResultados << "\t*    Procesos que esperaron para entrar: " << obj.res.contTermProceso << "\t*\n";
			archResultados << "\t*    Tiempo promedio de atencion: " << setprecision(4) << obj.res.sumTiempo << "\t\t*\n";
			archResultados << "\t*                                               *\n";
			archResultados << "\t*                                               *\n";
			archResultados << "\t*************************************************\n\n";
			archResultados.close();
		}
		strcpy(obj.res.nombrePlan,"SJF");
		archResGen.open("GeneralResultados.dat",ios::binary|ios::app|ios::in);
		archResGen.write((char *)&obj.res,sizeof(Resultados));
		archResGen.close();
		archResPlan.open("SJFbinResultados.dat",ios::binary|ios::app|ios::in);
		archResPlan.write((char *)&obj.res,sizeof(Resultados));
		archResPlan.close();
		return out;
	}

	ostream &operator << (ostream &out, LTL &obj){
		//Estadisticas inician
		fstream archResultados, archResGen, archResPlan;
		char res;
		out << endl << endl;
		printf("\033[1;35m");
		printf("\t%c",201);
		for(int i=0;i<47;i++)printf("%c",205);
		printf("%c\n",187);
		printf("\t%c",186);
		out << "                                               ";
		printf("%c\n",186);
		printf("\t%c",186);
		printf("\033[0m");
		out << "        Estadisticas de la simulacion:         ";
		printf("\033[1;35m");
		printf("%c\n",186);
		printf("\t%c",186);
		out << "                                               ";
		printf("%c\n",186);
		printf("\t%c",186);
		printf("\033[0m");
		out << "    Ciclos promedio en finalizar: " << obj.res.ciclos << " ciclos";
		printf("\033[1;35m");
		printf("\t%c\n",186);
		printf("\t%c",186);
		printf("\033[0m");
		out << "    Cantidad de procesos iniciados: " << obj.res.idM << "\t\t";
		printf("\033[1;35m");
		printf("%c\n",186);
		printf("\t%c",186);
		printf("\033[0m");
		out << "    Procesos que esperaron para entrar: " << obj.res.contTermProceso << "\t";
		printf("\033[1;35m");
		printf("%c\n",186);
		printf("\t%c",186);
		printf("\033[0m");
		out << "    Tiempo promedio de atencion: " << setprecision(4) << obj.res.sumTiempo << "\t\t";
		printf("\033[1;35m");
		printf("%c\n",186);
		printf("\t%c",186);
		out << "                                               ";
		printf("%c\n",186);
		printf("\t%c",186);
		out << "                                               ";
		printf("%c\n",186);
		printf("\t%c",200);
		for(int i=0;i<47;i++)printf("%c",205);
		printf("%c\n",188);
		printf("\033[0m");
		cout << "\tQuiere guardar los resultados en un archivo? (y/n) ";//Falta agregar a los demas simuladores
		cin >> res;
		res = toupper(res);
		if(res=='Y'){
			archResultados.open("LTLResultados.txt",ios::app|ios::out);
			archResultados << "\t*************************************************\n";
			archResultados << "\t*                                               *\n";
			archResultados << "\t*               	  L T L		                 *\n";
			archResultados << "\t*         Estadisticas de la simulacion         *\n";
			archResultados << "\t*                                               *\n";
			archResultados << "\t*    Ciclos promedio en finalizar: " << obj.res.ciclos << " ciclos" << "\t*\n";
			archResultados << "\t*    Cantidad de procesos iniciados: " << obj.res.idM << "\t\t*\n";
			archResultados << "\t*    Procesos que esperaron para entrar: " << obj.res.contTermProceso << "\t*\n";
			archResultados << "\t*    Tiempo promedio de atencion: " << setprecision(4) << obj.res.sumTiempo << "\t\t*\n";
			archResultados << "\t*                                               *\n";
			archResultados << "\t*                                               *\n";
			archResultados << "\t*************************************************\n\n";
			archResultados.close();
		}
		strcpy(obj.res.nombrePlan,"LTL");
		archResGen.open("GeneralResultados.dat",ios::binary|ios::app|ios::in);
		archResGen.write((char *)&obj.res,sizeof(Resultados));
		archResGen.close();
		archResPlan.open("LTLbinResultados.dat",ios::binary|ios::app|ios::in);
		archResPlan.write((char *)&obj.res,sizeof(Resultados));
		archResPlan.close();
		return out;
	}

	istream &operator >> (istream &in, RoundRobin &obj){
		char op, res;
		fstream arch;
		bool band = false;
		char nombre[TAMSTRING];
		GuardarParametros aux, param;
		cout << "\tQuiere cargar una configuracion previamente usada (y/n)? ";
		cin >> op;
		op = toupper(op);
		if(op!='Y'){
			in.exceptions(std::ios_base::failbit);
			do {
				cout << "\tIngrese el tamano de la memoria: 1/4/8 MB: ";
				insertar(obj.MemPart);
				switch (obj.MemPart) {
					case 1:
						obj.prin.tam = 1024;
						obj.MemSelec = 1024 / TAMPART;
						break;
					case 4:
						obj.prin.tam = 4096;
						obj.MemSelec = 4096 / TAMPART;
						break;
					case 8:
						obj.prin.tam = 8192;
						obj.MemSelec = 8192 / TAMPART;
						break;
					default: cout << "\tCantidad de memoria no disponible... \n \tIngrese una cantidad valida... " << endl; obj.Saltos(2500);
				}
				obj.configuracion.MemoriaMax = obj.MemPart;
			} while (obj.MemPart != 1 && obj.MemPart != 4 && obj.MemPart != 8);
			do{
				cout << "\tIngrese la cantidad de Cuantos maximo que hara cada proceso por ronda: ";//Cuanto tiempo puede ejecutarse un proceso por ciclo (timer)
				insertar(obj.configuracion.cuantosM);
				if(obj.configuracion.cuantosM <= 0){
					cout << "\t>Ingrese una cantidad mayor a cero<" << endl;
				}
			}while(obj.configuracion.cuantosM<=0);
			do{
				cout << "\tIngrese la cantidad de Cuantos maximo que se asignara a los procesos: ";//Cuanto tiempo necesita para completar su ejecucion
				insertar(obj.configuracion.cuantosProceso);
				if(obj.configuracion.cuantosProceso <= 0){
					cout << "\t>Ingrese una cantidad mayor a cero<" << endl;
				}
			}while(obj.configuracion.cuantosProceso<=0);
			do{
			cout << "\tIngrese la cantidad de memoria maxima que se le puede asignar a los procesos (en KiloBytes): ";//Cuanto es lo max. que puede medir el proceso
			insertar(obj.configuracion.MemoMaxProc);
			if(obj.configuracion.MemoMaxProc <= 0){
				cout << "\t>Ingrese una cantidad mayor a cero<" << endl;
			}
			}while(obj.configuracion.MemoMaxProc<=0);
			cout << "\tIngrese la cantidad de procesos que se desean generar: ";
			insertar(obj.configuracion.NumProceso);
			obj.Saltos(1000);
			cout << "\tDesea guardar la configuracion realizada (y/n)? ";
			cin >> res;
			res = toupper(res);
			if(res == 'Y'){
				cout << "\tNombre de la configuracion: ";
				cin >> aux.nombre;
				aux.datos = obj.configuracion;
				arch.open("configuracionRoundRobin.dat",ios::binary|ios::app|ios::in);
				arch.write((char *)&aux,sizeof(GuardarParametros));
				arch.close();
			}
		}
		else{
			arch.open("configuracionRoundRobin.dat",ios::binary|ios::app|ios::in);
			do{
				while(arch.read((char *)&aux,sizeof(GuardarParametros))){
					cout << "\n\n\tNombre: " << aux.nombre;
					cout << "\n\tMemoria asignada: " << aux.datos.MemoriaMax << " MB";
					cout << "\n\tCuantos maximos que se completan: " << aux.datos.cuantosProceso;
					cout << "\n\tCuantos maximos que se asignan: " << aux.datos.cuantosM;
					cout << "\n\tMemoria maxima que se asigna: " << aux.datos.MemoMaxProc;
					cout << "\n\tPrcesos que se generan: " << aux.datos.NumProceso;
				}
				cout << "\n\n\tNombre de la configuracion a cargar: ";
				cin >> nombre;
				arch.close();
				arch.open("configuracionRoundRobin.dat",ios::binary|ios::app|ios::in);
				while(arch.read((char *)&aux,sizeof(GuardarParametros))){
					if(strcmp(aux.nombre,nombre)==0){
						param = aux;
						band = true;
						break;
					}
				}
				if(!band){
					cout << "\tNo se encontro la configuracion con ese nombre! Desea cancelar el proceso (y/n)? ";
					cin >> res;
					res = toupper(res);
					obj.configuracion.MemoriaMax = 0;
				}else{
					obj.configuracion = param.datos;
					switch (param.datos.MemoriaMax) {
						case 1:
							obj.prin.tam = 1024;
							obj.MemSelec = 1024 / TAMPART;
							break;
						case 4:
							obj.prin.tam = 4096;
							obj.MemSelec = 4096 / TAMPART;
							break;
						case 8:
							obj.prin.tam = 8192;
							obj.MemSelec = 8192 / TAMPART;
					}
					res = 'Y';
				}
			}while(res != 'Y');
			arch.close();
		}
		return in;
	}

	istream &operator >> (istream &in, PEPS &obj){
		char op, res;
		fstream arch;
		char nombre[TAMSTRING];
		bool band = false;
		GuardarParametros aux, param;
		cout << "\tQuiere cargar una configuracion previamente usada (y/n)? ";
		cin >> op;
		op = toupper(op);
		if(op!='Y'){
			in.exceptions(std::ios_base::failbit);
			do {
				cout << "\tIngrese el tamano de la memoria: 1/4/8 MB: ";
				insertar(obj.MemPart);
				switch (obj.MemPart) {
					case 1:
						obj.prin.tam = 1024;
						obj.MemSelec = 1024 / TAMPART;
						break;
					case 4:
						obj.prin.tam = 4096;
						obj.MemSelec = 4096 / TAMPART;
						break;
					case 8:
						obj.prin.tam = 8192;
						obj.MemSelec = 8192 / TAMPART;
						break;
					default: cout << "\tCantidad de memoria no disponible... \n \tIngrese una cantidad valida... " << endl; obj.Saltos(2500);
				}
				obj.configuracion.MemoriaMax = obj.MemPart;
			} while (obj.MemPart != 1 && obj.MemPart != 4 && obj.MemPart != 8);
			do{
				cout << "\tIngrese la cantidad de Cuantos maximo que hara cada proceso por ronda: ";//Cuanto tiempo puede ejecutarse un proceso por ciclo (timer)
				insertar(obj.configuracion.cuantosM);
				if(obj.configuracion.cuantosM <= 0){
					cout << "\t>Ingrese una cantidad mayor a cero<" << endl;
				}
			}while(obj.configuracion.cuantosM<=0);
			do{
				cout << "\tIngrese la cantidad de Cuantos maximo que se asignara a los procesos: ";//Cuanto tiempo necesita para completar su ejecucion
				insertar(obj.configuracion.cuantosProceso);
				if(obj.configuracion.cuantosProceso <= 0){
					cout << "\t>Ingrese una cantidad mayor a cero<" << endl;
				}
			}while(obj.configuracion.cuantosProceso<=0);
			do{
			cout << "\tIngrese la cantidad de memoria maxima que se le puede asignar a los procesos (en KiloBytes): ";//Cuanto es lo max. que puede medir el proceso
			insertar(obj.configuracion.MemoMaxProc);
			if(obj.configuracion.MemoMaxProc <= 0){
				cout << "\t>Ingrese una cantidad mayor a cero<" << endl;
			}
			}while(obj.configuracion.MemoMaxProc<=0);
			cout << "\tIngrese la cantidad de procesos que se desean generar: ";
			insertar(obj.configuracion.NumProceso);
			obj.Saltos(1000);
			cout << "\tDesea guardar la configuracion realizada (y/n)? ";
			cin >> res;
			res = toupper(res);
			if(res == 'Y'){
				cout << "\tNombre de la configuracion: ";
				cin >> aux.nombre;
				aux.datos = obj.configuracion;
				arch.open("configuracionPEPS.dat",ios::binary|ios::app|ios::in);
				arch.write((char *)&aux,sizeof(GuardarParametros));
				arch.close();
			}
		}
		else{
			arch.open("configuracionPEPS.dat",ios::binary|ios::app|ios::in);
			do{
				while(arch.read((char *)&aux,sizeof(GuardarParametros))){
					cout << "\n\n\tNombre: " << aux.nombre;
					cout << "\n\tMemoria asignada: " << aux.datos.MemoriaMax << " MB";
					cout << "\n\tCuantos maximos que se completan: " << aux.datos.cuantosProceso;
					cout << "\n\tCuantos maximos que se asignan: " << aux.datos.cuantosM;
					cout << "\n\tMemoria maxima que se asigna: " << aux.datos.MemoMaxProc;
					cout << "\n\tPrcesos que se generan: " << aux.datos.NumProceso;
				}
				cout << "\n\n\tNombre de la configuracion a cargar: ";
				cin >> nombre;
				arch.close();
				arch.open("configuracionPEPS.dat",ios::binary|ios::app|ios::in);
				while(arch.read((char *)&aux,sizeof(GuardarParametros))){
					if(strcmp(aux.nombre,nombre)==0){
						param = aux;
						band = true;
						break;
					}
				}
				if(!band){
					cout << "\tNo se encontro la configuracion con ese nombre! Desea cancelar el proceso (y/n)? ";
					cin >> res;
					res = toupper(res);
					obj.configuracion.MemoriaMax = 0;
				}else{
					obj.configuracion = param.datos;
					switch (param.datos.MemoriaMax) {
						case 1:
							obj.prin.tam = 1024;
							obj.MemSelec = 1024 / TAMPART;
							break;
						case 4:
							obj.prin.tam = 4096;
							obj.MemSelec = 4096 / TAMPART;
							break;
						case 8:
							obj.prin.tam = 8192;
							obj.MemSelec = 8192 / TAMPART;
					}
					res = 'Y';
				}
			}while(res != 'Y');
			arch.close();
		}
		return in;	
	}

	istream &operator >> (istream &in, SJF &obj){
		char op, res;
		fstream arch;
		char nombre[TAMSTRING];
		bool band = false;
		GuardarParametros aux, param;
		cout << "\tQuiere cargar una configuracion previamente usada (y/n)? ";
		cin >> op;
		op = toupper(op);
		if(op!='Y'){
			in.exceptions(std::ios_base::failbit);
			do {
				cout << "\tIngrese el tamano de la memoria: 1/4/8 MB: ";
				insertar(obj.MemPart);
				switch (obj.MemPart) {
					case 1:
						obj.prin.tam = 1024;
						obj.MemSelec = 1024 / TAMPART;
						break;
					case 4:
						obj.prin.tam = 4096;
						obj.MemSelec = 4096 / TAMPART;
						break;
					case 8:
						obj.prin.tam = 8192;
						obj.MemSelec = 8192 / TAMPART;
						break;
					default: cout << "\tCantidad de memoria no disponible... \n \tIngrese una cantidad valida... " << endl; obj.Saltos(2500);
				}
				obj.configuracion.MemoriaMax = obj.MemPart;
			} while (obj.MemPart != 1 && obj.MemPart != 4 && obj.MemPart != 8);
			do{
				cout << "\tIngrese la cantidad de Cuantos maximo que hara cada proceso por ronda: ";//Cuanto tiempo puede ejecutarse un proceso por ciclo (timer)
				insertar(obj.configuracion.cuantosM);
				if(obj.configuracion.cuantosM <= 0){
					cout << "\t>Ingrese una cantidad mayor a cero<" << endl;
				}
			}while(obj.configuracion.cuantosM<=0);
			do{
				cout << "\tIngrese la cantidad de Cuantos maximo que se asignara a los procesos: ";//Cuanto tiempo necesita para completar su ejecucion
				insertar(obj.configuracion.cuantosProceso);
				if(obj.configuracion.cuantosProceso <= 0){
					cout << "\t>Ingrese una cantidad mayor a cero<" << endl;
				}
			}while(obj.configuracion.cuantosProceso<=0);
			do{
			cout << "\tIngrese la cantidad de memoria maxima que se le puede asignar a los procesos (en KiloBytes): ";//Cuanto es lo max. que puede medir el proceso
			insertar(obj.configuracion.MemoMaxProc);
			if(obj.configuracion.MemoMaxProc <= 0){
				cout << "\t>Ingrese una cantidad mayor a cero<" << endl;
			}
			}while(obj.configuracion.MemoMaxProc<=0);
			cout << "\tIngrese la cantidad de procesos que se desean generar: ";
			insertar(obj.configuracion.NumProceso);
			obj.Saltos(1000);
			cout << "\tDesea guardar la configuracion realizada (y/n)? ";
			cin >> res;
			res = toupper(res);
			if(res == 'Y'){
				cout << "\tNombre de la configuracion: ";
				cin >> aux.nombre;
				aux.datos = obj.configuracion;
				arch.open("configuracionSJF.dat",ios::binary|ios::app|ios::in);
				arch.write((char *)&aux,sizeof(GuardarParametros));
				arch.close();
			}
		}
		else{
			arch.open("configuracionSJF.dat",ios::binary|ios::app|ios::in);
			do{
				while(arch.read((char *)&aux,sizeof(GuardarParametros))){
					cout << "\n\n\tNombre: " << aux.nombre;
					cout << "\n\tMemoria asignada: " << aux.datos.MemoriaMax << " MB";
					cout << "\n\tCuantos maximos que se completan: " << aux.datos.cuantosProceso;
					cout << "\n\tCuantos maximos que se asignan: " << aux.datos.cuantosM;
					cout << "\n\tMemoria maxima que se asigna: " << aux.datos.MemoMaxProc;
					cout << "\n\tPrcesos que se generan: " << aux.datos.NumProceso;
				}
				cout << "\n\n\tNombre de la configuracion a cargar: ";
				cin >> nombre;
				arch.close();
				arch.open("configuracionSJF.dat",ios::binary|ios::app|ios::in);
				while(arch.read((char *)&aux,sizeof(GuardarParametros))){
					if(strcmp(aux.nombre,nombre)==0){
						param = aux;
						band = true;
						break;
					}
				}
				if(!band){
					cout << "\tNo se encontro la configuracion con ese nombre! Desea cancelar el proceso (y/n)? ";
					cin >> res;
					res = toupper(res);
					obj.configuracion.MemoriaMax = 0;
				}else{
					obj.configuracion = param.datos;
					switch (param.datos.MemoriaMax) {
						case 1:
							obj.prin.tam = 1024;
							obj.MemSelec = 1024 / TAMPART;
							break;
						case 4:
							obj.prin.tam = 4096;
							obj.MemSelec = 4096 / TAMPART;
							break;
						case 8:
							obj.prin.tam = 8192;
							obj.MemSelec = 8192 / TAMPART;
					}
					res = 'Y';
				}
			}while(res != 'Y');
			arch.close();
		}
		return in;	
	}
	istream &operator >> (istream &in, LTL &obj){
		char op, res;
		fstream arch;
		char nombre[TAMSTRING];
		bool band = false;
		GuardarParametros aux, param;
		cout << "\tQuiere cargar una configuracion previamente usada (y/n)? ";
		cin >> op;
		op = toupper(op);
		if(op!='Y'){
			in.exceptions(std::ios_base::failbit);
			do {
				cout << "\tIngrese el tamano de la memoria: 1/4/8 MB: ";
				insertar(obj.MemPart);
				switch (obj.MemPart) {
					case 1:
						obj.prin.tam = 1024;
						obj.MemSelec = 1024 / TAMPART;
						break;
					case 4:
						obj.prin.tam = 4096;
						obj.MemSelec = 4096 / TAMPART;
						break;
					case 8:
						obj.prin.tam = 8192;
						obj.MemSelec = 8192 / TAMPART;
						break;
					default: cout << "\tCantidad de memoria no disponible... \n \tIngrese una cantidad valida... " << endl; obj.Saltos(2500);
				}
				obj.configuracion.MemoriaMax = obj.MemPart;
			} while (obj.MemPart != 1 && obj.MemPart != 4 && obj.MemPart != 8);
			do{
				cout << "\tIngrese la cantidad de Cuantos maximo que hara cada proceso por ronda: ";//Cuanto tiempo puede ejecutarse un proceso por ciclo (timer)
				insertar(obj.configuracion.cuantosM);
				if(obj.configuracion.cuantosM <= 0){
					cout << "\t>Ingrese una cantidad mayor a cero<" << endl;
				}
			}while(obj.configuracion.cuantosM<=0);
			do{
				cout << "\tIngrese la cantidad de Cuantos maximo que se asignara a los procesos: ";//Cuanto tiempo necesita para completar su ejecucion
				insertar(obj.configuracion.cuantosProceso);
				if(obj.configuracion.cuantosProceso <= 0){
					cout << "\t>Ingrese una cantidad mayor a cero<" << endl;
				}
			}while(obj.configuracion.cuantosProceso<=0);
			do{
			cout << "\tIngrese la cantidad de memoria maxima que se le puede asignar a los procesos (en KiloBytes): ";//Cuanto es lo max. que puede medir el proceso
			insertar(obj.configuracion.MemoMaxProc);
			if(obj.configuracion.MemoMaxProc <= 0){
				cout << "\t>Ingrese una cantidad mayor a cero<" << endl;
			}
			}while(obj.configuracion.MemoMaxProc<=0);
			cout << "\tIngrese la cantidad de procesos que se desean generar: ";
			insertar(obj.configuracion.NumProceso);
			obj.Saltos(1000);
			cout << "\tDesea guardar la configuracion realizada (y/n)? ";
			cin >> res;
			res = toupper(res);
			if(res == 'Y'){
				cout << "\tNombre de la configuracion: ";
				cin >> aux.nombre;
				aux.datos = obj.configuracion;
				arch.open("configuracionLTL.dat",ios::binary|ios::app|ios::in);
				arch.write((char *)&aux,sizeof(GuardarParametros));
				arch.close();
			}
		}
		else{
			arch.open("configuracionLTL.dat",ios::binary|ios::app|ios::in);
			do{
				while(arch.read((char *)&aux,sizeof(GuardarParametros))){
					cout << "\n\n\tNombre: " << aux.nombre;
					cout << "\n\tMemoria asignada: " << aux.datos.MemoriaMax << " MB";
					cout << "\n\tCuantos maximos que se completan: " << aux.datos.cuantosProceso;
					cout << "\n\tCuantos maximos que se asignan: " << aux.datos.cuantosM;
					cout << "\n\tMemoria maxima que se asigna: " << aux.datos.MemoMaxProc;
					cout << "\n\tPrcesos que se generan: " << aux.datos.NumProceso;
				}
				cout << "\n\n\tNombre de la configuracion a cargar: ";
				cin >> nombre;
				arch.close();
				arch.open("configuracionLTL.dat",ios::binary|ios::app|ios::in);
				while(arch.read((char *)&aux,sizeof(GuardarParametros))){
					if(strcmp(aux.nombre,nombre)==0){
						param = aux;
						band = true;
						break;
					}
				}
				if(!band){
					cout << "\tNo se encontro la configuracion con ese nombre! Desea cancelar el proceso (y/n)? ";
					cin >> res;
					res = toupper(res);
					obj.configuracion.MemoriaMax = 0;
				}else{
					obj.configuracion = param.datos;
					switch (param.datos.MemoriaMax) {
						case 1:
							obj.prin.tam = 1024;
							obj.MemSelec = 1024 / TAMPART;
							break;
						case 4:
							obj.prin.tam = 4096;
							obj.MemSelec = 4096 / TAMPART;
							break;
						case 8:
							obj.prin.tam = 8192;
							obj.MemSelec = 8192 / TAMPART;
					}
				}
				res = 'Y';
			}while(res != 'Y');
			arch.close();
		}
		return in;	
	}

//Templates

	//Menu Ordenaciones
	template<class F>
	void menuReporte(F &obj){
		char op;
		do {
			system("cls");
			cout << "\n\n";
			printf("\033[1;35m");
			printf("\t%c",201);
			for(int i=0;i<46;i++)printf("%c",205);
			printf("%c\n",187);
			printf("\t%c",186);

			cout << "                                              ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);

			cout << "                 ORDENACIONES                 ";

			printf("%c\n",186);
			printf("\t%c",186);

			cout << "             Ordenar reporte por:             ";

			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "                                              ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "            Ciclos promedio........1          ";
			printf("\033[1;35m");		
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "            Tiempo promedio........2          ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "            Procesos realizados....3          ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "            Procesos esperaron.....4          ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "            Salir..................0          ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "                                              ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "                                              ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",200);
			for(int i=0;i<46;i++)printf("%c",205);
			printf("%c\n",188);
			printf("\033[0m");
			cout << "\t Eliga una opcion: \t";
			fflush(stdin);
			cin >> op;
			switch (op) {
				case '0': 
					break;
				case '1':			
					obj.ReporteCiclos();
					cout << "\tReporte Generado exitosamente..." << endl;
					break;
				case '2':
					obj.ReporteTiempo();
					cout << "\tReporte Generado exitosamente..." << endl;
					break;
				case '3':
					obj.ReporteIniciados();
					cout << "\tReporte Generado exitosamente..." << endl;
					break;
				case '4':
					obj.ReporteEspera();
					cout << "\tReporte Generado exitosamente..." << endl;
					break;
				default: 
					cout << "\n\t >Ingrese una opcion valida<";
					mostrarLomito();
			}
			cout << "\t";
			system("pause");
		} while(op!='0');
	}

	template <class S>
	void Iniciador(S &Plan){
		cin >> Plan;
		if(Plan.configuracion.MemoriaMax != 0){
			Plan.inicia();
			cout << Plan;
		}else{
			cout << "\n\tSe cancelo el procedimiento";
		}
	} 
	
//MAIN y Menus

	int main(){
		MenuLogin();
		//MenuBusquedas();
	//	MenuOrdenaciones();
	}

	void MenuPlanificadores() {
		//setlocale(LC_CTYPE, "Spanish");
		cin.exceptions(std::ios_base::failbit);
		int op;
		RoundRobin PlanRound, PlanRRVacio;
		PEPS PlanPEPS, PEPSVacio;
		SJF PlanSJF, SJFVacio;
		LTL PlanLTL, LTLVacio;
		GenerarArch();
		do {
			system("cls");
			cout << "\n\n";
			printf("\033[1;35m");
			printf("\t%c",201);
			for(int i=0;i<46;i++)printf("%c",205);
			printf("%c\n",187);
			printf("\t%c",186);
			printf("\033[0m"); 
			cout << "                                              ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m"); 
			cout << "                     MENU                     ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m"); 
			cout << "                Planificadores                ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m"); 
			cout << "                                              ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m"); 
			cout << "            Round Robin..........1            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m"); 
			cout << "            PEPS.................2            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m"); 
			cout << "            SJF..................3            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m"); 
			cout << "            LTL..................4            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m"); 
			cout << "            Generar Reporte......5            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m"); 
			cout << "            Buscar resultados....6            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "            Salir................0            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m"); 
			cout << "                                              ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m"); 
			cout << "                                              ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",200);
			for(int i=0;i<46;i++)printf("%c",205);
			printf("%c\n",188);
			printf("\033[0m"); 
			cout << "\t Eliga una opcion: \t";
			fflush(stdin);
			insertar(op);
			switch (op) {
				case 0:
					 cout << "\n\t Finalizando programa...\n";
					 break;
				case 1:
					PlanRound = PlanRRVacio;
					cout << "\n\t\t....BIENVENIDO A ROUND ROBIN...." << endl;
					PlanRound.Saltos(1500);
					Iniciador(PlanRound);
					break;
				case 2:
					PlanPEPS = PEPSVacio;
					cout << "\n\t\t....BIENVENIDO A PEPS...." << endl;
					PlanRound.Saltos(1500);
					Iniciador(PlanPEPS);
					break;
				case 3:
					PlanSJF = SJFVacio;
					cout << "\n\t\t....BIENVENIDO A SJF...." << endl;
					PlanSJF.Saltos(1500);
					Iniciador(PlanSJF);
					break;
				case 4:
					PlanLTL = LTLVacio;
					cout << "\n\t\t....BIENVENIDO A LTL...." << endl;
					PlanLTL.Saltos(1500);
					Iniciador(PlanLTL);
					break;
				case 5:
					MenuOrdenaciones();
					break;
				case 6:
					MenuBusquedas();
					break;
				case 2412:
					Grinch(); 
					break;
				case 666:
					Pentagrama();
					break;
				default: 
					cout << "\n\t >Ingrese una opcion valida<";
					mostrarLomito();
			}
			cout << "\n\n\t Presione una tecla para continuar...";
			fflush(stdin);
			cin.get();
			system("cls");
		} while (op != 0);
	}

	void MenuLogin(){
		IngreUsu Log;
		int aux;
		char op;
		fstream arch;
		arch.open("Invitados.txt",ios::app|ios::in|ios::out);
		do{
			system("cls");
			cout << "\n\n";
			printf("\033[1;35m");
			printf("\t%c",201);
			for(int i=0;i<46;i++)printf("%c",205);
			printf("%c\n",187);
			printf("\t%c",186);
			printf("\033[0m"); 
			cout << "                                              ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m"); 
			cout << "            Seleccione una opcion             ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m"); 
			cout << "                   Usuarios                   ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m"); 
			cout << "                                              ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m"); 	
			cout << "            Iniciar sesion.......1            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m"); 
			cout << "            Iniciar invitado.....2            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m"); 
			cout << "            Registrarse..........3            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m"); 
			cout << "            Verificar Usuarios...4            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m"); 
			cout << "            Verificar invitados..5            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m"); 
			cout << "            Salir................0            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m"); 
			cout << "                                              ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m"); 
			cout << "                                              ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",200);
			for(int i=0;i<46;i++)printf("%c",205);
			printf("%c\n",188);
			printf("\033[0m"); 
			cout << "\t Eliga una opcion: \t";
			cin >> op;
			switch(op){
				case '1': 
						cout << "\n\tIngrese su usuario: ";
						cin >> Log.Usuario;
						cout << "\tIngrese su contrasena: ";
						cin >> Log.Contrasena;
						if(Verificar(Log)){
							cout << "\tAccediendo" << endl;
							Sleep(1000);
							MenuPlanificadores();
						}
						else{
							cout << "\tIntente de nuevo..." << endl;
						}

						break;
				case '2': 
						cout << "\tIngresando como invitado" << endl;
						IniciarInvitado();
						cout << "\tBienvenido..." << endl;
						MenuPlanificadores();
						Sleep(500);
						//guardamos contador para registar que se guarde la cantidad de usuarios que han entrado en modo invitados
						//iniciamos el simulador
						break;
				case '3': 
						cout << "\tIngrese su usuario: ";
						cin >> Log.Usuario;
						cout << "\tIngrese su contrasena: ";
						cin >> Log.Contrasena;
						Registrar(Log);
						break;
				case '4':
					UsuariosReg(); 
					system("pause");
					break;
				case '5':
					VerificarInvitados();
					system("pause");
					break;
				case '0': cout << "\tSaliendo del programa..."; break;
				default: 
					cout << " \tIngrese una opcion valida...\n";
					mostrarLomito();
					cout << "\t";
					system("pause");
					break;
			}
		}while(op!='0');
		arch.close();
	}

	void MenuBusquedas(){
		//setlocale(LC_CTYPE, "Spanish");
		char op;
		char nombreArch[25];
		Planificadores aux;
		do {
			system("cls");
			cout << "\n\n";
			printf("\033[1;35m");
			printf("\t%c",201);
			for(int i=0;i<46;i++)printf("%c",205);
			printf("%c\n",187);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "                                              ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "                     MENU                     ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "                  Busquedas                   ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "                                              ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "            Round Robin..........1            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "            PEPS.................2            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "            SJF..................3            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "            LTL..................4            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "            Busqueda general.....5            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "            Salir................0            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "                                              ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "                                              ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",200);
			for(int i=0;i<46;i++)printf("%c",205);
			printf("%c\n",188);
			printf("\033[0m");
			cout << "\t Eliga una opcion: \t";
			fflush(stdin);
			cin >> op;
			switch (op) {
				case '0':
					 cout << "\n\t Saliendo de las busquedas...\n";
					 break;
				case '1':
					cout << "\n\t\t....BIENVENIDO A ROUND ROBIN...." << endl;
					strcpy(nombreArch,"RoundRobinBinResultados");
					strcat(nombreArch,".dat");
					MenuBusquedaEspecifico(nombreArch);
					break;
				case '2':
					cout << "\n\t\t....BIENVENIDO A PEPS...." << endl;
					strcpy(nombreArch,"PEPSbinResultados");
					strcat(nombreArch,".dat");
					MenuBusquedaEspecifico(nombreArch);
					break;
				case '3':
					cout << "\n\t\t....BIENVENIDO A SJF...." << endl;
					strcpy(nombreArch,"SJFbinResultados");
					strcat(nombreArch,".dat");
					MenuBusquedaEspecifico(nombreArch);
					break;
				case '4':
					cout << "\n\t\t....BIENVENIDO A LTL...." << endl;
					strcpy(nombreArch,"LTLbinResultados");
					strcat(nombreArch,".dat");
					MenuBusquedaEspecifico(nombreArch);
					break;
				case '5':
					cout << "\n\t\t....BIENVENIDO A BUSQUEDA GENERAL...." << endl;
					strcpy(nombreArch,"GeneralResultados");
					strcat(nombreArch,".dat");
					MenuBusquedaEspecifico(nombreArch);
					break;
				default: 
					cout << "\n\t >Ingrese una opcion valida<";
					mostrarLomito();
					cout << "\t";
					system("pause");
			}
		} while (op != '0');
	}

	void MenuOrdenaciones(){
		//setlocale(LC_CTYPE, "Spanish");
		char op;
		char nombreArch[25];
		RoundRobin RR;
		PEPS PP;
		LTL LL;
		SJF SJ;
		Planificadores aux;

		do {
			system("cls");
			cout << "\n\n";
			printf("\033[1;35m");
			printf("\t%c",201);
			for(int i=0;i<46;i++)printf("%c",205);
			printf("%c\n",187);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "                                              ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "                     MENU                     ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "                 Ordenaciones                 ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "                                              ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "            Round Robin..........1            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "            PEPS.................2            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "            SJF..................3            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "            LTL..................4            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "            Ordenacion general...5            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "            Salir................0            ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "                                              ";
			printf("\033[0m");
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",186);
			printf("\033[0m");
			cout << "                                              ";
			printf("\033[1;35m");
			printf("%c\n",186);
			printf("\t%c",200);
			for(int i=0;i<46;i++)printf("%c",205);
			printf("%c\n",188);
			printf("\033[0m");
			cout << "\t Eliga una opcion: \t";
			fflush(stdin);
			cin >> op;
			switch (op) {
				case '0':
					 break;
				case '1':
					system("cls");
					menuReporte(RR);
					break;
				case '2':
					system("cls");
					menuReporte(PP);
					break;
				case '3':
					system("cls");
					menuReporte(SJ);
					break;
				case '4':
					system("cls");
					menuReporte(LL);
					break;
				case '5':
					system("cls");
					menuReporte(aux);
					break;
				default: 
					cout << "\n\t >Ingrese una opcion valida<";
					mostrarLomito();
					cout << "\t";
					system("pause");
			}
		} while (op != '0');
	}
	
//Funciones de busqueda	

	void MenuBusquedaEspecifico(char Nombre[]){
		cin.exceptions(std::ios_base::failbit);
		char op;
		int busq;
		do {
				system("cls");
				cout << "\n\n";
				printf("\033[1;35m");
				printf("\t%c",201);
				for(int i=0;i<46;i++)printf("%c",205);
				printf("%c\n",187);
				printf("\t%c",186);
				printf("\033[0m");
				cout << "                                              ";
				printf("\033[1;35m");
				printf("%c\n",186);
				printf("\t%c",186);
				printf("\033[0m");
				cout << "               Que desea buscar               ";
				printf("\033[1;35m");
				printf("%c\n",186);
				printf("\t%c",186);
				printf("\033[0m");
				cout << "                                              ";
				printf("\033[1;35m");
				printf("%c\n",186);
				printf("\t%c",186);
				printf("\033[0m");
				cout << "        Promedio de ciclos.......1            ";
				printf("\033[1;35m");
				printf("%c\n",186);
				printf("\t%c",186);
				printf("\033[0m");
				cout << "        Promedio de tiempo.......2            ";
				printf("\033[1;35m");
				printf("%c\n",186);
				printf("\t%c",186);
				printf("\033[0m");
				cout << "        Procesos Esperaron.......3            ";
				printf("\033[1;35m");
				printf("%c\n",186);
				printf("\t%c",186);
				printf("\033[0m");
				cout << "        Procesos realizados......4            ";
				printf("\033[1;35m");
				printf("%c\n",186);
				printf("\t%c",186);
				printf("\033[0m");
				cout << "        Salir....................0            ";
				printf("\033[1;35m");
				printf("%c\n",186);
				printf("\t%c",186);
				printf("\033[0m");
				cout << "                                              ";
				printf("\033[1;35m");
				printf("%c\n",186);
				printf("\t%c",186);
				printf("\033[0m");
				cout << "                                              ";
				printf("\033[1;35m");
				printf("%c\n",186);
				printf("\t%c",200);
				for(int i=0;i<46;i++)printf("%c",205);
				printf("%c\n",188);
				printf("\033[0m");
				cout << "\t Eliga una opcion: \t";
				fflush(stdin);
				cin >> op;
				switch (op) {
					case '0':
						break;
					case '1':
						system("cls");
						cout << "\n\t\t....Buscando por promedio de ciclos...." << endl;
						cout << "\tValor que desea buscar: ";
						insertar(busq);
						BusquedaCiclos(Nombre, busq);
						break;
					case '2':
						system("cls");
						cout << "\n\t\t....Buscando por promedio de tiempo...." << endl;
						cout << "\tValor que desea buscar: ";
						insertar(busq);
						BusquedaTiempo(Nombre, busq);
						break;
					case '3':
						system("cls");
						cout << "\n\t\t....Buscando por procesos que esperaron...." << endl;
						cout << "\tValor que desea buscar: ";
						insertar(busq);
						BusquedaTerminados(Nombre, busq);
						break;
					case '4':
						system("cls");
						cout << "\n\t\t....Buscando por procesos realizados...." << endl;
						cout << "\tValor que desea buscar: ";
						insertar(busq);
						BusquedaGenerados(Nombre, busq);
						break;
					default: 
						cout << "\n\t >Ingrese una opcion valida<";
						mostrarLomito();
				}
				cout << "\n\n\t Presione una tecla para continuar...";
				fflush(stdin);
				cin.get();
				system("cls");
			} while (op != '0');
	}

	void BusquedaCiclos(char Nombre[], int valor){
		fstream arch;
		Resultados aux;
		int cont=0;
		arch.open(Nombre,ios::binary|ios::in);
		while(arch.read((char*)&aux,sizeof(Resultados))){
			if(valor==aux.ciclos){
				cout << "\t*************************************************\n";
				cout << "\t*                                               *\n";
				cout << "\t*\t\t\t"<< aux.nombrePlan ;
				if(strlen(aux.nombrePlan)<6) cout<< "\t";
				cout << "\t\t*\n";
				cout << "\t*         Estadisticas de la simulacion         *\n";
				cout << "\t*                                               *\n";
                cout << "\t*    Ciclos promedio en finalizar: " << aux.ciclos << " ciclos" << "\t*\n";
				cout << "\t*    Cantidad de procesos iniciados: " << aux.idM << "\t\t*\n";
				cout << "\t*    Procesos que esperaron para entrar: " << aux.contTermProceso << "\t*\n";
				cout << "\t*    Tiempo promedio de atencion: " << setprecision(4) << aux.sumTiempo << "\t\t*\n";
				cout << "\t*                                               *\n";
				cout << "\t*                                               *\n";
				cout << "\t*************************************************\n\n";
				cont++;
			}
		}
		cout << endl << "\tResultados encontrados: " << cont << endl;
		arch.close();
	}

	void BusquedaTerminados(char Nombre[], int valor){
		fstream arch;
		Resultados aux;
		int cont=0;
		arch.open(Nombre,ios::binary|ios::in);
		while(arch.read((char*)&aux,sizeof(Resultados))){
			if(valor==aux.contTermProceso){
			cout << "\t*************************************************\n";
			cout << "\t*                                               *\n";
			cout << "\t*\t\t\t"<< aux.nombrePlan ;
			if(strlen(aux.nombrePlan)<6) cout<< "\t";
			cout << "\t\t*\n";
			cout << "\t*         Estadisticas de la simulacion         *\n";
			cout << "\t*                                               *\n";
			cout << "\t*    Ciclos promedio en finalizar: " << aux.ciclos << " ciclos" << "\t*\n";
			cout << "\t*    Cantidad de procesos iniciados: " << aux.idM << "\t\t*\n";
			cout << "\t*    Procesos que esperaron para entrar: " << aux.contTermProceso << "\t*\n";
			cout << "\t*    Tiempo promedio de atencion: " << setprecision(4) << aux.sumTiempo << "\t\t*\n";
			cout << "\t*                                               *\n";
			cout << "\t*                                               *\n";
			cout << "\t*************************************************\n\n";
				cont++;
			}
		}
		cout << endl << "\tResultados encontrados: " << cont << endl;
		arch.close();
	}

	void BusquedaGenerados(char Nombre[], int valor){
		fstream arch;
		Resultados aux;
		int cont=0;
		arch.open(Nombre,ios::binary|ios::in);
		while(arch.read((char*)&aux,sizeof(Resultados))){
			if(valor==aux.idM){
			cout << "\t*************************************************\n";
			cout << "\t*                                               *\n";
			cout << "\t*\t\t\t"<< aux.nombrePlan ;
			if(strlen(aux.nombrePlan)<6) cout<< "\t";
			cout << "\t\t*\n";
			cout << "\t*         Estadisticas de la simulacion         *\n";
			cout << "\t*                                               *\n";
			cout << "\t*    Ciclos promedio en finalizar: " << aux.ciclos << " ciclos" << "\t*\n";
			cout << "\t*    Cantidad de procesos iniciados: " << aux.idM << "\t\t*\n";
			cout << "\t*    Procesos que esperaron para entrar: " << aux.contTermProceso << "\t*\n";
			cout << "\t*    Tiempo promedio de atencion: " << setprecision(4) << aux.sumTiempo << "\t\t*\n";
			cout << "\t*                                               *\n";
			cout << "\t*                                               *\n";
			cout << "\t*************************************************\n\n";
				cont++;
			}
		}
		cout << endl << "\tResultados encontrados: " << cont << endl;
		arch.close();
	}

	void BusquedaTiempo(char Nombre[], int valor){
		fstream arch;
		Resultados aux;
		int cont=0;
		arch.open(Nombre,ios::binary|ios::in);
		while(arch.read((char*)&aux,sizeof(Resultados))){
			if(valor==aux.sumTiempo){
			cout << "\t*************************************************\n";
			cout << "\t*                                               *\n";
			cout << "\t*\t\t\t"<< aux.nombrePlan ;
			if(strlen(aux.nombrePlan)<6) cout<< "\t";
			cout << "\t\t*\n";
			cout << "\t*         Estadisticas de la simulacion         *\n";
			cout << "\t*                                               *\n";
			cout << "\t*    Ciclos promedio en finalizar: " << aux.ciclos << " ciclos" << "\t*\n";
			cout << "\t*    Cantidad de procesos iniciados: " << aux.idM << "\t\t*\n";
			cout << "\t*    Procesos que esperaron para entrar: " << aux.contTermProceso << "\t*\n";
			cout << "\t*    Tiempo promedio de atencion: " << setprecision(4) << aux.sumTiempo << "\t\t*\n";
			cout << "\t*                                               *\n";
			cout << "\t*                                               *\n";
			cout << "\t*************************************************\n\n";
				cont++;
			}
		}
		cout << endl << "\tResultados encontrados: " << cont << endl;
		arch.close();
	}

//Funciones Ordenaciones
	//Ordenaciones generales

		void Planificadores::ReporteCiclos(){
			fstream archin, archout;
			char archNombre[TAMSTRING];
			Resultados agregar, aux, anterior;
			int cont = 0, cant = 0, obtenido = 0, almacenado = 0;
			int *respaldo = NULL, *listos = NULL;
			bool band = false;
			cout << "\n\tNombre del archivo que desea: ";
			cin >> archNombre;
			strcat(archNombre,".txt");
			archout.open(archNombre,ios::app|ios::in|ios::out);
			archin.open("GeneralResultados.dat",ios::binary|ios::in|ios::out);
			while(archin.read((char *)&aux,sizeof(Resultados))) cont++; //Cuenta la cantidad de datos que hay en el archivo binario
			while(cant <= cont){ //Mientras no se inserten todos los datos
				archin.close();
				archin.open("GeneralResultados.dat",ios::binary|ios::in|ios::out); //Reinicia el archivo
				obtenido = 1;
				almacenado = 0;
				agregar.ciclos = 50000; //Se asigna un valor muy grande
				cant++;
				while(archin.read((char *)&aux,sizeof(Resultados))){ //Lee todo el archivo
					obtenido++; //Se aumenta para saber que numero de registro se esta guardando
					if(aux.ciclos < agregar.ciclos){ //Obtiene el que hizo menos ciclos
						for(int i=0; i<cant; i++){
							if(listos!=NULL){
								if(obtenido == listos[i]){ //Si el más chico ya esta en el archivo
									band = true; //Enciende la bandera que ese no lo debe guardar
								}
							}
						}
						if(!band){ //Si la bandera es falsa, guarda el nuevo elemento 
							anterior = agregar;
							agregar = aux;
							almacenado = obtenido;
						}
						band = false; //Reinicia la bandera en falso
					}
				}
				respaldo = listos;
				listos = new int[cant];
				if(respaldo!=NULL){
					for(int i=0; i<cant-1;i++){
						listos[i] = respaldo[i];
					}
					delete respaldo;
				}
				listos[cant-1] = almacenado;
				if(agregar.ciclos != anterior.ciclos){
					if(strlen(agregar.nombrePlan)<6){
						strcat(agregar.nombrePlan,"\t");
					}
					archout << "\t*************************************************\n";
					archout << "\t*                                               *\n";
					archout << "\t*\t\t"<< agregar.nombrePlan <<"\t\t\t*\n";
					archout << "\t*         Estadisticas de la simulacion         *\n";
					archout << "\t*                                               *\n";
					archout << "\t*    Ciclos promedio en finalizar: " << agregar.ciclos << " ciclos" << "\t*\n";
					archout << "\t*    Cantidad de procesos iniciados: " << agregar.idM << "\t\t*\n";
					archout << "\t*    Procesos que esperaron para entrar: " << agregar.contTermProceso << "\t*\n";
					archout << "\t*    Tiempo promedio de atencion: " << setprecision(4) << agregar.sumTiempo << "\t\t*\n";
					archout << "\t*                                               *\n";
					archout << "\t*                                               *\n";
					archout << "\t*************************************************\n\n";
				}
			}
			archin.close();
			archout.close();
		}

		void Planificadores::ReporteTiempo(){
			fstream archin, archout;
			char archNombre[TAMSTRING];
			Resultados agregar, aux, anterior;
			int cont = 0, cant = 0, obtenido = 0, almacenado = 0;
			int *respaldo = NULL, *listos = NULL;
			bool band = false;
			cout << "\n\tNombre del archivo que desea: ";
			cin >> archNombre;
			strcat(archNombre,".txt");
			archout.open(archNombre,ios::app|ios::in|ios::out);
			archin.open("GeneralResultados.dat",ios::binary|ios::in|ios::out);
			while(archin.read((char *)&aux,sizeof(Resultados))) cont++; //Cuenta la cantidad de datos que hay en el archivo binario
			while(cant <= cont){ //Mientras no se inserten todos los datos
				archin.close();
				archin.open("GeneralResultados.dat",ios::binary|ios::in|ios::out); //Reinicia el archivo
				obtenido = 1;
				almacenado = 0;
				agregar.sumTiempo = 50000; //Se asigna un valor muy grande
				cant++;
				while(archin.read((char *)&aux,sizeof(Resultados))){ //Lee todo el archivo
					obtenido++; //Se aumenta para saber que numero de registro se esta guardando
					if(aux.sumTiempo < agregar.sumTiempo){ //Obtiene el que hizo menos ciclos
						for(int i=0; i<cant; i++){
							if(listos!=NULL){
								if(obtenido == listos[i]){ //Si el más chico ya esta en el archivo
									band = true; //Enciende la bandera que ese no lo debe guardar
								}
							}
						}
						if(!band){ //Si la bandera es falsa, guarda el nuevo elemento 
							anterior = agregar;
							agregar = aux;
							almacenado = obtenido;
						}
						band = false; //Reinicia la bandera en falso
					}
				}
				respaldo = listos;
				listos = new int[cant];
				if(respaldo!=NULL){
					for(int i=0; i<cant-1;i++){
						listos[i] = respaldo[i];
					}
					delete respaldo;
				}
				listos[cant-1] = almacenado;
				if(agregar.sumTiempo != anterior.sumTiempo){
					if(strlen(agregar.nombrePlan)<6){
						strcat(agregar.nombrePlan,"\t");
					}
					archout << "\t*************************************************\n";
					archout << "\t*                                               *\n";
					archout << "\t*\t\t"<< agregar.nombrePlan <<"\t\t\t*\n";
					archout << "\t*         Estadisticas de la simulacion         *\n";
					archout << "\t*                                               *\n";
					archout << "\t*    Ciclos promedio en finalizar: " << agregar.ciclos << " ciclos" << "\t*\n";
					archout << "\t*    Cantidad de procesos iniciados: " << agregar.idM << "\t\t*\n";
					archout << "\t*    Procesos que esperaron para entrar: " << agregar.contTermProceso << "\t*\n";
					archout << "\t*    Tiempo promedio de atencion: " << setprecision(4) << agregar.sumTiempo << "\t\t*\n";
					archout << "\t*                                               *\n";
					archout << "\t*                                               *\n";
					archout << "\t*************************************************\n\n";
				}
			}
			archin.close();
			archout.close();
		}

		void Planificadores::ReporteIniciados(){
			fstream archin, archout;
			char archNombre[TAMSTRING];
			Resultados agregar, aux, anterior;
			int cont = 0, cant = 0, obtenido = 0, almacenado = 0;
			int *respaldo = NULL, *listos = NULL;
			bool band = false;
			cout << "\n\tNombre del archivo que desea: ";
			cin >> archNombre;
			strcat(archNombre,".txt");
			archout.open(archNombre,ios::app|ios::in|ios::out);
			archin.open("GeneralResultados.dat",ios::binary|ios::in|ios::out);
			while(archin.read((char *)&aux,sizeof(Resultados))) cont++; //Cuenta la cantidad de datos que hay en el archivo binario
			while(cant <= cont){ //Mientras no se inserten todos los datos
				archin.close();
				archin.open("GeneralResultados.dat",ios::binary|ios::in|ios::out); //Reinicia el archivo
				obtenido = 1;
				almacenado = 0;
				agregar.idM = 50000; //Se asigna un valor muy grande
				cant++;
				while(archin.read((char *)&aux,sizeof(Resultados))){ //Lee todo el archivo
					obtenido++; //Se aumenta para saber que numero de registro se esta guardando
					if(aux.idM < agregar.idM){ //Obtiene el que hizo menos ciclos
						for(int i=0; i<cant; i++){
							if(listos!=NULL){
								if(obtenido == listos[i]){ //Si el más chico ya esta en el archivo
									band = true; //Enciende la bandera que ese no lo debe guardar
								}
							}
						}
						if(!band){ //Si la bandera es falsa, guarda el nuevo elemento 
							anterior = agregar;
							agregar = aux;
							almacenado = obtenido;
						}
						band = false; //Reinicia la bandera en falso
					}
				}
				respaldo = listos;
				listos = new int[cant];
				if(respaldo!=NULL){
					for(int i=0; i<cant-1;i++){
						listos[i] = respaldo[i];
					}
					delete respaldo;
				}
				listos[cant-1] = almacenado;
				if(agregar.idM != anterior.idM){
					if(strlen(agregar.nombrePlan)<6){
						strcat(agregar.nombrePlan,"\t");
					}
					archout << "\t*************************************************\n";
					archout << "\t*                                               *\n";
					archout << "\t*\t\t"<< agregar.nombrePlan <<"\t\t\t*\n";
					archout << "\t*         Estadisticas de la simulacion         *\n";
					archout << "\t*                                               *\n";
					archout << "\t*    Ciclos promedio en finalizar: " << agregar.ciclos << " ciclos" << "\t*\n";
					archout << "\t*    Cantidad de procesos iniciados: " << agregar.idM << "\t\t*\n";
					archout << "\t*    Procesos que esperaron para entrar: " << agregar.contTermProceso << "\t*\n";
					archout << "\t*    Tiempo promedio de atencion: " << setprecision(4) << agregar.sumTiempo << "\t\t*\n";
					archout << "\t*                                               *\n";
					archout << "\t*                                               *\n";
					archout << "\t*************************************************\n\n";
				}
			}
			archin.close();
			archout.close();
		}

		void Planificadores::ReporteEspera(){
			fstream archin, archout;
			char archNombre[TAMSTRING];
			Resultados agregar, aux, anterior;
			int cont = 0, cant = 0, obtenido = 0, almacenado = 0;
			int *respaldo = NULL, *listos = NULL;
			bool band = false;
			cout << "\n\tNombre del archivo que desea: ";
			cin >> archNombre;
			strcat(archNombre,".txt");
			archout.open(archNombre,ios::app|ios::in|ios::out);
			archin.open("GeneralResultados.dat",ios::binary|ios::in|ios::out);
			while(archin.read((char *)&aux,sizeof(Resultados))) cont++; //Cuenta la cantidad de datos que hay en el archivo binario
			while(cant <= cont){ //Mientras no se inserten todos los datos
				archin.close();
				archin.open("GeneralResultados.dat",ios::binary|ios::in|ios::out); //Reinicia el archivo
				obtenido = 1;
				almacenado = 0;
				agregar.contTermProceso = 50000; //Se asigna un valor muy grande
				cant++;
				while(archin.read((char *)&aux,sizeof(Resultados))){ //Lee todo el archivo
					obtenido++; //Se aumenta para saber que numero de registro se esta guardando
					if(aux.contTermProceso < agregar.contTermProceso){ //Obtiene el que hizo menos ciclos
						for(int i=0; i<cant; i++){
							if(listos!=NULL){
								if(obtenido == listos[i]){ //Si el más chico ya esta en el archivo
									band = true; //Enciende la bandera que ese no lo debe guardar
								}
							}
						}
						if(!band){ //Si la bandera es falsa, guarda el nuevo elemento 
							anterior = agregar;
							agregar = aux;
							almacenado = obtenido;
						}
						band = false; //Reinicia la bandera en falso
					}
				}
				respaldo = listos;
				listos = new int[cant];
				if(respaldo!=NULL){
					for(int i=0; i<cant-1;i++){
						listos[i] = respaldo[i];
					}
					delete respaldo;
				}
				listos[cant-1] = almacenado;
				if(agregar.contTermProceso != anterior.contTermProceso){
					if(strlen(agregar.nombrePlan)<6){
						strcat(agregar.nombrePlan,"\t");
					}
					archout << "\t*************************************************\n";
					archout << "\t*                                               *\n";
					archout << "\t*\t\t"<< agregar.nombrePlan <<"\t\t\t*\n";
					archout << "\t*         Estadisticas de la simulacion         *\n";
					archout << "\t*                                               *\n";
					archout << "\t*    Ciclos promedio en finalizar: " << agregar.ciclos << " ciclos" << "\t*\n";
					archout << "\t*    Cantidad de procesos iniciados: " << agregar.idM << "\t\t*\n";
					archout << "\t*    Procesos que esperaron para entrar: " << agregar.contTermProceso << "\t*\n";
					archout << "\t*    Tiempo promedio de atencion: " << setprecision(4) << agregar.sumTiempo << "\t\t*\n";
					archout << "\t*                                               *\n";
					archout << "\t*                                               *\n";
					archout << "\t*************************************************\n\n";
				}
			}
			archin.close();
			archout.close();
		}

	//Ordenaciones RoundRobin

		void RoundRobin::ReporteCiclos(){
			fstream archin, archout;
			char archNombre[TAMSTRING];
			Resultados agregar, aux, anterior;
			int cont = 0, cant = 0, obtenido = 0, almacenado = 0;
			int *respaldo = NULL, *listos = NULL;
			bool band = false;
			cout << "\n\tNombre del archivo que desea: ";
			cin >> archNombre;
			strcat(archNombre,".txt");
			archout.open(archNombre,ios::app|ios::in|ios::out);
			archin.open("RoundRobinBinResultados.dat",ios::binary|ios::in|ios::out);
			while(archin.read((char *)&aux,sizeof(Resultados))) cont++; //Cuenta la cantidad de datos que hay en el archivo binario
			while(cant <= cont){ //Mientras no se inserten todos los datos
				archin.close();
				archin.open("RoundRobinBinResultados.dat",ios::binary|ios::in|ios::out); //Reinicia el archivo
				obtenido = 1;
				almacenado = 0;
				agregar.ciclos = 50000; //Se asigna un valor muy grande
				cant++;
				while(archin.read((char *)&aux,sizeof(Resultados))){ //Lee todo el archivo
					obtenido++; //Se aumenta para saber que numero de registro se esta guardando
					if(aux.ciclos < agregar.ciclos){ //Obtiene el que hizo menos ciclos
						for(int i=0; i<cant; i++){
							if(listos!=NULL){
								if(obtenido == listos[i]){ //Si el más chico ya esta en el archivo
									band = true; //Enciende la bandera que ese no lo debe guardar
								}
							}
						}
						if(!band){ //Si la bandera es falsa, guarda el nuevo elemento 
							anterior = agregar;
							agregar = aux;
							almacenado = obtenido;
						}
						band = false; //Reinicia la bandera en falso
					}
				}
				respaldo = listos;
				listos = new int[cant];
				if(respaldo!=NULL){
					for(int i=0; i<cant-1;i++){
						listos[i] = respaldo[i];
					}
					delete respaldo;
				}
				listos[cant-1] = almacenado;
				if(agregar.ciclos != anterior.ciclos){
					archout << "\t*************************************************\n";
					archout << "\t*                                               *\n";
					archout << "\t*         Estadisticas de la simulacion         *\n";
					archout << "\t*                                               *\n";
					archout << "\t*    Ciclos promedio en finalizar: " << agregar.ciclos << " ciclos" << "\t*\n";
					archout << "\t*    Cantidad de procesos iniciados: " << agregar.idM << "\t\t*\n";
					archout << "\t*    Procesos que esperaron para entrar: " << agregar.contTermProceso << "\t*\n";
					archout << "\t*    Tiempo promedio de atencion: " << setprecision(4) << agregar.sumTiempo << "\t\t*\n";
					archout << "\t*                                               *\n";
					archout << "\t*                                               *\n";
					archout << "\t*************************************************\n\n";
				}
			}
			archin.close();
			archout.close();
		}

		void RoundRobin::ReporteTiempo(){
			fstream archin, archout;
			char archNombre[TAMSTRING];
			Resultados agregar, aux, anterior;
			int cont = 0, cant = 0, obtenido = 0, almacenado = 0;
			int *respaldo = NULL, *listos = NULL;
			bool band = false;
			cout << "\n\tNombre del archivo que desea: ";
			cin >> archNombre;
			strcat(archNombre,".txt");
			archout.open(archNombre,ios::app|ios::in|ios::out);
			archin.open("RoundRobinBinResultados.dat",ios::binary|ios::in|ios::out);
			while(archin.read((char *)&aux,sizeof(Resultados))) cont++; //Cuenta la cantidad de datos que hay en el archivo binario
			while(cant <= cont){ //Mientras no se inserten todos los datos
				archin.close();
				archin.open("RoundRobinBinResultados.dat",ios::binary|ios::in|ios::out); //Reinicia el archivo
				obtenido = 1;
				almacenado = 0;
				agregar.sumTiempo = 50000; //Se asigna un valor muy grande
				cant++;
				while(archin.read((char *)&aux,sizeof(Resultados))){ //Lee todo el archivo
					obtenido++; //Se aumenta para saber que numero de registro se esta guardando
					if(aux.sumTiempo < agregar.sumTiempo){ //Obtiene el que hizo menos ciclos
						for(int i=0; i<cant; i++){
							if(listos!=NULL){
								if(obtenido == listos[i]){ //Si el más chico ya esta en el archivo
									band = true; //Enciende la bandera que ese no lo debe guardar
								}
							}
						}
						if(!band){ //Si la bandera es falsa, guarda el nuevo elemento 
							anterior = agregar;
							agregar = aux;
							almacenado = obtenido;
						}
						band = false; //Reinicia la bandera en falso
					}
				}
				respaldo = listos;
				listos = new int[cant];
				if(respaldo!=NULL){
					for(int i=0; i<cant-1;i++){
						listos[i] = respaldo[i];
					}
					delete respaldo;
				}
				listos[cant-1] = almacenado;
				if(agregar.sumTiempo != anterior.sumTiempo){
					archout << "\t*************************************************\n";
					archout << "\t*                                               *\n";
					archout << "\t*         Estadisticas de la simulacion         *\n";
					archout << "\t*                                               *\n";
					archout << "\t*    Ciclos promedio en finalizar: " << agregar.ciclos << " ciclos" << "\t*\n";
					archout << "\t*    Cantidad de procesos iniciados: " << agregar.idM << "\t\t*\n";
					archout << "\t*    Procesos que esperaron para entrar: " << agregar.contTermProceso << "\t*\n";
					archout << "\t*    Tiempo promedio de atencion: " << setprecision(4) << agregar.sumTiempo << "\t\t*\n";
					archout << "\t*                                               *\n";
					archout << "\t*                                               *\n";
					archout << "\t*************************************************\n\n";
				}
			}
			archin.close();
			archout.close();
		}

		void RoundRobin::ReporteIniciados(){
			fstream archin, archout;
			char archNombre[TAMSTRING];
			Resultados agregar, aux, anterior;
			int cont = 0, cant = 0, obtenido = 0, almacenado = 0;
			int *respaldo = NULL, *listos = NULL;
			bool band = false;
			cout << "\n\tNombre del archivo que desea: ";
			cin >> archNombre;
			strcat(archNombre,".txt");
			archout.open(archNombre,ios::app|ios::in|ios::out);
			archin.open("RoundRobinBinResultados.dat",ios::binary|ios::in|ios::out);
			while(archin.read((char *)&aux,sizeof(Resultados))) cont++; //Cuenta la cantidad de datos que hay en el archivo binario
			while(cant <= cont){ //Mientras no se inserten todos los datos
				archin.close();
				archin.open("RoundRobinBinResultados.dat",ios::binary|ios::in|ios::out); //Reinicia el archivo
				obtenido = 1;
				almacenado = 0;
				agregar.idM = 50000; //Se asigna un valor muy grande
				cant++;
				while(archin.read((char *)&aux,sizeof(Resultados))){ //Lee todo el archivo
					obtenido++; //Se aumenta para saber que numero de registro se esta guardando
					if(aux.idM < agregar.idM){ //Obtiene el que hizo menos ciclos
						for(int i=0; i<cant; i++){
							if(listos!=NULL){
								if(obtenido == listos[i]){ //Si el más chico ya esta en el archivo
									band = true; //Enciende la bandera que ese no lo debe guardar
								}
							}
						}
						if(!band){ //Si la bandera es falsa, guarda el nuevo elemento 
							anterior = agregar;
							agregar = aux;
							almacenado = obtenido;
						}
						band = false; //Reinicia la bandera en falso
					}
				}
				respaldo = listos;
				listos = new int[cant];
				if(respaldo!=NULL){
					for(int i=0; i<cant-1;i++){
						listos[i] = respaldo[i];
					}
					delete respaldo;
				}
				listos[cant-1] = almacenado;
				if(agregar.idM != anterior.idM){
					archout << "\t*************************************************\n";
					archout << "\t*                                               *\n";
					archout << "\t*         Estadisticas de la simulacion         *\n";
					archout << "\t*                                               *\n";
					archout << "\t*    Ciclos promedio en finalizar: " << agregar.ciclos << " ciclos" << "\t*\n";
					archout << "\t*    Cantidad de procesos iniciados: " << agregar.idM << "\t\t*\n";
					archout << "\t*    Procesos que esperaron para entrar: " << agregar.contTermProceso << "\t*\n";
					archout << "\t*    Tiempo promedio de atencion: " << setprecision(4) << agregar.sumTiempo << "\t\t*\n";
					archout << "\t*                                               *\n";
					archout << "\t*                                               *\n";
					archout << "\t*************************************************\n\n";
				}
			}
			archin.close();
			archout.close();
		}

		void RoundRobin::ReporteEspera(){
			fstream archin, archout;
			char archNombre[TAMSTRING];
			Resultados agregar, aux, anterior;
			int cont = 0, cant = 0, obtenido = 0, almacenado = 0;
			int *respaldo = NULL, *listos = NULL;
			bool band = false;
			cout << "\n\tNombre del archivo que desea: ";
			cin >> archNombre;
			strcat(archNombre,".txt");
			archout.open(archNombre,ios::app|ios::in|ios::out);
			archin.open("RoundRobinBinResultados.dat",ios::binary|ios::in|ios::out);
			while(archin.read((char *)&aux,sizeof(Resultados))) cont++; //Cuenta la cantidad de datos que hay en el archivo binario
			while(cant <= cont){ //Mientras no se inserten todos los datos
				archin.close();
				archin.open("RoundRobinBinResultados.dat",ios::binary|ios::in|ios::out); //Reinicia el archivo
				obtenido = 1;
				almacenado = 0;
				agregar.contTermProceso = 50000; //Se asigna un valor muy grande
				cant++;
				while(archin.read((char *)&aux,sizeof(Resultados))){ //Lee todo el archivo
					obtenido++; //Se aumenta para saber que numero de registro se esta guardando
					if(aux.contTermProceso < agregar.contTermProceso){ //Obtiene el que hizo menos ciclos
						for(int i=0; i<cant; i++){
							if(listos!=NULL){
								if(obtenido == listos[i]){ //Si el más chico ya esta en el archivo
									band = true; //Enciende la bandera que ese no lo debe guardar
								}
							}
						}
						if(!band){ //Si la bandera es falsa, guarda el nuevo elemento 
							anterior = agregar;
							agregar = aux;
							almacenado = obtenido;
						}
						band = false; //Reinicia la bandera en falso
					}
				}
				respaldo = listos;
				listos = new int[cant];
				if(respaldo!=NULL){
					for(int i=0; i<cant-1;i++){
						listos[i] = respaldo[i];
					}
					delete respaldo;
				}
				listos[cant-1] = almacenado;
				if(agregar.contTermProceso != anterior.contTermProceso){
					archout << "\t*************************************************\n";
					archout << "\t*                                               *\n";
					archout << "\t*         Estadisticas de la simulacion         *\n";
					archout << "\t*                                               *\n";
					archout << "\t*    Ciclos promedio en finalizar: " << agregar.ciclos << " ciclos" << "\t*\n";
					archout << "\t*    Cantidad de procesos iniciados: " << agregar.idM << "\t\t*\n";
					archout << "\t*    Procesos que esperaron para entrar: " << agregar.contTermProceso << "\t*\n";
					archout << "\t*    Tiempo promedio de atencion: " << setprecision(4) << agregar.sumTiempo << "\t\t*\n";
					archout << "\t*                                               *\n";
					archout << "\t*                                               *\n";
					archout << "\t*************************************************\n\n";
				}
			}
			archin.close();
			archout.close();
		}

	//Ordenaciones PEPS

		void PEPS::ReporteCiclos(){
			fstream archin, archout;
			char archNombre[TAMSTRING];
			Resultados agregar, aux, anterior;
			int cont = 0, cant = 0, obtenido = 0, almacenado = 0;
			int *respaldo = NULL, *listos = NULL;
			bool band = false;
			cout << "\n\tNombre del archivo que desea: ";
			cin >> archNombre;
			strcat(archNombre,".txt");
			archout.open(archNombre,ios::app|ios::in|ios::out);
			archin.open("PEPSbinResultados.dat",ios::binary|ios::in|ios::out);
			while(archin.read((char *)&aux,sizeof(Resultados))) cont++; //Cuenta la cantidad de datos que hay en el archivo binario
			while(cant <= cont){ //Mientras no se inserten todos los datos
				archin.close();
				archin.open("PEPSbinResultados.dat",ios::binary|ios::in|ios::out); //Reinicia el archivo
				obtenido = 1;
				almacenado = 0;
				agregar.ciclos = 50000; //Se asigna un valor muy grande
				cant++;
				while(archin.read((char *)&aux,sizeof(Resultados))){ //Lee todo el archivo
					obtenido++; //Se aumenta para saber que numero de registro se esta guardando
					if(aux.ciclos < agregar.ciclos){ //Obtiene el que hizo menos ciclos
						for(int i=0; i<cant; i++){
							if(listos!=NULL){
								if(obtenido == listos[i]){ //Si el más chico ya esta en el archivo
									band = true; //Enciende la bandera que ese no lo debe guardar
								}
							}
						}
						if(!band){ //Si la bandera es falsa, guarda el nuevo elemento 
							anterior = agregar;
							agregar = aux;
							almacenado = obtenido;
						}
						band = false; //Reinicia la bandera en falso
					}
				}
				respaldo = listos;
				listos = new int[cant];
				if(respaldo!=NULL){
					for(int i=0; i<cant-1;i++){
						listos[i] = respaldo[i];
					}
					delete respaldo;
				}
				listos[cant-1] = almacenado;
				if(agregar.ciclos != anterior.ciclos){
					archout << "\t*************************************************\n";
					archout << "\t*                                               *\n";
					archout << "\t*         Estadisticas de la simulacion         *\n";
					archout << "\t*                                               *\n";
					archout << "\t*    Ciclos promedio en finalizar: " << agregar.ciclos << " ciclos" << "\t*\n";
					archout << "\t*    Cantidad de procesos iniciados: " << agregar.idM << "\t\t*\n";
					archout << "\t*    Procesos que esperaron para entrar: " << agregar.contTermProceso << "\t*\n";
					archout << "\t*    Tiempo promedio de atencion: " << setprecision(4) << agregar.sumTiempo << "\t\t*\n";
					archout << "\t*                                               *\n";
					archout << "\t*                                               *\n";
					archout << "\t*************************************************\n\n";
				}
			}
			archin.close();
			archout.close();
		}

		void PEPS::ReporteTiempo(){
			fstream archin, archout;
			char archNombre[TAMSTRING];
			Resultados agregar, aux, anterior;
			int cont = 0, cant = 0, obtenido = 0, almacenado = 0;
			int *respaldo = NULL, *listos = NULL;
			bool band = false;
			cout << "\n\tNombre del archivo que desea: ";
			cin >> archNombre;
			strcat(archNombre,".txt");
			archout.open(archNombre,ios::app|ios::in|ios::out);
			archin.open("PEPSbinResultados.dat",ios::binary|ios::in|ios::out);
			while(archin.read((char *)&aux,sizeof(Resultados))) cont++; //Cuenta la cantidad de datos que hay en el archivo binario
			while(cant <= cont){ //Mientras no se inserten todos los datos
				archin.close();
				archin.open("PEPSbinResultados.dat",ios::binary|ios::in|ios::out); //Reinicia el archivo
				obtenido = 1;
				almacenado = 0;
				agregar.sumTiempo = 50000; //Se asigna un valor muy grande
				cant++;
				while(archin.read((char *)&aux,sizeof(Resultados))){ //Lee todo el archivo
					obtenido++; //Se aumenta para saber que numero de registro se esta guardando
					if(aux.sumTiempo < agregar.sumTiempo){ //Obtiene el que hizo menos ciclos
						for(int i=0; i<cant; i++){
							if(listos!=NULL){
								if(obtenido == listos[i]){ //Si el más chico ya esta en el archivo
									band = true; //Enciende la bandera que ese no lo debe guardar
								}
							}
						}
						if(!band){ //Si la bandera es falsa, guarda el nuevo elemento 
							anterior = agregar;
							agregar = aux;
							almacenado = obtenido;
						}
						band = false; //Reinicia la bandera en falso
					}
				}
				respaldo = listos;
				listos = new int[cant];
				if(respaldo!=NULL){
					for(int i=0; i<cant-1;i++){
						listos[i] = respaldo[i];
					}
					delete respaldo;
				}
				listos[cant-1] = almacenado;
				if(agregar.sumTiempo != anterior.sumTiempo){
					archout << "\t*************************************************\n";
					archout << "\t*                                               *\n";
					archout << "\t*         Estadisticas de la simulacion         *\n";
					archout << "\t*                                               *\n";
					archout << "\t*    Ciclos promedio en finalizar: " << agregar.ciclos << " ciclos" << "\t*\n";
					archout << "\t*    Cantidad de procesos iniciados: " << agregar.idM << "\t\t*\n";
					archout << "\t*    Procesos que esperaron para entrar: " << agregar.contTermProceso << "\t*\n";
					archout << "\t*    Tiempo promedio de atencion: " << setprecision(4) << agregar.sumTiempo << "\t\t*\n";
					archout << "\t*                                               *\n";
					archout << "\t*                                               *\n";
					archout << "\t*************************************************\n\n";
				}
			}
			archin.close();
			archout.close();
		}

		void PEPS::ReporteIniciados(){
			fstream archin, archout;
			char archNombre[TAMSTRING];
			Resultados agregar, aux, anterior;
			int cont = 0, cant = 0, obtenido = 0, almacenado = 0;
			int *respaldo = NULL, *listos = NULL;
			bool band = false;
			cout << "\n\tNombre del archivo que desea: ";
			cin >> archNombre;
			strcat(archNombre,".txt");
			archout.open(archNombre,ios::app|ios::in|ios::out);
			archin.open("PEPSbinResultados.dat",ios::binary|ios::in|ios::out);
			while(archin.read((char *)&aux,sizeof(Resultados))) cont++; //Cuenta la cantidad de datos que hay en el archivo binario
			while(cant <= cont){ //Mientras no se inserten todos los datos
				archin.close();
				archin.open("PEPSbinResultados.dat",ios::binary|ios::in|ios::out); //Reinicia el archivo
				obtenido = 1;
				almacenado = 0;
				agregar.idM = 50000; //Se asigna un valor muy grande
				cant++;
				while(archin.read((char *)&aux,sizeof(Resultados))){ //Lee todo el archivo
					obtenido++; //Se aumenta para saber que numero de registro se esta guardando
					if(aux.idM < agregar.idM){ //Obtiene el que hizo menos ciclos
						for(int i=0; i<cant; i++){
							if(listos!=NULL){
								if(obtenido == listos[i]){ //Si el más chico ya esta en el archivo
									band = true; //Enciende la bandera que ese no lo debe guardar
								}
							}
						}
						if(!band){ //Si la bandera es falsa, guarda el nuevo elemento 
							anterior = agregar;
							agregar = aux;
							almacenado = obtenido;
						}
						band = false; //Reinicia la bandera en falso
					}
				}
				respaldo = listos;
				listos = new int[cant];
				if(respaldo!=NULL){
					for(int i=0; i<cant-1;i++){
						listos[i] = respaldo[i];
					}
					delete respaldo;
				}
				listos[cant-1] = almacenado;
				if(agregar.idM != anterior.idM){
					archout << "\t*************************************************\n";
					archout << "\t*                                               *\n";
					archout << "\t*         Estadisticas de la simulacion         *\n";
					archout << "\t*                                               *\n";
					archout << "\t*    Ciclos promedio en finalizar: " << agregar.ciclos << " ciclos" << "\t*\n";
					archout << "\t*    Cantidad de procesos iniciados: " << agregar.idM << "\t\t*\n";
					archout << "\t*    Procesos que esperaron para entrar: " << agregar.contTermProceso << "\t*\n";
					archout << "\t*    Tiempo promedio de atencion: " << setprecision(4) << agregar.sumTiempo << "\t\t*\n";
					archout << "\t*                                               *\n";
					archout << "\t*                                               *\n";
					archout << "\t*************************************************\n\n";
				}
			}
			archin.close();
			archout.close();
		}

		void PEPS::ReporteEspera(){
			fstream archin, archout;
			char archNombre[TAMSTRING];
			Resultados agregar, aux, anterior;
			int cont = 0, cant = 0, obtenido = 0, almacenado = 0;
			int *respaldo = NULL, *listos = NULL;
			bool band = false;
			cout << "\n\tNombre del archivo que desea: ";
			cin >> archNombre;
			strcat(archNombre,".txt");
			archout.open(archNombre,ios::app|ios::in|ios::out);
			archin.open("PEPSbinResultados.dat",ios::binary|ios::in|ios::out);
			while(archin.read((char *)&aux,sizeof(Resultados))) cont++; //Cuenta la cantidad de datos que hay en el archivo binario
			while(cant <= cont){ //Mientras no se inserten todos los datos
				archin.close();
				archin.open("PEPSbinResultados.dat",ios::binary|ios::in|ios::out); //Reinicia el archivo
				obtenido = 1;
				almacenado = 0;
				agregar.contTermProceso = 50000; //Se asigna un valor muy grande
				cant++;
				while(archin.read((char *)&aux,sizeof(Resultados))){ //Lee todo el archivo
					obtenido++; //Se aumenta para saber que numero de registro se esta guardando
					if(aux.contTermProceso < agregar.contTermProceso){ //Obtiene el que hizo menos ciclos
						for(int i=0; i<cant; i++){
							if(listos!=NULL){
								if(obtenido == listos[i]){ //Si el más chico ya esta en el archivo
									band = true; //Enciende la bandera que ese no lo debe guardar
								}
							}
						}
						if(!band){ //Si la bandera es falsa, guarda el nuevo elemento 
							anterior = agregar;
							agregar = aux;
							almacenado = obtenido;
						}
						band = false; //Reinicia la bandera en falso
					}
				}
				respaldo = listos;
				listos = new int[cant];
				if(respaldo!=NULL){
					for(int i=0; i<cant-1;i++){
						listos[i] = respaldo[i];
					}
					delete respaldo;
				}
				listos[cant-1] = almacenado;
				if(agregar.contTermProceso != anterior.contTermProceso){
					archout << "\t*************************************************\n";
					archout << "\t*                                               *\n";
					archout << "\t*         Estadisticas de la simulacion         *\n";
					archout << "\t*                                               *\n";
					archout << "\t*    Ciclos promedio en finalizar: " << agregar.ciclos << " ciclos" << "\t*\n";
					archout << "\t*    Cantidad de procesos iniciados: " << agregar.idM << "\t\t*\n";
					archout << "\t*    Procesos que esperaron para entrar: " << agregar.contTermProceso << "\t*\n";
					archout << "\t*    Tiempo promedio de atencion: " << setprecision(4) << agregar.sumTiempo << "\t\t*\n";
					archout << "\t*                                               *\n";
					archout << "\t*                                               *\n";
					archout << "\t*************************************************\n\n";
				}
			}
			archin.close();
			archout.close();
		}

	//Ordenaciones SJF

		void SJF::ReporteCiclos(){
			fstream archin, archout;
			char archNombre[TAMSTRING];
			Resultados agregar, aux, anterior;
			int cont = 0, cant = 0, obtenido = 0, almacenado = 0;
			int *respaldo = NULL, *listos = NULL;
			bool band = false;
			cout << "\n\tNombre del archivo que desea: ";
			cin >> archNombre;
			strcat(archNombre,".txt");
			archout.open(archNombre,ios::app|ios::in|ios::out);
			archin.open("SJFbinResultados.dat",ios::binary|ios::in|ios::out);
			while(archin.read((char *)&aux,sizeof(Resultados))) cont++; //Cuenta la cantidad de datos que hay en el archivo binario
			while(cant <= cont){ //Mientras no se inserten todos los datos
				archin.close();
				archin.open("SJFbinResultados.dat",ios::binary|ios::in|ios::out); //Reinicia el archivo
				obtenido = 1;
				almacenado = 0;
				agregar.ciclos = 50000; //Se asigna un valor muy grande
				cant++;
				while(archin.read((char *)&aux,sizeof(Resultados))){ //Lee todo el archivo
					obtenido++; //Se aumenta para saber que numero de registro se esta guardando
					if(aux.ciclos < agregar.ciclos){ //Obtiene el que hizo menos ciclos
						for(int i=0; i<cant; i++){
							if(listos!=NULL){
								if(obtenido == listos[i]){ //Si el más chico ya esta en el archivo
									band = true; //Enciende la bandera que ese no lo debe guardar
								}
							}
						}
						if(!band){ //Si la bandera es falsa, guarda el nuevo elemento 
							anterior = agregar;
							agregar = aux;
							almacenado = obtenido;
						}
						band = false; //Reinicia la bandera en falso
					}
				}
				respaldo = listos;
				listos = new int[cant];
				if(respaldo!=NULL){
					for(int i=0; i<cant-1;i++){
						listos[i] = respaldo[i];
					}
					delete respaldo;
				}
				listos[cant-1] = almacenado;
				if(agregar.ciclos != anterior.ciclos){
					archout << "\t*************************************************\n";
					archout << "\t*                                               *\n";
					archout << "\t*         Estadisticas de la simulacion         *\n";
					archout << "\t*                                               *\n";
					archout << "\t*    Ciclos promedio en finalizar: " << agregar.ciclos << " ciclos" << "\t*\n";
					archout << "\t*    Cantidad de procesos iniciados: " << agregar.idM << "\t\t*\n";
					archout << "\t*    Procesos que esperaron para entrar: " << agregar.contTermProceso << "\t*\n";
					archout << "\t*    Tiempo promedio de atencion: " << setprecision(4) << agregar.sumTiempo << "\t\t*\n";
					archout << "\t*                                               *\n";
					archout << "\t*                                               *\n";
					archout << "\t*************************************************\n\n";
				}
			}
			archin.close();
			archout.close();
		}

		void SJF::ReporteTiempo(){
			fstream archin, archout;
			char archNombre[TAMSTRING];
			Resultados agregar, aux, anterior;
			int cont = 0, cant = 0, obtenido = 0, almacenado = 0;
			int *respaldo = NULL, *listos = NULL;
			bool band = false;
			cout << "\n\tNombre del archivo que desea: ";
			cin >> archNombre;
			strcat(archNombre,".txt");
			archout.open(archNombre,ios::app|ios::in|ios::out);
			archin.open("SJFbinResultados.dat",ios::binary|ios::in|ios::out);
			while(archin.read((char *)&aux,sizeof(Resultados))) cont++; //Cuenta la cantidad de datos que hay en el archivo binario
			while(cant <= cont){ //Mientras no se inserten todos los datos
				archin.close();
				archin.open("SJFbinResultados.dat",ios::binary|ios::in|ios::out); //Reinicia el archivo
				obtenido = 1;
				almacenado = 0;
				agregar.sumTiempo = 50000; //Se asigna un valor muy grande
				cant++;
				while(archin.read((char *)&aux,sizeof(Resultados))){ //Lee todo el archivo
					obtenido++; //Se aumenta para saber que numero de registro se esta guardando
					if(aux.sumTiempo < agregar.sumTiempo){ //Obtiene el que hizo menos ciclos
						for(int i=0; i<cant; i++){
							if(listos!=NULL){
								if(obtenido == listos[i]){ //Si el más chico ya esta en el archivo
									band = true; //Enciende la bandera que ese no lo debe guardar
								}
							}
						}
						if(!band){ //Si la bandera es falsa, guarda el nuevo elemento 
							anterior = agregar;
							agregar = aux;
							almacenado = obtenido;
						}
						band = false; //Reinicia la bandera en falso
					}
				}
				respaldo = listos;
				listos = new int[cant];
				if(respaldo!=NULL){
					for(int i=0; i<cant-1;i++){
						listos[i] = respaldo[i];
					}
					delete respaldo;
				}
				listos[cant-1] = almacenado;
				if(agregar.sumTiempo != anterior.sumTiempo){
					archout << "\t*************************************************\n";
					archout << "\t*                                               *\n";
					archout << "\t*         Estadisticas de la simulacion         *\n";
					archout << "\t*                                               *\n";
					archout << "\t*    Ciclos promedio en finalizar: " << agregar.ciclos << " ciclos" << "\t*\n";
					archout << "\t*    Cantidad de procesos iniciados: " << agregar.idM << "\t\t*\n";
					archout << "\t*    Procesos que esperaron para entrar: " << agregar.contTermProceso << "\t*\n";
					archout << "\t*    Tiempo promedio de atencion: " << setprecision(4) << agregar.sumTiempo << "\t\t*\n";
					archout << "\t*                                               *\n";
					archout << "\t*                                               *\n";
					archout << "\t*************************************************\n\n";
				}
			}
			archin.close();
			archout.close();
		}

		void SJF::ReporteIniciados(){
			fstream archin, archout;
			char archNombre[TAMSTRING];
			Resultados agregar, aux, anterior;
			int cont = 0, cant = 0, obtenido = 0, almacenado = 0;
			int *respaldo = NULL, *listos = NULL;
			bool band = false;
			cout << "\n\tNombre del archivo que desea: ";
			cin >> archNombre;
			strcat(archNombre,".txt");
			archout.open(archNombre,ios::app|ios::in|ios::out);
			archin.open("SJFbinResultados.dat",ios::binary|ios::in|ios::out);
			while(archin.read((char *)&aux,sizeof(Resultados))) cont++; //Cuenta la cantidad de datos que hay en el archivo binario
			while(cant <= cont){ //Mientras no se inserten todos los datos
				archin.close();
				archin.open("SJFbinResultados.dat",ios::binary|ios::in|ios::out); //Reinicia el archivo
				obtenido = 1;
				almacenado = 0;
				agregar.idM = 50000; //Se asigna un valor muy grande
				cant++;
				while(archin.read((char *)&aux,sizeof(Resultados))){ //Lee todo el archivo
					obtenido++; //Se aumenta para saber que numero de registro se esta guardando
					if(aux.idM < agregar.idM){ //Obtiene el que hizo menos ciclos
						for(int i=0; i<cant; i++){
							if(listos!=NULL){
								if(obtenido == listos[i]){ //Si el más chico ya esta en el archivo
									band = true; //Enciende la bandera que ese no lo debe guardar
								}
							}
						}
						if(!band){ //Si la bandera es falsa, guarda el nuevo elemento 
							anterior = agregar;
							agregar = aux;
							almacenado = obtenido;
						}
						band = false; //Reinicia la bandera en falso
					}
				}
				respaldo = listos;
				listos = new int[cant];
				if(respaldo!=NULL){
					for(int i=0; i<cant-1;i++){
						listos[i] = respaldo[i];
					}
					delete respaldo;
				}
				listos[cant-1] = almacenado;
				if(agregar.idM != anterior.idM){
					archout << "\t*************************************************\n";
					archout << "\t*                                               *\n";
					archout << "\t*         Estadisticas de la simulacion         *\n";
					archout << "\t*                                               *\n";
					archout << "\t*    Ciclos promedio en finalizar: " << agregar.ciclos << " ciclos" << "\t*\n";
					archout << "\t*    Cantidad de procesos iniciados: " << agregar.idM << "\t\t*\n";
					archout << "\t*    Procesos que esperaron para entrar: " << agregar.contTermProceso << "\t*\n";
					archout << "\t*    Tiempo promedio de atencion: " << setprecision(4) << agregar.sumTiempo << "\t\t*\n";
					archout << "\t*                                               *\n";
					archout << "\t*                                               *\n";
					archout << "\t*************************************************\n\n";
				}
			}
			archin.close();
			archout.close();
		}

		void SJF::ReporteEspera(){
			fstream archin, archout;
			char archNombre[TAMSTRING];
			Resultados agregar, aux, anterior;
			int cont = 0, cant = 0, obtenido = 0, almacenado = 0;
			int *respaldo = NULL, *listos = NULL;
			bool band = false;
			cout << "\n\tNombre del archivo que desea: ";
			cin >> archNombre;
			strcat(archNombre,".txt");
			archout.open(archNombre,ios::app|ios::in|ios::out);
			archin.open("SJFbinResultados.dat",ios::binary|ios::in|ios::out);
			while(archin.read((char *)&aux,sizeof(Resultados))) cont++; //Cuenta la cantidad de datos que hay en el archivo binario
			while(cant <= cont){ //Mientras no se inserten todos los datos
				archin.close();
				archin.open("SJFbinResultados.dat",ios::binary|ios::in|ios::out); //Reinicia el archivo
				obtenido = 1;
				almacenado = 0;
				agregar.contTermProceso = 50000; //Se asigna un valor muy grande
				cant++;
				while(archin.read((char *)&aux,sizeof(Resultados))){ //Lee todo el archivo
					obtenido++; //Se aumenta para saber que numero de registro se esta guardando
					if(aux.contTermProceso < agregar.contTermProceso){ //Obtiene el que hizo menos ciclos
						for(int i=0; i<cant; i++){
							if(listos!=NULL){
								if(obtenido == listos[i]){ //Si el más chico ya esta en el archivo
									band = true; //Enciende la bandera que ese no lo debe guardar
								}
							}
						}
						if(!band){ //Si la bandera es falsa, guarda el nuevo elemento 
							anterior = agregar;
							agregar = aux;
							almacenado = obtenido;
						}
						band = false; //Reinicia la bandera en falso
					}
				}
				respaldo = listos;
				listos = new int[cant];
				if(respaldo!=NULL){
					for(int i=0; i<cant-1;i++){
						listos[i] = respaldo[i];
					}
					delete respaldo;
				}
				listos[cant-1] = almacenado;
				if(agregar.contTermProceso != anterior.contTermProceso){
					archout << "\t*************************************************\n";
					archout << "\t*                                               *\n";
					archout << "\t*         Estadisticas de la simulacion         *\n";
					archout << "\t*                                               *\n";
					archout << "\t*    Ciclos promedio en finalizar: " << agregar.ciclos << " ciclos" << "\t*\n";
					archout << "\t*    Cantidad de procesos iniciados: " << agregar.idM << "\t\t*\n";
					archout << "\t*    Procesos que esperaron para entrar: " << agregar.contTermProceso << "\t*\n";
					archout << "\t*    Tiempo promedio de atencion: " << setprecision(4) << agregar.sumTiempo << "\t\t*\n";
					archout << "\t*                                               *\n";
					archout << "\t*                                               *\n";
					archout << "\t*************************************************\n\n";
				}
			}
			archin.close();
			archout.close();
		}

	//Ordenaciones LTL

		void LTL::ReporteCiclos(){
			fstream archin, archout;
			char archNombre[TAMSTRING];
			Resultados agregar, aux, anterior;
			int cont = 0, cant = 0, obtenido = 0, almacenado = 0;
			int *respaldo = NULL, *listos = NULL;
			bool band = false;
			cout << "\n\tNombre del archivo que desea: ";
			cin >> archNombre;
			strcat(archNombre,".txt");
			archout.open(archNombre,ios::app|ios::in|ios::out);
			archin.open("LTLbinResultados.dat",ios::binary|ios::in|ios::out);
			while(archin.read((char *)&aux,sizeof(Resultados))) cont++; //Cuenta la cantidad de datos que hay en el archivo binario
			while(cant <= cont){ //Mientras no se inserten todos los datos
				archin.close();
				archin.open("LTLbinResultados.dat",ios::binary|ios::in|ios::out); //Reinicia el archivo
				obtenido = 1;
				almacenado = 0;
				agregar.ciclos = 50000; //Se asigna un valor muy grande
				cant++;
				while(archin.read((char *)&aux,sizeof(Resultados))){ //Lee todo el archivo
					obtenido++; //Se aumenta para saber que numero de registro se esta guardando
					if(aux.ciclos < agregar.ciclos){ //Obtiene el que hizo menos ciclos
						for(int i=0; i<cant; i++){
							if(listos!=NULL){
								if(obtenido == listos[i]){ //Si el más chico ya esta en el archivo
									band = true; //Enciende la bandera que ese no lo debe guardar
								}
							}
						}
						if(!band){ //Si la bandera es falsa, guarda el nuevo elemento 
							anterior = agregar;
							agregar = aux;
							almacenado = obtenido;
						}
						band = false; //Reinicia la bandera en falso
					}
				}
				respaldo = listos;
				listos = new int[cant];
				if(respaldo!=NULL){
					for(int i=0; i<cant-1;i++){
						listos[i] = respaldo[i];
					}
					delete respaldo;
				}
				listos[cant-1] = almacenado;
				if(agregar.ciclos != anterior.ciclos){
					archout << "\t*************************************************\n";
					archout << "\t*                                               *\n";
					archout << "\t*         Estadisticas de la simulacion         *\n";
					archout << "\t*                                               *\n";
					archout << "\t*    Ciclos promedio en finalizar: " << agregar.ciclos << " ciclos" << "\t*\n";
					archout << "\t*    Cantidad de procesos iniciados: " << agregar.idM << "\t\t*\n";
					archout << "\t*    Procesos que esperaron para entrar: " << agregar.contTermProceso << "\t*\n";
					archout << "\t*    Tiempo promedio de atencion: " << setprecision(4) << agregar.sumTiempo << "\t\t*\n";
					archout << "\t*                                               *\n";
					archout << "\t*                                               *\n";
					archout << "\t*************************************************\n\n";
				}
			}
			archin.close();
			archout.close();
		}

		void LTL::ReporteTiempo(){
			fstream archin, archout;
			char archNombre[TAMSTRING];
			Resultados agregar, aux, anterior;
			int cont = 0, cant = 0, obtenido = 0, almacenado = 0;
			int *respaldo = NULL, *listos = NULL;
			bool band = false;
			cout << "\n\tNombre del archivo que desea: ";
			cin >> archNombre;
			strcat(archNombre,".txt");
			archout.open(archNombre,ios::app|ios::in|ios::out);
			archin.open("LTLbinResultados.dat",ios::binary|ios::in|ios::out);
			while(archin.read((char *)&aux,sizeof(Resultados))) cont++; //Cuenta la cantidad de datos que hay en el archivo binario
			while(cant <= cont){ //Mientras no se inserten todos los datos
				archin.close();
				archin.open("LTLbinResultados.dat",ios::binary|ios::in|ios::out); //Reinicia el archivo
				obtenido = 1;
				almacenado = 0;
				agregar.sumTiempo = 50000; //Se asigna un valor muy grande
				cant++;
				while(archin.read((char *)&aux,sizeof(Resultados))){ //Lee todo el archivo
					obtenido++; //Se aumenta para saber que numero de registro se esta guardando
					if(aux.sumTiempo < agregar.sumTiempo){ //Obtiene el que hizo menos ciclos
						for(int i=0; i<cant; i++){
							if(listos!=NULL){
								if(obtenido == listos[i]){ //Si el más chico ya esta en el archivo
									band = true; //Enciende la bandera que ese no lo debe guardar
								}
							}
						}
						if(!band){ //Si la bandera es falsa, guarda el nuevo elemento 
							anterior = agregar;
							agregar = aux;
							almacenado = obtenido;
						}
						band = false; //Reinicia la bandera en falso
					}
				}
				respaldo = listos;
				listos = new int[cant];
				if(respaldo!=NULL){
					for(int i=0; i<cant-1;i++){
						listos[i] = respaldo[i];
					}
					delete respaldo;
				}
				listos[cant-1] = almacenado;
				if(agregar.sumTiempo != anterior.sumTiempo){
					archout << "\t*************************************************\n";
					archout << "\t*                                               *\n";
					archout << "\t*         Estadisticas de la simulacion         *\n";
					archout << "\t*                                               *\n";
					archout << "\t*    Ciclos promedio en finalizar: " << agregar.ciclos << " ciclos" << "\t*\n";
					archout << "\t*    Cantidad de procesos iniciados: " << agregar.idM << "\t\t*\n";
					archout << "\t*    Procesos que esperaron para entrar: " << agregar.contTermProceso << "\t*\n";
					archout << "\t*    Tiempo promedio de atencion: " << setprecision(4) << agregar.sumTiempo << "\t\t*\n";
					archout << "\t*                                               *\n";
					archout << "\t*                                               *\n";
					archout << "\t*************************************************\n\n";
				}
			}
			archin.close();
			archout.close();
		}

		void LTL::ReporteIniciados(){
			fstream archin, archout;
			char archNombre[TAMSTRING];
			Resultados agregar, aux, anterior;
			int cont = 0, cant = 0, obtenido = 0, almacenado = 0;
			int *respaldo = NULL, *listos = NULL;
			bool band = false;
			cout << "\n\tNombre del archivo que desea: ";
			cin >> archNombre;
			strcat(archNombre,".txt");
			archout.open(archNombre,ios::app|ios::in|ios::out);
			archin.open("LTLbinResultados.dat",ios::binary|ios::in|ios::out);
			while(archin.read((char *)&aux,sizeof(Resultados))) cont++; //Cuenta la cantidad de datos que hay en el archivo binario
			while(cant <= cont){ //Mientras no se inserten todos los datos
				archin.close();
				archin.open("LTLbinResultados.dat",ios::binary|ios::in|ios::out); //Reinicia el archivo
				obtenido = 1;
				almacenado = 0;
				agregar.idM = 50000; //Se asigna un valor muy grande
				cant++;
				while(archin.read((char *)&aux,sizeof(Resultados))){ //Lee todo el archivo
					obtenido++; //Se aumenta para saber que numero de registro se esta guardando
					if(aux.idM < agregar.idM){ //Obtiene el que hizo menos ciclos
						for(int i=0; i<cant; i++){
							if(listos!=NULL){
								if(obtenido == listos[i]){ //Si el más chico ya esta en el archivo
									band = true; //Enciende la bandera que ese no lo debe guardar
								}
							}
						}
						if(!band){ //Si la bandera es falsa, guarda el nuevo elemento 
							anterior = agregar;
							agregar = aux;
							almacenado = obtenido;
						}
						band = false; //Reinicia la bandera en falso
					}
				}
				respaldo = listos;
				listos = new int[cant];
				if(respaldo!=NULL){
					for(int i=0; i<cant-1;i++){
						listos[i] = respaldo[i];
					}
					delete respaldo;
				}
				listos[cant-1] = almacenado;
				if(agregar.idM != anterior.idM){
					archout << "\t*************************************************\n";
					archout << "\t*                                               *\n";
					archout << "\t*         Estadisticas de la simulacion         *\n";
					archout << "\t*                                               *\n";
					archout << "\t*    Ciclos promedio en finalizar: " << agregar.ciclos << " ciclos" << "\t*\n";
					archout << "\t*    Cantidad de procesos iniciados: " << agregar.idM << "\t\t*\n";
					archout << "\t*    Procesos que esperaron para entrar: " << agregar.contTermProceso << "\t*\n";
					archout << "\t*    Tiempo promedio de atencion: " << setprecision(4) << agregar.sumTiempo << "\t\t*\n";
					archout << "\t*                                               *\n";
					archout << "\t*                                               *\n";
					archout << "\t*************************************************\n\n";
				}
			}
			archin.close();
			archout.close();
		}

		void LTL::ReporteEspera(){
			fstream archin, archout;
			char archNombre[TAMSTRING];
			Resultados agregar, aux, anterior;
			int cont = 0, cant = 0, obtenido = 0, almacenado = 0;
			int *respaldo = NULL, *listos = NULL;
			bool band = false;
			cout << "\n\tNombre del archivo que desea: ";
			cin >> archNombre;
			strcat(archNombre,".txt");
			archout.open(archNombre,ios::app|ios::in|ios::out);
			archin.open("LTLbinResultados.dat",ios::binary|ios::in|ios::out);
			while(archin.read((char *)&aux,sizeof(Resultados))) cont++; //Cuenta la cantidad de datos que hay en el archivo binario
			while(cant <= cont){ //Mientras no se inserten todos los datos
				archin.close();
				archin.open("LTLbinResultados.dat",ios::binary|ios::in|ios::out); //Reinicia el archivo
				obtenido = 1;
				almacenado = 0;
				agregar.contTermProceso = 50000; //Se asigna un valor muy grande
				cant++;
				while(archin.read((char *)&aux,sizeof(Resultados))){ //Lee todo el archivo
					obtenido++; //Se aumenta para saber que numero de registro se esta guardando
					if(aux.contTermProceso < agregar.contTermProceso){ //Obtiene el que hizo menos ciclos
						for(int i=0; i<cant; i++){
							if(listos!=NULL){
								if(obtenido == listos[i]){ //Si el más chico ya esta en el archivo
									band = true; //Enciende la bandera que ese no lo debe guardar
								}
							}
						}
						if(!band){ //Si la bandera es falsa, guarda el nuevo elemento 
							anterior = agregar;
							agregar = aux;
							almacenado = obtenido;
						}
						band = false; //Reinicia la bandera en falso
					}
				}
				respaldo = listos;
				listos = new int[cant];
				if(respaldo!=NULL){
					for(int i=0; i<cant-1;i++){
						listos[i] = respaldo[i];
					}
					delete respaldo;
				}
				listos[cant-1] = almacenado;
				if(agregar.contTermProceso != anterior.contTermProceso){
					archout << "\t*************************************************\n";
					archout << "\t*                                               *\n";
					archout << "\t*         Estadisticas de la simulacion         *\n";
					archout << "\t*                                               *\n";
					archout << "\t*    Ciclos promedio en finalizar: " << agregar.ciclos << " ciclos" << "\t*\n";
					archout << "\t*    Cantidad de procesos iniciados: " << agregar.idM << "\t\t*\n";
					archout << "\t*    Procesos que esperaron para entrar: " << agregar.contTermProceso << "\t*\n";
					archout << "\t*    Tiempo promedio de atencion: " << setprecision(4) << agregar.sumTiempo << "\t\t*\n";
					archout << "\t*                                               *\n";
					archout << "\t*                                               *\n";
					archout << "\t*************************************************\n\n";
				}
			}
			archin.close();
			archout.close();
		}

//Funciones de usuario

	void mostrarLomito(){
		srand(time(NULL));
		int tam = (rand()%3)+1;
		printf("\033[1;33m");
		switch(tam){
			case 1: cout<<"\n";
			cout<<"\t __      _\n";
			cout<<"\to'')}____//\n";
			cout<<"\t`_/      )\n";
			cout<<"\t(_(_/-(_/\n\n"; break;
			case 2: cout<<"\n";
			cout<<"\t       __       \n";
			cout<<"\t      /  \\      \n";
			cout<<"\t     / ..|\\     \n";
			cout<<"\t    (_\\  |_)    \n";
			cout<<"\t     /  \\@'     \n";
			cout<<"\t    /     \\     \n";
			cout<<"\t_  /  `   |     \n";
			cout<<"\t\\/  \\  | _\\    \n";
			cout<<"\t \\   /_ || \\_  \n";
			cout<<"\t  \\____)|_) \\_) \n\n";break;
			case 3: cout<<"\n";
			cout<<"\t    ___						\n";
			cout<<"\t__/ / \\						\n";
			cout<<"\t|    |/\\						\n";
			cout<<"\t|_--\\   \\              /-      \n";
			cout<<"\t     \\   \\-___________/ /		\n";
			cout<<"\t      \\                :		\n";
			cout<<"\t      |                :		\n";
			cout<<"\t      |       ___ \\    )		\n";
			cout<<"\t      \\|  __/     \\  )			\n";
			cout<<"\t       | /         \\  \\		\n";
			cout<<"\t       |l           ( l		\n";
			cout<<"\t       |l            ll		\n";
			cout<<"\t       |l            |l		\n";
			cout<<"\t      / l           / l		\n";
			cout<<"\t      --/           --			\n\n"; break;
			default: cout<<"\tEliga una opcion valida...\n\n";
		}
		printf("\033[0m");
	}

	bool Verificar(IngreUsu Log){
		fstream arch;
		IngreUsu temp;
		bool band;
		arch.open("Registros.txt", ios::in);
		arch.seekg(0);
		while(arch >> temp.Usuario >> temp.Contrasena){
			if(Log.Usuario==temp.Usuario){
				if(Log.Contrasena==temp.Contrasena){	
					band = true;
					break;
				}
				else{
					band = false;
				}
				
			}
			else{
				band = false;
			}
		}
		if(band){
			cout << "\tBienvenido " << temp.Usuario << endl;	
		}
		else{
			cout << "\tError en el usuario o la contrasena" << endl;
		}
		Sleep(500);
		arch.close();
		return band;
	}
	
	void Registrar(IngreUsu Log){
		fstream arch;
		arch.open("Registros.txt", ios::out|ios::app);
		arch.seekp(0,ios::end);
		arch << Log.Usuario << " " << Log.Contrasena << endl;
		arch.close();
	}

	void UsuariosReg(){
		fstream arch;
		IngreUsu temp;
		arch.open("Registros.txt",ios::in|ios::app);
		while(arch >> temp.Usuario >> temp.Contrasena){
			cout << "\tUsuario: " << temp.Usuario << "\t" << temp.Contrasena << endl;
		}
		arch.close();
	}

	void IniciarInvitado(){
		fstream arch;
		std::string texto;
		cout << "\tIngrese un nombre para registrarlo: ";
		cin >> texto;
		arch.open("Invitados.txt",ios::out|ios::app);
		arch.seekp(0,ios::end);
		arch << texto << endl;
		arch.close();
	}

	void VerificarInvitados(){
		fstream arch;
		std::string texto;
		arch.open("Invitados.txt",ios::in);
		while(arch >> texto)
		cout << "\tinvitado registrado: " << texto << endl;
		arch.close();
	}
	
//Archivos

	void GenerarArch(){
		fstream aux1,aux2,aux3,aux4;
		aux1.open("RoundRobinResultados.txt",ios::in);
		aux2.open("PEPSResultados.txt",ios::in);
		aux3.open("LTLResultados.txt",ios::in);
		aux4.open("SJFResultados.txt",ios::in);
		if(!(aux1)){
			aux1.open("RoundRobinResultados.txt",ios::app);
		}
		if(!(aux2)){
			aux2.open("PEPSResultados.txt",ios::app);
		}
		if(!(aux3)){
			aux3.open("LTLResultados.txt",ios::app);
		}
		if(!(aux4)){
			aux4.open("SJFResultados.txt",ios::app);
		}
		aux1.close();
		aux2.close();
		aux3.close();
		aux4.close();
		aux1.open("RoundRobinBinResultados.dat",ios::binary|ios::in);
		aux2.open("PEPSbinResultados.dat",ios::binary|ios::in);
		aux3.open("LTLbinResultados.dat",ios::binary|ios::in);
		aux4.open("SJFbinResultados.dat",ios::binary|ios::in);
		if(!(aux1)){
			aux1.open("RoundRobinBinResultados.dat",ios::binary|ios::app);
		}
		if(!(aux2)){
			aux2.open("PEPSbinResultados.dat",ios::binary|ios::app);
		}
		if(!(aux3)){
			aux3.open("LTLbinResultados.dat",ios::binary|ios::app);
		}
		if(!(aux4)){
			aux4.open("SJFbinResultados.dat",ios::binary|ios::app);
		}
		aux1.close();
		aux2.close();
		aux3.close();
		aux4.close();
		aux1.open("configuracionRoundRobin.dat",ios::binary|ios::in);
		aux2.open("configuracionPEPS.dat",ios::binary|ios::in);
		aux3.open("configuracionSJF.dat",ios::binary|ios::in);
		aux4.open("configuracionLTL.dat",ios::binary|ios::in);
		if(!(aux1)){
			aux1.open("configuracionRoundRobin.dat",ios::binary|ios::app);
		}
		if(!(aux2)){
			aux2.open("configuracionPEPS.dat",ios::binary|ios::app);
		}
		if(!(aux3)){
			aux3.open("configuracionSJF.dat",ios::binary|ios::app);
		}
		if(!(aux4)){
			aux4.open("configuracionLTL.dat",ios::binary|ios::app);
		}
		aux1.close();
		aux2.close();
		aux3.close();
		aux4.close();
		aux1.open("GeneralResultados.dat",ios::binary|ios::in);
		if(!(aux1)){
			aux1.open("GeneralResultados.dat",ios::binary|ios::app);
		}
		aux1.close();
	}

//Funciones para los planificadores

	Planificadores::Planificadores() {
		actual.cuanto = actual.cuantoCiclo = actual.id = actual.tam = actual.tiempo = 0;
		res.ciclos = res.sumTiempo = res.contTermProceso = configuracion.cuantosM = configuracion.cuantosProceso = res.idM = configuracion.MemoMaxProc = configuracion.NumProceso = 0;
		prin.espacioMemoria = prin.tam = 0;
		prin.cola = NULL;
		prin.ocupado = NULL;
		procesosListos = NULL;
	}
	
	Planificadores::~Planificadores() {
		
	}
	
	void Planificadores::Saltos(int a){
		Sleep(a);
		system("cls");
		cout << endl << endl;
	}
	
//Funciones para procesos

	Lista* Planificadores::Crea() {//Estructuras... Crear NODO
		return new Lista;
	}
	
	Lista* Planificadores::Inserta(Lista *ListaFin) {//Estructuras... Insertar al final
		Lista* q = NULL, * p = ListaFin;
		if (ListaFin == NULL) {
			ListaFin = Crea();
			ListaFin->listado.tam = actual.tam;
			ListaFin->listado.cuanto = actual.cuanto;
			ListaFin->listado.cuantoCiclo = actual.cuantoCiclo;
			ListaFin->listado.id = actual.id;
			ListaFin->listado.espacios = actual.espacios;
			ListaFin->listado.tiempo = actual.tiempo;
			ListaFin->liga = NULL;
		}
		else {
			while (p->liga != NULL) p = p->liga;
			q = Crea();
			q->listado.tam = actual.tam;
			q->listado.cuanto = actual.cuanto;
			q->listado.cuantoCiclo = actual.cuantoCiclo;
			q->listado.id = actual.id;
			q->listado.espacios = actual.espacios;
			q->listado.tiempo = actual.tiempo;
			q->liga = NULL;
			p->liga = q;
		}
		return ListaFin;
	}
	
	void Planificadores::GenerarProceso() {
		Lista *q=NULL;
		srand(time(NULL));
		if(procesosNoListos==NULL){
			actual.cuanto = (rand() % configuracion.cuantosProceso) + 1;/*rand(rand() % (genera numeros, menores al numero que se inserte en este parentesis))*/
			actual.tam = TamAleatorio();//Le da un tamano aleatorio a un proceso
			actual.espacios = actual.tam/TAMPART;
			while((actual.tam) > (actual.espacios*TAMPART)){
				actual.espacios++;
			}
			actual.id = ++res.idM;//Le da un identificador al proceso nuevo (siempre id del proceso nuevo > id del proceso anterior)
			actual.cuantoCiclo = (rand() % configuracion.cuantosM) + 1;/*Cantidad de cuantos que puede hacer en cada ciclo*/
			if (verificarEspacio()) { //Se crea un proceso, pero se tiene que verificar que se pueda ingresar en memoria 
				procesosListos = Inserta(procesosListos);//Si hay memoria disponible se inserta el proceso lista de procesos listos
				/*Se podria sacar la linea 103, para que aunque no haya RAM disponible, aun asi se acomode el proceso a la lista de listos*/
				AsignarMemoria();//Ingresa el proceso en la cola de la memoria RAM (espacio en memoria)
				cout << "\n\tEntra Proceso " << actual.id << ": [" << actual.id << ", " << actual.tam << ", " << actual.cuanto << "]";
			}
			else {
				cout << "\n\tMemoria insuficiente, no se puede cargar... [" << actual.id << ", " << actual.tam << ", " << actual.cuanto << "]"; //Si no hay espacio en memoria, no la asigna
				procesosNoListos=Inserta(procesosNoListos);
				res.contTermProceso++;
				Sleep(500);
			}
			TodosProcesos = Inserta(TodosProcesos);
		}
		
		else{
			q=procesosNoListos;
			while(q!=NULL){
				actual=procesosNoListos->listado;
				if (verificarEspacio()) { //Se crea un proceso, pero se tiene que verificar que se pueda ingresar en memoria 
					procesosListos = Inserta(procesosListos);//Si hay memoria disponible se inserta el proceso lista de procesos listos
					/*Se podria sacar la linea 103, para que aunque no haya RAM disponible, aun asi se acomode el proceso a la lista de listos*/
					AsignarMemoria();//Ingresa el proceso en la cola de la memoria RAM (espacio en memoria)
					cout << "\n\tEntra Proceso " << actual.id << ": [" << actual.id << ", " << actual.tam << ", " << actual.cuanto << "]";
					eliminaProcesoNo();
				}
				else {
					cout << "\n\tMemoria insuficiente, no se puede cargar... [" << actual.id << ", " << actual.tam << ", " << actual.cuanto << "]"; //Si no hay espacio en memoria, no la asigna
					Sleep(500);
				}	
				q=q->liga;
			}	
		}
		manejoMemoria();
	}
	
	int Planificadores::TamAleatorio() {
		int auxAleatorio;//Funciona como una variable opc en un menu
		srand(time(NULL));
		auxAleatorio = rand() % 9;
		switch (auxAleatorio) {
			case 0: auxAleatorio = configuracion.MemoMaxProc / 16; break;
			case 1: auxAleatorio = configuracion.MemoMaxProc / 14; break;
			case 2: auxAleatorio = configuracion.MemoMaxProc / 12; break;
			case 3: auxAleatorio = configuracion.MemoMaxProc / 10; break;
			case 4: auxAleatorio = configuracion.MemoMaxProc / 8; break;
			case 5: auxAleatorio = configuracion.MemoMaxProc / 6; break;
			case 6: auxAleatorio = configuracion.MemoMaxProc / 4; break;
			case 7: auxAleatorio = configuracion.MemoMaxProc / 2; break;
			case 8: auxAleatorio = configuracion.MemoMaxProc; break;
			default: auxAleatorio = 0; break;
		}
		if(auxAleatorio == 0){
			auxAleatorio = 1;
		}
		return auxAleatorio;
	}
	
	Lista* Planificadores::eliminaProceso() {
		Lista* q = procesosListos;
		/*Copy-Paste de Planificadores::verificarEspacio()*/
		for (int i = 0; i < prin.espacioMemoria; i++) {//Checa los espacios en memoria RAM
			if (prin.cola[i] == actual.id) {//Si en el espacio analizado esta el id del proceso a eliminar...
				prin.cola[i] = 0;//Se pone un 0 para saber que no hay ningun proceso ahi
				prin.ocupado[i] = false;//Los mismos espacios cambian a false para decir que ya no hay procesos
			}
		}
		res.ciclos += actual.tiempo;
		if (procesosListos != NULL) {
			procesosListos = q->liga;
			delete(q);
		}
		return procesosListos;
	}
	
	Lista* Planificadores::eliminaProcesoNo() {
		Lista* q = procesosNoListos;
		/*Copy-Paste de Planificadores::verificarEspacio()*/
		if (procesosNoListos->listado.id == actual.id) {
			procesosNoListos = procesosNoListos->liga;
			delete(q);
		}
		return procesosNoListos;
	}

//Funciones para la memoria

	void Planificadores::GenerarMemoria() {
		prin.espacioMemoria = MemSelec;
		prin.cola = new int[MemSelec];//Se crea un vector de tamano de megas que eligio el ususario entre 64 (Ej. con la primera opcion 1024/64;)
		prin.ocupado = new bool[MemSelec]; //Se crea un vector identico al anterior solo que guardara bools :D
		for(int i=0; i<MemSelec; i++){
			prin.cola[i] = 0;
			prin.ocupado[i] = false;
		}
		
	}
	
	void Planificadores::AsignarMemoria() {
		int cont = 0/*Es un indice para saber en que espacio se puede almacenar*/;
		int *vec;/*Vector del tamano de la cantidad de espacios que necesitara el proceso para ejecutarse*/
		vec = new int[actual.espacios];
		for (int i = 0; i < prin.espacioMemoria; i++) {
			if (!prin.ocupado[i]) {//Checa la cola de bool para saber si ese espacio en especifico esta libre para almacenar al proceso (o una parte del proceso)
				if (cont <= actual.espacios) {//Si el contador es menor o igual al tamano que necesitamos
					vec[cont++] = i;//Guarda la posicion(es) de la RAM que esta disponible 
					if (cont == actual.espacios) {//Si ya se tiene el numero exacto de espacios que necesita el proceso para ejecutarse
						break;//Se sale del for
					}
				}
			}
			else if (cont < actual.espacios) {//Si no hay espacios contiguos de memoria para almacenar al proceso
				cont = 0;//El contador se reinicia 
			}
		}
		if (cont == actual.espacios) {//Si se inserto el proceso:
			for (int i = 0; i < actual.espacios; i++) {
				prin.cola[vec[i]] = actual.id;//Se guarda en los espacios de memoria  corrspondiente el ID  del proceso insertado
				prin.ocupado[vec[i]] = true;//Se llena la misma cantidad de trues en la cola de bools para indicar que esos espacios tienen un proceso
			}
		}
	}
	
	bool Planificadores::verificarEspacio() {//Verifiqua si hay espacios disponibles en RAM para almacenar un proceso
		int cont = 0;
		for (int i = 0; i < prin.espacioMemoria; i++) {//Checa los espacios en memoria RAM
			if (!prin.ocupado[i]) {//Si hay espacios libres contiguos
				cont++;//El contador aumenta
				if (cont == actual.espacios) {
					break;
				}
			}
			else if (cont < actual.espacios) {//Si no hay espacios contiguos
				cont = 0;//Se reinicia el cont
			}
		}
		if (cont >= actual.espacios) {//Si hay espacio en RAM suficiente o mayor, retorna true
			//prin.ocupado[prin.espacioMemoria-1]=true;
			return true;
		}
		else return false;// Si no, un false
	}
	
	void Planificadores::manejoMemoria(){
		int disponibleMemoria, espaciosOcupados = 0, contProcesos = 0, espacioProceso, j = 1;
		Lista *p = NULL;
		cout << "\n\tProcesos en memoria:\n\t";
		for(int i=0; i<prin.espacioMemoria; i++){ //Mientras no sea el final de la memoria ram...
			if(j%5 == 0){//Hace salto cada que muestre 5 datos
				cout << "\n\t";
			}
			disponibleMemoria = 0;
			while(prin.cola[i] == 0){//Obtiene cuanta memoria vacia hay
				if(i>=prin.espacioMemoria) break;
				disponibleMemoria += TAMPART;
				i++;
			}
			if(disponibleMemoria != 0){//Si hay huecos, los muestra
				cout << "[" << 0 << ", " << disponibleMemoria << ", " << 0 << "]";
				i--;
				j++;
			}
			p = procesosListos;
			while(p!=NULL){ //Mientras haya procesos
				if(p!=NULL){
					if(p->listado.id == prin.cola[i]){ //Si coincide el ID con el proceso lo muestra
						//Saca el proceso en pantalla si el ID se encuentra en memoria
						cout << "[" << p->listado.id << ", " << p->listado.tam << ", " << p->listado.cuanto << "]";
						espacioProceso = (p->listado.espacios) * TAMPART; //Si tiene una fragmentaci�n interna...
						if((p->listado.tam) < espacioProceso){ //Muestra la fragmentacion interna que tiene
							espacioProceso = espacioProceso - (p->listado.tam);
							cout << "[" << 0 << ", " << espacioProceso << ", " << 0 << "]"; //Hueco
							j++;
						}
						j++;
						while(p->listado.id == prin.cola[i]) i++; //Si hay m�s hucos que ocupe el proceso los salta
						i--;
						break; //Si ya encontro el proceso con el ID, rompe el ciclo
					}
				}
				p = p->liga;
			}
		}
	}
	
	void Planificadores::mostrarMemoria(){
		int disponibleMemoria, espaciosOcupados = 0, contProcesos = 0, espacioProceso, j = 1;
		Lista *p = NULL;
		cout << "\n\tProcesos en memoria:\n\t";
		for(int i=0; i<prin.espacioMemoria; i++){ //Mientras no sea el final de la memoria ram...
			if(j%5 == 0){//Hace salto cada que muestre 5 datos
				cout << "\n\t";
			}
			disponibleMemoria = 0;
			while(prin.cola[i] == 0){//Obtiene cuanta memoria vacia hay
				if(i>=prin.espacioMemoria) break;
				disponibleMemoria += TAMPART;
				i++;
			}
			if(disponibleMemoria != 0){//Si hay huecos, los muestra
				cout << "[" << 0 << ", " << disponibleMemoria << ", " << 0 << "]";
				i--;
				j++;
			}
			p = procesosListos;
			while(p!=NULL){ //Mientras haya procesos
				if(p!=NULL){
					if(p->listado.id == prin.cola[i]){ //Si coincide el ID con el proceso lo muestra
						//Saca el proceso en pantalla si el ID se encuentra en memoria
						cout << "[0, " << p->listado.tam << ", 0]";
						espacioProceso = (p->listado.espacios) * TAMPART; //Si tiene una fragmentaci�n interna...
						if((p->listado.tam) < espacioProceso){ //Muestra la fragmentacion interna que tiene
							espacioProceso = espacioProceso - (p->listado.tam);
							cout << "[" << 0 << ", " << espacioProceso << ", " << 0 << "]"; //Hueco
							j++;
						}
						j++;
						while(p->listado.id == prin.cola[i]) i++; //Si hay m�s hucos que ocupe el proceso los salta
						i--;
						break; //Si ya encontro el proceso con el ID, rompe el ciclo
					}
				}
				p = p->liga;
			}
		}
	}
	
	void Planificadores::listaProcesos(){
		int i=0;
		Lista *p= procesosListos;
		if(p!=NULL){
			cout << "\n\tLista de procesos:\n\t";
		}
		while(p!=NULL){
			if(p!=NULL){
				//Saca el proceso en pantalla
				cout << "[" << p->listado.id << ", " << p->listado.tam << ", " << p->listado.cuanto << "]";
			}
			p = p->liga;
			if(++i%5 == 0){
				cout << "\n\t";
			}
		}
		Sleep(4000);
	}

//LTL Listo
	
	void LTL::inicia(){
		cout << "\t\tINICIANDO... " << endl;
		Saltos(1000);
		GenerarMemoria();
		Saltos(1000);
		manejoMemoria();
		cout << endl;
		if(configuracion.NumProceso>0){
			GenerarProceso();
			while (procesosListos != NULL){
				AsignarCuanto();
				cout << endl;
				res.sumTiempo++;
				if (res.idM < configuracion.NumProceso){
					GenerarProceso();	
				} 
				else if(procesosNoListos!=NULL){
					GenerarProceso();	
				} 
			}
		}
		if(res.ciclos != 0) {
			res.ciclos /= res.idM;
		}
		if(res.sumTiempo!=0){
			res.sumTiempo /= res.idM;
		}
	
	}

	void LTL::AsignarCuanto(){
		procesosListos = menosTiempo();
		actual = procesosListos->listado;
		cout << "\n\tAtendiendo proceso " << actual.id << ": [" << actual.id << ", " << actual.tam << ", " << actual.cuanto << "]";
		actual.cuanto -= actual.cuantoCiclo;
		actual.tiempo++; //Aumenta el tiempo que tarda el proceso en ejecutarse
		if (actual.cuanto <= 0) {
			actual.cuanto = 0;
		}
		
		//cout << "\t[" << actual.id << ", " << actual.tam << ", " << actual.cuanto << ", " << actual.cuantoCiclo<< "]" <<endl;
		Sleep(500);
		if (actual.cuanto == 0){
			cout << "\n\t>Descargando proceso " << actual.id << "<";
			mostrarMemoria();
			procesosListos = eliminaProceso();
			cout << "\n\t>Condensando<";
			manejoMemoria();
		}else{
			procesosListos->listado = actual;
			manejoMemoria();
		}
		procesosListos = menosTiempo();
		listaProcesos();
	}

	Lista * LTL::menosTiempo(){//Editar
		Lista *t = procesosListos, *q = NULL, *peq = NULL, *ant = NULL;
		bool band = false, band1 = false;
		int cuantosMenor = configuracion.MemoMaxProc;
		while(t!=NULL){
			if(!band){
				q = NULL;
				t = procesosListos;
			}else{
				q = t;
				t = t->liga;
			}
			if(t!=NULL){
				if(cuantosMenor > (t->listado.cuanto)){
					cuantosMenor = (t->listado.cuanto);
					peq = t;
					ant = q;
				}
			}
			band = true;
		}
		if(ant!=NULL){
			cout << "Proceso anterior: " << ant->listado.id;
			actual = peq->listado;
			ant->liga = peq->liga;
			q = Crea();
			q->listado = actual;
			q->liga = procesosListos;
			procesosListos = q;
			delete peq;
		}
		return procesosListos;
	}
	 
	LTL::LTL(){
		actual.cuanto = actual.cuantoCiclo = actual.id = actual.tam = actual.tiempo = 0;
		res.ciclos = res.sumTiempo = res.contTermProceso = configuracion.cuantosM = configuracion.cuantosProceso = res.idM = configuracion.MemoMaxProc = configuracion.NumProceso = 0;
		prin.espacioMemoria = prin.tam = 0;
		prin.cola = NULL;
		prin.ocupado = NULL;
		procesosListos = NULL;
	}
	//ANALIZAR PARA CONSTRUCTOR COPIA
	LTL::LTL(const LTL &obj){
		this->MemPart = obj.MemPart;
		this->prin.tam = obj.prin.tam;
		this->MemSelec = obj.MemSelec;
		this->configuracion.MemoriaMax = obj.configuracion.MemoriaMax;
		this->configuracion.cuantosM = obj.configuracion.cuantosM;
		this->configuracion.cuantosProceso = obj.configuracion.cuantosProceso;
		this->configuracion.NumProceso = obj.configuracion.NumProceso;
		this->configuracion.MemoMaxProc = obj.configuracion.MemoMaxProc;
		this->configuracion.cuantosProceso = obj.configuracion.cuantosProceso;
		this->actual.cuanto = obj.actual.cuanto;
		this->actual.cuantoCiclo = obj.actual.cuantoCiclo;
		this->actual.id = obj.actual.id;
		this->actual.tam = obj.actual.tam;
		this->actual.tiempo = obj.actual.tiempo;
		this->actual.espacios = obj.actual.espacios;
		this->res.ciclos = obj.res.ciclos;
		this->res.sumTiempo = obj.res.sumTiempo;
		this->res.contTermProceso = obj.res.contTermProceso; 
		this->res.idM = obj.res.idM;
		this->prin.espacioMemoria = prin.tam = 0;
		this->prin.cola = obj.prin.cola;
		this->prin.ocupado = obj.prin.ocupado;
		this->procesosListos = obj.procesosListos;
	}
	LTL::LTL(int maxCuantos, int cuantosNecesario, int tamExactoProce, int ramDeseada, int cantProcesos){
		configuracion.cuantosM = maxCuantos;
		configuracion.cuantosProceso = cuantosNecesario;
		configuracion.MemoMaxProc = tamExactoProce;
		configuracion.MemoriaMax = ramDeseada;
		configuracion.NumProceso = cantProcesos;
	}
	LTL::~LTL(){
		cout << "\t Finalizando Planificador LTL" << endl;
		Sleep(500);
	}

//SJF Listo

	void SJF::inicia(){
		cout << "\t\tINICIANDO... " << endl;
		Saltos(1000);
		GenerarMemoria();
		Saltos(1000);
		manejoMemoria();
		cout << endl;
		if(configuracion.NumProceso>0){
			GenerarProceso();
			while (procesosListos != NULL){
				AsignarCuanto();
				cout << endl;
				res.sumTiempo++;
				if (res.idM < configuracion.NumProceso){
					GenerarProceso();	
				} 
				else if(procesosNoListos!=NULL){
					GenerarProceso();	
				} 
			}
		}
		if(res.ciclos != 0) {
			res.ciclos /= res.idM;
		}
		if(res.sumTiempo!=0){
			res.sumTiempo /= res.idM;
		}
		
	}
		
	void SJF::AsignarCuanto(){
		actual = procesosListos->listado;
		cout << "\n\tAtendiendo proceso " << actual.id << ": [" << actual.id << ", " << actual.tam << ", " << actual.cuanto << "]";
		actual.cuanto -= actual.cuantoCiclo;
		actual.tiempo++; //Aumenta el tiempo que tarda el proceso en ejecutarse
		if (actual.cuanto <= 0) {
			actual.cuanto = 0;
		}

		//cout << "\t[" << actual.id << ", " << actual.tam << ", " << actual.cuanto << ", " << actual.cuantoCiclo<< "]" <<endl;
		Sleep(500);
		if (actual.cuanto == 0){
			cout << "\n\t>Descargando proceso " << actual.id << "<";
			mostrarMemoria();
			procesosListos = eliminaProceso();
			procesosListos = masPeq();
			cout << "\n\t>Condensando<";
			manejoMemoria();
		}
		else{
			procesosListos->listado = actual;
			manejoMemoria();
		}
		listaProcesos();
	}
		
	Lista * SJF::masPeq(){//Listo
		Lista *t = procesosListos, *q = NULL, *peq = NULL, *ant = NULL;
		bool band = false, band1 = false;
		int cuantosMenor = configuracion.MemoMaxProc;
		while(t!=NULL){
			if(!band){
				q = NULL;
				t = procesosListos;
			}else{
				q = t;
				t = t->liga;
			}
			if(t!=NULL){
				if(cuantosMenor > (t->listado.cuanto)){
					cuantosMenor = (t->listado.cuanto);
					peq = t;
					ant = q;
				}
			}
			band = true;
		}
		if(ant!=NULL){
			actual = peq->listado;
			ant->liga = peq->liga;
			q = Crea();
			q->listado = actual;
			q->liga = procesosListos;
			procesosListos = q;
			delete peq;
		}
		return procesosListos;
	}
		
	SJF::SJF(){
		actual.cuanto = actual.cuantoCiclo = actual.id = actual.tam = actual.tiempo = 0;
		res.ciclos = res.sumTiempo = res.contTermProceso = configuracion.cuantosM = configuracion.cuantosProceso = res.idM = configuracion.MemoMaxProc = configuracion.NumProceso = 0;
		prin.espacioMemoria = prin.tam = 0;
		prin.cola = NULL;
		prin.ocupado = NULL;
		procesosListos = NULL;
	}
	SJF::SJF(int maxCuantos, int cuantosNecesario, int tamExactoProce, int ramDeseada, int cantProcesos){
		configuracion.cuantosM = maxCuantos;
		configuracion.cuantosProceso = cuantosNecesario;
		configuracion.MemoMaxProc = tamExactoProce;
		configuracion.MemoriaMax = ramDeseada;
		configuracion.NumProceso = cantProcesos;
	}
	SJF::SJF(const SJF &obj){
		this->MemPart = obj.MemPart;
		this->prin.tam = obj.prin.tam;
		this->MemSelec = obj.MemSelec;
		this->configuracion.MemoriaMax = obj.configuracion.MemoriaMax;
		this->configuracion.cuantosM = obj.configuracion.cuantosM;
		this->configuracion.cuantosProceso = obj.configuracion.cuantosProceso;
		this->configuracion.NumProceso = obj.configuracion.NumProceso;
		this->configuracion.MemoMaxProc = obj.configuracion.MemoMaxProc;
		this->configuracion.cuantosProceso = obj.configuracion.cuantosProceso;
		this->actual.cuanto = obj.actual.cuanto;
		this->actual.cuantoCiclo = obj.actual.cuantoCiclo;
		this->actual.id = obj.actual.id;
		this->actual.tam = obj.actual.tam;
		this->actual.tiempo = obj.actual.tiempo;
		this->actual.espacios = obj.actual.espacios;
		this->res.ciclos = obj.res.ciclos;
		this->res.sumTiempo = obj.res.sumTiempo;
		this->res.contTermProceso = obj.res.contTermProceso; 
		this->res.idM = obj.res.idM;
		this->prin.espacioMemoria = prin.tam = 0;
		this->prin.cola = obj.prin.cola;
		this->prin.ocupado = obj.prin.ocupado;
		this->procesosListos = obj.procesosListos;
	}
		
	SJF::~SJF(){
		cout << "\t Finalizando Planificador SJF" << endl;
		Sleep(500);
	}

//PEPS Listo

	void PEPS::inicia(){
		cout << "\t\tINICIANDO... " << endl;
		Saltos(1000);
		GenerarMemoria();
		Saltos(1000);
		manejoMemoria();
		cout << endl;
		if(configuracion.NumProceso>0){
			GenerarProceso();
			while (procesosListos != NULL){
				AsignarCuanto();
				cout << endl;
				res.sumTiempo++;
				if (res.idM < configuracion.NumProceso){
					GenerarProceso();	
				} 
				else if(procesosNoListos!=NULL){
					GenerarProceso();	
				} 
			}
		}
		if(res.ciclos != 0) {
			res.ciclos /= res.idM;
		}
		if(res.sumTiempo!=0){
			res.sumTiempo /= res.idM;
		}
		
	}

	void PEPS::AsignarCuanto() {//Asigna un cuanto al proceso
		actual = procesosListos->listado;
		cout << "\n\tAtendiendo proceso " << actual.id << ": [" << actual.id << ", " << actual.tam << ", " << actual.cuanto << "]";
		actual.cuanto -= actual.cuantoCiclo;
		actual.tiempo++; //Aumenta el tiempo que tarda el proceso en ejecutarse
		if (actual.cuanto <= 0) {
			actual.cuanto = 0;
		}
		//cout << "\t[" << actual.id << ", " << actual.tam << ", " << actual.cuanto << ", " << actual.cuantoCiclo<< "]" <<endl;
		Sleep(500);
		if (actual.cuanto == 0){
			cout << "\n\t>Descargando proceso " << actual.id << "<";
			mostrarMemoria();
			procesosListos = eliminaProceso();
			cout << "\n\t>Condensando<";
			manejoMemoria();
		}
		else{
			procesosListos->listado = actual;
			manejoMemoria();
		}
		listaProcesos();
	}

	PEPS::~PEPS() {
		cout << "\t Finalizando Planificador PEPS" << endl;
		Sleep(500);
	}
	PEPS::PEPS(const PEPS &obj){
		this->MemPart = obj.MemPart;
		this->prin.tam = obj.prin.tam;
		this->MemSelec = obj.MemSelec;
		this->configuracion.MemoriaMax = obj.configuracion.MemoriaMax;
		this->configuracion.cuantosM = obj.configuracion.cuantosM;
		this->configuracion.cuantosProceso = obj.configuracion.cuantosProceso;
		this->configuracion.NumProceso = obj.configuracion.NumProceso;
		this->configuracion.MemoMaxProc = obj.configuracion.MemoMaxProc;
		this->configuracion.cuantosProceso = obj.configuracion.cuantosProceso;
		this->actual.cuanto = obj.actual.cuanto;
		this->actual.cuantoCiclo = obj.actual.cuantoCiclo;
		this->actual.id = obj.actual.id;
		this->actual.tam = obj.actual.tam;
		this->actual.tiempo = obj.actual.tiempo;
		this->actual.espacios = obj.actual.espacios;
		this->res.ciclos = obj.res.ciclos;
		this->res.sumTiempo = obj.res.sumTiempo;
		this->res.contTermProceso = obj.res.contTermProceso; 
		this->res.idM = obj.res.idM;
		this->prin.espacioMemoria = prin.tam = 0;
		this->prin.cola = obj.prin.cola;
		this->prin.ocupado = obj.prin.ocupado;
		this->procesosListos = obj.procesosListos;
	}
	PEPS::PEPS(int maxCuantos, int cuantosNecesario, int tamExactoProce, int ramDeseada, int cantProcesos){
		configuracion.cuantosM = maxCuantos;
		configuracion.cuantosProceso = cuantosNecesario;
		configuracion.MemoMaxProc = tamExactoProce;
		configuracion.MemoriaMax = ramDeseada;
		configuracion.NumProceso = cantProcesos;
	}
	PEPS::PEPS() {
		actual.cuanto = actual.cuantoCiclo = actual.id = actual.tam = actual.tiempo = 0;
		res.ciclos = res.sumTiempo = res.contTermProceso = configuracion.cuantosM = configuracion.cuantosProceso = res.idM = configuracion.MemoMaxProc = configuracion.NumProceso = 0;
		prin.espacioMemoria = prin.tam = 0;
		prin.cola = NULL;
		prin.ocupado = NULL;
		procesosListos = NULL;
	}

//Round Robin Listo

	Lista* RoundRobin::finalLista() {//Agrega el proceso al final de la lista de listos (Estructuras: Inserta al Final)
		Lista* t = procesosListos, * q = NULL;
		if (t == NULL) {
			procesosListos = Crea();
			procesosListos->listado = actual;
			procesosListos->liga = NULL;
		}
		else {
			q = Crea();
			while (t->liga != NULL) {
				t = t->liga;
			}
			q->listado = actual;
			q->liga = NULL;
			t->liga = q;
			t = procesosListos;
			procesosListos = procesosListos->liga;
			delete t;
		}
		return procesosListos;
	}

	void RoundRobin::inicia(){
		cout << "\t\tINICIANDO... " << endl;
		Saltos(1000);
		GenerarMemoria();
		Saltos(1000);
		manejoMemoria();
		cout << endl;
		if(configuracion.NumProceso>0){
			GenerarProceso();
			while (procesosListos != NULL){
				AsignarCuanto();
				cout << endl;
				res.sumTiempo++;
				if (res.idM < configuracion.NumProceso){
					GenerarProceso();	
				} 
				else if(procesosNoListos!=NULL){
					GenerarProceso();	
				} 
			}
		}
		if(res.ciclos != 0) {
			res.ciclos /= res.idM;
		}
		if(res.sumTiempo!=0){
			res.sumTiempo /= res.idM;
		}
	}

	void RoundRobin::AsignarCuanto() {//Asigna un cuanto al proceso
		actual = procesosListos->listado;
		cout << "\n\tAtendiendo proceso " << actual.id << ": [" << actual.id << ", " << actual.tam << ", " << actual.cuanto << "]";
		actual.cuanto -= actual.cuantoCiclo;
		actual.tiempo++; //Aumenta el tiempo que tarda el proceso en ejecutarse
		
		if (actual.cuanto <= 0) {
			actual.cuanto = 0;
		}
		
		//cout << "\t[" << actual.id << ", " << actual.tam << ", " << actual.cuanto << ", " << actual.cuantoCiclo<< "]" <<endl;
		Sleep(500);
		if (actual.cuanto == 0){
			cout << "\n\t>Descargando proceso " << actual.id << "<";
			mostrarMemoria();
			procesosListos = eliminaProceso();	
			cout << "\n\t>Condensando<";
			manejoMemoria();
		}
		else{
			procesosListos = finalLista();
			manejoMemoria();
		}
		listaProcesos();
	}

	RoundRobin::RoundRobin() {
		actual.cuanto = actual.cuantoCiclo = actual.id = actual.tam = actual.tiempo = 0;
		res.ciclos = res.sumTiempo = res.contTermProceso = configuracion.cuantosM = configuracion.cuantosProceso = res.idM = configuracion.MemoMaxProc = configuracion.NumProceso = 0;
		prin.espacioMemoria = prin.tam = 0;
		prin.cola = NULL;
		prin.ocupado = NULL;
		procesosListos = NULL;
	}
	RoundRobin::RoundRobin(const RoundRobin &obj){
		this->MemPart = obj.MemPart;
		this->prin.tam = obj.prin.tam;
		this->MemSelec = obj.MemSelec;
		this->configuracion.MemoriaMax = obj.configuracion.MemoriaMax;
		this->configuracion.cuantosM = obj.configuracion.cuantosM;
		this->configuracion.cuantosProceso = obj.configuracion.cuantosProceso;
		this->configuracion.NumProceso = obj.configuracion.NumProceso;
		this->configuracion.MemoMaxProc = obj.configuracion.MemoMaxProc;
		this->configuracion.cuantosProceso = obj.configuracion.cuantosProceso;
		this->actual.cuanto = obj.actual.cuanto;
		this->actual.cuantoCiclo = obj.actual.cuantoCiclo;
		this->actual.id = obj.actual.id;
		this->actual.tam = obj.actual.tam;
		this->actual.tiempo = obj.actual.tiempo;
		this->actual.espacios = obj.actual.espacios;
		this->res.ciclos = obj.res.ciclos;
		this->res.sumTiempo = obj.res.sumTiempo;
		this->res.contTermProceso = obj.res.contTermProceso; 
		this->res.idM = obj.res.idM;
		this->prin.espacioMemoria = prin.tam = 0;
		this->prin.cola = obj.prin.cola;
		this->prin.ocupado = obj.prin.ocupado;
		this->procesosListos = obj.procesosListos;
	}
	RoundRobin::RoundRobin(int maxCuantos, int cuantosNecesario, int tamExactoProce, int ramDeseada, int cantProcesos){
		configuracion.cuantosM = maxCuantos;
		configuracion.cuantosProceso = cuantosNecesario;
		configuracion.MemoMaxProc = tamExactoProce;
		configuracion.MemoriaMax = ramDeseada;
		configuracion.NumProceso = cantProcesos;
	}
	RoundRobin::~RoundRobin() {
		cout << "\t Finalizando Planificador Round Robin" << endl;
		Sleep(500);
	}

//EasterEgg

	void Grinch(){
		system("cls");
		printf("\033[0;31m");
		cout << "                       __..._\n";
		cout << "                    ,-`      `',\n";
		cout << "                  ,'            \\\n";
		cout << "                /               |\n";
		cout << "               ,'       ,        \\\n";
		cout << "             ,'       ,/-'`       \\\n";
		printf("\033[0m");
		cout << "         _";
		printf("\033[0;31m");
		cout << " ./     ,.'`/            \\\n";
		printf("\033[0m");
		cout << "      .-` `^\\";
		printf("\033[0;31m");
		cout << "_,";
		cout << ".'`";
		cout << "   /              `\\";
		printf("\033[0m");
		cout << "__\n";
		cout << "      7     / ";
		printf("\033[0;31m");
		cout << "      /   ";
		printf("\033[0m");
		cout << "_,._,.,_,.-'.`  `\\\n";
		cout << "      \\A  __/   ,-```-``   `,   `,   `  ,`)\n";
		cout << "        ^-`    /      `                 ,/\n";     
		cout << "               (        ,   ,_   ,-,_,<`\n";        
		cout << "                \\__ T--` `''` ```   ";
		printf("\033[0;32m");
		cout << " _,\\\n";
		cout << "                  \\_/|\\_         ,.-` | |\n";
		cout << "                  _/ | |T\\_   _,'Y    / +--,_\n";
		cout << "              <```   \\_\\/_/  `\\_/   /       `\\\n";
		cout << "              /  ,--   ` _,--,_`----`   _,,_   \\\n";
		cout << "             /  ` |     <_._._ >       `  \\ `  \\`\n";
		cout << "            |     |       ,   `           |     |\n";
		cout << "             V|   \\       |                |   |`\n";
		cout << "              \\    \\      /               /    /\n";
		cout << "               \\x   \\_   |             /-`    /\n";
		cout << "                 \\    `-,|        ,/--`     /`\n";
		cout << "                  \\x_    \\_  /--'`       , /\n";
		cout << "                     \\x_   ``        ,,/` `\n";
		cout << "                        `-,_,-'   ,'`\n";
		printf("\033[0m");
		cout << "                         _";
		printf("\033[0;32m");
		cout << "|       |";
		printf("\033[0m");
		cout << "`\\\n";
		cout << "                        ( `-``/``/`_/\n";
		cout << "                         `-`-,.-.-`\n";
		system("pause");
		system("cls");
	}

	void Pentagrama(){
		system("cls");
		system("color 40");
		cout << "\n\n";
		cout << "\t                  ooo OOOAOOO ooo\n";
		cout << "\t              oOO       / \\       OOo\n";
		cout << "\t          oOO          /   \\          OOo\n";
		cout << "\t       oOO            /     \\            OOo\n";
		cout << "\t     oOO             /       \\             OOo\n";
		cout << "\t   oOO -_-----------/---------\\-----------_- OOo\n";
		cout << "\t  oOO     -_       /           \\       _-     OOo\n";
		cout << "\t oOO         -_   /             \\   _-         OOo\n";
		cout << "\toOO             -/_             _\\-             OOo\n";
		cout << "\toOO             /  -_         _-  \\             OOo\n";
		cout << "\toOO            /      -_   _-      \\            OOo\n";
		cout << "\toOO           /         _-_         \\           OOo\n";
		cout << "\toOO          /       _-     -_       \\          OOo\n";
		cout << "\t oOO        /     _-           -_     \\        OOo\n";
		cout << "\t  oOO      /   _-                 -_   \\      OOo\n";
		cout << "\t   oOO    / _-                       -_ \\    OOo\n";
		cout << "\t     oOO _-                             -_ OOo\n";
		cout << "\t      oOO                                OOo\n";
		cout << "\t          oOO                         OOo\n";
		cout << "\t             oOO                 OOo\n";
		cout << "\t                 ooo OOO OOO ooo\n";
		system("pause");
		system("cls");
		system("color 07");
	}
