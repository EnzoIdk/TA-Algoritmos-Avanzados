
/* 
 * File:   Mapa.h
 * Author: Lucas
 *
 * Created on 29 de noviembre de 2024, 01:13 PM
 */

#ifndef MAPA_H
#define MAPA_H

#include "Distrito.h"

class Mapa {
private:
    //Los datos necesarios para la hora de inicio y fin de este mapa
    int horaInicio;
    int horaFin;
    //La acumulacion de los distritos, la llave es el id y el second es el
    //distrito en sí
    std::map<int, class Distrito> rutas;
    int cantidadDistritos;
    
    //METODOS
    void leerHoras(std::ifstream &arch);
    void leerDatos(std::ifstream &arch);
public:
    //CONSTRUCTOR, COPIA, DESTRUCTOR
    Mapa();
    Mapa(const char * nombre);
    Mapa(const class Mapa &orig);
    virtual ~Mapa();
    
    //GETTERS Y SETTERS
    void setHorario(int horaInicio, int horaFin);
    int getHoraInicio() const;
    int getHoraFin() const;
    int getCantidadDistritos() const;
    
    //METODOS
    void operator =(const class Mapa &orig);
    class Distrito operator [](int id) const;
    
    void leerArchivo(const char * nombre);
    bool enHora(int hora) const;
};

//FUNCIONES

#endif /* MAPA_H */

