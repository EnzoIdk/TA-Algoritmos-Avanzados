#include <algorithm>
#include <iostream>
//<---------------->//
#include <cfloat>
#include <climits>
#include <cmath>
#include <ctime>

#include "Genetico.h"

#define NUM_IND 1
#define NUM_ITER 100
#define PROB_MUTA 0.1
#define TAM_SELEC 0.3

//CONSTRUCTOR, COPIA, DESTRUCTOR
Genetico::Genetico() {
    cantidadDistritos = 0;
    horaActual = 0;
}

Genetico::~Genetico() {}

//GETTERS Y SETTERS
void Genetico::SetHoraActual(int horaActual) {
    this->horaActual = horaActual;
}

int Genetico::getHoraActual() const{
    return this->horaActual;
}

//METODOS
void Genetico::buscarMejorRuta(int horaInic, int ciudadInic, int ciudadFin){
    //1. Leemos los datos
    this->leerArchivos();
    //2. Establecemos el mapa incial
    this->establecerMapa(horaInic);
    //2. Damos inicio al algoritmo
    this->iniciarAlgoritmo(ciudadInic, ciudadFin);
    
}

void Genetico::leerArchivos(){
    std::vector<std::string> files = {
        "./data/Datos0630-0700.txt",
        "./data/Datos0700-0730.txt",
        "./data/Datos0730-0800.txt",
        "./data/Datos0800-0830.txt",
        "./data/Datos0830-0900.txt",
        "./data/Datos0900-0930.txt",
        "./data/Datos0930-1000.txt",
        "./data/Datos1000-1030.txt",
        "./data/Datos1030-1100.txt",
        "./data/Datos1100-1130.txt",
        "./data/Datos1130-1200.txt",
        "./data/Datos1200-1230.txt",
        "./data/Datos1230-1300.txt",
        "./data/Datos1300-1330.txt",
        "./data/Datos1330-1400.txt",
        "./data/Datos1400-1430.txt",
        "./data/Datos1430-1500.txt",
        "./data/Datos1500-1530.txt",
        "./data/Datos1530-1600.txt",
        "./data/Datos1600-1630.txt",
        "./data/Datos1630-1700.txt",
        "./data/Datos1700-1730.txt",
        "./data/Datos1730-1800.txt",
        "./data/Datos1800-1830.txt",
        "./data/Datos1830-1900.txt",
        "./data/Datos1900-1930.txt",
        "./data/Datos1930-2000.txt",
        "./data/Datos2000-2030.txt",
        "./data/Datos2030-2100.txt",
    };
    //Leemos todos los archivos - Todos tienen el mismo tamaño
    for(std::string name:files){
        class Mapa temp;
        //Para obtener el puntero a char contenido en la clase string
        temp.leerArchivo(name.c_str());
        this->mapas.push_back(temp);
    }
}

void Genetico::establecerMapa(int hora){
    this->horaActual = hora;
    //Buscamos el mapa
    for(class Mapa mapa:mapas){
        if(mapa.enHora(hora)){
            mapaGlobal = mapa;
            break;
        }
    }
    //
    this->cantidadDistritos = mapaGlobal.getCantidadDistritos();
}

void Genetico::iniciarAlgoritmo(const int ciudadInic, const int ciudadFin){
    //Semilla aleatoria
    srand(time(NULL));
    //En este vector guardamos a todos los individuos
    std::vector<std::vector<int>> poblacion;
    //Generamos la poblacion inicial
    poblacion = generarPoblacionInicial(mapaGlobal, ciudadInic, ciudadFin);
    mostrarPoblacion(poblacion);
    //Iteracion
    for(int i=0; i<NUM_ITER; i++){
//        vector<vector<int>> padres;
//        //Realizamos una seleccion aleatoria basada en el fitness de cada
//        //individuo
//        padres = seleccion(poblacion, mapa);
//        //Operadores geneticos
//        //-> casamiento(padres, poblacion, mapa, ciudadInic, ciudadFin);
//        mutacion(padres, poblacion, mapa, ciudadInic, ciudadFin);
//        //Controlamos la poblacion
//        controlarDuplicados(poblacion);
//        controlarPoblacion(poblacion, mapa);
//        //Impresiones
//        //mostrarPoblacion(poblacion);
//        muestraMejor(poblacion, mapa);
    }
}

