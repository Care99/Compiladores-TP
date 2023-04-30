#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define caracterVacio NULL
struct nodo
{
    long long  esInicio;
    long long  esFin;
    long long  esVisitado;
    char* identificador;
    struct vertice* listaVertices;
    struct nodo* siguienteNodo;
    struct nodo* anteriorNodo;
    struct nodo* primerElemento;
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
char*** reducirNFA(char*** diagramaEstado, long long  tamanoListadoNodos, long long  tamanoAux);
char* identificadorSubconjunto(struct nodo* subconjunto);
char* NFA_a_Regex(struct nodo* listadoNodos);
long long  compararCadenas(char* cadena1, char* cadena2);
long long  esDiferente(struct estado* listadoEstados, char* expresion);
long long  subconjuntoAjeno(struct nodo* subconjuntoMinimizado);
long long  verificarMismoSubconjunto(struct nodo* subconjuntoMinimizado, char* identificador);
long long  visitarParentesis(char* regex);
long long  exponente(long long  base, long long  exponente);
struct estado* agregarEstado(struct estado * listadoEstados,char*estado);
struct estado* agregarEstados(struct estado * listadoEstados,struct nodo * listadoNodos);
struct nodo* agregarEstadosExtremos(struct nodo* listadoNodos);
struct nodo* agregarNodo(struct nodo* listadoNodos, char* identificador);
struct nodo* agregarSubconjunto(struct nodo* DFA, char* identificador, char* expresion);
struct nodo* crearNodo(char* identificador);
struct nodo* DFA_a_DFA_Minimizado(struct nodo* DFA);
struct nodo* epsilonCerrar(struct nodo* listadoNodos, struct nodo* estadosIniciales);
struct nodo* NFA_a_DFA(struct nodo* listadoNodos);
struct nodo* NFA_Thompson_Concatenacion(struct nodo* thompson, char* regex);
struct nodo* NFA_Thompson_Estrella(struct nodo* thompson, char* regex);
struct nodo* NFA_Thompson_Parentesis(struct nodo* thompson, char* regex);
struct nodo* mover(struct nodo* listadoNodos, char* expresion);
struct nodo* Regex_a_NFA_Thompson(struct nodo* thompson, char* regex);
struct nodo* renombrarNodos(struct nodo* DFA);
void agregarVertice(struct nodo* origen, struct nodo* destino, char* expresion);
void reiniciarVisibilidad(struct nodo* listadoNodos);
void subconjuntoMinimizadoIniciales(struct nodo* subconjuntoMinimizado, struct nodo* DFA);

struct nodo * crearNodo(char * identificador)
{
    struct nodo * n = (struct nodo *)malloc(sizeof(struct nodo));
    n->esInicio      = 0;
    n->identificador = (char*)malloc(sizeof(char) * 1000);
    n->esFin         = 0;
    sprintf(n->identificador,"%s",identificador);
    n->listaVertices = NULL;
    n->siguienteNodo = NULL;
    n->primerElemento = NULL;
    return n;
}
struct nodo* agregarNodo(struct nodo * listadoNodos,char * identificador)
{
    struct nodo * aux = crearNodo(identificador);
    if( listadoNodos == NULL )
    {
        aux->esInicio = 1;
        aux->primerElemento = aux;
    }
    else
    {
        aux->siguienteNodo = listadoNodos;
        aux->primerElemento = listadoNodos->primerElemento;
        listadoNodos->anteriorNodo = aux;
    }
    listadoNodos = aux;
    return aux;
}
void agregarVertice(struct nodo * origen, struct nodo * destino, char * expresion)
{
    struct vertice* aux = (struct vertice*)malloc(sizeof(struct vertice));
    aux->destino = destino;
    aux->expresion = (char*)malloc(sizeof(char) * 1000);
    if(expresion!=caracterVacio)
    {
        if(strlen(expresion)==1)
        {
            aux->expresion[0]=expresion[0];
            aux->expresion[1]='\0';
        }
        else
        {
            strcpy(aux->expresion,expresion);
        }
    }
    else{
        aux->expresion=NULL;
    }
    aux->siguienteVertice = NULL;
    if(origen->listaVertices != NULL)
    {
        aux->siguienteVertice = origen->listaVertices;
    }
    origen->listaVertices = aux;
}

struct estado* agregarEstado(struct estado * listadoEstados,char*estado)
{
    struct estado * aux = (struct estado *)malloc(sizeof(struct estado));
    aux->expresion=(char*)malloc(sizeof(char)*1000);
    if( strlen(estado)==1 )
    {
        aux->expresion[0]=estado[0];
        aux->expresion[1]='\0';
    }
    else
    {
        strcpy(aux->expresion,estado);
    }
    aux->siguienteEstado = listadoEstados;
    listadoEstados = aux;
}

struct nodo * agregarEstadosExtremos(struct nodo * listadoNodos)
{
    char* identificador = (char*)malloc(sizeof(char) * 1000);
    sprintf(identificador,"%d",0);
    struct nodo* extremoInicial = crearNodo(identificador);
    sprintf(identificador,"%d",atoi(listadoNodos->identificador) + 2);
    struct nodo* extremoFinal = crearNodo(identificador);
    struct nodo* aux = listadoNodos;
    extremoInicial->esInicio = 1;
    extremoInicial->esFin = 1;
    
