/* 
 * File:   Distrito.cpp
 * Author: Lucas
 * 
 * Created on 30 de noviembre de 2024, 04:27 PM
 */

#include <iostream>

#include <cstring>

#include "Distrito.h"

//CONSTRUCTOR, COPIA, DESTRUCTOR
Distrito::Distrito() {
    //Por defecto
}

Distrito::Distrito(const class Distrito &orig) {
    *this = orig;
}

Distrito::~Distrito() {
    //Por defecto
}

//GETTERS Y SETTERS
void Distrito::setId(int id){
    this->id = id;
}

int Distrito::getId() const {
    return this->id;
}

void Distrito::setNombre(std::string nombre){
    this->nombre = nombre;
}

std::string Distrito::getNombre() const{
    return this->nombre;
}

//METODOS
void Distrito::operator =(const class Distrito &orig){
    this->id = orig.id;
    this->nombre = orig.nombre;
    this->vecinos = orig.vecinos;
}

void Distrito::insertarVecino(int id, int tiempo){
    if(not vecinos.count(id)) vecinos[id] = tiempo;
}

bool Distrito::estaConectado(int id) const{
    return (bool)vecinos.count(id);
}

void Distrito::leerVecinos(std::ifstream &arch){
    int idVecino, tiempoVecino;
    //id, tiempo, ...., \n
    while(true){
        //Leemos
        arch>>idVecino;
        if(arch.eof()) break;
        arch.get();
        arch>>tiempoVecino;
        //Insertamos
        this->insertarVecino(idVecino, tiempoVecino);
        if(arch.get() == '\n') break;
    }
}

int Distrito::vecinoAleatorio() const{
    int idx = rand()%(vecinos.size()), i = 0;
    for(std::pair<int, int> temp:vecinos){
        if(i == idx) return temp.first;
        i++;
    }
    return 0;
}

void Distrito::imprimir(){
    std::cout<<id<<" - "<<nombre<<std::endl;
}

double Distrito::getDistancia(int idVecino){
    if(not vecinos.count(idVecino)){
        std::cout<<"Error evaluando al vecino "<<idVecino<<" desde "<<nombre<<
                std::endl;
        exit(1);
    }
    return (double)(vecinos[idVecino]);
}