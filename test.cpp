#include<cstdlib>
#include<iostream>
#include<map>
#include<string>
#include<vector>
class nodo;
class vertice;
void Regex_a_NFA_Thompson(std::vector<nodo*> * thompson, std::string regex);
void imprimirNodo(std::vector<nodo*> * listadoNodos);
class nodo
{
    public:
        int esInicio;
        int esFin;
        std::string esVisitado;
        std::string identificador;
        std::vector<vertice> listaVertices;
        nodo(std::string id)
        {
            esInicio=0;
            esFin=0;
            esVisitado.clear();
            identificador=id;
            listaVertices.clear();
        }
};
class vertice
{
    public:
        nodo * destino = NULL;
        std::string expresion;
        vertice(nodo * des,std::string exp)
        {
            destino=des;
            expresion=exp;
        }
};
int esVacio(std::string expresion)
{
    if(expresion.length()==0)
    {
        return 0;
    }
    std::string caracterVacio = "ε";
    int respuesta = 0;
    if( expresion.compare(caracterVacio)==0 )
    {
        respuesta=1;
    }
    return respuesta;
}
void agregarNodo(std::vector<nodo*>*listadoNodos,std::string identificador)
{
    nodo * nodoNuevo = new nodo(identificador);
    listadoNodos->push_back(nodoNuevo);
}
void agregarVertice(nodo * origen, nodo * destino, std::string expresion)
{
    vertice aux(destino,expresion);
    origen->listaVertices.push_back(aux);
}
void renombrarNodos(std::vector<nodo*> * DFA)
{
    std::vector<nodo*>::iterator aux;
    int i = 0;
    std::string identificador;
    for (aux= DFA->begin();aux<DFA->end();aux++)
    {
        identificador = std::to_string(i);
        aux[0]->identificador = identificador;
        i++;
    }
}
void agregarEstadosExtremos(std::vector<nodo*> * listadoNodos)
{
    std::string caracterVacio = "ε";
    std::vector<nodo*>::iterator aux;
    nodo * extremoInicial=NULL;
    nodo * extremoFinal=NULL;
    listadoNodos->insert(listadoNodos->begin(),new nodo("0"));
    listadoNodos->push_back(new nodo("0"));
    extremoInicial = listadoNodos->at(0);
    extremoFinal = listadoNodos->at(listadoNodos->size()-1);
    extremoInicial->esInicio=1;
    extremoFinal->esFin=1;
    for(aux= listadoNodos->begin()+1;aux<listadoNodos->end()-1;aux++)
    {
        if(aux[0]->esInicio==1)
        {
            agregarVertice(extremoInicial,aux[0],caracterVacio);
            aux[0]->esInicio=0;
        }
        if(aux[0]->esFin==1)
        {
            agregarVertice(aux[0],extremoFinal,caracterVacio);
            aux[0]->esFin=0;
        }
    }
    renombrarNodos(listadoNodos);
}
std::vector<std::vector<std::string>> crearDiagramaEstado(std::vector<nodo*> * listadoNodos)
{
    int tamanoListadoNodos = listadoNodos->size();
    int i;
    int j;
    int identificadorOrigen = 0;
    int identificadorDestino = 0;
    std::vector<std::vector<std::string>> diagramaEstado (tamanoListadoNodos);
    std::vector<nodo*>::iterator origenVertice;
    std::vector<vertice>::iterator iteracionVertice;
    std::string expresion("");
    nodo * destinoVertice;
    for(i=0;i<(int)listadoNodos->size();i++)
    {
        for(j=0;j<(int)listadoNodos->size();j++)
        {
            diagramaEstado[i].push_back("");
            diagramaEstado[i][j][0]='\0';
        }
    }
    for(origenVertice=listadoNodos->begin();origenVertice<listadoNodos->end();origenVertice++)
    {
        identificadorOrigen = atoi(origenVertice[0]->identificador.c_str());
        
        for(iteracionVertice= origenVertice[0]->listaVertices.begin();iteracionVertice<origenVertice[0]->listaVertices.end();iteracionVertice++)
        {
            destinoVertice = iteracionVertice[0].destino;
            identificadorDestino = atoi(destinoVertice->identificador.c_str());
            expresion=iteracionVertice[0].expresion;
            if(diagramaEstado[identificadorOrigen][identificadorDestino][0]!='\0')
            {
                expresion = "("+diagramaEstado[identificadorOrigen][identificadorDestino]+")|("+iteracionVertice[0].expresion+")";
            }
            diagramaEstado[identificadorOrigen][identificadorDestino]=expresion;
        }
    }
    for(i = 0; i < tamanoListadoNodos; i++)
    {
        if(diagramaEstado[i][i].size()>0)
        {
            diagramaEstado[i][i] ="("+diagramaEstado[i][i]+")*";
        }
    }
    return diagramaEstado;
}
void generarListadoVertices(std::map<std::string,int>* listadoVertices,std::vector<std::string>* listaCompleta, std::vector<nodo*>* DFA, std::vector<nodo*>* DFA_minimizado)
{
    listadoVertices->clear();
    listaCompleta->clear();
    std::vector<nodo*>::iterator auxDFA;
    std::vector<vertice>::iterator auxVertice;
    std::string identificador;
    std::pair<std::map<std::string,int>::iterator,bool> hayInsercion;
    int i=0;
    int indiceDFA = 0;
    int posicionLista = 0;
    int posicionIdentificador = 0;
    for( i = 0; i <(int) DFA->size();i++)
    {
        listaCompleta->push_back("");
    }
    i=0;
    for( auxDFA = DFA->begin();auxDFA<DFA->end();auxDFA++)
    {
        identificador.clear();
        for( auxVertice = auxDFA[0]->listaVertices.begin(); auxVertice < auxDFA[0]->listaVertices.end(); auxVertice++ )
        {
            identificador += auxVertice[0].expresion +'\n'+ auxVertice[0].destino->identificador+'\n';
        }
        identificador += '\0';
        hayInsercion = listadoVertices->insert(std::make_pair(identificador,i));
        posicionLista = std::stoi(auxDFA[0]->identificador);
        listaCompleta->at(posicionLista) = identificador;
        if(hayInsercion.second)
        {
            agregarNodo(DFA_minimizado,std::to_string(listadoVertices->size()-1));
            auxDFA[0]->esVisitado = std::to_string(indiceDFA);
            indiceDFA++;
            DFA_minimizado->at( DFA_minimizado->size()-1 )->esInicio = auxDFA[0]->esInicio;
            DFA_minimizado->at( DFA_minimizado->size()-1 )->esFin = auxDFA[0]->esFin;
        }
        else
        {
            posicionIdentificador = std::stoi(DFA->at(listadoVertices->find(listaCompleta->at(posicionLista))->second)->esVisitado);
            DFA_minimizado->at( posicionIdentificador )->esInicio |= auxDFA[0]->esInicio;
            DFA_minimizado->at( posicionIdentificador )->esFin |= auxDFA[0]->esFin;
        }
        i++;
    }
}
void minimizarDFA(std::map<std::string,int> listadoVertices,std::vector<std::string> listaCompleta, std::vector<nodo*>* DFA, std::vector<nodo*>* DFA_minimizado)
{
    int posicionOrigen = 0;
    int posicionDestino = 0;
    int posicionIdentificador = 0;
    nodo * origen = NULL;
    nodo * destino = NULL;
    std::map<std::string,int>::iterator auxListado;
    std::vector<vertice>::iterator auxVertices;
    std::string expresion;
    for(auxListado = listadoVertices.begin();auxListado != listadoVertices.end(); auxListado++)
    {
        posicionOrigen = auxListado->second;
        posicionIdentificador = std::stoi(DFA->at(posicionOrigen)->esVisitado);
        origen = DFA_minimizado->at(posicionIdentificador);
        for(auxVertices=DFA->at(posicionOrigen)->listaVertices.begin();auxVertices!=DFA->at(posicionOrigen)->listaVertices.end();auxVertices++)
        {
            posicionIdentificador = std::stoi(auxVertices[0].destino->identificador);
            posicionDestino = std::stoi(DFA->at(listadoVertices[listaCompleta[posicionIdentificador]])->esVisitado);
            destino = DFA_minimizado->at(posicionDestino);
            expresion = auxVertices[0].expresion;
            agregarVertice(origen,destino,expresion);
        }
    }
}
void mezclarSubconjuntosParecidos(std::vector<nodo*> * DFA)
{
    std::map<std::string,int>listaVertices;
    std::vector<std::string>listaCompleta(DFA->size());
    std::vector<nodo*> nuevoDFA;
    std::vector<nodo*> * DFA_minimizado = &nuevoDFA;
    generarListadoVertices(&listaVertices,&listaCompleta,DFA,DFA_minimizado);
    while( listaVertices.size() != listaCompleta.size() )
    {
        minimizarDFA(listaVertices,listaCompleta, DFA,DFA_minimizado);
        DFA->clear();
        DFA->insert(DFA->begin(),DFA_minimizado->begin(),DFA_minimizado->end());
        DFA_minimizado->clear();
        generarListadoVertices(&listaVertices,&listaCompleta,DFA,DFA_minimizado);
    }
}
void DFA_a_DFA_Minimizado(std::vector<nodo*> * DFA)
{
    renombrarNodos(DFA);
    mezclarSubconjuntosParecidos(DFA);
    renombrarNodos(DFA);
}
bool  esDiferente(std::vector<std::string> listadoEstados, std::string expresion)
{
    std::vector<std::string>::iterator aux;
    bool  sonDiferentes;
    bool  respuesta = true;
    for(aux=listadoEstados.begin();aux<listadoEstados.end();aux++)
    {
        sonDiferentes = aux[0].compare(expresion);
        if (!sonDiferentes)
        {
            respuesta = false;
            aux = listadoEstados.end();
        }
    }
    return respuesta;
}
std::vector<std::string> agregarEstados(std::vector<std::string> listadoEstados, std::vector<nodo*> * listadoNodos)
{
    std::vector<vertice>::iterator iteracionVertice;
    std::vector<nodo*>::iterator nodoAux;
    std::string expresion;
    for(nodoAux=listadoNodos->begin();nodoAux<listadoNodos->end();nodoAux++)
    {
        for(iteracionVertice=nodoAux[0]->listaVertices.begin();iteracionVertice<nodoAux[0]->listaVertices.end()&&!esVacio(iteracionVertice[0].expresion);iteracionVertice++)
        {
            expresion = iteracionVertice[0].expresion;
            if (esDiferente(listadoEstados, expresion))
            {
                listadoEstados.push_back(expresion);
            }
        }
    }
    return listadoEstados;
}
void reiniciarVisibilidad(std::vector<nodo*> * listadoNodos)
{
    std::vector<nodo*>::iterator aux;
    for(aux=listadoNodos->begin();aux<listadoNodos->end();aux++)
    {
        aux[0]->esVisitado.clear();
    }
}
void mover(std::vector<nodo*>* nuevoEstado, std::vector<nodo*> * listadoNodos, std::string expresion)
{
    reiniciarVisibilidad(listadoNodos);
    nuevoEstado->clear();
    std::vector<nodo*>::iterator aux;
    std::vector<vertice>::iterator iteracionVertice;
    std::string identificador;
    for(aux=listadoNodos->begin();aux!=listadoNodos->end();aux++)
    {
        aux[0]->esVisitado="1";
        for(iteracionVertice=aux[0]->listaVertices.begin();iteracionVertice<aux[0]->listaVertices.end();iteracionVertice++)
        {
            if (iteracionVertice[0].expresion.compare(expresion)==0 && iteracionVertice[0].destino->esVisitado.size()==0)
            {
                iteracionVertice[0].destino->esVisitado = "1";
                identificador = iteracionVertice[0].destino->identificador;
                agregarNodo(nuevoEstado,identificador);
                nuevoEstado->at(nuevoEstado->size()-1)->listaVertices=iteracionVertice[0].destino->listaVertices;
            }
        }
    }
}