    long long  i=0;
    long long  tamanoListadoNodos = atoi(listadoNodos->identificador);
    while( aux!=NULL)
    {
        sprintf(identificador,"%d",atoi(aux->identificador) + 1);
        if( strlen(identificador)==1 )
        {
            aux->identificador[0]=identificador[0];
            aux->identificador[1]='\0';
        }
        else
        {
            strcpy(aux->identificador, identificador);
        }
        if( listadoNodos[i].esInicio == 1 )
        {
            agregarVertice(extremoInicial,aux,caracterVacio);
            listadoNodos[i].esInicio=0;
        }
        if( listadoNodos[i].esFin == 1 )
        {
            agregarVertice(aux,extremoFinal,caracterVacio);
            listadoNodos[i].esFin=0;
        }
        aux=aux->siguienteNodo;
    }

    aux = listadoNodos->primerElemento;
    aux->siguienteNodo=extremoInicial;
    extremoFinal->siguienteNodo = listadoNodos;
    listadoNodos = extremoFinal;

    return listadoNodos;
}
char *** crearDiagramaEstado(struct nodo * listadoNodos)
{
    long long  tamanoListadoNodos = atoi(listadoNodos->identificador);
    long long  i,j;
    char*** diagramaEstado = (char***)malloc(sizeof(char**) * tamanoListadoNodos);
    char* identificador = (char*)malloc(sizeof(char) * 1000);
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

            strcpy(identificador, diagramaEstado[i][atoi(destinoVertice->identificador)]);
            if(strlen(identificador)==1)
            {
                iteracionVertice->expresion[0]=identificador[0];
                iteracionVertice->expresion[1]='\0';
            }
            else
            {
                strcpy(iteracionVertice->expresion,identificador);
            }
            
            iteracionVertice = iteracionVertice->siguienteVertice;
        }
    }
    return diagramaEstado;
}

