#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;
#include <cfloat>
#include <climits>
#include <cmath>
#include <ctime>

#define NUM_IND 3
#define NUM_ITER 100
#define PROB_MUTA 0.1
#define TAM_SELEC 0.3

/*
 * Como empeza a solucionar este problema.
 * - Codifiquemos la hora de 0000 a 2359
 * - Cada mapa estará dentro de un rango
 */

#include "Genetico.h"

int main(int argc, char ** argv) {
    //1. Debemos hacer una funcion que lea el archivo
    //2. Debemos mantener la hora tras cada movimiento
    //3. Debemos mantener 
    int ciudadInic = 0, ciudadFin = 5, horaInicio = 630;
    
    class Genetico solver;
    solver.buscarMejorRuta(horaInicio, ciudadInic, ciudadFin);    
    
    return 0;
}

