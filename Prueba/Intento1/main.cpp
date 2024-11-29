#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
#include <climits>
#include <cmath>
#include <ctime>

#define NUM_IND 10
#define NUM_ITER 1000
#define PROB_CASA 0.5
#define PROB_MUTA 0.1
#define TAM_SELEC 0.3

/*
 * Como empeza a solucionar este problema.
 * - Codifiquemos la hora de 0000 a 2359
 * - Cada mapa estará dentro de un rango
 */

int ciudadAleatoria(vector<int> vecinos){
    int idx;
    while(true){
        idx = rand()%vecinos.size();
        if(vecinos[idx]) return idx;
    }
}

bool rutaValida(vector<int> cromosoma, int pos, 
        const vector<vector<int>> &mapa){
    //Evaluamos el anterior
    if(not mapa[cromosoma[pos]][cromosoma[pos-1]]) return false;
    //Evaluamos el siguiente
    if(not mapa[cromosoma[pos]][cromosoma[pos+1]]) return false;
    //Pasa la prueba
    return true;
}

bool esAberracion(vector<int> cromosoma, const vector<vector<int>> &mapa, 
        const int ciudadInic, const int ciudadFin){
    //Para controlar a los repetidos
    map<int, bool> elegidos;
    for(int i=0; i<cromosoma.size(); i++){
        if(elegidos.count(cromosoma[i])) return true;
        elegidos[cromosoma[i]] = true;
    }
    //Para controlar que el inicio y el fin se mantengan
    if(cromosoma[0] != ciudadInic) return true;
    if(cromosoma[cromosoma.size() - 1] != ciudadFin) return true;
    //Ahora debemos validar que la ruta tenga sentido
    for(int i=1; i<cromosoma.size()-1; i++){ //Podemos probar con +=2
        if(not rutaValida(cromosoma, i, mapa)) return true;
    }
    //Paso todas las pruebas
    return false;
}

vector<vector<int>> generarPoblacionInicial(const vector<vector<int>> &mapa, 
        const int ciudadInic, const int ciudadFin){
    int counter = 0, act = ciudadInic, temp = 0;
    vector<vector<int>> res;
    //Iteracion para la cantidad de individuos
    while(counter < NUM_IND){
        /*
         * Como plantear el cromosoma:
         * 1. No deberiamos recorrer la misma ciudad 2 veces
         * 2. Siempre debemos empezar en el nodo indicado (ciudadInic)
         * 3. Siempre debemos terminar en el nodo indicado (ciudadFin)
         * 4. Los nodos en la ruta deben estar conectados siempre
         */
        map<int, bool> elegidos;
        elegidos[act] = true;
        vector<int> aux(1, act = ciudadInic);
        while(true){
            //Escogemos una ciudad aleatoria
            act = ciudadAleatoria(mapa[act]);
            //Si llegamos al final entonces terminamos el bucle
            if(act == ciudadFin) break;
            //La agregamos a la ruta
            aux.push_back(act);
        }
        //Mandamos el final
        aux.push_back(act);
        //Debemos controlar la aberracion, aunque en esta generacion no
        //deberian haber aberraciones
        if(not esAberracion(aux, mapa, ciudadInic, ciudadFin)){
            res.push_back(aux);
            counter++;
        }
    }
    return res;
}

void mostrarPoblacion(vector<vector<int>> poblacion, 
        const vector<vector<int>> &mapa){
    for(int i=0; i<poblacion.size(); i++){
        cout<<"Poblacion "<<i<<':'<<endl;
        for(int j=0; j<poblacion[i].size(); j++) cout<<poblacion[i][j]+1<<' ';
        cout<<endl;
    }
}

int fitness(){
    
}

void buscarMejorRuta(const vector<vector<int>> &mapa, const int ciudadInic, 
        const int ciudadFin){
    //Semilla aleatoria
    srand(time(NULL));
    //En este vector guardamos a todos los individuos
    vector<vector<int>> poblacion;
    //Generamos la poblacion inicial
    poblacion = generarPoblacionInicial(mapa, ciudadInic, ciudadFin);
    mostrarPoblacion(poblacion, mapa);
    
    //Iteracion
    for(int i=0; i<NUM_ITER; i++){
        vector<vector<int>> padres;
        //Realizamos una seleccion aleatoria basada en el fitness de cada
        //individuo
//        padres = seleccion(poblacion, mapa);
//        //Operadores geneticos
//        casamiento(padres, poblacion);
//        mutacion(padres, poblacion);
//        //Controlamos la poblacion
//        controlarDuplicados(poblacion);
//        controlarPoblacion(poblacion, ganancia);
//        //Impresiones
//        mostrarPoblacion(poblacion, mapa);
//        muestraMejor(poblacion, ganancia);
    }
}

int main(int argc, char ** argv) {
    //1. Debemos hacer una funcion que lea el archivo
    //2. Debemos mantener la hora tras cada movimiento
    //3. Debemos mantener 
    int ciudadInic = 0, ciudadFin = 4, horaInicio = 630;
    
    vector<vector<int>> mapa = {
        //1   2   3   4   5
        { 0, 30,  0, 20,  0}, //1
        {30,  0, 40, 20,  0}, //2
        { 0, 40,  0, 20, 15}, //3
        {20, 20, 20,  0,  0}, //4
        { 0,  0, 15,  0,  0}  //5
    };
//    vector<vector<int>> mapa = {
//        //1   2   3   4   5   6   7   8   9  10
//        { 0, 30, 25,  0, 20, 15,  0, 10,  0,  0 }, // Nodo 1
//        {30,  0,  0, 35,  0,  0, 40,  0,  0,  0 }, // Nodo 2
//        {25,  0,  0,  0,  5,  0,  0, 20,  0, 15 }, // Nodo 3
//        { 0, 35,  0,  0,  0, 25, 30,  0,  0,  0 }, // Nodo 4
//        {20,  0,  5,  0,  0, 10,  0,  0, 45,  0 }, // Nodo 5
//        {15,  0,  0, 25, 10,  0, 50,  0,  0,  0 }, // Nodo 6
//        { 0, 40,  0, 30,  0, 50,  0, 35,  0,  0 }, // Nodo 7
//        {10,  0, 20,  0,  0,  0, 35,  0, 40,  0 }, // Nodo 8
//        { 0,  0,  0,  0, 45,  0,  0, 40,  0, 25 }, // Nodo 9
//        { 0,  0, 15,  0,  0,  0,  0,  0, 25,  0 }  // Nodo 10
//    };
    
    buscarMejorRuta(mapa, ciudadInic, ciudadFin);
    
    return 0;
}

