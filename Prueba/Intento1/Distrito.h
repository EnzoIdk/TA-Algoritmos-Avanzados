/*
 * Intento de Lucas Mattias Alvites Galarza - 20221943
 */

/* 
 * File:   Distrito.h
 * Author: Lucas
 *
 * Created on 30 de noviembre de 2024, 04:27 PM
 */

#ifndef DISTRITO_H
#define DISTRITO_H

#include <fstream>
#include <map>
#include <string>

class Distrito {
private:
    //El nombre del distrito
    int id;
    std::string nombre;
    //Un mapa de sus vecinos, donde la llave es el first es el id y el second es
    //la distancia. En caso no tenga conexion, entonces vecinos.count(n) == 0
    std::map<int, int> vecinos;
public:
    //CONSTRUCTOR, COPIA, DESTRUCTOR
    Distrito();
    Distrito(const class Distrito &orig);
    virtual ~Distrito();
    
    //GETTERS Y SETTERS
    void setId(int id);
    int getId() const;
    std::string getNombre() const;
    void setNombre(std::string nombre);
    
    //METODOS
    void operator =(const class Distrito &orig);
    
    void insertarVecino(int id, int tiempo);
    bool estaConectado(int id) const;
    
    void leerVecinos(std::ifstream &arch);
};

#endif /* DISTRITO_H */

