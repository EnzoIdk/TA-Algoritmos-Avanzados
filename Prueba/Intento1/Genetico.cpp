/*
 * Intento de Lucas Mattias Alvites Galarza - 20221943
 */

/* 
 * File:   Genetico.cpp
 * Author: Lucas
 * 
 * Created on 29 de noviembre de 2024, 01:32 PM
 */

#include <iostream>
using namespace std;
#include <cstring>

#include "Genetico.h"

//CONSTRUCTOR, COPIA, DESTRUCTOR
Genetico::Genetico() {}

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
    
}

//FUNCIONES
