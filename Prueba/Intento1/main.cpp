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

vector<vector<int>> mapaGlobal;

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
    int counter = 0, act = ciudadInic;
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

void mostrarPoblacion(vector<vector<int>> poblacion){
    for(int i=0; i<poblacion.size(); i++){
        cout<<"Poblacion "<<i<<':'<<endl;
        for(int j=0; j<poblacion[i].size(); j++) cout<<poblacion[i][j]+1<<' ';
        cout<<endl;
    }
}

double fitness(const vector<vector<int>> &mapa, const vector<int> &cromosoma){
    double total = 0;
    for(int i=0; i<cromosoma.size()-1; i++) 
        total += mapa[cromosoma[i]][cromosoma[i+1]];
    return 1.0/total;
}

void calcularSupervivencia(const vector<vector<int>> &poblacion, 
            vector<int> &supervivencia, const vector<vector<int>> &mapa){
    double suma=0;
    int fit;
    //Hallamos la sumatoria
    for(int i=0; i<poblacion.size(); i++) suma+=fitness(mapa, poblacion[i]);
    //Hallamos el fitness de cada individuo
    for(int i=0; i<poblacion.size(); i++){
        //Sin round porque podria redondear de mas y generar mas de 100 tickets
        fit=(fitness(mapa, poblacion[i])/suma)*100;
        supervivencia.push_back(fit);
    }
}

void cargaRuleta(const vector<int> &supervivencia, vector<int> &ruleta){
    int idx=0;
    //Les asignamos tickets a cada individuo
    for(int i=0; i<supervivencia.size(); i++){
        //La cantidad de tickets sera segun su supervivencia
        for(int j=0; j<supervivencia[i]; j++) ruleta[idx++]=i;
    }
}

vector<vector<int>> seleccion(const vector<vector<int>> &poblacion, 
        const vector<vector<int>> &mapa){
    vector<vector<int>> padres;
    //Generamos la ruleta y la cantidad de tickets por individuo
    vector<int> ruleta(100, -1);
    int numSeleccionados = poblacion.size()*TAM_SELEC, idx;
    vector<int> supervivencia;
    //Calculamos los datos necesarios
    calcularSupervivencia(poblacion, supervivencia, mapa);
    cargaRuleta(supervivencia, ruleta);
    //
    for(int i=0; i<numSeleccionados; i++){
        idx=rand()%100;
        if(ruleta[idx]>-1) padres.push_back(poblacion[ruleta[idx]]);
        else i--;
    }
    return padres;
}

void mutacion(vector<vector<int>> padres, 
        vector<vector<int>> &poblacion, const vector<vector<int>> &mapa, 
        const int ciudadInic, const int ciudadFin){
    int idx, cantMutaciones;
    //Recorremos a todos los padres de la muestra
    for(int i=0; i<padres.size(); i++){
        cantMutaciones=round(padres[i].size()*PROB_MUTA);
        //Realizamos una cantidad de mutaciones al padre
        for(int j=0; j<cantMutaciones; j++){
            idx=rand()%padres[i].size();
            //Seleccionamos uno de sus nodos a mutar
            padres[i][idx] = ciudadAleatoria(mapa[padres[i][idx]]);
        }
        //Que no sea aberracion
        if(not esAberracion(padres[i], mapa, ciudadInic, ciudadFin))
            poblacion.push_back(padres[i]);
    }
}

bool comparaCromosoma(const vector<int> &a, const vector<int> &b){
    double sumA=0, sumB=0;
    //Comparamos los fitness
    for(int i=0; i<a.size(); i++) sumA+=fitness(mapaGlobal, a);
    for(int i=0; i<b.size(); i++) sumB+=fitness(mapaGlobal, b);
    return sumA>sumB;
}

void controlarPoblacion(vector<vector<int>> &poblacion, 
        const vector<vector<int>> &mapa){
    mapaGlobal = mapa;
    //Ordenamos
    sort(poblacion.begin(), poblacion.end(), comparaCromosoma);
    //Controlamos la poblacion
    if(poblacion.size()>NUM_IND)
        poblacion.erase(poblacion.begin()+NUM_IND, poblacion.end());
}

