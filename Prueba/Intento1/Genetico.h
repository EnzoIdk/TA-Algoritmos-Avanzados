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
    std::vector<class Distrito> mejorRuta;
    class Mapa mapaGlobal;
    int cantidadDistritos;
    
    void leerArchivos();
    void iniciarAlgoritmo(const int ciudadInic, const int ciudadFin);
    void establecerMapa(int hora);
    int ciudadAleatoria(const class Distrito &distrito);
    bool rutaValida(const std::vector<int> &cromosoma, int pos, 
            const class Mapa &mapa);
public:
    //CONSTRUCTOR, COPIA, DESTRUCTOR
    Genetico();
    virtual ~Genetico();
    
    //GETTERS Y SETTERS
    void SetHoraActual(int horaActual);
    int getHoraActual() const;
    //METODOS
    void buscarMejorRuta(int horaInic, int ciudadInic, int ciudadFin);
    
    std::vector<std::vector<int>> generarPoblacionInicial(
            const class Mapa &mapa, const int ciudadInic, const int ciudadFin);
    bool esAberracion(const std::vector<int> &cromosoma, const class Mapa &mapa, 
            const int ciudadInic, const int ciudadFin);
    void mostrarPoblacion(const std::vector<std::vector<int>> &poblacion);
};

//FUNCIONES

#endif /* GENETICO_H */

