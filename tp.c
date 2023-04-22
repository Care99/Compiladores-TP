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