void epsilonCerrar(std::vector<nodo*> * subconjunto, std::vector<nodo*> * estadosIniciales)
{
    std::string identificador;
    int esFin = 0;
    int i;
    std::vector<nodo*> visitados;
    std::vector<nodo*> * nodosVisitados = &visitados;
    std::vector<nodo*>::iterator auxIniciales;
    std::vector<vertice>::iterator iteracionVertice;
    subconjunto->clear();
    for(auxIniciales=estadosIniciales->begin();auxIniciales<estadosIniciales->end();auxIniciales++)
    {
        if(auxIniciales[0]->esFin==1)
        {
            esFin=1;
        }

        identificador = auxIniciales[0]->identificador;

        agregarNodo(subconjunto,identificador);
        subconjunto->at(subconjunto->size()-1)->listaVertices = auxIniciales[0]->listaVertices;
        agregarNodo(nodosVisitados,identificador);
        nodosVisitados->at(nodosVisitados->size()-1)->listaVertices = auxIniciales[0]->listaVertices;
    }

    for(i=0;i<(int)nodosVisitados->size();i++)
    {
        nodosVisitados->at(i)->esVisitado = "1";
        for(iteracionVertice=nodosVisitados->at(i)->listaVertices.begin();iteracionVertice<nodosVisitados->at(i)->listaVertices.end();iteracionVertice++)
        {

            if (esVacio(iteracionVertice[0].expresion) && iteracionVertice[0].destino->esVisitado.size() == 0)
            {
                if(iteracionVertice[0].destino->esFin==1)
                {
                    esFin=1;
                }
                identificador = iteracionVertice[0].destino->identificador;
                
                agregarNodo(subconjunto,identificador);
                subconjunto->at(subconjunto->size()-1)->listaVertices = iteracionVertice[0].destino->listaVertices;
                
                agregarNodo(nodosVisitados,identificador);
                nodosVisitados->at(nodosVisitados->size()-1)->listaVertices = iteracionVertice[0].destino->listaVertices;
                
                iteracionVertice[0].destino->esVisitado="1";
            }
        }
    }

    if(esFin==1)
    {
        subconjunto->at(subconjunto->size()-1)->esFin=1;
    }
}
void copiarSubconjunto(std::vector<std::vector<nodo*>*>*subconjunto,std::vector<nodo*>*nuevoSubconjunto)
{
    std::vector<nodo*>*alojarSubconjunto=new std::vector<nodo*>();
    std::vector<nodo*>::iterator auxSubconjuto;
    subconjunto->push_back(alojarSubconjunto);
    for(auxSubconjuto=nuevoSubconjunto->begin();auxSubconjuto!=nuevoSubconjunto->end();auxSubconjuto++)
    {
        subconjunto->at(subconjunto->size()-1)->push_back(auxSubconjuto[0]);
    }
}
void agregarSubconjunto(std::vector<nodo*> * DFA,std::vector<std::vector<nodo*>*> * subconjunto, nodo * iteracionDFA, std::vector<nodo*> * subconjuntoActual, std::string identificadorResultante, std::string expresion)
{
    std::string identificadorIteracion;
    int i;
    nodo * origen = NULL;
    nodo * destino = NULL;
    for(i=0;i<(int)DFA->size();i++)
    {
        identificadorIteracion = DFA->at(i)->identificador;
        if ( identificadorIteracion.compare(identificadorResultante)==0)
        {
            origen = iteracionDFA;
            destino = DFA->at(i);
            agregarVertice(origen, destino, expresion);
            break;
        }
        else
        {
            if (i==(int)DFA->size()-1)
            {
                agregarNodo(DFA,identificadorResultante);
                
                if(subconjuntoActual->at(subconjuntoActual->size()-1)->esFin==1)
                {
                    DFA->at(DFA->size()-1)->esFin=1;
                }

                origen = iteracionDFA;
                destino = DFA->at(DFA->size()-1);
                
                agregarVertice(origen, destino, expresion);
                copiarSubconjunto(subconjunto,subconjuntoActual);
                break;
            }
        }
    }
}
std::string identificadorSubconjunto(std::vector<nodo*> * subconjunto)
{
    std::string identificador;
    std::string primeraParte;
    std::string segundaParte;
    int i = 0;
    int numeroActual=0;
    int numeroIdentificador;
    int comaEncontrado=0;
    int posicionComa=0;
    std::vector<nodo*>::iterator aux;

    for(aux=subconjunto->begin();aux<subconjunto->end();aux++)
    {
        if(identificador.size()==0)
        {
            identificador=","+aux[0]->identificador+",";
        }
        else
        {
            primeraParte.clear();
            segundaParte.clear();
            i=0;
            while(identificador[i]!='\0')
            {
                if(identificador[i]==',')
                {
                    if( comaEncontrado==0 )
                    {
                        comaEncontrado=1;
                        posicionComa=i;
                    }
                    else
                    {
                        numeroIdentificador = std::stoi(aux[0]->identificador);
                        if( numeroIdentificador < numeroActual )
                        {
                            primeraParte = identificador.substr(0,posicionComa+1);
                            segundaParte = identificador.substr(posicionComa+1);
                            identificador=primeraParte+aux[0]->identificador+","+segundaParte;
                            comaEncontrado=0;
                            numeroActual=0;
                            break;
                        }
                        else
                        {
                            posicionComa=i;
                            numeroActual=0;
                        }
                    }
                }
                else
                {
                    numeroActual = numeroActual * 10 + (identificador[i] -'0');
                }
                i++;
                if(identificador[i]=='\0')
                {
                    identificador=identificador+aux[0]->identificador+",";
                    comaEncontrado=0;
                    numeroActual=0;
                    break;
                }
            }
        }
    }

    return identificador;
}
void NFA_a_DFA(std::vector<nodo*> * listadoNodos)
{
    std::string identificador = "0";
    std::string expresion;
    int i=0;
    std::vector<nodo*> nodos;
    std::vector<nodo*> aux;
    std::vector<nodo*> subconjuntoTemp;
    std::vector<nodo*> * moverAux = &aux;
    std::vector<nodo*> * nuevoSubconjunto = &subconjuntoTemp;
    std::vector<nodo*> * DFA = &nodos;
    std::vector<std::string> listadoEstados;
    std::vector<std::string>::iterator estado;
    std::vector<std::vector<nodo*>*> subconjunto;

    renombrarNodos(listadoNodos);
    listadoEstados = agregarEstados(listadoEstados, listadoNodos);

    agregarNodo(moverAux,"0");
    moverAux->at(0)->listaVertices = listadoNodos->at(0)->listaVertices;

    reiniciarVisibilidad(listadoNodos);
    epsilonCerrar(nuevoSubconjunto,moverAux);
    copiarSubconjunto(&subconjunto,nuevoSubconjunto);
    identificador = identificadorSubconjunto(subconjunto.at(0));

    agregarNodo(DFA,identificador);
    DFA->at(0)->identificador = identificador;
    DFA->at(0)->esInicio=1;
    
    for(i=0;i<(int)DFA->size();i++)
    {
        for(estado=listadoEstados.begin();estado<listadoEstados.end();estado++)
        {
            expresion = estado[0];
            reiniciarVisibilidad(listadoNodos);
            moverAux->clear();
            mover(moverAux, subconjunto.at(i), expresion);
            if(moverAux->size()>0)
            {
                reiniciarVisibilidad(listadoNodos);
                epsilonCerrar(nuevoSubconjunto, moverAux);
                identificador = identificadorSubconjunto(nuevoSubconjunto);
                agregarSubconjunto(DFA,&subconjunto,DFA->at(i),nuevoSubconjunto,identificador,expresion);
            }
        }
    }
    imprimirNodo(DFA);
    renombrarNodos(DFA);
    listadoNodos->clear();
    listadoNodos->insert(listadoNodos->begin(),DFA->begin(),DFA->end());
}

