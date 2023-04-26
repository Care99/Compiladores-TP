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
struct nodo* agregarNodo(struct nodo* listadoNodos, char* identificador);
struct nodo* crearNodo(char* identificador);
struct nodo* DFA_a_DFA_Minimizado(struct nodo* DFA);
struct nodo* epsilonCerrar(struct nodo* listadoNodos, struct nodo* estadosIniciales);
struct nodo* NFA_a_DFA(struct nodo* listadoNodos);
struct nodo* NFA_Thompson_Concatenacion(struct nodo* thompson, char* regex);
struct nodo* NFA_Thompson_Estrella(struct nodo* thompson, char* regex);
struct nodo* NFA_Thompson_Parentesis(struct nodo* thompson, char* regex);
struct nodo* mover(struct nodo* listadoNodos, char* expresion);
struct nodo* Regex_a_NFA_Thompson(struct nodo* thompson, char* regex);
void agregarEstado(struct estado* listadoEstados, char* estado);
void agregarEstados(struct estado* listadoEstados, struct nodo* listadoNodos);
void agregarSubconjunto(struct nodo* DFA, char* identificador, char* expresion);
void agregarVertice(struct nodo* origen, struct nodo* destino, char* expresion);
void reiniciarVisibilidad(struct nodo* listadoNodos);
void renombrarNodos(struct nodo* DFA);
void subconjuntoMinimizadoIniciales(struct nodo* subconjuntoMinimizado, struct nodo* DFA);

char* crearCadena(int identificador)
{
    char* cadena = (char*)malloc(sizeof(char) * 1000);
    char valorDigito = '0';
    int digitosIdentificador = -1;
    int aux = identificador;
    int digito = 0;
    long exp = 0;
    if( identificador == 0 )
    {
        valorDigito = '0';
        strcat(cadena,&valorDigito);
        return cadena;
    }
    while (aux > 0)
    {
        aux /= 10;
        digitosIdentificador++;
    }
    aux = identificador;
    while (aux > 0)
    {
        exp = exponente(10,digitosIdentificador);
        digitosIdentificador--;
        digito = (aux / (exp));
        valorDigito = '0' + digito;
        strcat(cadena,&valorDigito);
        digito = digito * (exp);
        aux = aux - digito;
    }
    return cadena;
}
struct nodo * crearNodo(char * identificador)
{
    struct nodo * n = (struct nodo *)malloc(sizeof(struct nodo));
    n->esInicio      = 0;
    n->identificador = (char*)malloc(sizeof(char) * 1000);
    n->esFin         = 0;
    strcpy(n->identificador, identificador);
    n->listaVertices = NULL;
    n->siguienteNodo = NULL;
    return n;
}
struct nodo* agregarNodo(struct nodo * listadoNodos,char * identificador)
{
    struct nodo * aux = crearNodo(identificador);
    if( listadoNodos == NULL )
    {
        aux->esInicio = 1;
    }
    aux->siguienteNodo = listadoNodos;
    listadoNodos = aux;
    return aux;
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
    aux->expresion=(char*)malloc(sizeof(char)*1000);
    strcpy(aux->expresion,estado);
    aux->siguienteEstado = listadoEstados;
    listadoEstados = aux;
}

struct nodo * agregarEstadosExtremos(struct nodo * listadoNodos)
{
    char* identificador = crearCadena(0);
    struct nodo* extremoInicial = crearNodo(identificador);
    identificador = crearCadena(atoi(listadoNodos->identificador) + 2);
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
            agregarVertice(&listadoNodos[i],extremoFinal,caracterVacio);
            listadoNodos[i].esFin=0;
        }
    }

    listadoNodos[tamanoListadoNodos-1].siguienteNodo = extremoInicial;
    extremoFinal->siguienteNodo = &listadoNodos[0];
    listadoNodos = extremoFinal;

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
            diagramaEstado[i][j] = caracterVacio;
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

