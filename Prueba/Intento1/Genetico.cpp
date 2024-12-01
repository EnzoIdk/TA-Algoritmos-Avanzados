#include <algorithm>
#include <iostream>
//<---------------->//
#include <cfloat>
#include <climits>
#include <cmath>
#include <ctime>

#include "Genetico.h"

#define NUM_IND 5
#define NUM_ITER 10
#define PROB_MUTA 0.1
#define TAM_SELEC 0.3

//Variables globales
class Mapa mapaGlobal;
std::vector<class Mapa> mapas;
int cantidadDistritos;
int horaGlobal = 0; //En realidad guarda el dato de la hora inicial

//CONSTRUCTOR, COPIA, DESTRUCTOR
Genetico::Genetico() {
    cantidadDistritos = 0;
}

Genetico::~Genetico() {}

//METODOS
void Genetico::buscarMejorRuta(int horaInic, int ciudadInic, int ciudadFin){
    //1. Leemos los datos
    this->leerArchivos();
    //2. Establecemos el mapa incial
    this->establecerMapa(horaInic);
    //mapaGlobal.imprimir();
    //2. Damos inicio al algoritmo
    horaGlobal = horaInic; //Guardamos la hora incial
    this->iniciarAlgoritmo(ciudadInic, ciudadFin);
}

void Genetico::leerArchivos(){
    std::vector<std::string> files = {
        "./data/Datos0630-0700.txt",
        "./data/Datos0700-0730.txt",
        "./data/Datos0730-0800.txt",
        "./data/Datos0800-0830.txt",
        "./data/Datos0830-0900.txt",
        "./data/Datos0900-0930.txt",
        "./data/Datos0930-1000.txt",
        "./data/Datos1000-1030.txt",
        "./data/Datos1030-1100.txt",
        "./data/Datos1100-1130.txt",
        "./data/Datos1130-1200.txt",
        "./data/Datos1200-1230.txt",
        "./data/Datos1230-1300.txt",
        "./data/Datos1300-1330.txt",
        "./data/Datos1330-1400.txt",
        "./data/Datos1400-1430.txt",
        "./data/Datos1430-1500.txt",
        "./data/Datos1500-1530.txt",
        "./data/Datos1530-1600.txt",
        "./data/Datos1600-1630.txt",
        "./data/Datos1630-1700.txt",
        "./data/Datos1700-1730.txt",
        "./data/Datos1730-1800.txt",
        "./data/Datos1800-1830.txt",
        "./data/Datos1830-1900.txt",
        "./data/Datos1900-1930.txt",
        "./data/Datos1930-2000.txt",
        "./data/Datos2000-2030.txt",
        "./data/Datos2030-2100.txt",
    };
    //Leemos todos los archivos - Todos tienen el mismo tamaño
    for(std::string name:files){
        class Mapa temp;
        //Para obtener el puntero a char contenido en la clase string
        temp.leerArchivo(name.c_str());
        mapas.push_back(temp);
    }
}

void Genetico::establecerMapa(int hora){
    //Buscamos el mapa
    for(class Mapa mapa:mapas){
        if(mapa.enHora(hora)){
            mapaGlobal = mapa;
            break;
        }
    }
    //
    cantidadDistritos = mapaGlobal.getCantidadDistritos();
}

void Genetico::iniciarAlgoritmo(const int ciudadInic, const int ciudadFin){
    //Semilla aleatoria
    srand(time(NULL));
    //En este vector guardamos a todos los individuos
    std::vector<std::vector<int>> poblacion;
    //Generamos la poblacion inicial
    poblacion = generarPoblacionInicial(mapaGlobal, ciudadInic, ciudadFin);
    mostrarPoblacion(poblacion);
    //Iteracion
    for(int i=0; i<NUM_ITER; i++){
        std::vector<std::vector<int>> padres;
        //Realizamos una seleccion aleatoria basada en el fitness de cada
        //individuo
        padres = this->seleccion(poblacion, mapaGlobal);
        //Operadores geneticos
        this-> casamiento(padres, poblacion, mapaGlobal, ciudadInic, ciudadFin);
        this->mutacion(padres, poblacion, mapaGlobal, ciudadInic, ciudadFin);
        //Controlamos la poblacion
        this->controlarDuplicados(poblacion);
        this->controlarPoblacion(poblacion, mapaGlobal);
        //Impresiones
        this->mostrarPoblacion(poblacion);
        this->muestraMejor(poblacion, mapaGlobal);
    }
}

