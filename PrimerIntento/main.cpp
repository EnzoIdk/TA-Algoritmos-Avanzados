
/* 
 * File:   main.cpp
 *
 * Created on 26 de noviembre de 2024, 01:19 AM
 */

#include <iostream>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>
#define N 5
#define nIteraciones 1000
#define nIndividuos 20
#define tasaSeleccion 0.3
#define tasaCasamiento 0.8
#define tasaMutacion 0.1

using namespace std;

void empresaAG(int *ganancia, int n);
void generaPoblacionInicial(vector<vector<int>>&poblacion,int n);
bool esAberracion(vector<int>cromo);
bool validarMasDeUnoPorEspacio(vector<int>cromo);
void muestraPoblacion(vector<vector<int>> poblacion,int *ganancia);
void seleccion(vector<vector<int>>&padres,vector<vector<int>>poblacion,int *ganancia);
void calculaSupervivencia(vector<vector<int>>poblacion,vector<int>&supervivencia, int *ganancia);
void cargaRuleta(vector<int>supervivencia,int *ruleta);
int calculaFitness(vector<int> cromo,int *ganancia);
void casamiento(vector<vector<int>>padres,vector<vector<int>>&poblacion,int *ganancia);
void generaHijo(vector<int>padre,vector<int>madre,vector<int>&hijo);
void inversion(vector<vector<int>>&poblacion,vector<vector<int>>padres);
void mutacion(vector<vector<int>> &poblacion,vector<vector<int>> padres);
void eliminaRepetidos(vector<vector<int>> &poblacion);
int compacta(vector<int>cromo);
void generarPoblacion(vector<vector<int>> &poblacion,int *ganancia);
bool cmp(vector<int>a,vector<int>b);
void muestraMejor(vector<vector<int>> poblacion,int *ganancia);

int *gGanancia;

int main(int argc, char** argv) {
    // la ganancia la dejaremos como arreglo
    // aberracion cuando hay más de 1 en un grupo de 5
    // a cada bloque de 5 un rand de 0 a 1
    int ganancia[N*N]={
        9,2,7,8,6,
        6,4,3,7,8,
        5,8,1,8,3,
        7,6,9,4,2,
        8,6,7,5,9
    };
    int n=sizeof(ganancia)/sizeof(ganancia[0]);
    empresaAG(ganancia,n);
    return 0;
}
// condicional, siempre cuando el size sea mayor a nIndividuos
// buscamos maximizar ganancia (entre 40 y 42)
void empresaAG(int *ganancia, int n){
    int cont=0;
    // guardaremos nIndividuos que cumplan
    vector<vector<int>> poblacion;
    // plantando semilla de numeros aleatorios
    srand(time(NULL));
    // veremos casos que no sean aberraciones
    generaPoblacionInicial(poblacion,n);
    // probamos como va quedando 
    muestraPoblacion(poblacion,ganancia); 
    while(cont<nIteraciones){
        vector<vector<int>> padres;
        seleccion(padres,poblacion,ganancia);
        casamiento(padres,poblacion,ganancia);
        cout << endl;
        inversion(poblacion,padres);
        mutacion(poblacion,padres);
        eliminaRepetidos(poblacion);
        generarPoblacion(poblacion,ganancia);
        muestraPoblacion(poblacion,ganancia); 
        muestraMejor(poblacion,ganancia);
        cont++;
    }
}

void generaPoblacionInicial(vector<vector<int>>&poblacion,int n){
    int cont=0;
    while(cont<nIndividuos){
        vector<int> vaux;
        for(int i=0;i<n;i++){
            // inicializamos auxiliar en 0
            vaux.push_back(0);
        }
        // asignamos la tarea a un trabajador
        for(int i=0;i<N;i++){
            int ind=rand()%N;
            vaux[i*N+ind]=1;
        }
//        for(int i=0;i<n;i++){
//            // llenamos auxiliar de 1 si realiza la tarea o 0 si no
//            cout<<vaux[i]<<' ';
//        }
        if(!esAberracion(vaux)){
            poblacion.push_back(vaux);
            cont++;
        }
    }
}

// aberracion será si una tarea es realizada por más de un trabajador
bool esAberracion(vector<int>cromo){
    vector<int>empleados;
    for(int i=0;i<cromo.size();i++){
        if(cromo[i]==1) empleados.push_back(i%N);
    }    
    // verificamos que no se repitan los empleados
    map<int,int> indices;
    vector<int> aux;
    for(int i=0;i<empleados.size();i++){
        indices[empleados[i]]=cromo[empleados[i]];
    }
    for(map<int,int>::iterator it=indices.begin();it!=indices.end();it++){
        aux.push_back(it->second);
    }
    bool condicion1=(aux.size()!=N);
    bool condicion2=validarMasDeUnoPorEspacio(cromo);
    return condicion1 or condicion2;
}

bool validarMasDeUnoPorEspacio(vector<int>cromo){
    int cant=0;
    for(int i=0;i<cromo.size();i++){
        cant+=cromo[i];
        if((i+1)%5==0){
            if(cant>1) return true;
            cant=0;
        }
    }  
    return false;
}

void muestraPoblacion(vector<vector<int>> poblacion,int *ganancia){
    for(int i=0;i<poblacion.size();i++){
        for(int j=0;j<poblacion[i].size();j++){
            cout << poblacion[i][j] << "  ";
            if((j+1)%5==0) cout<<' ';
        }
        cout <<" Funcion objetivo = "<< calculaFitness(poblacion[i],ganancia) <<endl;
    }
}