string compactar(const vector<int> &cromosoma){
    string code = "";
    //Le damos como un "numero hash" para diferenciarlos
    for(int i=0; i<cromosoma.size(); i++){
        code.push_back((char)(cromosoma[i] + 49));
    }
    return code;
}

void controlarDuplicados(vector<vector<int>> &poblacion){
    map<string, vector<int>> unicos;
    string code;
    //Llenamos el mapa
    for(int i=0; i<poblacion.size(); i++){
        code = compactar(poblacion[i]);
        //cout<<code<<endl;
        if(unicos.count(code) == 0) unicos[code] = poblacion[i];
    }
    //Limpiamos la poblacion
    poblacion.clear();
    //Llenamoscon los elementos no reptidos
    for(pair<string, vector<int>> par:unicos) poblacion.push_back(par.second);
}

int calculaRuta(vector<int> cromosoma, const vector<vector<int>> &mapa){
    int total = 0;
    for(int i=0; i<cromosoma.size()-1; i++) 
        total += mapa[cromosoma[i]][cromosoma[i+1]];
    return total;
}

void muestraMejor(const vector<vector<int>> &poblacion, 
        const vector<vector<int>> &mapa){
    int idxMejor=0;
    double fitnessMejor=DBL_MIN, fitnessTemp;
    //Compramos y buscamos cual tiene el mejor fitness
    for(int i=0; i<poblacion.size(); i++){
        fitnessTemp=fitness(mapa, poblacion[i]);
        if(fitnessMejor<fitnessTemp){
            idxMejor=i;
            fitnessMejor=fitnessTemp;
        }
    }
    //Impresion
    cout<<"La mejor solucion es: "<<calculaRuta(poblacion[idxMejor], mapa)<<
            endl;
    for(int i=0; i<poblacion[idxMejor].size(); i++)
        cout<<poblacion[idxMejor][i]+1<<' ';
    cout<<endl<<endl;
}

void buscarMejorRuta(const vector<vector<int>> &mapa, const int ciudadInic, 
        const int ciudadFin){
    //Semilla aleatoria
    srand(time(NULL));
    //En este vector guardamos a todos los individuos
    vector<vector<int>> poblacion;
    //Generamos la poblacion inicial
    poblacion = generarPoblacionInicial(mapa, ciudadInic, ciudadFin);
    mostrarPoblacion(poblacion);
    
    //Iteracion
    for(int i=0; i<NUM_ITER; i++){
        vector<vector<int>> padres;
        //Realizamos una seleccion aleatoria basada en el fitness de cada
        //individuo
        padres = seleccion(poblacion, mapa);
        //Operadores geneticos
        mutacion(padres, poblacion, mapa, ciudadInic, ciudadFin);
        //Controlamos la poblacion
        controlarDuplicados(poblacion);
        controlarPoblacion(poblacion, mapa);
        //Impresiones
        //mostrarPoblacion(poblacion);
        muestraMejor(poblacion, mapa);
    }
}

#include "Mapa.h"

int main(int argc, char ** argv) {
    Mapa temp;
    temp.leerArchivo("Datos0700-0730.txt");
    
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
//    vector<vector<int>> mapa(50, vector<int>(50, 0));
//    int N = 50;
//    // Generar valores aleatorios y asegurar simetría
//    for (int i = 0; i < 50; ++i) {
//        for (int j = i + 1; j < 50; ++j) {
//            int peso = std::rand() % 51; // Pesos entre 0 y 50
//            mapa[i][j] = peso;
//            mapa[j][i] = peso;
//        }
//    }
//    // Imprimir la matriz de adyacencia en el formato solicitado
//    std::cout << "vector<vector<int>> mapa = {\n";
//    for (int i = 0; i < N; ++i) {
//        std::cout << "    { ";
//        for (int j = 0; j < N; ++j) {
//            std::cout << mapa[i][j];
//            if (j < N - 1) std::cout << ", ";
//        }
//        std::cout << " }"; 
//        if (i < N - 1) std::cout << ","; // Coma al final de cada fila excepto la última
//        std::cout << " // Nodo " << (i + 1) << "\n";
//    }
//    std::cout << "};\n";
    
    //buscarMejorRuta(mapa, ciudadInic, ciudadFin);
    
    return 0;
}