std::vector<std::vector<int>> Genetico::generarPoblacionInicial(
        const class Mapa &mapa, const int ciudadInic, const int ciudadFin){
    int counter = 0, act = ciudadInic, temp = 0;
    std::vector<std::vector<int>> res;
    //Iteracion para la cantidad de individuos
    while(counter < NUM_IND){
        /*
         * Como plantear el cromosoma:
         * 1. No deberiamos recorrer la misma ciudad 2 veces
         * 2. Siempre debemos empezar en el nodo indicado (ciudadInic)
         * 3. Siempre debemos terminar en el nodo indicado (ciudadFin)
         * 4. Los nodos en la ruta deben estar conectados siempre
         */
        std::vector<int> aux(1, act = ciudadInic);
        while(true){
            if(act == ciudadFin) break;
            if(aux.size() > cantidadDistritos) break;
            //Escogemos una ciudad aleatoria
            act = this->ciudadAleatoria(mapa[act]);
            //Si llegamos al final entonces terminamos el bucle
            //La agregamos a la ruta
            aux.push_back(act);
        }
        //Debemos controlar la aberracion
        if(not esAberracion(aux, mapa, ciudadInic, ciudadFin)){
            res.push_back(aux);
            counter++;
        }
    }
    return res;
}

int Genetico::ciudadAleatoria(const class Distrito &distrito){
    return distrito.vecinoAleatorio();
}