std::vector<std::vector<std::string>>reducirNFA(std::vector<std::vector<std::string>>diagramaEstado, int tamanoListadoNodos, int tamanoAux)
{
    std::vector<std::vector<std::string>>diagramaAuxiliar (tamanoAux);
    std::string expresion("");
    std::string expresionOrigen("");
    std::string expresionCiclo("");
    std::string expresionDestino("");

    int i=0;
    int j=0;
    int iPostNodo = 0;
    int jPostNodo = 0;
    int nodoEliminado = tamanoAux-1;
    int fila=0;
    int columna=0;

    for (i = 0; i + iPostNodo < tamanoListadoNodos; i++)
    {
        jPostNodo=0;
        if(i>=nodoEliminado)
        {
            iPostNodo = 1;
        }
        fila=i+iPostNodo;
        for (j = 0; j + jPostNodo < tamanoListadoNodos; j++)
        {
            diagramaAuxiliar[i].push_back("");
            diagramaAuxiliar[i][j][0]='\0';
            if(j>=nodoEliminado)
            {
                jPostNodo = 1;
            }
            columna=j+jPostNodo;
            diagramaAuxiliar[i][j]= diagramaEstado[fila][columna];
        }
    }

    if(diagramaEstado[nodoEliminado][nodoEliminado][0] != '\0')
    {
        expresion = diagramaEstado[nodoEliminado][nodoEliminado];
        if(diagramaEstado[nodoEliminado][nodoEliminado].compare("ε")!=0)
        {
            expresionCiclo = "("+expresion+")*";
        }
        else{
            expresionCiclo = expresion;
        }
    }
    else
    {
        expresionCiclo = "ε";
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
            expresionDestino=diagramaEstado[nodoEliminado][columna];
            for (i = 0; i + iPostNodo< tamanoListadoNodos; i++)
            {
                if(i>=nodoEliminado)
                {
                    iPostNodo=1;
                }
                fila=i+iPostNodo;
                if (diagramaEstado[fila][nodoEliminado][0]!='\0')
                {
                    expresionOrigen=diagramaEstado[fila][nodoEliminado];
                    expresion = expresionOrigen;
                    if( expresionCiclo.compare("ε")!= 0 )
                    {
                        if( expresion.compare("ε")!=0 )
                        {
                            expresion += expresionCiclo;
                        }
                        else
                        {
                            expresion = expresionCiclo;
                        }
                    }
                    if( expresionDestino.compare("ε")!=0 )
                    {
                        if( expresion.compare("ε")!=0 )
                        {
                            expresion += expresionDestino;
                        }
                        else
                        {
                            expresion = expresionDestino;
                        }
                    }
                    if (diagramaEstado[fila][columna][0]!='\0')
                    {
                        
                        expresion="(("+diagramaEstado[fila][columna]+")|("+expresion+"))";
                    }
                    diagramaAuxiliar[i][j]=expresion;
                }
            }
        }
    }
    return diagramaAuxiliar;
}
void imprimirDiagramaEstado(std::vector<std::vector<std::string>> diagramaEstado,int tamano)
{
    int i=0;
    int j=0;
    for(i=0;i<tamano;i++)
    {
        for(j=0;j<tamano;j++)
        {
            std::cout<<"diagramaEstado["<<i<<"]["<<j<<"]:"<<diagramaEstado[i][j]<<"\n";
        }
        printf("\n");
    }
    printf("tabla %d x %d:\n",tamano,tamano);
}
std::string NFA_a_Regex(std::vector<nodo*> * listadoNodos)
{
    int tamanoListadoNodos = listadoNodos->size();
    int tamanoAux = 0;
    std::vector<std::vector<std::string>> diagramaEstado(tamanoListadoNodos);
    std::string regex;
    if (tamanoListadoNodos < 1)
    {
        printf("El NFA cuenta con menos de dos nodos.");
        return NULL;
    }

    agregarEstadosExtremos(listadoNodos);
    tamanoListadoNodos = atoi(listadoNodos->at(listadoNodos->size()-1)->identificador.c_str())+1;
    tamanoAux = tamanoListadoNodos-1;
    diagramaEstado = crearDiagramaEstado(listadoNodos);
    imprimirDiagramaEstado(diagramaEstado,tamanoListadoNodos);
    while (tamanoAux >= 2)
    {
        diagramaEstado = reducirNFA(diagramaEstado, tamanoListadoNodos, tamanoAux);
        imprimirDiagramaEstado(diagramaEstado,tamanoAux);
        tamanoAux--;
        tamanoListadoNodos--;
    }
    regex = diagramaEstado[0][1];
    return regex;
}
int visitarParentesis(std::string regex)
{
    int posicionParentesis = -1;
    int cantidadAparturaParentesis = 1;
    int cantidadCerraduraParentesis = 0;
    int i;
    int tamanoRegex = regex.size();
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
void NFA_Thompson_Concatenacion(std::vector<nodo*> * thompson, std::string regex)
{
    std::string identificador="0";
    std::string regexParentesis;
    std::string caracterVacio="ε";
    int aperturaParentesisA = 0;
    int cerraduraParentesisA = visitarParentesis(&regex[aperturaParentesisA]);
    int aperturaParentesisB = cerraduraParentesisA + 2;
    int cerraduraParentesisB = visitarParentesis(&regex[aperturaParentesisB]);
    std::vector<nodo*> a;
    std::vector<nodo*> b;
    std::vector<nodo*> * nodoA=&a;
    std::vector<nodo*> * nodoB=&b;
    std::vector<nodo*> nodos;
    std::vector<nodo*> * concatenacion = &nodos;
    nodo * aperturaNodoA;
    nodo * aperturaNodoB;
    nodo * cerraduraNodoA;
    nodo * cerraduraNodoB;
    nodo * aperturaConcatenacion;
    nodo * cerraduraConcatenacion;
    if (cerraduraParentesisA < 2 || aperturaParentesisB + cerraduraParentesisB < 6)
    {
        printf("Caracter | invalido");
        exit(EXIT_FAILURE);
    }
    regexParentesis = regex.substr(aperturaParentesisA,cerraduraParentesisA+1);
    
    Regex_a_NFA_Thompson(nodoA, regexParentesis);
    concatenacion->insert(concatenacion->begin(),nodoA->begin(),nodoA->end());

    regexParentesis.clear();
    regexParentesis = regex.substr(aperturaParentesisB,cerraduraParentesisB+1);
    
    Regex_a_NFA_Thompson(nodoB, regexParentesis);
    concatenacion->insert(concatenacion->end(),nodoB->begin(),nodoB->end());


    concatenacion->push_back(new nodo("0"));
    
    if(thompson->size()==0)
    {
        concatenacion->insert(concatenacion->begin(),new nodo("0"));
        aperturaConcatenacion = concatenacion->at(0);
    }
    
    aperturaNodoA = concatenacion->at(1);
    cerraduraNodoA = concatenacion->at(nodoA->size());
    
    aperturaNodoB = concatenacion->at(nodoA->size()+1);
    cerraduraNodoB = concatenacion->at(concatenacion->size()-2);
    
    cerraduraConcatenacion = concatenacion->at(concatenacion->size()-1);

    if(thompson->size()!=0)
    {
        aperturaConcatenacion = thompson->at(thompson->size()-1);
    }

    agregarVertice(aperturaConcatenacion, aperturaNodoA, caracterVacio);
    agregarVertice(aperturaConcatenacion, aperturaNodoB, caracterVacio);
    agregarVertice(cerraduraNodoA, cerraduraConcatenacion, caracterVacio);
    agregarVertice(cerraduraNodoB, cerraduraConcatenacion, caracterVacio);

    thompson->insert(thompson->end(),concatenacion->begin(),concatenacion->end());
    renombrarNodos(thompson);
}
void NFA_Thompson_Estrella(std::vector<nodo*> * thompson, std::string regex)
{
    nodo * aperturaNodo;
    nodo * cerraduraNodo;
    nodo * aperturaEstrella;
    nodo * cerraduraEstrella;
    std::vector<nodo*> nodos;
    std::vector<nodo*> * estrella = &nodos;
    std::string identificador="0";
    std::string regexParentesis;
    std::string caracterVacio="ε";
    int aperturaParentesis = 0;
    int cerraduraParentesis = visitarParentesis(&regex[aperturaParentesis]);
    if (cerraduraParentesis < 1)
    {
        printf("Caracter * invalido");
        exit(EXIT_FAILURE);
    }
    regexParentesis = regex.substr(aperturaParentesis,cerraduraParentesis+1);

    Regex_a_NFA_Thompson(estrella, regexParentesis);
    
    estrella->push_back(new nodo("0"));

    if(thompson->size()==0)
    {
        estrella->insert(estrella->begin(),new nodo("0"));
        aperturaEstrella = estrella->at(0);
    }

    aperturaNodo = estrella->at(1);
    cerraduraNodo = estrella->at(estrella->size()-2);
    
    cerraduraEstrella = estrella->at(estrella->size()-1);
    
    if(thompson->size()!=0)
    {
        aperturaEstrella = thompson->at(thompson->size()-1);
    }

    agregarVertice(aperturaEstrella, cerraduraEstrella, caracterVacio);
    agregarVertice(aperturaEstrella, aperturaNodo, caracterVacio);
    agregarVertice(cerraduraNodo, aperturaNodo, caracterVacio);
    agregarVertice(cerraduraNodo, cerraduraEstrella, caracterVacio);

    thompson->insert(thompson->end(),estrella->begin(),estrella->end());
    renombrarNodos(thompson);
}
void NFA_Thompson_Parentesis(std::vector<nodo*> * thompson, std::string regex)
{
    int i;
    int aperturaParentesis = 0;
    int cerraduraParentesis = 0;
    int tamanoParentesisRegex = visitarParentesis(&regex[aperturaParentesis]);
    int aperturaParentesisB = cerraduraParentesis + 2;
    int cerraduraParentesisB = aperturaParentesisB + visitarParentesis(&regex[aperturaParentesisB]);
    std::string expresion;
    std::string identificador;
    char simboloParentesis = '(';
    char simboloActual = '(';
    char simboloConcatenacion = '|';
    char simboloEstrella = '*';
    nodo * aperturaNodo = NULL;
    nodo * cerraduraNodo = NULL;
    if(thompson->size()==0)
    {
        identificador="0";
    }
    else
    {
        identificador = thompson->size()+1;
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
                    aperturaParentesisB = cerraduraParentesis + 2;
                    cerraduraParentesisB = aperturaParentesisB + visitarParentesis(&regex[aperturaParentesisB]);
                    NFA_Thompson_Concatenacion(thompson, &regex[i]);
                    i=cerraduraParentesisB;
                }
                if (simboloActual == simboloEstrella)
                {
                    NFA_Thompson_Estrella(thompson, &regex[i]);
                    i=cerraduraParentesis+1;
                }
            }
        }
        else
        {
            if(regex[i]!=')')
            {
                expresion = regex[i];
                if (thompson->size() == 0)
                {
                    agregarNodo(thompson,identificador);
                }
                aperturaNodo = thompson->at(thompson->size()-1);
                agregarNodo(thompson,identificador);
                cerraduraNodo = thompson->at(thompson->size()-1);
                
                agregarVertice(aperturaNodo, cerraduraNodo, expresion);
            }
        }
    }
    renombrarNodos(thompson);
}

