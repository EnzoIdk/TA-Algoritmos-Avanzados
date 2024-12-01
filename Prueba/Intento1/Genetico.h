#ifndef GENETICO_H
#define GENETICO_H

#include <vector>

#include "Mapa.h"

class Genetico {
public:
    std::vector<int> mejorRuta;

    //CONSTRUCTOR, COPIA, DESTRUCTOR
    Genetico();
    virtual ~Genetico();
    
    //GETTERS Y SETTERS
    void SetHoraActual(int horaActual);
    int getHoraActual() const;
    //METODOS
    void leerArchivos();
    void iniciarAlgoritmo(const int ciudadInic, const int ciudadFin);
    static void establecerMapa(int hora);
    
    int ciudadAleatoria(const class Distrito &distrito);
    bool rutaValida(const std::vector<int> &cromosoma, int pos, 
            const class Mapa &mapa);
    void calcularSupervivencia(const std::vector<std::vector<int>> &poblacion, 
            std::vector<int> &supervivencia, const class Mapa &mapa);
    void cargaRuleta(const std::vector<int> &supervivencia, 
            std::vector<int> &ruleta);
    
    void buscarMejorRuta(int horaInic, int ciudadInic, int ciudadFin);
    
    std::vector<std::vector<int>> generarPoblacionInicial(
            const class Mapa &mapa, const int ciudadInic, const int ciudadFin);
    bool esAberracion(const std::vector<int> &cromosoma, const class Mapa &mapa, 
            const int ciudadInic, const int ciudadFin);
    void mostrarPoblacion(const std::vector<std::vector<int>> &poblacion);
    std::vector<std::vector<int>> seleccion(
            const std::vector<std::vector<int>> &poblacion, 
            const class Mapa &mapa);
    static double fitness(const class Mapa &mapa, 
            const std::vector<int> &cromosoma);
    
    void mutacion(std::vector<std::vector<int>> padres, 
            std::vector<std::vector<int>> &poblacion, const class Mapa &mapa,
            const int ciudadInic, const int ciudadFin);
    void casamiento(std::vector<std::vector<int>> &padres, 
        std::vector<std::vector<int>> &poblacion, const class Mapa &mapa, 
        const int ciudadInic, const int ciudadFin);
    
    void encuentraIgual(std::vector<int> &madre, std::vector<int> &padre, 
            int &idMenor, int &idMayor, int& ind);
    void controlarPoblacion(std::vector<std::vector<int>> &poblacion, 
            const class Mapa &mapa);
    void controlarDuplicados(std::vector<std::vector<int>> &poblacion);
    std::string compactar(const std::vector<int> &cromosoma);
    void muestraMejor(const std::vector<std::vector<int>> &poblacion, 
            const class Mapa &mapa);
    double calculaRuta(const std::vector<int> &cromosoma, 
            const class Mapa &mapa);
    
};

//FUNCIONES
bool comparaCromosoma(const std::vector<int> &a, const std::vector<int> &b);

#endif /* GENETICO_H */

