#include "tp.h"
void renombrarNodos(struct nodo* DFA)
{
    struct nodo* aux = DFA;
    int i=0;
    char* identificador=NULL;
    while(aux!=NULL)
    {
        identificador=crearCadena(i);
        strcpy(aux->identificador,identificador);
        i++;
        aux=aux->siguienteNodo;
    }
}
void subconjuntoMinimizadoIniciales(struct nodo** subconjuntoMinimizado, struct nodo* DFA)
{
    struct nodo* aux = DFA;
    subconjuntoMinimizado[0] = (struct nodo*)malloc(sizeof(struct nodo) * 1000);
    subconjuntoMinimizado[1] = (struct nodo*)malloc(sizeof(struct nodo) * 1000);
    while( aux != NULL )
    {
        if( aux->esFin == 1 )
        {
            agregarNodo(subconjuntoMinimizado[1],0);
        }
        else
        {
            agregarNodo(subconjuntoMinimizado[0],0);
        }
        aux = aux->siguienteNodo;
    }
}
int verificarMismoSubconjunto(struct nodo* subconjuntoMinimizado, char* identificador)
{
    struct nodo* aux = NULL;
    int pertenece = 1;
    while (aux != NULL)
    {
        if (!strcmp(aux->identificador, identificador))
        {
            pertenece = 0;
        }
        aux = aux->siguienteNodo;
    }
    return pertenece;
}
int subconjuntoAjeno( struct nodo* subconjuntoMinimizado )
{
    struct vertice* verticeAux = subconjuntoMinimizado->listaVertices;
    char* identificador = NULL;
    int ajenoEncontrado = 0;
    while( verticeAux != NULL )
    {
        strcpy(identificador,verticeAux->destino->identificador);
        if( !verificarMismoSubconjunto(subconjuntoMinimizado,identificador) )
        {
            if (!ajenoEncontrado)
            {
                ajenoEncontrado = 1;
            }
        }
    }
    return ajenoEncontrado;
}
struct nodo* DFA_a_DFA_Minimizado(struct nodo* DFA)
{
    struct nodo* subconjuntoMinimizado = (struct nodo*)malloc(sizeof(struct nodo) * 1000);
    struct nodo* minimizado = NULL;
    int i;
    int hayNuevoSubconjunto = 1;
    renombrarNodos(DFA);
    subconjuntoMinimizadoIniciales(subconjuntoMinimizado,DFA);
    while (hayNuevoSubconjunto)
    {
        hayNuevoSubconjunto = subcojuntoAjeno(subconjuntoMinimizado);
    }
    return minimizado;
}