void renombrarNodos(struct nodo* DFA)
{
    struct nodo* aux = DFA;
    int i=0;
    char* identificador=(char*)malloc(sizeof(char) * 1000);
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
    struct nodo* subconjuntoResto = &subconjuntoMinimizado[0];
    struct nodo* subconjuntoFin = &subconjuntoMinimizado[1];
    while( aux != NULL )
    {
        if( aux->esFin == 1 )
        {
            subconjuntoFin = agregarNodo(subconjuntoFin,0);
            subconjuntoFin->esFin = 1;
            strcpy(subconjuntoFin->identificador,aux->identificador);
            subconjuntoFin->listaVertices = aux->listaVertices;
        }
        else
        {
            subconjuntoResto = agregarNodo(subconjuntoResto,0);
            strcpy(subconjuntoResto->identificador,aux->identificador);
            subconjuntoResto->listaVertices = aux->listaVertices;
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
    char* identificador = (char*)malloc(sizeof(char) * 1000);
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
                nuevoSubconjunto = agregarNodo(nuevoSubconjunto,0);
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
                nuevoSubconjunto = agregarNodo(nuevoSubconjunto, 0);
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
    char* identificador = (char*)malloc(sizeof(char) * 1000);
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
        minimizado = agregarNodo(minimizado, identificador);
        aux = aux->siguienteNodo;
    }
    return minimizado;
}

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
    nuevoEstado = agregarNodo(nuevoEstado,identificador);
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
                nuevoEstado = agregarNodo(nuevoEstado,identificador);
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
            nuevoEstado = agregarNodo(nuevoEstado,identificador);
        }
        else
        {
            identificador = crearCadena(atoi(identificador) + 1);
            nuevoEstado = agregarNodo(nuevoEstado, identificador);
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
            if (iteracionVertice->expresion==caracterVacio && iteracionVertice->destino->esVisitado == 0)
            {
                identificador = crearCadena(atoi(identificador) + 1);
                nuevoEstado = agregarNodo(nuevoEstado, identificador);
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
            DFA = agregarNodo(DFA,identificador);
            nuevoNodo = DFA;
            agregarVertice(nuevoNodo,auxDFA,expresion);
        }
        else
        {
            auxDFA = auxDFA->siguienteNodo;
            if (auxDFA == NULL)
            {
                DFA = agregarNodo(DFA, identificador);
            }
        }
    }
}
char* identificadorSubconjunto(struct nodo* subconjunto)
{
    char* identificador = (char*)malloc(sizeof(char) * 1000);
    char separador = ',';
    struct nodo* aux = subconjunto;
    while (aux != NULL)
    {
        strcat(identificador, aux->identificador);
        strcat(identificador, &separador);
    }
    return identificador;
}
struct nodo * NFA_a_DFA(struct nodo * listadoNodos)
{
    char* identificador = (char*)malloc(sizeof(char) * 1000);
    char* expresion = (char*)malloc(sizeof(char) * 1000);
    struct estado* listadoEstados = NULL;
    struct estado* estado = NULL;
    struct nodo* DFA = NULL;
    struct nodo* iteracionDFA = NULL;
    struct nodo* nodoAux = NULL;
    agregarEstados(listadoEstados, listadoNodos);
    iteracionDFA = DFA;
    while (iteracionDFA != NULL)
    {
        if ( DFA == NULL)
        {
            identificador = identificadorSubconjunto(nodoAux);
            agregarSubconjunto(DFA, identificador,caracterVacio);
        }
        estado = listadoEstados;
        while (estado != NULL)
        {
            strcpy(expresion, estado->expresion);
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

char *** reducirNFA(char *** diagramaEstado, int tamanoListadoNodos, int tamanoAux)
{
    char * expresionOrigen  = (char*)malloc(sizeof(char)*1000);
    char * expresionCiclo   = (char*)malloc(sizeof(char)*1000);
    char * expresionDestino = (char*)malloc(sizeof(char)*1000);
    char * expresionNueva   = (char*)malloc(sizeof(char)*1000);
    char aperturaParentesis = '(';
    char cerraduraParentesis = ')';
    char estrella = '*';
    char concatenacion = '|';
    char *** diagramaAuxiliar;
    int i,j;
    diagramaAuxiliar = (char***)malloc(sizeof(char**) * tamanoAux);
    for( i = 0; i < tamanoAux; i++)
    {
        diagramaAuxiliar[i] = (char**)malloc(sizeof(char*) * tamanoAux);
        for( j = 0; j < tamanoListadoNodos; j++ )
        {
            diagramaAuxiliar[i][j] = (char*)malloc(sizeof(char) * 1000);
        }
    }
    strcpy(expresionCiclo,diagramaEstado[tamanoAux-1][tamanoAux-1]);
    if(expresionCiclo!=caracterVacio)
    {
        strcpy(expresionCiclo,strcat(&aperturaParentesis,expresionCiclo));
        strcpy(expresionCiclo,&cerraduraParentesis);
        strcpy(expresionCiclo,&estrella);
        aperturaParentesis='(';
    }
    for( i = 0; i < tamanoAux; i++ )
    {
        for( j = 0; j < tamanoAux; j++ )
        {
            strcpy(diagramaAuxiliar[i][j],diagramaEstado[i][j]);
        }
    }
    for( j = 0; j < tamanoListadoNodos; j++ )
    {
        if(j!=tamanoAux&&diagramaEstado[tamanoAux-1][j]!=caracterVacio)
        {
            strcpy(expresionDestino,diagramaEstado[tamanoAux-1][j]);
            for( i = 0; i < tamanoListadoNodos; i++ )
            {
                if(i!=tamanoAux&&diagramaEstado[i][tamanoAux-1]!=caracterVacio)
                {
                    if(diagramaEstado[i][j]!=caracterVacio)
                    {

                        strcpy(diagramaEstado[i][j],strcat(&aperturaParentesis,diagramaEstado[i][j]));
                        strcat(diagramaEstado[i][j],&cerraduraParentesis);
                        strcat(diagramaEstado[i][j],&concatenacion);
                        aperturaParentesis='(';
                    }
                    strcat(diagramaEstado[i][j],&aperturaParentesis);
                    strcpy(expresionOrigen,diagramaEstado[i][tamanoAux-1]);
                    strcpy(expresionNueva,expresionOrigen);
                    strcat(expresionNueva,expresionCiclo);
                    strcat(expresionNueva,expresionDestino);
                    strcpy(diagramaEstado[i][j],expresionNueva);
                    strcat(diagramaEstado[i][j],&cerraduraParentesis);
                }
            }
        }
    }
    return diagramaEstado;    
}
char * NFA_a_Regex(struct nodo * listadoNodos)
{
    int tamanoListadoNodos = 0;
    int tamanoAux = 0;
    char *** diagramaEstado = crearDiagramaEstado(listadoNodos);
    char * regex =(char*)malloc(sizeof(char)*1000);
    if(tamanoListadoNodos < 2)
    {
        printf("El NFA cuenta con menos de dos nodos.");
        return 0;
    }

    listadoNodos = agregarEstadosExtremos(listadoNodos);
    tamanoListadoNodos = atoi(listadoNodos->identificador);
    tamanoAux = tamanoListadoNodos; 
    diagramaEstado = crearDiagramaEstado(listadoNodos);
    while(tamanoAux > 3)
    {
        tamanoAux--;
        diagramaEstado = reducirNFA(diagramaEstado,tamanoListadoNodos,tamanoAux);
    }
    strcpy(regex,diagramaEstado[0][1]);
    return regex;
}

struct nodo* ultimoNodo(struct nodo* thompson)
{
    struct nodo* aux = thompson;
    while(aux!=NULL)
    {
        if(aux->siguienteNodo==NULL)
        {
            break;
        }
        aux = aux->siguienteNodo;
    }
    return aux;
}
int visitarParentesis(char * regex)
{
    int posicionParentesis = -1;
    int cantidadAparturaParentesis = 1;
    int cantidadCerraduraParentesis = 0;
    int i;
    int tamanoRegex = strlen(regex);
    char aperturaParentesis='(';
    char cerraduraParentesis=')';
    char simboloActual = '(';
    for(i=1;i<tamanoRegex;i++)
    {
        simboloActual = regex[i];
        if(simboloActual==aperturaParentesis)
        {
            cantidadAparturaParentesis++;
        }
        if(simboloActual==cerraduraParentesis)
        {
            cantidadCerraduraParentesis++;
        }
        if( cantidadCerraduraParentesis - cantidadAparturaParentesis == 0 )
        {
            posicionParentesis = i;
            i=tamanoRegex;
        }
    }
    return posicionParentesis;
}
struct nodo* NFA_Thompson_Concatenacion(struct nodo * thompson, char * regex)
{
    struct nodo * aperturaNodoA             = NULL;
    struct nodo * aperturaNodoB             = NULL;
    struct nodo * cerraduraNodoA            = NULL;
    struct nodo * cerraduraNodoB            = NULL;
    struct nodo * aperturaConcatenacion     = NULL;
    struct nodo * cerraduraConcatenacion    = NULL;
    char* identificador = crearCadena(atoi(thompson->identificador) + 1);
    int aperturaParentesisA     = 1;
    int cerraduraParentesisA    = visitarParentesis(&regex[aperturaParentesisA]);
    int aperturaParentesisB     = cerraduraParentesisA+2;
    if(atoi(identificador)<3)
    {
        printf("Caracter | invalido");
        return NULL;
    }

    aperturaNodoA   = thompson->siguienteNodo;
    aperturaNodoB   = NFA_Thompson_Parentesis(aperturaNodoB,&regex[aperturaParentesisB]);
    cerraduraNodoA  = thompson;
    cerraduraNodoB  = ultimoNodo(aperturaNodoB);

    thompson = agregarNodo(thompson,identificador);
    aperturaConcatenacion = thompson;
    identificador = crearCadena(atoi(identificador) + 1);
    thompson = agregarNodo(thompson,identificador);
    cerraduraConcatenacion = thompson;
    agregarVertice(aperturaConcatenacion,aperturaNodoA,caracterVacio);
    agregarVertice(aperturaConcatenacion,aperturaNodoB,caracterVacio);
    agregarVertice(cerraduraNodoA,cerraduraConcatenacion,caracterVacio);
    agregarVertice(cerraduraNodoB,cerraduraConcatenacion,caracterVacio);
    
    return aperturaConcatenacion;
}
struct nodo* NFA_Thompson_Estrella(struct nodo * thompson, char * regex)
{
    struct nodo * aperturaNodo      = NULL;
    struct nodo * cerraduraNodo     = NULL;
    struct nodo * aperturaEstrella  = NULL;
    struct nodo * cerraduraEstrella = NULL;
    char* identificador = crearCadena(atoi(thompson->identificador) + 1);
    int aperturaParentesis     = 0;
    int cerraduraParentesis    = visitarParentesis(&regex[aperturaParentesis]);
    if(cerraduraParentesis<1)
    {
        printf("Caracter * invalido");
        return NULL;
    }
    thompson = agregarNodo(thompson,identificador);
    identificador = crearCadena(atoi(identificador) + 1);
    aperturaEstrella = thompson;
    thompson = agregarNodo(thompson,thompson->identificador+1);
    cerraduraEstrella = thompson;
    NFA_Thompson_Parentesis(thompson, regex);
    cerraduraNodo = ultimoNodo(aperturaNodo);

    agregarVertice(aperturaEstrella,cerraduraEstrella,caracterVacio);
    agregarVertice(aperturaEstrella,aperturaNodo,caracterVacio);
    agregarVertice(cerraduraNodo,aperturaNodo,caracterVacio);
    agregarVertice(cerraduraNodo,cerraduraEstrella,caracterVacio);
    
    return aperturaEstrella;
}
struct nodo* NFA_Thompson_Parentesis(struct nodo * thompson, char * regex)
{
    int i;
    int aperturaParentesis = 0;
    int cerraduraParentesis = visitarParentesis(&regex[aperturaParentesis]);
    int tamanoParentesisRegex = visitarParentesis(regex);
    int tamanoSubParentesisRegex = 0;
    char* expresion = (char*)malloc(sizeof(char)*1000);
    char* identificador = (char*)malloc(sizeof(char) * 1000);
    char* regexParentesis = (char*)malloc(sizeof(char) * 1000);
    char simboloParentesis = '(';
    char simboloActual = '(';
    struct nodo* aperturaNodo = NULL;
    struct nodo* aperturaSubParentesis = NULL;
    struct nodo* cerraduraSubParentesis = NULL;
    struct nodo* cerraduraNodo = NULL;
    
    if(thompson==NULL)
    {
        identificador = crearCadena(0);
    }
    for (i = 1; i < tamanoParentesisRegex; i++)
    {
        simboloActual = regex[i];
        if (simboloActual == simboloParentesis)
        {
            if (strlen(expresion) == 0)
            {
                tamanoSubParentesisRegex = visitarParentesis(&regex[i]);
                strncpy(regexParentesis, regex, tamanoSubParentesisRegex);
                cerraduraSubParentesis = Regex_a_NFA_Thompson(thompson, regexParentesis);
                aperturaSubParentesis = ultimoNodo(aperturaSubParentesis);
            }
            else
            {
                if (thompson == NULL)
                {
                    identificador = crearCadena(atoi(identificador) + 1);
                    thompson = agregarNodo(thompson, identificador);
                }
                aperturaNodo = thompson;
                identificador = crearCadena(atoi(identificador) + 1);
                thompson = agregarNodo(thompson, identificador);
                cerraduraNodo = thompson;
                agregarVertice(aperturaNodo, cerraduraNodo, expresion);
                tamanoSubParentesisRegex = visitarParentesis(&regex[i]);
                strncpy(regexParentesis, regex, tamanoSubParentesisRegex);
                cerraduraSubParentesis = Regex_a_NFA_Thompson(thompson, regexParentesis);
                aperturaSubParentesis = ultimoNodo(aperturaSubParentesis);
            }
            aperturaSubParentesis->siguienteNodo = thompson;
            thompson = cerraduraSubParentesis;
        }
        else
        {
            strcat(expresion, &simboloActual);
        }
    }
    if (strlen(expresion) != 0)
    {
        if (thompson == NULL)
        {
            identificador = crearCadena(atoi(identificador) + 1);
            thompson = agregarNodo(thompson, identificador);
        }
        aperturaNodo = thompson;
        identificador = crearCadena(atoi(identificador) + 1);
        thompson = agregarNodo(thompson, identificador);
        cerraduraNodo = thompson;
        agregarVertice(aperturaNodo, cerraduraNodo, expresion);
    }
    return thompson;
}

struct nodo* Regex_a_NFA_Thompson(struct nodo* thompson, char* regex)
{
    int i;
    int tamanoRegex = strlen(regex);
    char simboloParentesis = '(';
    char simboloConcatenacion = '|';
    char simboloEstrella = '*';
    char simboloActual = '(';
    for(i=0;i<tamanoRegex;i++)
    {
        simboloActual=regex[i];
        if(simboloActual==simboloParentesis)
        {
            thompson = NFA_Thompson_Parentesis(thompson,&regex[i]);
            i=visitarParentesis(&regex[i]);
        }
        if(simboloActual==simboloConcatenacion)
        {
            thompson = NFA_Thompson_Concatenacion(thompson,&regex[i]);
        }
        if(simboloActual==simboloEstrella)
        {
            thompson = NFA_Thompson_Estrella(thompson,&regex[i]);
        }
    }
    return thompson;
}

long exponente(long base, long exponente)
{
    if(exponente==0)
    {
        return 1;
    }
    long respuesta = 1;
    while (exponente > 0)
    {
        respuesta *= base;
        exponente--;
    }
    return base;
}
char* corregirRegex(char*regex)
{
    char* regexPreParentesis = (char*)malloc(sizeof(char) * 1000);
    char* regexParentesis = (char*)malloc(sizeof(char) * 1000);
    char* regexPostParentesis = (char*)malloc(sizeof(char) * 1000);
    char simboloParentesis = '(';
    char simboloFinParentesis = ')';
    char simboloConcatenacion = '|';
    char simboloEstrella = '*';
    char simboloActual = '(';
    int i=0;
    int tamanoRegex = strlen(regex);
    int hayParentesis = 0;
    for(i=0;i<tamanoRegex;i++)
    {
        simboloActual = regex[i];
        if( simboloActual == simboloParentesis )
        {
            if( hayParentesis )
            {
                strncpy(regexPreParentesis,regex,i);
                strncpy(regexParentesis,&regex[i+1],visitarParentesis(&regex[i])-i);
                strcpy(regexPostParentesis,&regex[visitarParentesis(&regex[i+1])]);
                strcpy(regex,regexPreParentesis);
                strcpy(regex,regexParentesis);
                strcpy(regex,regexPostParentesis);
            }
            else
            {
                hayParentesis = 1;
            }
        }
        if( simboloActual == simboloFinParentesis )
        {
            hayParentesis = 0;
        }
    }
    return regex;
}
int main()
{
    //char* identificador=(char*)malloc(sizeof(char) * 1000);
    //char* expresion=(char*)malloc(sizeof(char) * 1000);
    char* regex=(char*)malloc(sizeof(char) * 1000);
    struct nodo* listadoNodos = NULL;
    //struct nodo* nodoInicial = NULL;
    //struct nodo* nodoFinal = NULL;
    //int i = 0;
    //int j = 0;
    //int inicio = 0;
    //int fin = 11;
    //identificador = crearCadena(0);
    //agregarNodo(listadoNodos, identificador);
    //listadoNodos->esInicio = 1;
    //nodoInicial = listadoNodos;
    //identificador = crearCadena(1);
    //agregarNodo(listadoNodos, identificador);
    //listadoNodos->esFin = 1;
    //nodoFinal = listadoNodos;
    //strcpy(expresion, (char*)'a');
    //agregarVertice(nodoInicial, nodoFinal, expresion);
    /*
        for (i = 0; i < 12; i++)
        {
            identificador = crearCadena(i);
            agregarNodo(listadoNodos,identificador);
        }
        i=0;
        for (j = 1; j < 11; j++)
        {
            identificador = crearCadena(i);
            agregarVertice( &listadoNodos[i], &listadoNodos[j], identificador );
        }
        for (i = 1; i < 11; i++)
        {
            for (j = 1; j < 12; j++)
            {
                if (i != j)
                {
                    identificador = crearCadena(i);
                    agregarVertice( &listadoNodos[i], &listadoNodos[j], identificador );
                }
            }
        }
    */
    //convertir a regex
    //char * regex = NFA_a_Regex(listadoNodos);
    //convertir a NFA con el algoritmo de Thompson
    strcpy(regex,"((a)|(b))*(abb)");
    listadoNodos = NULL;
    regex = corregirRegex(regex);
    listadoNodos = Regex_a_NFA_Thompson(listadoNodos,regex);
    //convertir a DFA
    listadoNodos = NFA_a_DFA(listadoNodos);
    //convertir a DFA minimizado
    listadoNodos = DFA_a_DFA_Minimizado(listadoNodos);
    return 0;
}