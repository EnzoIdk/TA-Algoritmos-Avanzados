/*
 * Intento de Lucas Mattias Alvites Galarza - 20221943
 */

/* 
 * File:   Genetico.h
 * Author: Lucas
 *
 * Created on 29 de noviembre de 2024, 01:32 PM
 */

#ifndef GENETICO_H
#define GENETICO_H

#include "Mapa.h"

class Genetico {
private:
    static int horaActual;
    vector<class Mapa> mapas;
public:
    //CONSTRUCTOR, COPIA, DESTRUCTOR
    Genetico();
    virtual ~Genetico();
    
    //GETTERS Y SETTERS
    static void SetHoraActual(int hora);   
    //METODOS
    static void buscarMejorRuta();

};

//FUNCIONES

#endif /* GENETICO_H */

