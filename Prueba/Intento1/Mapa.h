/*
 * Intento de Lucas Mattias Alvites Galarza - 20221943
 */

/* 
 * File:   Mapa.h
 * Author: Lucas
 *
 * Created on 29 de noviembre de 2024, 01:13 PM
 */

#ifndef MAPA_H
#define MAPA_H

#include <vector>
using namespace std;

class Mapa {
private:
    int horaInicio;
    int horaFin;
    vector<vector<int>> rutas;
public:
    //CONSTRUCTOR, COPIA, DESTRUCTOR
    Mapa();
    Mapa(const char * nombre);
    Mapa(const class Mapa &orig);
    virtual ~Mapa();
    
    //GETTERS Y SETTERS
    void setRutas(vector<vector<int>> rutas);
    vector<vector<int>> getRutas() const;
    void setHoraFin(int horaFin);
    int getHoraFin() const;
    void setHoraInicio(int horaInicio);
    int getHoraInicio() const;

    //METODOS
    void leerArchivo(const char * nombre);
};

//FUNCIONES

#endif /* MAPA_H */

