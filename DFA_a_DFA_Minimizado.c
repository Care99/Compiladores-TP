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
void subconjuntoMinimizadoIniciales(struct nodo* subconjuntoMinimizado, struct nodo* DFA)
{
    struct nodo* aux = DFA;
    while( aux != NULL )
    {
        if( aux->esFin == 1 )
        {
            agregarNodo(&subconjuntoMinimizado[1],0);
        }
        else
        {
            agregarNodo(&subconjuntoMinimizado[0],0);
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
    struct nodo* nuevoSubconjunto = subconjuntoMinimizado;
    char* identificador = NULL;
    int ajenoEncontrado = 0;
    while( verticeAux != NULL )
    {
        strcpy(identificador,verticeAux->destino->identificador);
        if( !verificarMismoSubconjunto(subconjuntoMinimizado,identificador) )
        {
            if (ajenoEncontrado==0)
            {
                ajenoEncontrado = 1;
                while (nuevoSubconjunto != NULL)
                {
                    nuevoSubconjunto = nuevoSubconjunto->siguienteNodo;
                }
                agregarNodo(nuevoSubconjunto,0);
            }
            else
            {
                while (nuevoSubconjunto != NULL)
                {
                    nuevoSubconjunto = nuevoSubconjunto->siguienteNodo;
                    if (nuevoSubconjunto->siguienteNodo == NULL)
                    {
                        break;
                    }
                }
                agregarNodo(nuevoSubconjunto, 0);
            }
        }
    }
    return ajenoEncontrado;
}
struct nodo* DFA_a_DFA_Minimizado(struct nodo* DFA)
{
    struct nodo* subconjuntoMinimizado = (struct nodo*)malloc(sizeof(struct nodo) * 1000);
    struct nodo* minimizado = NULL;
    struct nodo* aux = NULL;
    char* identificador = NULL;
    int hayNuevoSubconjunto = 1;
    renombrarNodos(DFA);
    subconjuntoMinimizadoIniciales(subconjuntoMinimizado,DFA);
    while (hayNuevoSubconjunto)
    {
        hayNuevoSubconjunto = subconjuntoAjeno(subconjuntoMinimizado);
    }
    aux = subconjuntoMinimizado;
    while (aux != NULL)
    {
        strcpy(identificador, identificadorSubconjunto(aux));
        agregarNodo(minimizado, identificador);
        aux = aux->siguienteNodo;
    }
    return minimizado;
}