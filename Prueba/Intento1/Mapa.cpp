#include <iostream>

#include <cstring>

#include "Mapa.h"

//CONSTRUCTOR, COPIA, DESTRUCTOR
Mapa::Mapa() {
    horaFin = 0;
    horaInicio = 0;
    cantidadDistritos = 0;
}

Mapa::Mapa(const class Mapa &orig) {
    *this = orig;
}

Mapa::~Mapa() {
    //Por defecto
}

//GETTERS Y SETTERS

void Mapa::setHorario(int horaInicio, int horaFin) {
    if(horaInicio < horaFin){
        this->horaInicio = horaInicio;
        this->horaFin = horaFin;
    }
}

int Mapa::getHoraFin() const {
    return this->horaFin;
}

int Mapa::getHoraInicio() const {
    return this->horaInicio;
}

int Mapa::getCantidadDistritos() const{
    return this->cantidadDistritos;
}

//METODOS
void Mapa::operator =(const class Mapa &orig){
    this->horaInicio = orig.horaInicio;
    this->horaFin = orig.horaFin;
    this->rutas = orig.rutas;
    this->cantidadDistritos = orig.cantidadDistritos;
}

class Distrito Mapa::operator [](int id) const{
    return rutas.at(id);
}

void Mapa::leerArchivo(const char * nombre) {
    std::ifstream arch(nombre, std::ios::in);
    if(not arch){
        std::cout<<"Error: No se pudo leer el archivo: "<<nombre<<std::endl;
        exit(1);
    }
    //Ahora leemos la hora inicio y hora fin
    this->leerHoras(arch);
    //Leemos los datos de las conexiones
    this->leerDatos(arch);
}

void Mapa::leerHoras(std::ifstream &arch){
    arch>>horaInicio;
    arch.get();
    arch>>horaFin;
}

void Mapa::leerDatos(std::ifstream &arch){
    int id;
    char nombre[100] = {};
    //1, San Miguel, 2, 40, 3, 15
    while(true){
        if(arch.eof()) break;
        //Lectura del distrito
        class Distrito temp;
        //Lectura del id
        arch>>id;
        arch.get();
        temp.setId(id);
        //Lectura del nombre
        arch.getline(nombre, 100, ',');
        temp.setNombre(nombre);
        //Lectura de las conexiones de ese distrito
        temp.leerVecinos(arch);
        //Lo agregamos al mapa
        rutas[id] = temp;
    }
    this->cantidadDistritos = rutas.size();
}

bool Mapa::enHora(int hora) const{
    return (hora >= horaInicio) && (hora <=horaFin);
}