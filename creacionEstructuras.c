#include "tp.h"
char* crearCadena(int identificador)
{
    char* cadena = (char*)malloc(sizeof(char) * 1000);
    int digitosIdentificador = 0;
    int aux = identificador;
    int digito = 0;
    int exponente = 0;
    while (aux > 0)
    {
        aux /= 10;
        digitosIdentificador++;
    }
    aux = identificador;
    while (digitosIdentificador > 0)
    {
        exponente = pow(10,digitosIdentificador);
        digito = (aux / (exponente));
        strcat(cadena, digito + '0');
        digito = digito * (exponente);
        aux = aux - digito;
    }
    return cadena;
}
struct nodo * crearNodo(char * identificador)
{
    struct nodo * n = (struct nodo *)malloc(sizeof(struct nodo));
    n->esInicio      = 0;
    n->esFin         = 0;
    strcpy(n->identificador, identificador);
    n->listaVertices = NULL;
    n->siguienteNodo = NULL;
    return n;
}
void agregarNodo(struct nodo * listadoNodos,char * identificador)
{
    struct nodo * aux = crearNodo(identificador);
    if( listadoNodos == NULL )
    {
        aux->esInicio = 1;
    }
    if (strcmp(listadoNodos->identificador, "11"))
    {
        aux->esFin = 1;
    }
    aux->siguienteNodo = listadoNodos;
    listadoNodos = aux;
}
void agregarVertice(struct nodo * origen, struct nodo * destino, char * expresion)
{
    struct vertice* aux = (struct vertice*)malloc(sizeof(struct vertice));
    aux->destino = destino;
    aux->expresion = (char*)malloc(sizeof(char) * 1000);
    strcpy(aux->expresion,expresion);
    aux->siguienteVertice = NULL;
    if(origen->listaVertices != NULL)
    {
        aux->siguienteVertice = origen->listaVertices;
    }
    origen->listaVertices = aux;
}

void agregarEstado(struct estado * listadoEstados,char*estado)
{
    struct estado * aux = (struct estado *)malloc(sizeof(struct estado));
    strcpy(aux->expresion,estado);
    aux->siguienteEstado = listadoEstados;
    listadoEstados = aux;
}

struct nodo * agregarEstadosExtremos(struct nodo * listadoNodos)
{
    char* identificador = crearCadena(0);
    struct nodo* extremoInicial = crearNodo(identificador);
    identificador = crearCadena(listadoNodos->identificador + 2);
    struct nodo* extremoFinal = crearNodo(identificador);

    extremoInicial->esInicio = 1;
    extremoInicial->esFin = 1;
    
    int i;
    int tamanoListadoNodos = atoi(listadoNodos->identificador);
    for(i=0;i<tamanoListadoNodos;i++)
    {
        strcpy(listadoNodos[i].identificador, crearCadena(atoi(listadoNodos[i].identificador) + 1));
        if( listadoNodos[i].esInicio == 1 )
        {
            agregarVertice(extremoInicial,&listadoNodos[i],caracterVacio);
            listadoNodos[i].esInicio=0;
        }
        if( listadoNodos[i].esFin == 1 )
        {
            agregarVertice(extremoFinal,&listadoNodos[i],caracterVacio);
            listadoNodos[i].esFin=0;
        }
    }

    listadoNodos[tamanoListadoNodos-1].siguienteNodo = &extremoInicial;
    extremoFinal->siguienteNodo = &listadoNodos[0];
    listadoNodos = &extremoFinal;

    return listadoNodos;
}
char *** crearDiagramaEstado(struct nodo * listadoNodos)
{
    int tamanoListadoNodos = atoi(listadoNodos->identificador);
    int i,j;
    char *** diagramaEstado = (char***)malloc(sizeof(char**) * tamanoListadoNodos);
    struct vertice * iteracionVertice;
    struct nodo * destinoVertice;
    for( i = 0; i < tamanoListadoNodos; i++)
    {
        diagramaEstado[i] = (char**)malloc(sizeof(char*) * tamanoListadoNodos);
        for( j = 0; j < tamanoListadoNodos; j++ )
        {
            diagramaEstado[i][j] = (char*)malloc(sizeof(char) * 1000);
        }
    }
    for( i = 0; i < tamanoListadoNodos; i++ )
    {
        for( j = 0; j < tamanoListadoNodos; j++ )
        {
            strcpy(diagramaEstado[i][j],caracterVacio);
        }
    }
    for( i = 0; i < tamanoListadoNodos; i++ )
    {
        iteracionVertice = listadoNodos[i].listaVertices;
        while( iteracionVertice != NULL )
        {
            destinoVertice = iteracionVertice->destino;
            strcpy(iteracionVertice->expresion,diagramaEstado[i][atoi(destinoVertice->identificador)]);
            iteracionVertice = iteracionVertice->siguienteVertice;
        }
    }
    return diagramaEstado;
}