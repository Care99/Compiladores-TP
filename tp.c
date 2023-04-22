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
int pow(int base, int exponente)
{
    while (exponente > 0)
    {
        base *= base;
        exponente--;
    }
    return base;
}
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
char ** reducirNFA(char *** diagramaEstado, int tamanoListadoNodos, int tamanoAux)
{
    char * expresionOrigen  = (char*)malloc(sizeof(char)*1000);
    char * expresionCiclo   = (char*)malloc(sizeof(char)*1000);
    char * expresionDestino = (char*)malloc(sizeof(char)*1000);
    char * expresionNueva   = (char*)malloc(sizeof(char)*1000);
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
    if(!strcmp(expresionCiclo,caracterVacio))
    {
        strcpy(expresionCiclo,strcat((char*)'(',expresionCiclo));
        strcpy(expresionCiclo,")*");
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
        if(j!=tamanoAux&&!strcmp(diagramaEstado[tamanoAux-1][j],caracterVacio))
        {
            strcpy(expresionDestino,diagramaEstado[tamanoAux-1][j]);
            for( i = 0; i < tamanoListadoNodos; i++ )
            {
                if(i!=tamanoAux&&!strcmp(diagramaEstado[i][tamanoAux-1],caracterVacio))
                {
                    if(!strcmp(diagramaEstado[i][j],caracterVacio))
                    {

                        strcpy(diagramaEstado[i][j],strcat((char*)'(',diagramaEstado[i][j]));
                        strcat(diagramaEstado[i][j],")|");
                    }
                    strcat(diagramaEstado[i][j],(char*)'(');
                    strcpy(expresionOrigen,diagramaEstado[i][tamanoAux-1]);
                    strcpy(expresionNueva,expresionOrigen);
                    strcat(expresionNueva,expresionCiclo);
                    strcat(expresionNueva,expresionDestino);
                    strcpy(diagramaEstado[i][j],expresionNueva);
                    strcat(diagramaEstado[i][j],(char*)')');
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
    int i = 0;
    int j = 0;
    char *** diagramaEstado = creardiagramaEstado(listadoNodos);
    char * regex =(char*)malloc(sizeof(char)*1000);
    if(tamanoListadoNodos < 2)
    {
        printf("El NFA cuenta con menos de dos nodos.");
        return 0;
    }

    listadoNodos = agregarEstadosExtremos(diagramaEstado);
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
int visitarParentesis(char * regex)
{
    int posicionParentesis = -1;
    int cantidadAparturaParentesis = 1;
    int cantidadCerraduraParentesis = 0;
    int i;
    int tamanoRegex = strlen(regex);
    for(i=1;i<tamanoRegex;i++)
    {
        if( strcmp(&regex[i],(char*)'(') )
        {
            cantidadAparturaParentesis++;
        }
        if( strcmp(&regex[i],(char*)')') )
        {
            cantidadCerraduraParentesis++;
        }
        if( cantidadCerraduraParentesis - cantidadAparturaParentesis == 0 )
        {
            posicionParentesis = i;
            i=0;
        }
    }
    return posicionParentesis;
}
void NFA_Thompson_Concatenacion(struct nodo * thompson, char * regex)
{
    struct nodo * aperturaNodoA             = NULL;
    struct nodo * aperturaNodoB             = NULL;
    struct nodo * cerraduraNodoA            = NULL;
    struct nodo * cerraduraNodoB            = NULL;
    struct nodo * aperturaConcatenacion     = NULL;
    struct nodo * cerraduraConcatenacion    = NULL;
    char* identificador = crearCadena(atoi(thompson->identificador) + 1);
    int aperturaParentesisA     = 0;
    int cerraduraParentesisA    = visitarParentesis(regex);
    int aperturaParentesisB     = cerraduraParentesisA+2;
    int cerraduraParentesisB    = visitarParentesis(&regex[aperturaParentesisB]);;
    if(aperturaNodoB<3)
    {
        printf("Caracter | invalido");
        return NULL;
    }

    NFA_Thompson_Parentesis(aperturaNodoA,&regex[aperturaParentesisA]);
    NFA_Thompson_Parentesis(aperturaNodoB,&regex[aperturaParentesisA]);
    cerraduraNodoA          = ultimoNodo(aperturaNodoA);
    cerraduraNodoB          = ultimoNodo(aperturaNodoB);

    agregarNodo(thompson,identificador);
    aperturaConcatenacion = thompson;
    identificador = crearCadena(atoi(identificador) + 1);
    agregarNodo(thompson,identificador);
    cerraduraConcatenacion = thompson;
    agregarVertice(aperturaConcatenacion,aperturaNodoA,caracterVacio);
    agregarVertice(aperturaConcatenacion,aperturaNodoB,caracterVacio);
    agregarVertice(cerraduraNodoA,cerraduraConcatenacion,caracterVacio);
    agregarVertice(cerraduraNodoB,cerraduraConcatenacion,caracterVacio);
    
    return aperturaConcatenacion;
}
void NFA_Thompson_Estrella(struct nodo * thompson, char * regex)
{
    struct nodo * aperturaNodo      = NULL;
    struct nodo * cerraduraNodo     = NULL;
    struct nodo * aperturaEstrella  = NULL;
    struct nodo * cerraduraEstrella = NULL;
    char* identificador = crearCadena(atoi(thompson->identificador) + 1);
    int aperturaParentesis     = 0;
    int cerraduraParentesis    = visitarParentesis(regex);
    if(cerraduraParentesis<1)
    {
        printf("Caracter * invalido");
        return NULL;
    }
    agregarNodo(thompson,identificador);
    identificador = crearCadena(atoi(identificador) + 1);
    aperturaEstrella = thompson;
    agregarNodo(thompson,thompson->identificador+1);
    cerraduraEstrella = thompson;
    NFA_Thompson_Parentesis(thompson, regex);
    cerraduraNodo = ultimoNodo(aperturaNodo);

    agregarVertice(aperturaEstrella,cerraduraEstrella,caracterVacio);
    agregarVertice(aperturaEstrella,aperturaNodo,caracterVacio);
    agregarVertice(cerraduraNodo,aperturaNodo,caracterVacio);
    agregarVertice(cerraduraNodo,cerraduraEstrella,caracterVacio);
    
    return aperturaEstrella;
}
void NFA_Thompson_Parentesis(struct nodo * thompson, char * regex)
{
    int tamanoRegex = strlen(regex);
    int i;
    int idNodoThompson = 0;
    int aperturaParentesis = 0;
    int cerraduraParentesis;
    char* identificador = crearCadena(atoi(thompson->identificador) + 1);
    char * simboloParentesis = '(';
    char * simboloConcatenacion = '|';
    char * simboloEstrella = '*';
    char * simboloActual = NULL;
    struct nodo * aperturaNodo = NULL;
    struct nodo * cerraduraNodo = NULL;
    char * expresion = (char*)malloc(sizeof(char)*1000);
    agregarNodo(thompson,identificador);
    aperturaNodo = thompson;
    identificador = crearCadena(atoi(identificador) + 1);
    agregarNodo(thompson,identificador);
    cerraduraNodo = thompson;
    for(i=1;i<tamanoRegex;i++)
    {
        strcmp(simboloActual,&regex[i]);
        if(strcmp(simboloActual,simboloParentesis))
        {
            NFA_Thompson_Concatenacion(thompson,&regex[i]);
        }
        cerraduraParentesis = visitarParentesis(&regex[i]);
        if(cerraduraParentesis<tamanoRegex)
        {
            strcmp(simboloActual,&regex[cerraduraParentesis+1]);
            if( strcmp(simboloActual,simboloConcatenacion) )
            {
                NFA_Thompson_Concatenacion(thompson,&regex[i]);
            }
            if( strcmp(simboloActual,simboloEstrella) )
            {
                NFA_Thompson_Estrella(thompson,&regex[i]);
            }
        }
        
        strncat(expresion,&regex[aperturaParentesis+1],cerraduraParentesis-aperturaParentesis-2);
        i=cerraduraParentesis;
    }
    agregarVertice(aperturaNodo,cerraduraNodo,expresion);
    return thompson;
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
            nodoAux = mover(iteracionDFA, estado->expresion);
            nodoAux = epsilonCerrar(listadoNodos,nodoAux);
            identificador = identificadorSubconjunto(nodoAux);
            agregarSubconjunto(DFA, identificador,estado->expresion);
            estado = estado->siguienteEstado;
        }
        iteracionDFA = iteracionDFA->siguienteNodo;
    }
    return DFA;
}
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
int subconjuntoAjeno( struct nodo* subconjuntoMinimizado )
{
    struct vertice* verticeAux = subconjuntoMinimizado->listaVertices;
    char* identificador = NULL;
    while( verticeAux != NULL )
    {
        strcpy(identificador,verticeAux->destino->identificador);
        if( nodoAjeno(subconjuntoMinimizado,identificador) )
        {
            
        }
    }
}
struct nodo* DFA_a_DFA_Minimizado(struct nodo* DFA)
{
    struct nodo* subconjuntoMinimizado = (struct nodo*)malloc(sizeof(struct nodo) * 1000);
    struct nodo* minimizado = NULL;
    int i;
    int hayNuevoSubconjunto = 1;
    subconjuntoMinimizadoIniciales(subconjuntoMinimizado,DFA);
    renombrarNodos(DFA);
    while(hayNuevoSubconjunto)
    {
        if( subcojuntoAjeno(subconjuntoMinimizado) )
        {
            hayNuevoSubconjunto = 1;
        }
    }
    return minimizado;
}
int main()
{
    char* identificador;
    struct nodo* listadoNodos = NULL;
    int i, j;
    int inicio = 0;
    int fin = 11;
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
    //convertir a regex
    char * regex = NFA_a_Regex(listadoNodos);
    //convertir a NFA con el algoritmo de Thompson
    listadoNodos = NULL;
    NFA_Thompson_Parentesis(listadoNodos,regex);
    //convertir a DFA
    listadoNodos = NFA_a_DFA(listadoNodos);
    //convertir a DFA minimizado
    //listadoNodos = DFA_a_DFA_Minimizado(listadoNodos);
    return 0;
}