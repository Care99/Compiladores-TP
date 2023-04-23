#include "tp.h"
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