bool Genetico::esAberracion(const std::vector<int> &cromosoma, 
        const class Mapa &mapa, const int ciudadInic, const int ciudadFin){
    //Para controlar a los repetidos
    std::map<int, bool> elegidos;
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

bool Genetico::rutaValida(const std::vector<int> &cromosoma, int pos, 
        const class Mapa &mapa){
    Distrito dis = mapa[cromosoma[pos]];
    //Evaluamos el anterior
    if(not dis.estaConectado(cromosoma[pos - 1])) 
        return false;
    //Evaluamos el siguiente
    if(not dis.estaConectado(cromosoma[pos + 1])) 
        return false;
    //Pasa la prueba
    return true;
}

void Genetico::mostrarPoblacion(const std::vector<std::vector<int>> &poblacion){
    for(int i=0; i<poblacion.size(); i++){
        std::cout<<"Poblacion "<<i<<':'<<std::endl;
        for(int j=0; j<poblacion[i].size(); j++) 
            std::cout<<poblacion[i][j]<<' ';
        std::cout<<std::endl;
    }
}

std::vector<std::vector<int>> Genetico::seleccion(
        const std::vector<std::vector<int>> &poblacion, const class Mapa &mapa){
    //Generamos la ruleta y la cantidad de tickets por individuo
    std::vector<std::vector<int>> padres;
    std::vector<int> ruleta(100, -1), supervivencia;
    int numSeleccionados = poblacion.size()*TAM_SELEC, idx;
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

void Genetico::calcularSupervivencia(
        const std::vector<std::vector<int>> &poblacion, 
        std::vector<int> &supervivencia, const class Mapa &mapa){
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

void Genetico::cargaRuleta(const std::vector<int> &supervivencia, 
        std::vector<int> &ruleta){
    int idx=0;
    //Les asignamos tickets a cada individuo
    for(int i=0; i<supervivencia.size(); i++){
        //La cantidad de tickets sera segun su supervivencia
        for(int j=0; j<supervivencia[i]; j++) ruleta[idx++]=i;
    }
}

double Genetico::fitness(const class Mapa &mapa, 
        const std::vector<int>& cromosoma){
    double total = 0, temp;
    int auxHora = horaGlobal;
    
    for(int i=0; i<cromosoma.size()-1; i++){
        temp = mapa[cromosoma[i]].getDistancia(cromosoma[i+1]);
        auxHora += (int)temp;
        Genetico::establecerMapa(auxHora);
        total += temp;
    }
        
    return 1.0/total;
}

void Genetico::mutacion(std::vector<std::vector<int>> padres, 
        std::vector<std::vector<int>> &poblacion, const class Mapa &mapa,
        const int ciudadInic, const int ciudadFin){
    int idx, cantMutaciones;
    //Recorremos a todos los padres de la muestra
    for(int i=0; i<padres.size(); i++){
        cantMutaciones=round(padres[i].size()*PROB_MUTA);
        //Realizamos una cantidad de mutaciones al padre
        for(int j=0; j<cantMutaciones; j++){
            idx=rand()%padres[i].size();
            //Seleccionamos uno de sus nodos a mutar
            padres[i][idx] = this->ciudadAleatoria(mapa[padres[i][idx]]);
        }
        //Que no sea aberracion
        if(not esAberracion(padres[i], mapa, ciudadInic, ciudadFin))
            poblacion.push_back(padres[i]);
    }
}

void Genetico::casamiento(std::vector<std::vector<int>> &padres, 
        std::vector<std::vector<int>> &poblacion, const class Mapa &mapa, 
        const int ciudadInic, const int ciudadFin){
    std::vector<int> hijo1;
    std::vector<int> hijo2;
    for(int i=0;i<padres.size();i++){
        for(int j=0;j<padres.size();j++){
            int idMenor=0, idMayor=0, ind=0;
            if(i!=j){
                  std::vector<int> hijo1;
                  std::vector<int> hijo2;
                //primero es la madre, después el padre, padre menor
                 encuentraIgual(padres[i], padres[j], idMenor, idMayor, ind);
                 std::cout<<"nuevo"<<std::endl;
                 for(int y=0;y<padres[i].size();y++){
                     std::cout<<padres[i][y]<<' ';
                 }
                 std::cout<<std::endl;
                 for(int u=0;u<padres[j].size();u++){
                     std::cout<<padres[j][u]<<' ';
                 }
                 std::cout<<std::endl;
                if(idMenor > 0 && idMayor > 0 && idMenor < padres[i].size() && idMayor < padres[j].size() && ind){
                    if(idMenor + 1 < padres[i].size() && idMayor + 1 < padres[j].size()){
                        if (ind == 1) {
                            hijo1.insert(hijo1.end(), padres[i].begin(), padres[i].begin() + idMayor + 1);
                            hijo1.insert(hijo1.end(), padres[j].begin() + idMenor + 1, padres[j].end());
                            hijo2.insert(hijo2.end(), padres[j].begin(), padres[j].begin() + idMenor + 1);
                            hijo2.insert(hijo2.end(), padres[i].begin() + idMayor + 1, padres[i].end());
                        } else {
                            // Segundo hijo: Mayor padre + Menor padre
                            hijo1.insert(hijo1.end(), padres[i].begin(), padres[i].begin() + idMenor + 1);
                            hijo1.insert(hijo1.end(), padres[j].begin() + idMayor + 1, padres[j].end());
                            hijo2.insert(hijo2.end(), padres[j].begin(), padres[j].begin() + idMayor + 1);
                            hijo2.insert(hijo2.end(), padres[i].begin() + idMenor + 1, padres[i].end());
                        }
                    if(not esAberracion(hijo1, mapa, ciudadInic, ciudadFin)){
                         poblacion.push_back(hijo1);
                    }
                    if(not esAberracion(hijo2, mapa, ciudadInic, ciudadFin)){
                        poblacion.push_back(hijo2);
                     }
                     std::cout<<"hijos"<<std::endl;
                     for(int y=0;y<hijo1.size();y++){
                         std::cout<<hijo1[y]<<' ';
                     }
                     std::cout<<std::endl;
                     for(int u=0;u<hijo2.size();u++){
                         std::cout<<hijo2[u]<<' ';
                     }
                     std::cout<<std::endl;
                    }
                }
            }
        }
    }
}

void Genetico::encuentraIgual(std::vector<int> &madre, std::vector<int> &padre, 
        int &idMenor, int &idMayor, int& ind){
    std::vector<int>menor=padre;
    std::vector<int>mayor=madre;
    //por default el menor es el padre
    ind=1;
    if(madre.size()<padre.size()) {
        ind=2;
        menor=madre;
        mayor=padre;
        //ahora el menor es el de la madre
    }
    for(int i=1;i<menor.size()-1;i++){
        for(int j=1;j<mayor.size()-1;j++){
            //std::cout<<menor[i]<<' '<<mayor[j]<<std::endl;
            if(menor[i]==mayor[j]){
                idMenor = i;
                idMayor= j;
                return;
            }
        }
    }
}

void Genetico::controlarPoblacion(std::vector<std::vector<int>> &poblacion, 
        const class Mapa &mapa){
    mapaGlobal = mapa;
    //Ordenamos
    std::sort(poblacion.begin(), poblacion.end(), comparaCromosoma);
    //Controlamos la poblacion
    if(poblacion.size()>NUM_IND)
        poblacion.erase(poblacion.begin() + NUM_IND, poblacion.end());
}

void Genetico::controlarDuplicados(std::vector<std::vector<int>> &poblacion){
    std::map<std::string, std::vector<int>> unicos;
    std::string code;
    //Llenamos el mapa
    for(int i=0; i<poblacion.size(); i++){
        code = compactar(poblacion[i]);
        if(unicos.count(code) == 0) unicos[code] = poblacion[i];
    }
    //Limpiamos la poblacion
    poblacion.clear();
    //Llenamoscon los elementos no reptidos
    for(std::pair<std::string, std::vector<int>> par:unicos) 
        poblacion.push_back(par.second);
}

std::string Genetico::compactar(const std::vector<int> &cromosoma){
    std::string code = "";
    //Le damos como un "numero hash" para diferenciarlos
    for(int i=0; i<cromosoma.size(); i++){
        code.push_back((char)(cromosoma[i] + 49));
    }
    return code;
}

void Genetico::muestraMejor(const std::vector<std::vector<int>> &poblacion, 
        const class Mapa &mapa) {
    int idxMejor=0;
    double fitnessMejor=DBL_MIN, fitnessTemp;
    //Compramos y buscamos cual tiene el mejor fitness
    for(int i=0; i<poblacion.size(); i++){
        fitnessTemp = this->fitness(mapa, poblacion[i]);
        if(fitnessMejor < fitnessTemp){
            idxMejor = i;
            fitnessMejor = fitnessTemp;
            this->mejorRuta = poblacion[i];
        }
    }
    //Impresion
    std::cout<<"La mejor solucion es: "<<calculaRuta(poblacion[idxMejor], mapa)
            <<std::endl;
    for(int i=0; i<mejorRuta.size(); i++) std::cout<<mejorRuta[i]<<' ';
    std::cout<<std::endl<<std::endl;
}

double Genetico::calculaRuta(const std::vector<int> &cromosoma, 
        const class Mapa &mapa){
    int total = 0;
    for(int i=0; i<cromosoma.size()-1; i++) 
        total += mapa[cromosoma[i]].getDistancia(cromosoma[i+1]);
    return total;
}


//FUNCIONES
bool comparaCromosoma(const std::vector<int> &a, const std::vector<int> &b){
    double sumA=0, sumB=0;
    //Comparamos los fitness
    for(int i=0; i<a.size(); i++) sumA+=Genetico::fitness(mapaGlobal, a);
    for(int i=0; i<b.size(); i++) sumB+=Genetico::fitness(mapaGlobal, b);
    return sumA>sumB;
}