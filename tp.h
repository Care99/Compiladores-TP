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
char*** crearDiagramaEstado(struct nodo* listadoNodos);
char*** reducirNFA(char*** diagramaEstado, int tamanoListadoNodos, int tamanoAux);
char* crearCadena(int identificador);
char* identificadorSubconjunto(struct nodo* subconjunto);
char* NFA_a_Regex(struct nodo* listadoNodos);
int esDiferente(struct estado* listadoEstados, char* expresion);
long exponente(long base, long exponente);
int subconjuntoAjeno(struct nodo* subconjuntoMinimizado);
int verificarMismoSubconjunto(struct nodo* subconjuntoMinimizado, char* identificador);
int visitarParentesis(char* regex);
struct nodo* agregarEstadosExtremos(struct nodo* listadoNodos);
struct nodo* crearNodo(char* identificador);
struct nodo* DFA_a_DFA_Minimizado(struct nodo* DFA);
struct nodo* epsilonCerrar(struct nodo* listadoNodos, struct nodo* estadosIniciales);
struct nodo* NFA_a_DFA(struct nodo* listadoNodos);
struct nodo* NFA_Thompson_Concatenacion(struct nodo* thompson, char* regex);
struct nodo* NFA_Thompson_Estrella(struct nodo* thompson, char* regex);
struct nodo* NFA_Thompson_Parentesis(struct nodo* thompson, char* regex);
struct nodo* mover(struct nodo* listadoNodos, char* expresion);
void agregarEstado(struct estado* listadoEstados, char* estado);
void agregarEstados(struct estado* listadoEstados, struct nodo* listadoNodos);
void agregarNodo(struct nodo* listadoNodos, char* identificador);
void agregarSubconjunto(struct nodo* DFA, char* identificador, char* expresion);
void agregarVertice(struct nodo* origen, struct nodo* destino, char* expresion);
void reiniciarVisibilidad(struct nodo* listadoNodos);
void renombrarNodos(struct nodo* DFA);
void subconjuntoMinimizadoIniciales(struct nodo* subconjuntoMinimizado, struct nodo* DFA);