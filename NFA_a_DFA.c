#include "tp.h"
int esDiferente(struct estado * listadoEstados,char*expresion)
{
    struct estado * aux = listadoEstados;
    int respuesta = 1;
    while( aux != NULL )
    {
        if (strcmp(aux->expresion, expresion))
        {
            respuesta = 0;
            aux = NULL;
        }
        else
        {
            aux = aux->siguienteEstado;
        }
    }
    return respuesta;
}
void agregarEstados(struct estado * listadoEstados,struct nodo * listadoNodos)
{
    struct vertice * iteracionVertice = NULL;
    char * expresion = NULL;
    int tamanoNodos = atoi(listadoNodos->identificador);
    for (int i = 0; i < tamanoNodos; i++)
    {
        iteracionVertice = listadoNodos->listaVertices;
        while (iteracionVertice != NULL)
        {
            strcpy(expresion,iteracionVertice->expresion);
            if( esDiferente(listadoEstados,expresion) )
            {
                agregarEstado(listadoEstados,expresion);
            }
        }
    }
}
void reiniciarVisibilidad(struct nodo * listadoNodos)
{
    struct nodo * aux = &listadoNodos[0];
    while( aux != NULL )
    {
        aux->esVisitado = 0;
        aux = aux->siguienteNodo;
    }
}

struct nodo * mover(struct nodo * listadoNodos,char * expresion)
{
    reiniciarVisibilidad(listadoNodos);
    struct nodo * nuevoEstado = NULL;
    struct nodo * aux = NULL;
    struct vertice * iteracionVertice = NULL;
    char* identificador = crearCadena(0);
    agregarNodo(nuevoEstado,identificador);
    aux = &listadoNodos[0];
    while(aux!=NULL)
    {
        iteracionVertice = aux->listaVertices;
        aux->esVisitado = 1;
        while(iteracionVertice!=NULL)
        {
            if( strcmp(iteracionVertice->expresion,expresion) && iteracionVertice->destino->esVisitado == 0 )
            {
                identificador = crearCadena(atoi(nuevoEstado->identificador) + 1);
                agregarNodo(nuevoEstado,identificador);
                nuevoEstado->listaVertices = aux->listaVertices;
            }
            iteracionVertice=iteracionVertice->siguienteVertice;
        }
        aux = aux->siguienteNodo;
    }
    return nuevoEstado;
}

struct nodo * epsilonCerrar(struct nodo * listadoNodos,struct nodo * estadosIniciales)
{
    char* identificador = crearCadena(0);
    struct nodo* nuevoEstado = NULL;
    struct nodo* aux = NULL;
    struct vertice* iteracionVertice = NULL;
    reiniciarVisibilidad(listadoNodos);
    aux = &estadosIniciales[0];
    while(aux!=NULL)
    {
        if(nuevoEstado==NULL)
        {
            agregarNodo(nuevoEstado,identificador);
        }
        else
        {
            identificador = crearCadena(atoi(identificador) + 1);
            agregarNodo(nuevoEstado, identificador);
        }
        nuevoEstado->listaVertices = aux->listaVertices;
        aux = aux->siguienteNodo;
    }
    aux = &listadoNodos[0];
    while(aux!=NULL)
    {
        iteracionVertice = aux->listaVertices;
        aux->esVisitado = 1;
        while(iteracionVertice!=NULL)
        {
            if (strcmp(iteracionVertice->expresion, caracterVacio) && iteracionVertice->destino->esVisitado == 0)
            {
                identificador = crearCadena(atoi(identificador) + 1);
                agregarNodo(nuevoEstado, identificador);
                nuevoEstado->listaVertices = aux->listaVertices;
            }
            iteracionVertice=iteracionVertice->siguienteVertice;
        }
        aux = aux->siguienteNodo;
    }
    return nuevoEstado;
}

void agregarSubconjunto(struct nodo* DFA, char* identificador, char* expresion)
{
    struct nodo * auxDFA = DFA;
    struct nodo * nuevoNodo = NULL;
    while (auxDFA != NULL)
    {
        if (strcmp(auxDFA->identificador, identificador))
        {
            agregarNodo(DFA,identificador);
            nuevoNodo = DFA;
            agregarVertice(nuevoNodo,auxDFA,expresion);
        }
        else
        {
            auxDFA = auxDFA->siguienteNodo;
            if (auxDFA == NULL)
            {
                agregarNodo(DFA, identificador);
            }
        }
    }
}
char* identificadorSubconjunto(struct nodo* subconjunto)
{
    char* identificador = (char*)malloc(sizeof(char) * 1000);
    struct nodo* aux = subconjunto;
    while (aux != NULL)
    {
        strcat(identificador, aux->identificador);
        strcat(identificador, (char*)',');
    }
    return identificador;
}
struct nodo * NFA_a_DFA(struct nodo * listadoNodos)
{
    char* identificador = NULL;
    char* expresion = NULL;
    struct estado* listadoEstados = NULL;
    struct estado* estado = NULL;
    struct nodo* DFA = NULL;
    struct nodo* iteracionDFA = NULL;
    struct nodo* nodoAux = NULL;
    int haySubconjuntoNuevo = 1;
    agregarEstados(listadoEstados, listadoNodos);
    iteracionDFA = DFA;
    while (iteracionDFA == NULL)
    {
        haySubconjuntoNuevo = 0;
        if ( DFA == NULL)
        {
            haySubconjuntoNuevo = 1;
            identificador = identificadorSubconjunto(nodoAux);
            agregarSubconjunto(DFA, identificador,caracterVacio);
        }
        estado = listadoEstados;
        while (estado != NULL)
        {
            strcpy(expresion, expresion);
            nodoAux = mover(iteracionDFA, expresion);
            nodoAux = epsilonCerrar(listadoNodos,nodoAux);
            identificador = identificadorSubconjunto(nodoAux);
            agregarSubconjunto(DFA, identificador,expresion);
            estado = estado->siguienteEstado;
        }
        iteracionDFA = iteracionDFA->siguienteNodo;
    }
    return DFA;
}