std::vector<std::vector<int>> Genetico::generarPoblacionInicial(
        const class Mapa &mapa, const int ciudadInic, const int ciudadFin){
    int counter = 0, act = ciudadInic, temp = 0;
    std::vector<std::vector<int>> res;
    //Iteracion para la cantidad de individuos
    while(counter < NUM_IND){
        /*
         * Como plantear el cromosoma:
         * 1. No deberiamos recorrer la misma ciudad 2 veces
         * 2. Siempre debemos empezar en el nodo indicado (ciudadInic)
         * 3. Siempre debemos terminar en el nodo indicado (ciudadFin)
         * 4. Los nodos en la ruta deben estar conectados siempre
         */
        std::vector<int> aux(1, act = ciudadInic);
        while(true){
            std::cout<<act<<std::endl;
            if(act == ciudadFin) break;
            if(aux.size() > this->cantidadDistritos) break;
            //Escogemos una ciudad aleatoria
            act = this->ciudadAleatoria(mapa[act]);
            std::cout<<"act"<<act<<std::endl;
            //Si llegamos al final entonces terminamos el bucle
            //La agregamos a la ruta
            aux.push_back(act);
        }
        std::cout<<"DONE"<<rand()%10<<std::endl;
        //Debemos controlar la aberracion
        if(not esAberracion(aux, mapa, ciudadInic, ciudadFin)){
            res.push_back(aux);
            counter++;
        }
    }
    return res;
}

int Genetico::ciudadAleatoria(const class Distrito &distrito){
    return distrito.vecinoAleatorio();
}

bool Genetico::esAberracion(const std::vector<int> &cromosoma, 
        const class Mapa &mapa, const int ciudadInic, const int ciudadFin){
//    for(int x:cromosoma) std::cout<<x<<' ';
//    std::cout<<'\n';
    std::cout<<"IN"<<rand()%10<<std::endl;
    //Para controlar a los repetidos
    std::map<int, bool> elegidos;
    for(int i=0; i<cromosoma.size(); i++){
        if(elegidos.count(cromosoma[i])) return true;
        elegidos[cromosoma[i]] = true;
    }
    //Para controlar que el inicio y el fin se mantengan
    if(cromosoma[0] != ciudadInic) return true;
    if(cromosoma[cromosoma.size() - 1] != ciudadFin) return true;
    //Ahora debemos validar que la ruta tenga sentido
    for(int i=1; i<cromosoma.size()-1; i++){ //Podemos probar con +=2
        if(not rutaValida(cromosoma, i, mapa)) return true;
    }
    //Paso todas las pruebas
    return false;
}

bool Genetico::rutaValida(const std::vector<int> &cromosoma, int pos, 
        const class Mapa &mapa){
    Distrito dis = mapa[cromosoma[pos]];
    //Evaluamos el anterior
    if(not dis.estaConectado(cromosoma[pos - 1])) 
        return false;
    //Evaluamos el siguiente
    if(not dis.estaConectado(cromosoma[pos + 1])) 
        return false;
    //Pasa la prueba
    return true;
}

void Genetico::mostrarPoblacion(const std::vector<std::vector<int>> &poblacion){
    for(int i=0; i<poblacion.size(); i++){
        std::cout<<"Poblacion "<<i<<':'<<std::endl;
        for(int j=0; j<poblacion[i].size(); j++) 
            std::cout<<poblacion[i][j]<<' ';
        std::cout<<std::endl;
    }
}