void Regex_a_NFA_Thompson(std::vector<nodo*> * thompson, std::string regex)
{
    int i;
    int tamanoRegex = regex.size();
    int posicionAperturaParentesis = 0;
    int posicionCerraduraParentesis = 0;
    int posicionAperturaParentesisB = 0;
    int posicionCerraduraParentesisB = 0;
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
                    NFA_Thompson_Concatenacion(thompson, &regex[i]);
                    i = posicionCerraduraParentesisB;
                    continue;
                }
                if (simboloActual == simboloEstrella)
                {
                    NFA_Thompson_Estrella(thompson, &regex[i]);
                    i = posicionCerraduraParentesis;
                    continue;
                }
                NFA_Thompson_Parentesis(thompson, &regex[i]);
                i = posicionCerraduraParentesis;
            }
            else
            {
                NFA_Thompson_Parentesis(thompson, &regex[i]);
                i = posicionCerraduraParentesis;
            }
        }
    }
    renombrarNodos(thompson);
    thompson->at(thompson->size()-1)->esFin=1;
}
void imprimirNodo(std::vector<nodo*> * listadoNodos)
{
    std::vector<nodo*>::iterator origen;
    std::vector<vertice>::iterator verticeAux;
    int i=0;
    for(origen=listadoNodos->begin();origen<listadoNodos->end();origen++)
    {
        std::cout<<"Nodo "<<i<<"\n";
        for(verticeAux = origen[0]->listaVertices.begin();verticeAux<origen[0]->listaVertices.end();verticeAux++)
        {
            std::cout<<origen[0]->identificador<<"->"<<verticeAux[0].expresion<<"->"<<verticeAux[0].destino->identificador<<"\n";
        }
        i++;
    }
}
void visualizarBNF(std::vector<nodo*> * listadoNodos)
{
    std::vector<nodo*>::iterator aux;
    std::vector<vertice>::iterator vertice;
    std::string expresion;
    std::string destino;
    for(aux= listadoNodos->begin();aux<listadoNodos->end();aux++)
    {
        std::cout<<"q"<<aux[0]->identificador<<"->";
        for(vertice=aux[0]->listaVertices.begin();vertice<aux[0]->listaVertices.end();vertice++)
        {
            expresion = vertice[0].expresion;
            destino = vertice[0].destino->identificador;
            if(expresion.compare("ε")!=0)
            {
                std::cout<<expresion;
            }
            std::cout<<"q"<<destino;
            if(vertice<aux[0]->listaVertices.end()-1)
            {
                std::cout<<"|";
            }
        }    
        if(aux[0]->esFin==1)
        {
            std::cout<<"|ε";
        }
        std::cout<<"\n";   
    }
}
int main()
{
    std::string identificador;
    std::string expresion;
    std::string regex;
    std::vector<nodo*> nodos;
    std::vector<nodo*> thompson;
    std::vector<nodo*> subconjuntos;
    std::vector<nodo*> minimizado;
    std::vector<nodo*> * listadoNodos = &nodos;
    std::vector<nodo*> * NFA_Thompson = &thompson;
    std::vector<nodo*> * DFA = &subconjuntos;
    std::vector<nodo*> * DFA_minimizado = &minimizado;
    nodo * origen = NULL;
    nodo * destino = NULL;
    nodo * nodoInicial = NULL;
    nodo * nodoFinal = NULL;
    int i = 0;
    identificador = "0";
    for (i = 0; i <= 5; i++)
    {
        identificador = std::to_string(i);
        agregarNodo(listadoNodos,identificador);
    }

    nodoInicial = listadoNodos->at(0);
    nodoInicial->esInicio=1;

    nodoFinal = listadoNodos->at(listadoNodos->size()-1);
    nodoFinal->esFin=1;

    /*
        *origen: nodo 0
        *destino: nodo 1
        *vertices: ε
    */
    i=0;
    origen = listadoNodos->at(i);
    destino = listadoNodos->at(i+1);
    expresion="ε";
    agregarVertice(origen,destino,expresion);
    /*
        *origen: nodo 1
        *destino: nodo 1
        *vertices:0,1,2,3,4,5,6,7
    */
    i=1;
    origen = listadoNodos->at(i); 
    destino = listadoNodos->at(i);
    for(i=0;i<=7;i++)
    {
        expresion=std::to_string(i);
        agregarVertice(origen,destino,expresion);
    }
    /*
        *origen: nodo 1
        *destino: nodo 2
        *vertices:8,9
    */
    i=1;
    destino=listadoNodos->at(i+1);
    for(i=8;i<=9;i++)
    {
        expresion=std::to_string(i);
        agregarVertice(origen,destino,expresion);
    }
    /*
        *origen: nodo 1
        *destino: nodo 3
        *vertices:' '
    */
    i=1;
    destino=listadoNodos->at(i+2);
    expresion="_";
    agregarVertice(origen,destino,expresion);
    /*
        *origen: nodo 1
        *destino: nodo 5
        *vertices: ε
    */
    i=1;
    destino=listadoNodos->at(listadoNodos->size()-1);
    expresion="ε";
    agregarVertice(origen,destino,expresion);
    /*
        *origen: nodo 2
        *destino: nodo 4
        *expresion: -
    */
    i=2;
    origen = listadoNodos->at(i);
    destino = listadoNodos->at(i+2);
    expresion="-";
    agregarVertice(origen,destino,expresion);
    /*
        *origen: nodo 3
        *destino: nodo 1
        *expresion: +
    */
    i=3;
    origen = listadoNodos->at(i);
    destino = listadoNodos->at(i-2);
    expresion="+";
    agregarVertice(origen,destino,expresion);
    /*
        *origen: nodo 4
        *destino: nodo 3
        *expresion: ' '
    */
    i=4;
    origen = listadoNodos->at(i);
    destino = listadoNodos->at(i-1);
    expresion="_";
    agregarVertice(origen,destino,expresion);
    /*
        *origen: nodo 4
        *destino: nodo 4
        *expresion: 0,1,2,3,4,5,6,7,8,9
    */
    i=4;
    destino = origen;
    for(i=0;i<=9;i++)
    {
        expresion=std::to_string(i);
        agregarVertice(origen,destino,expresion);
    }
    
    imprimirNodo(listadoNodos);
    renombrarNodos(listadoNodos);
    //convertir a regex
    regex = NFA_a_Regex(listadoNodos);
    std::cout<<"Regex: "<<regex<<"\n";
    
    //convertir a NFA con el algoritmo de Thompson
    Regex_a_NFA_Thompson(NFA_Thompson, regex);
    imprimirNodo(NFA_Thompson);

    //convertir a DFA
    DFA->insert(DFA->begin(),NFA_Thompson->begin(),NFA_Thompson->end());
    NFA_a_DFA(DFA);
    imprimirNodo(DFA);
    
    //convertir a DFA minimizado
    DFA_minimizado->insert(DFA_minimizado->begin(),DFA->begin(),DFA->end());
    DFA_a_DFA_Minimizado(DFA_minimizado);
    imprimirNodo(DFA_minimizado);

    visualizarBNF(DFA_minimizado);
    
    return 0;
    /*
        BNF resultante:
        A -> _ B | 0 A | 1 A | 2 A | 3 A | 4 A | 5 A | 6 A | 7 A | 8 C | 9 C | ϵ
        B -> + A
        C -> - D
        D -> _ B | 0 D | 1 D | 2 D | 3 D | 4 D | 5 D | 6 D | 7 D | 8 D | 9 D | ϵ
        P(A)={0,1,2,3,4,5,6,7,8,9,_,ϵ}              S(A)={$}
        P(B)={+}                                    S(B)={$}
        P(C)={-}                                    S(C)={$}
        P(D)={0,1,2,3,4,5,6,7,8,9,_,ϵ}              S(D)={$}
    */
}