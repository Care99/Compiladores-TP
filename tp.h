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