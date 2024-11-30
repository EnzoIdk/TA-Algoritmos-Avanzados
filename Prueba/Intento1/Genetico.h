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

#include <vector>

#include "Mapa.h"

class Genetico {
private:
    int horaActual;
    std::vector<class Mapa> mapas;
public:
    //CONSTRUCTOR, COPIA, DESTRUCTOR
    Genetico();
    virtual ~Genetico();
    
    //GETTERS Y SETTERS
    void SetHoraActual(int horaActual);
    int getHoraActual() const;
    //METODOS
    void buscarMejorRuta(int horaInic, int ciudadInic, int ciudadFin);
};

//FUNCIONES

#endif /* GENETICO_H */