struct nodo * renombrarNodos(struct nodo* DFA)
{
    struct nodo* aux = DFA;
    long long  i=0;
    char* identificador=(char*)malloc(sizeof(char) * 1000);
    while(aux!=NULL)
    {
        sprintf(identificador,"%d",i);
        if(strlen(identificador)==1)
        {
            aux->identificador[0]=identificador[0];
            aux->identificador[1]='\0';
        }
        else
        {
            strcpy(aux->identificador,identificador);
        }
        i++;
        aux=aux->siguienteNodo;
    }
    return DFA;
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
            if(strlen(aux->identificador)==1)
            {
                subconjuntoFin->identificador[0]=aux->identificador[0];
                subconjuntoFin->identificador[1]='\0';
            }
            else
            {
                strcpy(subconjuntoFin->identificador,aux->identificador);
            }
            subconjuntoFin->listaVertices = aux->listaVertices;
        }
        else
        {
            subconjuntoResto = agregarNodo(subconjuntoResto,0);
            if(strlen(aux->identificador)==1)
            {
                subconjuntoResto->identificador[0]=aux->identificador[0];
                subconjuntoResto->identificador[1]='\0';
            }
            else
            {
                strcpy(subconjuntoResto->identificador,aux->identificador);
            }
            
            subconjuntoResto->listaVertices = aux->listaVertices;
        }
        aux = aux->siguienteNodo;
    }
}
long long  verificarMismoSubconjunto(struct nodo* subconjuntoMinimizado, char* identificador)
{
    struct nodo* aux = NULL;
    long long  pertenece = 1;
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
long long  subconjuntoAjeno( struct nodo* subconjuntoMinimizado )
{
    struct vertice* verticeAux = subconjuntoMinimizado->listaVertices;
    struct nodo* nuevoSubconjunto = subconjuntoMinimizado;
    char* identificador = (char*)malloc(sizeof(char) * 1000);
    long long  ajenoEncontrado = 0;
    while( verticeAux != NULL )
    {
        if(strlen(verticeAux->destino->identificador)==1)
        {
            identificador[0]=verticeAux->destino->identificador[0];
            identificador[1]='\0';
        }
        else
        {
            strcpy(identificador,verticeAux->destino->identificador);
        }
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
    long long  hayNuevoSubconjunto = 1;
    renombrarNodos(DFA);
    subconjuntoMinimizadoIniciales(subconjuntoMinimizado,DFA);
    while (hayNuevoSubconjunto)
    {
        hayNuevoSubconjunto = subconjuntoAjeno(subconjuntoMinimizado);
    }
    aux = subconjuntoMinimizado;
    while (aux != NULL)
    {
        identificador = identificadorSubconjunto(aux);
        minimizado = agregarNodo(minimizado, identificador);
        aux = aux->siguienteNodo;
    }
    return minimizado;
}
long long  compararCadenas(char* cadena1, char* cadena2)
{
    long long  respuesta = 0;
    long long  tamanoCadena1 = strlen(cadena1);
    long long  tamanoCadena2 = strlen(cadena2);
    if( tamanoCadena1 != tamanoCadena2 )
    {
        respuesta = 1;
        return respuesta;
    }
    for(long long  i = 0; i < tamanoCadena1; i++)
    {
        if( cadena1[i] != cadena2[i] )
        {
            respuesta = 1;
            return respuesta;
        }
    }
    return respuesta;
}
long long  esDiferente(struct estado * listadoEstados,char*expresion)
{
    struct estado * aux = listadoEstados;
    long long  sonDiferentes = 0;
    long long  respuesta = 1;
    while( aux != NULL )
    {
        sonDiferentes = compararCadenas(aux->expresion,expresion);
        if (sonDiferentes)
        {
            aux = aux->siguienteEstado;
        }
        else
        {
            respuesta = 0;
            aux = NULL;
        }
    }
    return respuesta;
}
struct estado* agregarEstados(struct estado * listadoEstados,struct nodo * listadoNodos)
{
    struct vertice * iteracionVertice = NULL;
    struct nodo* nodoAux = listadoNodos;
    char* expresion = (char*)malloc(sizeof(char) * 1000);
    long long  tamanoNodos = atoi(nodoAux->identificador);
    while( nodoAux != NULL )
    {
        iteracionVertice = nodoAux->listaVertices;
        while (iteracionVertice != NULL && iteracionVertice->expresion != caracterVacio)
        {
            if(strlen(iteracionVertice->expresion)==1)
            {
                expresion[0]=iteracionVertice->expresion[0];
                expresion[1]='\0';
            }
            else
            {
                strcpy(expresion,iteracionVertice->expresion);
            }
            if( esDiferente(listadoEstados,expresion) )
            {
                listadoEstados = agregarEstado(listadoEstados,expresion);
            }
            iteracionVertice = iteracionVertice->siguienteVertice;
        }
        nodoAux = nodoAux->siguienteNodo;
    }
    return listadoEstados;
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
    char* identificador = (char*)malloc(sizeof(char) * 1000);
    sprintf(identificador,"%d",0);
    nuevoEstado = agregarNodo(nuevoEstado,identificador);
    aux = listadoNodos;
    while(aux!=NULL)
    {
        iteracionVertice = aux->listaVertices;
        aux->esVisitado = 1;
        while(iteracionVertice!=NULL)
        {
            if( strcmp(iteracionVertice->expresion,expresion) && iteracionVertice->destino != NULL && iteracionVertice->destino->esVisitado == 0 )
            {
                sprintf(identificador,"%d",atoi(nuevoEstado->identificador) + 1);
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
    char* identificador = (char*)malloc(sizeof(char) * 1000);
    sprintf(identificador,"%d",0);
    struct nodo* nuevoEstado = NULL;
    struct nodo* aux = NULL;
    struct vertice* iteracionVertice = NULL;
    reiniciarVisibilidad(listadoNodos);
    aux = estadosIniciales;
    while(aux!=NULL)
    {
        if(nuevoEstado==NULL)
        {
            nuevoEstado = agregarNodo(nuevoEstado,identificador);
        }
        else
        {
            sprintf(identificador,"%d",atoi(identificador) + 1);
            nuevoEstado = agregarNodo(nuevoEstado, identificador);
        }
        nuevoEstado->listaVertices = aux->listaVertices;
        aux = aux->siguienteNodo;
    }
    aux = listadoNodos;
    while(aux!=NULL)
    {
        iteracionVertice = aux->listaVertices;
        aux->esVisitado = 1;
        while(iteracionVertice!=NULL)
        {
            if (iteracionVertice->expresion==caracterVacio && iteracionVertice->destino != NULL && iteracionVertice->destino->esVisitado == 0)
            {
                sprintf(identificador,"%d",atoi(identificador) + 1);
                nuevoEstado = agregarNodo(nuevoEstado, identificador);
                nuevoEstado->listaVertices = iteracionVertice->destino->listaVertices;
            }
            iteracionVertice=iteracionVertice->siguienteVertice;
        }
        aux = aux->siguienteNodo;
    }
    return nuevoEstado;
}

struct nodo* agregarSubconjunto(struct nodo* DFA, char* identificador, char* expresion)
{
    struct nodo * auxDFA = DFA;
    struct nodo * nuevoNodo = NULL;
    if( auxDFA == NULL )
    {
        DFA = agregarNodo(DFA,identificador);
        return DFA;
    }
    while (auxDFA != NULL)
    {
        if (strcmp(auxDFA->identificador, identificador))
        {
            DFA = agregarNodo(DFA,identificador);
            nuevoNodo = DFA;
            agregarVertice(nuevoNodo,auxDFA,expresion);
            break;
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
    return DFA;
}
char* identificadorSubconjunto(struct nodo* subconjunto)
{
    char* identificador = (char*)malloc(sizeof(char) * 1000);
    char separador = ',';
    struct nodo* aux = subconjunto;
    while (aux != NULL)
    {
        strcat(identificador, aux->identificador);
        identificador[strlen(identificador)]=separador;
        aux=aux->siguienteNodo;
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
    listadoNodos = renombrarNodos(listadoNodos);
    listadoEstados = agregarEstados(listadoEstados, listadoNodos);

    nodoAux = epsilonCerrar(listadoNodos,listadoNodos->primerElemento);
    identificador = identificadorSubconjunto(nodoAux);
    DFA = agregarSubconjunto(DFA, identificador,caracterVacio);
    iteracionDFA = DFA;
    while (iteracionDFA != NULL)
    {
        estado = listadoEstados;
        while (estado != NULL)
        {
            if(strlen(estado->expresion)==1)
            {
                expresion[0]=estado->expresion[0];
                expresion[1]='\0';
            }
            else
            {
                strcpy(expresion, estado->expresion);
            }
            nodoAux = mover(iteracionDFA, expresion);
            nodoAux = epsilonCerrar(listadoNodos,nodoAux);
            identificador = identificadorSubconjunto(nodoAux);
            DFA = agregarSubconjunto(DFA, identificador,expresion);
            estado = estado->siguienteEstado;
        }
        iteracionDFA = iteracionDFA->siguienteNodo;
    }
    return DFA;
}

char *** reducirNFA(char *** diagramaEstado, long long  tamanoListadoNodos, long long  tamanoAux)
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
    long long  i,j;
    diagramaAuxiliar = (char***)malloc(sizeof(char**) * tamanoAux);
    for( i = 0; i < tamanoAux; i++)
    {
        diagramaAuxiliar[i] = (char**)malloc(sizeof(char*) * tamanoAux);
        for( j = 0; j < tamanoListadoNodos; j++ )
        {
            diagramaAuxiliar[i][j] = (char*)malloc(sizeof(char) * 1000);
        }
    }
    if(strlen(diagramaEstado[tamanoAux-1][tamanoAux-1])==1)
    {
        expresionCiclo[0]=diagramaEstado[tamanoAux-1][tamanoAux-1][0];
        expresionCiclo[1]='\0';
    }
    else
    {
        strcpy(expresionCiclo,diagramaEstado[tamanoAux-1][tamanoAux-1]);
    }
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
            if(strlen(diagramaEstado[i][j])==1)
            {
                diagramaAuxiliar[i][j][0]=diagramaEstado[i][j][0];
                diagramaAuxiliar[i][j][1]='\0';
            }
            else
            {
                strcpy(diagramaAuxiliar[i][j],diagramaEstado[i][j]);
            }
        }
    }
    for( j = 0; j < tamanoListadoNodos; j++ )
    {
        if(j!=tamanoAux&&diagramaEstado[tamanoAux-1][j]!=caracterVacio)
        {
            if(strlen(diagramaEstado[tamanoAux-1][j])==1)
            {
                expresionDestino[0]=diagramaEstado[tamanoAux-1][j][0];
                expresionDestino[1]='\0';
            }
            else
            {
                strcpy(expresionDestino,diagramaEstado[tamanoAux-1][j]);
            }
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
    long long  tamanoListadoNodos = 0;
    long long  tamanoAux = 0;
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
long long  visitarParentesis(char * regex)
{
    long long  posicionParentesis = -1;
    long long  cantidadAparturaParentesis = 1;
    long long  cantidadCerraduraParentesis = 0;
    long long  i;
    long long  tamanoRegex = strlen(regex);
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
    char* identificador = (char*)malloc(sizeof(char)*1000);
    char* regexParentesis = (char*)malloc(sizeof(char) * 1000);
    sprintf(identificador,"%d",0);
    long long  aperturaParentesisA     = 0;
    long long  cerraduraParentesisA    = visitarParentesis(&regex[aperturaParentesisA]);
    long long  aperturaParentesisB     = cerraduraParentesisA+2;
    long long  cerraduraParentesisB    = aperturaParentesisB + visitarParentesis(&regex[aperturaParentesisB]);
    if(cerraduraParentesisA<2 || cerraduraParentesisB<6)
    {
        printf("Caracter | invalido");
        return NULL;
    }

    strncpy(regexParentesis,&regex[aperturaParentesisA],cerraduraParentesisA+1);    
    cerraduraNodoA = Regex_a_NFA_Thompson(cerraduraNodoA,regexParentesis);
    aperturaNodoA = cerraduraNodoA->primerElemento;

    strncpy(regexParentesis,&regex[aperturaParentesisB],cerraduraParentesisB);
    cerraduraNodoB = NFA_Thompson_Parentesis(cerraduraNodoB,regexParentesis);
    aperturaNodoB = cerraduraNodoB->primerElemento;

    cerraduraNodoA->anteriorNodo = aperturaNodoB;
    aperturaNodoB->siguienteNodo = cerraduraNodoA;

    aperturaConcatenacion = crearNodo(identificador);
    aperturaConcatenacion->anteriorNodo = aperturaNodoA;
    aperturaNodoA->siguienteNodo = aperturaConcatenacion;

    cerraduraConcatenacion = crearNodo(identificador);
    cerraduraConcatenacion->siguienteNodo = cerraduraNodoB;
    cerraduraNodoB->anteriorNodo = cerraduraConcatenacion;

    agregarVertice(aperturaConcatenacion,aperturaNodoA,caracterVacio);
    agregarVertice(aperturaConcatenacion,aperturaNodoB,caracterVacio);
    agregarVertice(cerraduraNodoA,cerraduraConcatenacion,caracterVacio);
    agregarVertice(cerraduraNodoB,cerraduraConcatenacion,caracterVacio);
    
    if(thompson==NULL)
    {
        thompson = cerraduraConcatenacion;
        cerraduraConcatenacion->primerElemento = aperturaConcatenacion;
        aperturaConcatenacion->primerElemento = aperturaConcatenacion;
    }
    else
    {
        thompson->siguienteNodo = aperturaConcatenacion;
        aperturaConcatenacion->anteriorNodo = thompson;
        cerraduraConcatenacion->primerElemento = thompson->primerElemento;
        aperturaConcatenacion->primerElemento = thompson->primerElemento;
    }

    return thompson;
}
struct nodo* NFA_Thompson_Estrella(struct nodo * thompson, char * regex)
{
    struct nodo * aperturaNodo      = NULL;
    struct nodo * cerraduraNodo     = NULL;
    struct nodo * aperturaEstrella  = NULL;
    struct nodo * cerraduraEstrella = NULL;
    char* identificador = (char*)malloc(sizeof(char)*1000);
    sprintf(identificador,"%d",0);
    char* regexParentesis = (char*)malloc(sizeof(char) * 1000);;
    long long  aperturaParentesis     = 0;
    long long  cerraduraParentesis    = visitarParentesis(&regex[aperturaParentesis]);
    if(cerraduraParentesis<1)
    {
        printf("Caracter * invalido");
        return NULL;
    }
    
    strncpy(regexParentesis,&regex[aperturaParentesis],cerraduraParentesis+1);
    
    cerraduraNodo = Regex_a_NFA_Thompson(cerraduraNodo,regexParentesis);
    aperturaNodo = cerraduraNodo->primerElemento;
    
    aperturaEstrella = crearNodo(identificador);
    aperturaEstrella->anteriorNodo = aperturaNodo;
    aperturaNodo->siguienteNodo = aperturaEstrella;

    cerraduraEstrella = crearNodo(identificador);
    cerraduraEstrella->siguienteNodo = cerraduraNodo;
    cerraduraNodo->anteriorNodo = cerraduraEstrella;
    

    agregarVertice(aperturaEstrella,cerraduraEstrella,caracterVacio);
    agregarVertice(aperturaEstrella,aperturaNodo,caracterVacio);
    agregarVertice(cerraduraNodo,aperturaNodo,caracterVacio);
    agregarVertice(cerraduraNodo,cerraduraEstrella,caracterVacio);

    if(thompson==NULL)
    {
        thompson = cerraduraEstrella;
        cerraduraEstrella->primerElemento = aperturaEstrella;
        aperturaEstrella->primerElemento = aperturaEstrella;
    }
    else
    {
        thompson->siguienteNodo = aperturaEstrella;
        aperturaEstrella->anteriorNodo = thompson;
        cerraduraEstrella->primerElemento = thompson->primerElemento;
        aperturaEstrella->primerElemento = thompson->primerElemento;
    }
    
    return thompson;
}
struct nodo* NFA_Thompson_Parentesis(struct nodo * thompson, char * regex)
{
    long long  i;
    long long  aperturaParentesis = 0;
    long long  cerraduraParentesis = 0;
    long long  tamanoParentesisRegex = visitarParentesis(&regex[aperturaParentesis]);
    long long  tamanoSubParentesisRegex = 0;
    char* expresion = (char*)malloc(sizeof(char)*1000);
    char* identificador = (char*)malloc(sizeof(char) * 1000);
    char* regexParentesis = (char*)malloc(sizeof(char) * 1000);
    char simboloParentesis = '(';
    char simboloActual = '(';
    char simboloConcatenacion='|';
    char simboloEstrella = '*';
    struct nodo* aperturaNodo = NULL;
    struct nodo* aperturaSubParentesis = NULL;
    struct nodo* cerraduraSubParentesis = NULL;
    struct nodo* cerraduraNodo = NULL;
    
    sprintf(identificador,"%d",0);
    for (i = 1; i < tamanoParentesisRegex; i++)
    {
        simboloActual = regex[i];
        if (simboloActual == simboloParentesis)
        {
            aperturaParentesis = i;
            cerraduraParentesis = aperturaParentesis + visitarParentesis(&regex[aperturaParentesis]);
            if (strlen(expresion) > 0)
            {
                if (thompson == NULL)
                {
                    thompson = agregarNodo(thompson, identificador);
                }
                aperturaNodo = thompson;
                thompson = agregarNodo(thompson, identificador);
                cerraduraNodo = thompson;
                agregarVertice(aperturaNodo, cerraduraNodo, expresion);
            }

            if( cerraduraParentesis < tamanoParentesisRegex - 1 )
            {
                simboloActual = regex[cerraduraParentesis+1];
                if(simboloActual==simboloConcatenacion)
                {
                    thompson = NFA_Thompson_Concatenacion(thompson,&regex[i]);
                    return thompson;
                }
                if(simboloActual==simboloEstrella)
                {
                    thompson = NFA_Thompson_Estrella(thompson,&regex[i]);
                    return thompson;
                }
            }
            thompson = cerraduraSubParentesis;
        }
        else
        {
            expresion[0] = simboloActual;
            expresion[1] = '\0';
            if (strlen(expresion) != 0)
            {
                if (thompson == NULL)
                {
                    thompson = agregarNodo(thompson, identificador);
                }
                aperturaNodo = thompson;
                thompson = agregarNodo(thompson, identificador);
                cerraduraNodo = thompson;
                agregarVertice(aperturaNodo, cerraduraNodo, expresion);
            }
        }
    }
    return thompson;
}

struct nodo* Regex_a_NFA_Thompson(struct nodo* thompson, char* regex)
{
    long long  i;
    long long  tamanoRegex = strlen(regex);
    long long  posicionAperturaParentesis = 0;
    long long  posicionCerraduraParentesis = 0;
    char simboloParentesis = '(';
    char simboloConcatenacion = '|';
    char simboloEstrella = '*';
    char simboloActual = '(';
    for(i=0;i<tamanoRegex;i++)
    {
        simboloActual=regex[i];
        if(simboloActual==simboloParentesis)
        {
            posicionAperturaParentesis = i;
            posicionCerraduraParentesis = posicionAperturaParentesis + visitarParentesis(&regex[i]);
            if( posicionCerraduraParentesis < tamanoRegex - 1 )
            {
                simboloActual = regex[posicionCerraduraParentesis+1];
                if(simboloActual==simboloConcatenacion)
                {
                    thompson = NFA_Thompson_Concatenacion(thompson,&regex[i]);
                    i= posicionCerraduraParentesis;
                    continue;
                }
                if(simboloActual==simboloEstrella)
                {
                    thompson = NFA_Thompson_Estrella(thompson,&regex[i]);
                    i=posicionCerraduraParentesis;
                    continue;
                }
            }
            thompson = NFA_Thompson_Parentesis(thompson,&regex[i]);
            i=posicionCerraduraParentesis;
        }
    }
    return thompson;
}

long long  exponente(long long  base, long long  exponente)
{
    if(exponente==0)
    {
        return 1;
    }
    long long  respuesta = 1;
    while (exponente > 0)
    {
        respuesta *= base;
        exponente--;
    }
    return base;
}
long long  main()
{
    //char* identificador=(char*)malloc(sizeof(char) * 1000);
    //char* expresion=(char*)malloc(sizeof(char) * 1000);
    char* regex=(char*)malloc(sizeof(char) * 1000);
    struct nodo* listadoNodos = NULL;
    //struct nodo* nodoInicial = NULL;
    //struct nodo* nodoFinal = NULL;
    //long long  i = 0;
    //long long  j = 0;
    //long long  inicio = 0;
    //long long  fin = 11;
    //sprintf(identificador,"%d",0);
    //agregarNodo(listadoNodos, identificador);
    //listadoNodos->esInicio = 1;
    //nodoInicial = listadoNodos;
    //sprintf(identificador,"%d",1);
    //agregarNodo(listadoNodos, identificador);
    //listadoNodos->esFin = 1;
    //nodoFinal = listadoNodos;
    //strcpy(expresion, (char*)'a');
    //agregarVertice(nodoInicial, nodoFinal, expresion);
    /*
        for (i = 0; i < 12; i++)
        {
            sprintf(identificador,"%d",i);
            agregarNodo(listadoNodos,identificador);
        }
        i=0;
        for (j = 1; j < 11; j++)
        {
            identificador = sprintf(identificador,"%d",i);
            agregarVertice( &listadoNodos[i], &listadoNodos[j], identificador );
        }
        for (i = 1; i < 11; i++)
        {
            for (j = 1; j < 12; j++)
            {
                if (i != j)
                {
                    identificador = sprintf(identificador,"%d",i);
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
    //regex = corregirRegex(regex);
    listadoNodos = Regex_a_NFA_Thompson(listadoNodos,regex);
    //convertir a DFA
    listadoNodos = NFA_a_DFA(listadoNodos);
    //convertir a DFA minimizado
    listadoNodos = DFA_a_DFA_Minimizado(listadoNodos);
    return 0;
}