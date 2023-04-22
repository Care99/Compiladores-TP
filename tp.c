#include "tp.h"
int pow(int base, int exponente)
{
    while (exponente > 0)
    {
        base *= base;
        exponente--;
    }
    return base;
}
int main()
{
    char* identificador=NULL;
    char* expresion=NULL;
    struct nodo* listadoNodos = NULL;
    struct nodo* nodoInicial = NULL;
    struct nodo* nodoFinal = NULL;
    int i = 0;
    int j = 0;
    int inicio = 0;
    int fin = 11;
    identificador = crearCadena(0);
    agregarNodo(listadoNodos, identificador);
    listadoNodos->esInicio = 1;
    nodoInicial = listadoNodos;
    identificador = crearCadena(1);
    agregarNodo(listadoNodos, identificador);
    listadoNodos->esFin = 1;
    nodoFinal = listadoNodos;
    strcpy(expresion, 'a');
    agregarVertice(nodoInicial, nodoFinal, expresion);
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
    char * regex = NFA_a_Regex(listadoNodos);
    //convertir a NFA con el algoritmo de Thompson
    listadoNodos = NULL;
    NFA_Thompson_Parentesis(listadoNodos,regex);
    //convertir a DFA
    listadoNodos = NFA_a_DFA(listadoNodos);
    //convertir a DFA minimizado
    listadoNodos = DFA_a_DFA_Minimizado(listadoNodos);
    return 0;
}