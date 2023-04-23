#include "tp.h"
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
        if( strcmp(&simboloActual,&aperturaParentesis) )
        {
            cantidadAparturaParentesis++;
        }
        if( strcmp(&simboloActual,&cerraduraParentesis) )
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
struct nodo* NFA_Thompson_Concatenacion(struct nodo * thompson, char * regex)
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
    if(atoi(aperturaNodoB->identificador)<3)
    {
        printf("Caracter | invalido");
        return NULL;
    }

    NFA_Thompson_Parentesis(aperturaNodoA,&regex[aperturaParentesisA]);
    NFA_Thompson_Parentesis(aperturaNodoB,&regex[aperturaParentesisB]);
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
struct nodo* NFA_Thompson_Parentesis(struct nodo * thompson, char * regex)
{
    int tamanoRegex = strlen(regex);
    int i;
    int aperturaParentesis = 0;
    int cerraduraParentesis;
    char* identificador = crearCadena(atoi(thompson->identificador) + 1);
    char simboloParentesis = '(';
    char simboloConcatenacion = '|';
    char simboloEstrella = '*';
    char simboloActual = '(';
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
        simboloActual=regex[i];
        if(strcmp(&simboloActual,&simboloParentesis))
        {
            NFA_Thompson_Concatenacion(thompson,&regex[i]);
        }
        cerraduraParentesis = visitarParentesis(&regex[i]);
        if(cerraduraParentesis<tamanoRegex)
        {
            simboloActual=regex[cerraduraParentesis+1];
            if( strcmp(&simboloActual,&simboloConcatenacion) )
            {
                NFA_Thompson_Concatenacion(thompson,&regex[i]);
            }
            if( strcmp(&simboloActual,&simboloEstrella) )
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