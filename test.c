#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define numeroGrande 100000
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
int compararCadenas(char* expresion1,char* expresion2);
int esVacio(char* expresion);
long long  esDiferente(struct estado* listadoEstados, char* expresion);
int subconjuntoAjeno(struct nodo** subconjuntoMinimizado);
long long  verificarMismoSubconjunto(struct nodo* subconjuntoMinimizado, char* identificador);
long long  visitarParentesis(char* regex);
long long  exponente(long long  base, long long  exponente);
struct estado* agregarEstado(struct estado* listadoEstados, char* estado);
struct estado* agregarEstados(struct estado* listadoEstados, struct nodo* listadoNodos);
struct nodo* epsilonCerrar(struct nodo* listadoNodos, struct nodo* estadosIniciales);
struct nodo* agregarEstadosExtremos(struct nodo* listadoNodos);
struct nodo* agregarNodo(struct nodo* listadoNodos, char* identificador);
struct nodo* agregarSubconjunto(struct nodo* DFA,struct nodo** subconjunto, struct nodo* iteracionDFA, struct nodo* subconjuntoActual, char* identificadorResultante, char* expresion);
struct nodo* crearNodo(char* identificador);
struct nodo* DFA_a_DFA_Minimizado(struct nodo* DFA);
struct nodo* mover(struct nodo* listadoNodos, char* expresion);
struct nodo* NFA_a_DFA(struct nodo* listadoNodos);
struct nodo* NFA_Thompson_Concatenacion(struct nodo* thompson, char* regex);
struct nodo* NFA_Thompson_Estrella(struct nodo* thompson, char* regex);
struct nodo* NFA_Thompson_Parentesis(struct nodo* thompson, char* regex);
struct nodo* Regex_a_NFA_Thompson(struct nodo* thompson, char* regex);
struct nodo* renombrarNodos(struct nodo* DFA);
void agregarVertice(struct nodo* origen, struct nodo* destino, char* expresion);
struct nodo* reiniciarVisibilidad(struct nodo* listadoNodos);
struct nodo** subconjuntoMinimizadoIniciales(struct nodo** subconjuntoMinimizado, struct nodo* DFA);
void imprimirDiagramaEstado(char***diagramaEstado,int tamano);
int esVacio(char* expresion)
{
    if(expresion==NULL)
    {
        return 0;
    }
    char* caracterVacio = (char*)malloc(sizeof(char)*3);
    strcpy(caracterVacio,"ε");
    int respuesta = 0;
    if( strcmp(expresion,caracterVacio)==0 )
    {
        respuesta=1;
    }
    return respuesta;
}
struct nodo* crearNodo(char* identificador)
{
    struct nodo* n = (struct nodo*)malloc(sizeof(struct nodo));
    n->esInicio = 0;
    n->identificador = (char*)malloc(sizeof(char) * numeroGrande);
    n->esFin = 0;
    sprintf(n->identificador, "%s", identificador);
    n->listaVertices = NULL;
    n->siguienteNodo = NULL;
    n->primerElemento = NULL;
    return n;
}
struct nodo* agregarNodo(struct nodo* listadoNodos, char* identificador)
{
    struct nodo* aux = crearNodo(identificador);
    if (listadoNodos == NULL)
    {
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
void agregarVertice(struct nodo* origen, struct nodo* destino, char* expresion)
{
    struct vertice* aux = (struct vertice*)malloc(sizeof(struct vertice));
    
    aux->destino = destino;
    aux->expresion = (char*)malloc(sizeof(char) * numeroGrande);
    strcpy(aux->expresion, expresion);
    aux->siguienteVertice = NULL;
    
    if (origen->listaVertices != NULL)
    {
        aux->siguienteVertice = origen->listaVertices;
    }
    
    origen->listaVertices = aux;
}

struct estado* agregarEstado(struct estado* listadoEstados, char* estado)
{
    struct estado* aux = (struct estado*)malloc(sizeof(struct estado));
    aux->expresion = (char*)malloc(sizeof(char) * numeroGrande);
    if (strlen(estado) == 1)
    {
        aux->expresion[0] = estado[0];
        aux->expresion[1] = '\0';
    }
    else
    {
        strcpy(aux->expresion, estado);
    }
    aux->siguienteEstado = listadoEstados;
    listadoEstados = aux;
    return listadoEstados;
}

struct nodo* agregarEstadosExtremos(struct nodo* listadoNodos)
{
    char* caracterVacio = (char*)malloc(sizeof(char)*3);
    strcpy(caracterVacio,"ε");
    char* identificador = (char*)malloc(sizeof(char) * numeroGrande);
    struct nodo* aux = listadoNodos;
    struct nodo* extremoInicial = NULL;
    struct nodo* extremoFinal = NULL;

    sprintf(identificador, "%d", 0);
    extremoInicial = crearNodo(identificador);
    extremoInicial->anteriorNodo = listadoNodos->primerElemento;
    listadoNodos->primerElemento->siguienteNodo = extremoInicial;

    sprintf(identificador, "%d", atoi(listadoNodos->identificador) + 2);
    listadoNodos = agregarNodo(listadoNodos,identificador);
    extremoFinal = listadoNodos;
    extremoFinal->primerElemento = extremoInicial;

    listadoNodos = renombrarNodos(listadoNodos);
    while (aux != NULL)
    {

        aux->primerElemento = extremoInicial;

        if (aux->esInicio == 1)
        {
            agregarVertice(extremoInicial, aux, caracterVacio);
            aux->esInicio = 0;
        }

        if (aux->esFin == 1)
        {
            agregarVertice(aux, extremoFinal, caracterVacio);
            aux->esFin = 0;
        }
        aux = aux->siguienteNodo;
    }

    extremoInicial->esInicio=1;
    extremoFinal->esFin=1;

    return listadoNodos;
}
char*** crearDiagramaEstado(struct nodo* listadoNodos)
{
    int  tamanoListadoNodos = atoi(listadoNodos->identificador) + 1;
    int  i, j;
    char*** diagramaEstado = (char***)malloc(sizeof(char**) * tamanoListadoNodos);
    int identificadorOrigen = 0;
    int identificadorDestino = 0;
    char* expresion = (char*)malloc(sizeof(char) * numeroGrande);
    struct vertice* iteracionVertice;
    struct nodo* origenVertice=NULL;
    struct nodo* destinoVertice=NULL;

    for (i = 0; i < tamanoListadoNodos; i++)
    {
        diagramaEstado[i] = (char**)malloc(sizeof(char*) * tamanoListadoNodos);
        for (j = 0; j < tamanoListadoNodos; j++)
        {
            diagramaEstado[i][j] = (char*)malloc(sizeof(char) * numeroGrande);
        }
    }
    origenVertice = listadoNodos;
    while(origenVertice!=NULL)
    {
        identificadorOrigen = atoi(origenVertice->identificador);
        iteracionVertice = origenVertice->listaVertices;
        
        while (iteracionVertice != NULL)
        {
            destinoVertice = iteracionVertice->destino;
            identificadorDestino = atoi(destinoVertice->identificador);
            strcpy(expresion,iteracionVertice->expresion);
            if(strlen(diagramaEstado[identificadorOrigen][identificadorDestino])>0)
            {
                sprintf(expresion,"(%s)|(%s)",diagramaEstado[identificadorOrigen][identificadorDestino],iteracionVertice->expresion);
            }
            strcpy(diagramaEstado[identificadorOrigen][identificadorDestino],expresion);
            iteracionVertice = iteracionVertice->siguienteVertice;
        }
        origenVertice=origenVertice->siguienteNodo;
    }
    return diagramaEstado;
}

struct nodo* renombrarNodos(struct nodo* DFA)
{
    struct nodo* aux = DFA->primerElemento;
    int  i = 0;
    char* identificador = (char*)malloc(sizeof(char) * numeroGrande);
    while (aux != NULL)
    {
        sprintf(identificador, "%d", i);
        strcpy(aux->identificador, identificador);
        i++;
        aux = aux->anteriorNodo;
        if(i==1000000)
        {
            printf(".");
        }
    }
    return DFA;
}
struct nodo** subconjuntoMinimizadoIniciales(struct nodo** subconjuntoMinimizado, struct nodo* DFA)
{
    struct nodo* aux = DFA;
    struct nodo* subconjuntoResto = subconjuntoMinimizado[0];
    struct nodo* subconjuntoFin = subconjuntoMinimizado[1];
    while (aux != NULL)
    {
        if (aux->esFin == 1)
        {
            subconjuntoFin = agregarNodo(subconjuntoFin,aux->identificador);
            subconjuntoFin->listaVertices = aux->listaVertices;
        }
        else
        {
            subconjuntoResto = agregarNodo(subconjuntoResto,aux->identificador);
            subconjuntoResto->listaVertices = aux->listaVertices;
        }
        aux = aux->siguienteNodo;
    }
    return subconjuntoMinimizado;
}
long long  verificarMismoSubconjunto(struct nodo* subconjuntoMinimizado, char* identificador)
{
    struct nodo* aux = NULL;
    long long  pertenece = 1;
    while (aux != NULL)
    {
        if (strcmp(aux->identificador, identificador)==0 )
        {
            pertenece = 0;
        }
        aux = aux->siguienteNodo;
    }
    return pertenece;
}
int subconjuntoAjeno(struct nodo** subconjuntoMinimizado)
{
    struct nodo* nodoAux = subconjuntoMinimizado[0];
    struct vertice* verticeAux = subconjuntoMinimizado[0]->listaVertices;
    struct nodo* nuevoSubconjunto = subconjuntoMinimizado[0];
    char* identificador = (char*)malloc(sizeof(char) * numeroGrande);
    long long  ajenoEncontrado = 0;
    int i=0;
    while(nuevoSubconjunto!=NULL)
    {
        i++;
        nuevoSubconjunto = subconjuntoMinimizado[i];
    }
    while(nodoAux!=NULL)
    {
        verticeAux=nodoAux->listaVertices;
        while (verticeAux != NULL)
        {
            strcpy(identificador, verticeAux->destino->identificador);
            if (!verificarMismoSubconjunto(subconjuntoMinimizado[0], identificador))
            {
                nuevoSubconjunto = agregarNodo(nuevoSubconjunto, identificador);
            }
            verticeAux = verticeAux->siguienteVertice;
        }
        nodoAux=nodoAux->siguienteNodo;
    }
    return ajenoEncontrado;
}
struct nodo* DFA_a_DFA_Minimizado(struct nodo* DFA)
{
    struct nodo** subconjuntoMinimizado = (struct nodo**)malloc(sizeof(struct nodo*) * numeroGrande);
    struct nodo* minimizado = NULL;
    struct nodo* aux = NULL;
    char* identificador = (char*)malloc(sizeof(char) * numeroGrande);
    int i = 0;
    int hayNuevoSubconjunto = 1;
    DFA = renombrarNodos(DFA);
    subconjuntoMinimizado =  subconjuntoMinimizadoIniciales(subconjuntoMinimizado, DFA);
    while (hayNuevoSubconjunto)
    {
        hayNuevoSubconjunto = subconjuntoAjeno(subconjuntoMinimizado);
    }
    aux = subconjuntoMinimizado[0];
    i=0;
    while (aux != NULL)
    {
        minimizado = agregarNodo(minimizado, identificador);
        minimizado->listaVertices = aux->listaVertices;
        i++;
        aux=subconjuntoMinimizado[i];
    }
    return minimizado;
}
long long  esDiferente(struct estado* listadoEstados, char* expresion)
{
    struct estado* aux = listadoEstados;
    long long  sonDiferentes = 0;
    long long  respuesta = 1;
    while (aux != NULL)
    {
        sonDiferentes = strcmp(aux->expresion, expresion);
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
struct estado* agregarEstados(struct estado* listadoEstados, struct nodo* listadoNodos)
{
    struct vertice* iteracionVertice = NULL;
    struct nodo* nodoAux = listadoNodos;
    char* expresion = (char*)malloc(sizeof(char) * numeroGrande);
    while (nodoAux != NULL)
    {
        iteracionVertice = nodoAux->listaVertices;
        while (iteracionVertice != NULL && !esVacio(iteracionVertice->expresion))
        {
            strcpy(expresion, iteracionVertice->expresion);
            if (esDiferente(listadoEstados, expresion))
            {
                listadoEstados = agregarEstado(listadoEstados, expresion);
            }
            iteracionVertice = iteracionVertice->siguienteVertice;
        }
        nodoAux = nodoAux->siguienteNodo;
    }
    return listadoEstados;
}
struct nodo* reiniciarVisibilidad(struct nodo* listadoNodos)
{
    struct nodo* aux = &listadoNodos[0];
    while (aux != NULL)
    {
        aux->esVisitado = 0;
        aux = aux->siguienteNodo;
    }
    return listadoNodos;
}
int compararCadenas(char* expresion1,char* expresion2)
{
    int respuesta=0;
    if( strcmp(expresion1,expresion2) == 0 )
    {
        respuesta=1;
    }
    return respuesta;
}
struct nodo* mover(struct nodo* listadoNodos, char* expresion)
{
    reiniciarVisibilidad(listadoNodos);
    struct nodo* nuevoEstado = NULL;
    struct nodo* aux = NULL;
    struct vertice* iteracionVertice = NULL;
    char* identificador = (char*)malloc(sizeof(char) * numeroGrande);
    aux = listadoNodos;
    while (aux != NULL)
    {
        iteracionVertice = aux->listaVertices;
        aux->esVisitado = 1;
        while (iteracionVertice != NULL)
        {
            if (compararCadenas(iteracionVertice->expresion, expresion) && iteracionVertice->destino != NULL && iteracionVertice->destino->esVisitado == 0)
            {
                iteracionVertice->destino->esVisitado = 1;
                sprintf(identificador, "%s", iteracionVertice->destino->identificador);
                nuevoEstado = agregarNodo(nuevoEstado, identificador);
                nuevoEstado->listaVertices = aux->listaVertices->destino->listaVertices;
            }
            iteracionVertice = iteracionVertice->siguienteVertice;
        }
        aux = aux->siguienteNodo;
    }

    return nuevoEstado;
}

struct nodo* epsilonCerrar(struct nodo* listadoNodos, struct nodo* estadosIniciales)
{
    char* identificador = (char*)malloc(sizeof(char) * numeroGrande);
    struct nodo* subconjunto = NULL;
    struct nodo* nodosVisitados = NULL;
    struct nodo* aux = NULL;
    struct vertice* iteracionVertice = NULL;
    listadoNodos = reiniciarVisibilidad(listadoNodos);
    aux = estadosIniciales;
    while (aux != NULL)
    {
        strcpy(identificador, aux->identificador);

        subconjunto = agregarNodo(subconjunto, identificador);
        subconjunto->listaVertices = aux->listaVertices;

        aux = aux->siguienteNodo;
    }

    aux = estadosIniciales;
    while (aux != NULL)
    {
        iteracionVertice = aux->listaVertices;
        aux->esVisitado = 1;

        while (iteracionVertice != NULL)
        {
            if (iteracionVertice->destino->esVisitado == 0)
            {
                strcpy(identificador, iteracionVertice->destino->identificador);
                nodosVisitados = agregarNodo(nodosVisitados, identificador);
                nodosVisitados->listaVertices = iteracionVertice->destino->listaVertices;
            }

            if (esVacio(iteracionVertice->expresion) && iteracionVertice->destino != NULL && iteracionVertice->destino->esVisitado == 0)
            {
                strcpy(identificador, iteracionVertice->destino->identificador);
                subconjunto = agregarNodo(subconjunto, identificador);
                subconjunto->listaVertices = iteracionVertice->destino->listaVertices;

                iteracionVertice->destino->esVisitado = 1;
            }
            iteracionVertice = iteracionVertice->siguienteVertice;
        }
        aux = aux->siguienteNodo;
    }

    aux = nodosVisitados->primerElemento;
    while (aux != NULL)
    {
        iteracionVertice = aux->listaVertices;
        aux->esVisitado = 1;
        while (iteracionVertice != NULL)
        {
            if (iteracionVertice->destino->esVisitado == 0)
            {
                strcpy(identificador, iteracionVertice->destino->identificador);
                nodosVisitados = agregarNodo(nodosVisitados, iteracionVertice->destino->identificador);
                nodosVisitados->listaVertices = iteracionVertice->destino->listaVertices;
            }

            if (esVacio(iteracionVertice->expresion) && iteracionVertice->destino != NULL && iteracionVertice->destino->esVisitado == 0)
            {
                strcpy(identificador, iteracionVertice->destino->identificador);
                subconjunto = agregarNodo(subconjunto, identificador);
                subconjunto->listaVertices = iteracionVertice->destino->listaVertices;

                iteracionVertice->destino->esVisitado = 1;
            }
            iteracionVertice = iteracionVertice->siguienteVertice;
        }
        aux = aux->anteriorNodo;
    }
    return subconjunto;
}

struct nodo* agregarSubconjunto(struct nodo* DFA,struct nodo** subconjunto, struct nodo* iteracionDFA, struct nodo* subconjuntoActual, char* identificadorResultante, char* expresion)
{
    int i=0;
    char* identificadorIteracion = (char*)malloc(sizeof(char) * numeroGrande);
    struct nodo* auxDFA = NULL;
    struct nodo* origen = NULL;
    struct nodo* destino = NULL;
    auxDFA = DFA;
    while (auxDFA != NULL)
    {
        strcpy(identificadorIteracion,auxDFA->identificador);
        if (strcmp(identificadorIteracion, identificadorResultante)==0)
        {
            origen = iteracionDFA;
            destino = auxDFA;
            agregarVertice(origen, destino, expresion);
            return DFA;
        }
        else
        {
            if (auxDFA->siguienteNodo == NULL)
            {
                DFA = agregarNodo(DFA, identificadorResultante);
                origen = iteracionDFA;
                destino = DFA;
                agregarVertice(origen, destino, expresion);
                while(subconjunto[i]!=NULL)
                {
                    i++;
                }
                subconjunto[i]=subconjuntoActual;
            }
        }
        auxDFA=auxDFA->siguienteNodo;
    }
    return DFA;
}
char* identificadorSubconjunto(struct nodo* subconjunto)
{
    char* identificador = (char*)malloc(sizeof(char) * numeroGrande);
    int* hashIdentificador = (int*)malloc(sizeof(int)*numeroGrande);
    int i = 0;
    for(i=0;i<numeroGrande;i++)
    {
        hashIdentificador[i]=0;
    }
    struct nodo* aux = NULL;
    aux = subconjunto;
    while (aux != NULL)
    {
        hashIdentificador[atoi(aux->identificador)]=1;
        aux = aux->siguienteNodo;
    }
    for(i=0;i<numeroGrande;i++)
    {
        if(hashIdentificador[i]==1)
        {
            sprintf(identificador,"%s,%d",identificador,i);
        }
    }
    return identificador;
}
struct nodo* NFA_a_DFA(struct nodo* listadoNodos)
{
    char* identificador = (char*)malloc(sizeof(char) * numeroGrande);
    char* expresion = (char*)malloc(sizeof(char) * numeroGrande);
    long long i=0;
    struct nodo* nodoAux = NULL;
    struct estado* listadoEstados = NULL;
    struct estado* estado = NULL;
    struct nodo* DFA = NULL;
    struct nodo** subconjunto = (struct nodo**)malloc(sizeof(struct nodo*) * 10000);
    struct nodo* iteracionDFA = NULL;
    struct vertice* auxVertice = NULL;
    for(i=0;i<10000;i++)
    {
        subconjunto[i]=NULL;
    }

    listadoNodos = renombrarNodos(listadoNodos);
    listadoEstados = agregarEstados(listadoEstados, listadoNodos);

    nodoAux = agregarNodo(nodoAux, "0");
    nodoAux->listaVertices = listadoNodos->primerElemento->listaVertices;

    subconjunto[0] = epsilonCerrar(listadoNodos, nodoAux);
    identificador = identificadorSubconjunto(subconjunto[0]);
    DFA = agregarNodo(DFA,identificador);
    DFA->identificador = identificadorSubconjunto(subconjunto[0]);
    iteracionDFA = DFA->primerElemento;
    i=0;
    while (iteracionDFA!=NULL)
    {
        estado = listadoEstados;
        while (estado != NULL)
        {
            strcpy(expresion, estado->expresion);
            
            nodoAux = mover(subconjunto[i], expresion);
            nodoAux = epsilonCerrar(listadoNodos, nodoAux);
            
            identificador = identificadorSubconjunto(nodoAux);
            DFA = agregarSubconjunto(DFA,subconjunto,iteracionDFA,nodoAux,identificador,expresion);
            
            estado = estado->siguienteEstado;
        }
        iteracionDFA=iteracionDFA->anteriorNodo;
        i++;
    }
    DFA=renombrarNodos(DFA);
    nodoAux=DFA->primerElemento;
    while(nodoAux!=NULL)
    {
        auxVertice = nodoAux->listaVertices;
        while(auxVertice!=NULL)
        {
            printf("%s->%s->%s\n",nodoAux->identificador,auxVertice->expresion,auxVertice->destino->identificador);
            auxVertice=auxVertice->siguienteVertice;
        }
        nodoAux=nodoAux->anteriorNodo;
    }
    return DFA;
}

char*** reducirNFA(char*** diagramaEstado, long long  tamanoListadoNodos, long long  tamanoAux)
{
    char*** diagramaAuxiliar;

    char* expresion = (char*)malloc(sizeof(char) * numeroGrande);
    char* expresionOrigen = (char*)malloc(sizeof(char) * numeroGrande);
    char* expresionCiclo = (char*)malloc(sizeof(char) * numeroGrande);
    char* expresionDestino = (char*)malloc(sizeof(char) * numeroGrande);

    int i=0;
    int j=0;
    int iPostNodo = 0;
    int jPostNodo = 0;
    int nodoEliminado = tamanoAux-1;
    int fila=0;
    int columna=0;
    diagramaAuxiliar = (char***)malloc(sizeof(char**) * tamanoAux);
    for (i = 0; i + iPostNodo < tamanoListadoNodos; i++)
    {
        jPostNodo=0;
        if(i>=nodoEliminado)
        {
            iPostNodo = 1;
        }
        fila=i+iPostNodo;
        diagramaAuxiliar[i] = (char**)malloc(sizeof(char*) * tamanoAux);
        for (j = 0; j + jPostNodo < tamanoListadoNodos; j++)
        {
            diagramaAuxiliar[i][j] = (char*)malloc(sizeof(char) * numeroGrande);
            if(j>=nodoEliminado)
            {
                jPostNodo = 1;
            }
            columna=j+jPostNodo;
            strcpy(diagramaAuxiliar[i][j], diagramaEstado[fila][columna]);
        }
    }

    if(diagramaEstado[nodoEliminado][nodoEliminado][0]!='\0')
    {
        strcpy(expresion, diagramaEstado[nodoEliminado][nodoEliminado]);
        sprintf(expresionCiclo,"(%s)*",expresion);
    }
    else
    {
        sprintf(expresionCiclo,"%c",'\0');
    }

    i=0;
    j=0;
    iPostNodo=0;
    jPostNodo=0;
    fila=0;
    columna=0;
    for (j = 0; j + jPostNodo < tamanoListadoNodos; j++)
    {
        iPostNodo=0;
        if(j>=nodoEliminado)
        {
            jPostNodo=1;
        }
        columna=j+jPostNodo;
        if (diagramaEstado[nodoEliminado][columna][0]!='\0')
        {
            if(strcmp(diagramaEstado[nodoEliminado][columna],"ε")==0)
            {
                sprintf(expresionDestino,"%c",'\0');    
            }
            else
            {
                sprintf(expresionDestino,"%s",diagramaEstado[nodoEliminado][columna]);
            }
            for (i = 0; i + iPostNodo< tamanoListadoNodos; i++)
            {
                if(i>=nodoEliminado)
                {
                    iPostNodo=1;
                }
                fila=i+iPostNodo;
                if (diagramaEstado[fila][nodoEliminado][0]!='\0')
                {
                    if(strcmp(diagramaEstado[fila][nodoEliminado],"ε")==0)
                    {
                        sprintf(expresionOrigen,"%c",'\0');
                    }
                    else
                    {
                        strcpy(expresionOrigen,diagramaEstado[fila][nodoEliminado]);
                    }
                    
                    if (diagramaEstado[fila][columna][0]!='\0')
                    {
                        sprintf(expresion,"(%s)|(%s%s%s)",diagramaEstado[fila][columna],expresionOrigen,expresionCiclo,expresionDestino);
                    }
                    else
                    {
                        sprintf(expresion,"%s%s%s",expresionOrigen,expresionCiclo,expresionDestino);
                    }
                    strcpy(diagramaAuxiliar[i][j],expresion);
                    sprintf(expresionOrigen,"%c",'\0');
                    sprintf(expresionCiclo,"%c",'\0');
                    sprintf(expresionDestino,"%c",'\0');
                }
            }
        }
    }
    return diagramaAuxiliar;
}
void imprimirDiagramaEstado(char***diagramaEstado,int tamano)
{
    int i=0;
    int j=0;
    printf("tabla %d x %d\n",tamano,tamano);
    for(i=0;i<tamano;i++)
    {
        for(j=0;j<tamano;j++)
        {
            printf("%s,\t\t\t ",diagramaEstado[i][j]);
        }
        printf("\n");
    }
}
char* corregirParentesis(char* regex)
{
    int i=0;
    int posicionAperturaParentesis=0;
    int posicionCerraduraParentesis=0;
    int tamanoRegex = strlen(regex);
    char* primeraMitad =(char*)malloc(sizeof(char) * numeroGrande);
    char* segundaMitad =(char*)malloc(sizeof(char) * numeroGrande);
    for(i=0;i<tamanoRegex;i++)
    {
        if(regex[i]=='*')
        {
            continue;
        }
        else if(regex[i]=='|')
        {
            continue;
        }
        else if(regex[i]=='(')
        {
            posicionAperturaParentesis=i;
            posicionCerraduraParentesis=posicionAperturaParentesis + visitarParentesis(&regex[posicionAperturaParentesis]);
            i=posicionCerraduraParentesis;
        }
        else
        {
            strncpy(primeraMitad,regex,i);
            sprintf(segundaMitad,"%c",'(');
            while( i<tamanoRegex )
            {
                if(regex[i]!='(')
                {
                    strncat(segundaMitad,&regex[i],1);
                }else
                {
                    sprintf(segundaMitad,"%s%c",segundaMitad,')');
                    sprintf(regex,"%s%s",primeraMitad,segundaMitad);
                    sprintf(primeraMitad,"%c",'\0');
                    sprintf(segundaMitad,"%c",'\0');
                    i=i+1;
                    break;
                }
                i++;
                if(i==tamanoRegex)
                {
                    sprintf(segundaMitad,"%s%c",segundaMitad,')');
                    sprintf(regex,"%s%s",primeraMitad,segundaMitad);
                    sprintf(primeraMitad,"%c",'\0');
                    sprintf(segundaMitad,"%c",'\0');
                    return regex;
                }
            }
        }
    }
    return regex;
}
char* NFA_a_Regex(struct nodo* listadoNodos)
{
    long long  tamanoListadoNodos = atoi(listadoNodos->identificador);
    long long  tamanoAux = 0;
    char*** diagramaEstado = NULL;
    char* regex = (char*)malloc(sizeof(char) * numeroGrande);
    if (tamanoListadoNodos < 1)
    {
        printf("El NFA cuenta con menos de dos nodos.");
        return NULL;
    }

    listadoNodos = agregarEstadosExtremos(listadoNodos);
    tamanoListadoNodos = atoi(listadoNodos->identificador)+1;
    tamanoAux = tamanoListadoNodos-1;
    diagramaEstado = crearDiagramaEstado(listadoNodos);
    //imprimirDiagramaEstado(diagramaEstado,tamanoListadoNodos);
    while (tamanoAux >= 2)
    {
        diagramaEstado = reducirNFA(diagramaEstado, tamanoListadoNodos, tamanoAux);
        //imprimirDiagramaEstado(diagramaEstado,tamanoAux);
        tamanoAux--;
        tamanoListadoNodos--;
    }
    strcpy(regex, diagramaEstado[0][1]);
    regex = corregirParentesis(regex);
    return regex;
}
long long  visitarParentesis(char* regex)
{
    long long  posicionParentesis = -1;
    long long  cantidadAparturaParentesis = 1;
    long long  cantidadCerraduraParentesis = 0;
    long long  i;
    long long  tamanoRegex = strlen(regex);
    char aperturaParentesis = '(';
    char cerraduraParentesis = ')';
    char simboloActual = '(';
    for (i = 1; i < tamanoRegex; i++)
    {
        simboloActual = regex[i];
        if (simboloActual == aperturaParentesis)
        {
            cantidadAparturaParentesis++;
        }
        if (simboloActual == cerraduraParentesis)
        {
            cantidadCerraduraParentesis++;
        }
        if (cantidadCerraduraParentesis - cantidadAparturaParentesis == 0)
        {
            posicionParentesis = i;
            i = tamanoRegex;
        }
    }
    return posicionParentesis;
}
struct nodo* NFA_Thompson_Concatenacion(struct nodo* thompson, char* regex)
{
    struct nodo* aperturaNodoA = NULL;
    struct nodo* aperturaNodoB = NULL;
    struct nodo* cerraduraNodoA = NULL;
    struct nodo* cerraduraNodoB = NULL;
    struct nodo* aperturaConcatenacion = NULL;
    struct nodo* cerraduraConcatenacion = NULL;
    struct nodo* primerNodo = NULL;
    char* identificador = (char*)malloc(sizeof(char) * numeroGrande);
    char* regexParentesis = (char*)malloc(sizeof(char) * numeroGrande);
    char* caracterVacio = (char*)malloc(sizeof(char)*3);
    strcpy(caracterVacio,"ε");
    sprintf(identificador, "%d", 0);
    long long  aperturaParentesisA = 0;
    long long  cerraduraParentesisA = visitarParentesis(&regex[aperturaParentesisA]);
    long long  aperturaParentesisB = cerraduraParentesisA + 2;
    long long  cerraduraParentesisB = visitarParentesis(&regex[aperturaParentesisB]);
    if (cerraduraParentesisA < 2 || aperturaParentesisB + cerraduraParentesisB < 6)
    {
        printf("Caracter | invalido");
        return NULL;
    }

    strncpy(regexParentesis, &regex[aperturaParentesisA], cerraduraParentesisA + 1);
    cerraduraNodoA = Regex_a_NFA_Thompson(cerraduraNodoA, regexParentesis);
    aperturaNodoA = cerraduraNodoA->primerElemento;

    strncpy(regexParentesis, &regex[aperturaParentesisB], cerraduraParentesisB + 1);
    cerraduraNodoB = Regex_a_NFA_Thompson(cerraduraNodoB, regexParentesis);
    aperturaNodoB = cerraduraNodoB->primerElemento;

    cerraduraNodoA->anteriorNodo = aperturaNodoB;
    aperturaNodoB->siguienteNodo = cerraduraNodoA;

    aperturaConcatenacion = crearNodo(identificador);
    aperturaConcatenacion->anteriorNodo = aperturaNodoA;
    aperturaNodoA->siguienteNodo = aperturaConcatenacion;

    cerraduraConcatenacion = crearNodo(identificador);
    cerraduraConcatenacion->siguienteNodo = cerraduraNodoB;
    cerraduraNodoB->anteriorNodo = cerraduraConcatenacion;

    agregarVertice(aperturaConcatenacion, aperturaNodoA, caracterVacio);
    agregarVertice(aperturaConcatenacion, aperturaNodoB, caracterVacio);
    agregarVertice(cerraduraNodoA, cerraduraConcatenacion, caracterVacio);
    agregarVertice(cerraduraNodoB, cerraduraConcatenacion, caracterVacio);

    if (thompson == NULL)
    {
        thompson = cerraduraConcatenacion;
        primerNodo = aperturaConcatenacion;
    }
    else
    {
        thompson->anteriorNodo = aperturaConcatenacion;
        aperturaConcatenacion->siguienteNodo = thompson;
        thompson = cerraduraConcatenacion;
        primerNodo = thompson->primerElemento;
    }

    thompson->primerElemento = primerNodo;

    return thompson;
}
struct nodo* NFA_Thompson_Estrella(struct nodo* thompson, char* regex)
{
    struct nodo* aperturaNodo = NULL;
    struct nodo* cerraduraNodo = NULL;
    struct nodo* aperturaEstrella = NULL;
    struct nodo* cerraduraEstrella = NULL;
    char* identificador = (char*)malloc(sizeof(char) * numeroGrande);
    sprintf(identificador, "%d", 0);
    char* regexParentesis = (char*)malloc(sizeof(char) * numeroGrande);
    char* caracterVacio = (char*)malloc(sizeof(char)*3);
    strcpy(caracterVacio,"ε");
    long long  aperturaParentesis = 0;
    long long  cerraduraParentesis = visitarParentesis(&regex[aperturaParentesis]);
    if (cerraduraParentesis < 1)
    {
        printf("Caracter * invalido");
        return NULL;
    }

    strncpy(regexParentesis, &regex[aperturaParentesis], cerraduraParentesis + 1);

    cerraduraNodo = Regex_a_NFA_Thompson(cerraduraNodo, regexParentesis);
    aperturaNodo = cerraduraNodo->primerElemento;

    aperturaEstrella = crearNodo(identificador);
    aperturaEstrella->anteriorNodo = aperturaNodo;
    aperturaNodo->siguienteNodo = aperturaEstrella;

    cerraduraEstrella = crearNodo(identificador);
    cerraduraEstrella->siguienteNodo = cerraduraNodo;
    cerraduraNodo->anteriorNodo = cerraduraEstrella;


    agregarVertice(aperturaEstrella, cerraduraEstrella, caracterVacio);
    agregarVertice(aperturaEstrella, aperturaNodo, caracterVacio);
    agregarVertice(cerraduraNodo, aperturaNodo, caracterVacio);
    agregarVertice(cerraduraNodo, cerraduraEstrella, caracterVacio);

    if (thompson == NULL)
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

        thompson->anteriorNodo = aperturaEstrella;
        aperturaEstrella->siguienteNodo = thompson;
        thompson = cerraduraEstrella;
    }

    return thompson;
}
struct nodo* NFA_Thompson_Parentesis(struct nodo* thompson, char* regex)
{
    long long  i;
    long long  aperturaParentesis = 0;
    long long  cerraduraParentesis = 0;
    long long  tamanoParentesisRegex = visitarParentesis(&regex[aperturaParentesis]);
    char* expresion = (char*)malloc(sizeof(char) * numeroGrande);
    char* identificador = (char*)malloc(sizeof(char) * numeroGrande);
    char simboloParentesis = '(';
    char simboloActual = '(';
    char simboloConcatenacion = '|';
    char simboloEstrella = '*';
    struct nodo* aperturaNodo = NULL;
    struct nodo* cerraduraSubParentesis = NULL;
    struct nodo* cerraduraNodo = NULL;
    struct nodo* primerNodo = NULL;
    sprintf(identificador, "%d", 0);

    if(thompson!=NULL)
    {
        primerNodo=thompson->primerElemento;
    }

    for (i = 1; i < tamanoParentesisRegex; i++)
    {
        simboloActual = regex[i];
        if (simboloActual == simboloParentesis)
        {
            aperturaParentesis = i;
            cerraduraParentesis = aperturaParentesis + visitarParentesis(&regex[aperturaParentesis]);
            if (cerraduraParentesis < tamanoParentesisRegex - 1)
            {
                simboloActual = regex[cerraduraParentesis + 1];
                if (simboloActual == simboloConcatenacion)
                {
                    thompson = NFA_Thompson_Concatenacion(thompson, &regex[i]);
                    return thompson;
                }
                if (simboloActual == simboloEstrella)
                {
                    thompson = NFA_Thompson_Estrella(thompson, &regex[i]);
                    return thompson;
                }
            }
        }
        else
        {
            sprintf(expresion,"%c",regex[i]);
            if (thompson == NULL)
            {
                thompson = agregarNodo(thompson, identificador);
                primerNodo=thompson;
                thompson->primerElemento = primerNodo; 
            }
            aperturaNodo = thompson;
            thompson = agregarNodo(thompson, identificador);
            cerraduraNodo = thompson;
            agregarVertice(aperturaNodo, cerraduraNodo, expresion);
            cerraduraNodo->primerElemento=primerNodo;
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
    long long  posicionAperturaParentesisB = 0;
    long long  posicionCerraduraParentesisB = 0;
    struct nodo* nodoParentesis = NULL;
    char simboloParentesis = '(';
    char simboloConcatenacion = '|';
    char simboloEstrella = '*';
    char simboloActual = '(';
    for (i = 0; i < tamanoRegex; i++)
    {
        simboloActual = regex[i];
        if (simboloActual == simboloParentesis)
        {
            posicionAperturaParentesis = i;
            posicionCerraduraParentesis = posicionAperturaParentesis + visitarParentesis(&regex[i]);
            if (posicionCerraduraParentesis < tamanoRegex - 1)
            {
                simboloActual = regex[posicionCerraduraParentesis + 1];
                if (simboloActual == simboloConcatenacion)
                {
                    posicionAperturaParentesisB = posicionCerraduraParentesis + 2;
                    posicionCerraduraParentesisB = posicionAperturaParentesisB + visitarParentesis(&regex[posicionAperturaParentesisB]);
                    thompson = NFA_Thompson_Concatenacion(thompson, &regex[i]);
                    i = posicionCerraduraParentesisB;
                    continue;
                }
                if (simboloActual == simboloEstrella)
                {
                    thompson = NFA_Thompson_Estrella(thompson, &regex[i]);
                    i = posicionCerraduraParentesis;
                    continue;
                }
                nodoParentesis = NFA_Thompson_Parentesis(thompson, &regex[i]);
                thompson = nodoParentesis;
                i = posicionCerraduraParentesis;
            }
            else
            {
                if(thompson!=NULL)
                {
                    thompson = renombrarNodos(thompson);
                }
                nodoParentesis = NFA_Thompson_Parentesis(thompson, &regex[i]);
                thompson = nodoParentesis;
                i = posicionCerraduraParentesis;
            }
        }
    }
    return thompson;
}

long long  exponente(long long  base, long long  exponente)
{
    if (exponente == 0)
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
int  main()
{
    char* identificador=(char*)malloc(sizeof(char) * numeroGrande);
    char* expresion=(char*)malloc(sizeof(char) * numeroGrande);
    char* regex = (char*)malloc(sizeof(char) * numeroGrande);
    struct nodo* listadoNodos = NULL;
    struct nodo* origen = NULL;
    struct nodo* destino = NULL;
    struct nodo* nodoInicial = NULL;
    struct nodo* nodoFinal = NULL;
    struct vertice* verticeAux = NULL;
    int  i = 0;
    sprintf(identificador,"%d",0);
    
    for (i = 0; i < 12; i++)
    {
        sprintf(identificador,"%d",i);
        listadoNodos = agregarNodo(listadoNodos,identificador);
    }
    nodoInicial = listadoNodos->primerElemento;
    nodoInicial->esInicio=1;
    nodoFinal = listadoNodos;
    nodoFinal->esFin=1;

    origen = nodoInicial; 
    destino = nodoInicial->anteriorNodo;
    while(destino->identificador != nodoFinal->identificador)
    {
        sprintf(expresion,"%s",destino->identificador);
        agregarVertice(origen,destino,expresion);
        destino = destino->anteriorNodo;
    }
    origen = nodoInicial->anteriorNodo;
    while(origen->identificador != nodoFinal->identificador)
    {
        destino = nodoInicial->anteriorNodo;
        while(destino!=NULL)
        {
            if( origen->identificador != destino->identificador )
            {
                sprintf(expresion,"%s",destino->identificador);
                agregarVertice(origen,destino,expresion);
            }
            destino = destino->anteriorNodo;
        }
        origen = origen->anteriorNodo;
    } 
    
    origen=listadoNodos->primerElemento;
    //convertir a regex
    regex = NFA_a_Regex(listadoNodos);
    
    //convertir a NFA con el algoritmo de Thompson
    listadoNodos = NULL;
    
    //regex = corregirRegex(regex);
    sprintf(regex,"%s","((a)|(b))*(abb)");
    listadoNodos = Regex_a_NFA_Thompson(listadoNodos, regex);
    listadoNodos = renombrarNodos(listadoNodos);
    origen=listadoNodos->primerElemento;
    while(origen!=NULL)
    {
        verticeAux = origen->listaVertices;
        while(verticeAux!=NULL)
        {
            printf("%s->%s->%s\n",origen->identificador,verticeAux->expresion,verticeAux->destino->identificador);
            verticeAux=verticeAux->siguienteVertice;
        }
        origen=origen->anteriorNodo;
    }
    //convertir a DFA
    listadoNodos = NFA_a_DFA(listadoNodos);
    origen=listadoNodos->primerElemento;
    while(origen!=NULL)
    {
       verticeAux = origen->listaVertices;
        while(verticeAux!=NULL)
        {
            printf("%s->%s->%s\n",origen->identificador,verticeAux->expresion,verticeAux->destino->identificador);
            verticeAux=verticeAux->siguienteVertice;
        }
        origen=origen->anteriorNodo;
    }    
    //convertir a DFA minimizado
    listadoNodos = DFA_a_DFA_Minimizado(listadoNodos);
    return 0;
}