void seleccion(vector<vector<int>>&padres,vector<vector<int>>poblacion,int *ganancia){
    int ruleta[100]{-1};
    vector<int>supervivencia;
    // calculamos la probabilidad según el fitness de un elemento
    // en comparación al total
    calculaSupervivencia(poblacion,supervivencia,ganancia);
    cargaRuleta(supervivencia,ruleta);
    int nseleccionados= poblacion.size()*tasaSeleccion;        
    for(int i=0;i<nseleccionados;i++){
        int ind=rand()%100;
        if(ruleta[ind]>-1)
            padres.push_back(poblacion[ruleta[ind]]);
    } 
}

void calculaSupervivencia(vector<vector<int>>poblacion,vector<int>&supervivencia, int *ganancia){
    int suma=0;
    for(int i=0;i<poblacion.size();i++)
        suma+=calculaFitness(poblacion[i],ganancia);
    for(int i=0;i<poblacion.size();i++){
        int fit= round(100*(double)calculaFitness(poblacion[i],ganancia)/suma);
        supervivencia.push_back(fit);
    }
}

// cargamos ruleta según probabilidad de supervivencia hallado con su fitness
void cargaRuleta(vector<int>supervivencia,int *ruleta){
    int ind=0;
    for(int i=0;i<supervivencia.size();i++)
        for(int j=0;j<supervivencia[i];j++)
            ruleta[ind++]=i;
}

// qué tan apta es la solución
int calculaFitness(vector<int> cromo,int *ganancia){
    int cont=0;
    for(int i=0;i<cromo.size();i++)
        cont+=cromo[i]*ganancia[i];
    return cont;
}

// cruzando a padres
void casamiento(vector<vector<int>>padres,vector<vector<int>>&poblacion,int *ganancia){
    for(int i=0;i<padres.size();i++)
        for(int j=0;j<padres.size();j++)
            if(i!=j){
                vector<int>cromo;
                generaHijo(padres[i],padres[j],cromo);
                if(!esAberracion(cromo))
                    poblacion.push_back(cromo);
            }
}

// combinamos genes del padre y madre
void generaHijo(vector<int>padre,vector<int>madre,vector<int>&hijo){
    int pos=round(padre.size()*tasaCasamiento);
    for(int i=0;i<pos;i++)
        hijo.push_back(padre[i]);
    for(int i=pos;i<madre.size();i++)
        hijo.push_back(madre[i]);
}

// 1's por 0's y vemos si cumple
void inversion(vector<vector<int>>&poblacion,vector<vector<int>>padres){
    for(int i=0;i<padres.size();i++){
        for(int j=0;j<padres[i].size();j++){
            if(padres[i][j]==0)
                padres[i][j]=1;
            else
                padres[i][j]=0;
        }
        if(!esAberracion(padres[i]))
            poblacion.push_back(padres[i]);
    }   
}

void mutacion(vector<vector<int>> &poblacion,vector<vector<int>> padres){
    int cont=0;
    int nmuta=round(padres[0].size()*tasaMutacion);
    for(int i=0;i<padres.size();i++){
        while(cont<nmuta){
            int ind=rand()%padres[0].size();
            if(padres[i][ind]==0){
                padres[i][ind]=1;
            }
            else
                padres[i][ind]=0;
            cont++;
        }
        if(!esAberracion(padres[i]))
            poblacion.push_back(padres[i]);     
    }
}

void eliminaRepetidos(vector<vector<int>> &poblacion){
    map<int,vector<int>> mUnicos;
    for(int i=0;i<poblacion.size();i++){
        int num=compacta(poblacion[i]);
        mUnicos[num]=poblacion[i];
    }
    poblacion.clear();
    for(map<int,vector<int>>::iterator it=mUnicos.begin();
            it!=mUnicos.end();it++){
        poblacion.push_back(it->second);
    }
}

// pasamos de binario a decimal
int compacta(vector<int>cromo){
    int num=0;
    for(int i=0;i<cromo.size();i++)
        num+=pow(2,i)*cromo[i];   
    return num;
}

// generamos la nueva población con la cantidad de individuos
void generarPoblacion(vector<vector<int>> &poblacion,int *ganancia){
    gGanancia=ganancia;
    // de mayor a menor fitness
    sort(poblacion.begin(),poblacion.end(),cmp);
    if(poblacion.size()>=nIndividuos) poblacion.erase(poblacion.begin()+nIndividuos,poblacion.end());
}

bool cmp(vector<int>a,vector<int>b){
    int suma=0,sumb=0;   
    for(int i=0;i<a.size();i++)
        suma+=calculaFitness(a,gGanancia);
    for(int i=0;i<b.size();i++)
        sumb+=calculaFitness(b,gGanancia);
    return suma>sumb;
}

// obtenemos la mayor ganancia posible
void muestraMejor(vector<vector<int>> poblacion,int *ganancia){
    int mejor=0;
    for(int i=0;i<poblacion.size();i++)
        if(calculaFitness(poblacion[mejor],ganancia)<calculaFitness(poblacion[i],ganancia))
            mejor=i;
    
    cout << endl<<"La mejor solucion es:" << calculaFitness(poblacion[mejor],ganancia)<<endl;
    for(int i=0;i<poblacion[mejor].size();i++){
        cout << poblacion[mejor][i] << "  ";        
    }
    cout << endl;
}

