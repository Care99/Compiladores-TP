#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define caracterVacio NULL
struct nodo
{
    int esInicio;
    int esFin;
    int esVisitado;
    char* identificador;
    struct vertice* listaVertices;
    struct nodo* siguienteNodo;
};
struct vertice
{
    struct nodo* destino;
    char* expresion;
    struct vertice* siguienteVertice;
};
struct estado
{
    char* expresion;
    struct estado* siguienteEstado;
};
char* crearCadena(int identificador);
struct nodo* crearNodo(char* identificador);
void agregarNodo(struct nodo* listadoNodos, char* identificador);
void agregarVertice(struct nodo* origen, struct nodo* destino, char* expresion);
void agregarEstado(struct estado* listadoEstados, char* estado);
struct nodo* agregarEstadosExtremos(struct nodo* listadoNodos);
char*** crearDiagramaEstado(struct nodo* listadoNodos);
void renombrarNodos(struct nodo* DFA);
void subconjuntoMinimizadoIniciales(struct nodo** subconjuntoMinimizado, struct nodo* DFA);
int verificarMismoSubconjunto(struct nodo* subconjuntoMinimizado, char* identificador);
int subconjuntoAjeno(struct nodo* subconjuntoMinimizado);
struct nodo* DFA_a_DFA_Minimizado(struct nodo* DFA);
int esDiferente(struct estado* listadoEstados, char* expresion);
void agregarEstados(struct estado* listadoEstados, struct nodo* listadoNodos);
void reiniciarVisibilidad(struct nodo* listadoNodos);
struct nodo* mover(struct nodo* listadoNodos, char* expresion);
struct nodo* epsilonCerrar(struct nodo* listadoNodos, struct nodo* estadosIniciales);
void agregarSubconjunto(struct nodo* DFA, char* identificador, char* expresion);
char* identificadorSubconjunto(struct nodo* subconjunto);
struct nodo* NFA_a_DFA(struct nodo* listadoNodos);
char** reducirNFA(char*** diagramaEstado, int tamanoListadoNodos, int tamanoAux);
char* NFA_a_Regex(struct nodo* listadoNodos);
int visitarParentesis(char* regex);
void NFA_Thompson_Concatenacion(struct nodo* thompson, char* regex);
void NFA_Thompson_Estrella(struct nodo* thompson, char* regex);
void NFA_Thompson_Parentesis(struct nodo* thompson, char* regex);
int pow(int base, int exponente);