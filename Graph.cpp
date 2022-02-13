#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <list>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cfloat>
#include <iomanip>


using namespace std;

/**************************************************************************************************
 * Defining the Graph's methods
**************************************************************************************************/

// Constructor
Graph::Graph(int order, bool directed, bool weighted_edge, bool weighted_node)
{

    this->order = order;
    this->directed = directed;
    this->weighted_edge = weighted_edge;
    this->weighted_node = weighted_node;
    this->first_node = this->last_node = nullptr;
    this->number_edges = 0;
}

// Destructor
Graph::~Graph()
{

    Node *next_node = this->first_node;

    while (next_node != nullptr)
    {

        next_node->removeAllEdges();
        Node *aux_node = next_node->getNextNode();
        delete next_node;
        next_node = aux_node;
    }
}

// Getters
int Graph::getOrder()
{
    return this->order;
}
int Graph::getNumberEdges()
{
    return this->number_edges;
}
//Function that verifies if the graph is directed
bool Graph::getDirected()
{
    return this->directed;
}
//Function that verifies if the graph is weighted at the edges
bool Graph::getWeightedEdge()
{
    return this->weighted_edge;
}
//Function that verifies if the graph is weighted at the nodes
bool Graph::getWeightedNode()
{
    return this->weighted_node;
}

Node *Graph::getFirstNode()
{
    return this->first_node;
}

Node *Graph::getLastNode()
{
    return this->last_node;
}

void Graph::insertNode(int id_rotulo, float peso)
{
    Node *no = new Node(this->order + 1, id_rotulo); //cria um novo nó

    //verifica se o grafo tem peso nos nós
    if(this->getWeightedNode())
    {
        no->setWeight(peso); //insere o peso no novo nó
    }

    if(this->first_node == nullptr)  //verifica se existe nó no grafo, caso não exista, altera o primeiro e ultimo nó
    {
        this->first_node = no;
        this->last_node = no;
    }
    else {                              //caso exista, altera apenas o último nó e o seguinte do que antes era o último

        this->last_node->setNextNode(no);
        this->last_node = no;
    }

    order++;

}

void Graph::insertEdge(int id_rotulo, int target_id_rotulo, float weight)
{

    //Verifica se o nó Inicial existe
    if(!searchNode(id_rotulo))
    {
        insertNode(id_rotulo,0);
    }

    //Verifica se o nó Final existe
    if(!searchNode(target_id_rotulo))
    {
        insertNode(target_id_rotulo,0);
    }

    Node *noInicial = getNodeByRotulo(id_rotulo);   //recebe o nó inicial a partir do ID recebido
    Node *noFinal = getNodeByRotulo(target_id_rotulo);   //recebe o nó final a partir do ID recebido

    int id = noInicial->getId(); //recebe o id do nó inicial
    int target_id = noFinal->getId(); //recebe o id do nó final

    noInicial->insertEdge(target_id, weight, id, false, target_id_rotulo, id_rotulo); //insere a aresta a partir do nó inicial

    if(directed) //verifica se é um digrafo
    {
        getNode(id)->incrementOutDegree();
        getNode(target_id)->incrementInDegree();
    }
    else
    {
        // uma aresta auxiliar é adicionada entre os dois nós para o percurso inverso ser possível
//        Node *noAux = getNode(target_id);
        noFinal->insertEdge(id, weight, target_id, true, id_rotulo, target_id_rotulo);

        getNode(id)->incrementOutDegree();
        getNode(id)->incrementInDegree();

        getNode(target_id)->incrementOutDegree();
        getNode(target_id)->incrementInDegree();
    }
    this->number_edges ++;
}

void Graph::removeNode(int id){

    Node *no = this->first_node;
    Node *noAux = this->first_node;

    if(this->first_node->getId() == id) //verifica se o no a ser excluído é o primeiro nó do grafo
    {
        this->first_node = no->getNextNode();
        delete no;
        return;
    }

    no = no->getNextNode();  //nó recebe o próximo nó

    while (no != nullptr ) //loop que busca o nó a ser excluído e o seu antecessor
    {
        if (no->getId() == id)
        {
            noAux->setNextNode(no->getNextNode()); //altera o próximo nó do antecessor para ser o nó sucessor do nó que será excluído

            if (no == last_node) //verifica se o nó a ser excluído é o último do grafo
            {
                last_node = noAux;
            }

            delete no;
            return;
        }

        noAux = noAux->getNextNode();
        no = no->getNextNode();
    }

}

bool Graph::searchNode(int id)
{
    Node *no = this->first_node;

    while (no != nullptr) //loop responsavel por encontrar o nó no grafo
    {
        if (no->getIdRotulo() == id) //condicional que verifica se o ID Rotulo do nó foi encontrado
        {
            return true;
        }
        no = no->getNextNode();
    }

    return false;
}

Node *Graph::getNode(int id)
{
    Node *no = this->first_node;

    while (no != nullptr) //loop responsavel por encontrar o nó no grafo
    {
        if (no->getId() == id) //condicional que verifica se o ID do nó foi encontrado
        {
            return no;
        }
        no = no->getNextNode();
    }

    return nullptr;
}

Node *Graph::getNodeByRotulo(int id)
{
    Node *no = this->first_node;

    while (no != nullptr) //loop responsavel por encontrar o nó no grafo
    {
        if (no->getIdRotulo() == id) //condicional que verifica se o ID do nó foi encontrado
        {
            return no;
        }
        no = no->getNextNode();
    }

    return nullptr;
}

//NOSSOS METODOS

void Graph::montarCabecalhoGrafoDOT(string *grafo, string *arestaDOT){
    if(this->directed) //verifica se o grafo é direcionado
    {
        *grafo = "strict digraph G {\n";
        *arestaDOT = " -> ";
    }
    else {
        *grafo = "strict graph G {\n";
        *arestaDOT = " -- ";
    }
}

void Graph::montarArestaGrafoDOT(string *grafo, string *arestaDOT, int idRotuloNoAtual, int idRotuloNoAlvo, float pesoAresta, bool retorno){
    *grafo += "\t" + to_string(idRotuloNoAtual) + *arestaDOT + to_string(idRotuloNoAlvo);
    *grafo += " [weight = " + to_string(pesoAresta);
    *grafo += ", label = " + to_string(pesoAresta);
    if(retorno){
        *grafo += ", color = red";
    }
    *grafo += "]\n";
}

bool Graph::contidoNaLista(int elemento, list<int> lista) {
    for (auto it = lista.begin(); it != lista.end(); it++) {
        if (*it == elemento) {
            return true;
        }
    }

    return false;
}

int Graph::getElementoLista(list<int> lista, int indice) {
    int contador = 0;
    for (auto it = lista.begin(); it != lista.end(); it++) {
        if (contador == indice) {
            return *it;
        }

        contador++;
    }

    return -1;
}

void Graph::removeElementoLista(list<int> *lista, int indice) {
    int contador = 0;
    for (auto it = lista->begin(); it != lista->end(); it++) {
        if (contador == indice) {
            lista->erase(it);
            break;
        }

        contador++;
    }
}

void Graph::ordenarCrescentementeNosPorPeso(list<int> *listaCrescrenteNosPorPeso){
    // cout << "Ordenar por Peso" << endl;
    Node *noAtual = this->first_node;
    Node *noLista, *noFinalLista;

    listaCrescrenteNosPorPeso->push_back(noAtual->getIdRotulo()); //Insere o primeiro nó na lista
    noAtual = noAtual->getNextNode();

    //Loop que percorre os nós do grafo
    while(noAtual != nullptr){
        noFinalLista = getNodeByRotulo(listaCrescrenteNosPorPeso->back());
        //Loop que percorre a lista de Nós
        for(auto it = listaCrescrenteNosPorPeso->begin(); it!=listaCrescrenteNosPorPeso->end(); it++){
            noLista = getNodeByRotulo(*it);
            //Condição que permite inserir o nó automaticamente no final da lista caso o seu peso seja acima do ultimo elemento
            if(noAtual->getWeight() >= noFinalLista->getWeight()){
                listaCrescrenteNosPorPeso->push_back(noAtual->getIdRotulo());
                break;
            } else if(noAtual->getWeight() < noLista->getWeight()){
                listaCrescrenteNosPorPeso->insert(it,noAtual->getIdRotulo());
                break;
            }
        }
        noAtual = noAtual->getNextNode();
    }

     cout << "Lista Crescente por Peso: " << endl;

//    Imprime a lista para verificarmos se está ordenando corretamente. NÃO ESTARÁ PRESENTE NO TRABALHO FINAL!
     for(auto it = listaCrescrenteNosPorPeso->begin(); it!=listaCrescrenteNosPorPeso->end(); it++){
         cout << *it << " ";
     }
     cout << endl;
}

void Graph::ordenarDecrescentementeNosPorGrau(list<int> *listaDecrescrenteNosPorGrau){
    // cout << "Ordenar por Grau" << endl;
    Node *noAtual = this->first_node;
    Node *noLista, *noFinalLista;

    listaDecrescrenteNosPorGrau->push_back(noAtual->getIdRotulo()); //Insere o primeiro nó na lista
    noAtual = noAtual->getNextNode();

    //Loop que percorre os nós do grafo
    while(noAtual != nullptr){
        noFinalLista = getNodeByRotulo(listaDecrescrenteNosPorGrau->back());
        //Loop que percorre a lista de Nós
        for(auto it = listaDecrescrenteNosPorGrau->begin(); it!=listaDecrescrenteNosPorGrau->end(); it++){
            noLista = getNodeByRotulo(*it);
            //Condição que permite inserir o nó automaticamente no final da lista caso o seu grau seja abaixo do ultimo elemento
            if(noAtual->getOutDegree() <= noFinalLista->getOutDegree()){
                listaDecrescrenteNosPorGrau->push_back(noAtual->getIdRotulo());
                break;
            } else if(noAtual->getOutDegree() > noLista->getOutDegree()){
                listaDecrescrenteNosPorGrau->insert(it,noAtual->getIdRotulo());
                break;
            }
        }
        noAtual = noAtual->getNextNode();
    }

     cout << "Lista Decrescente por grau: " << endl;

//    Imprime a lista para verificarmos se está ordenando corretamente. NÃO ESTARÁ PRESENTE NO TRABALHO FINAL!
     for(auto it = listaDecrescrenteNosPorGrau->begin(); it != listaDecrescrenteNosPorGrau->end(); it++){
         cout << *it << " ";
     }
     cout << endl;
}

void Graph::inicializacaoClusters(int clusters, list<int> *listaCrescrenteNosPorPeso, list<int> *listaDecrescrenteNosPorGrau, list<int> matrizCluster[], int matrizMenorMaiorCluster[][3]) {
    // Deve mudar totalmente
    
    // float pontoCorte = float(this->order) / float(clusters);
    // bool divisivel;

    // if(this->order % clusters == 0){
    //     divisivel = true;
    // } else {
    //     divisivel = false;
    // }

    // int contadorLista = 1;
    // int contadorLinhaMatriz = 0;
    // int contadorNaoDivisivel = 1;

    // for(auto it = listaCrescrenteNosPorPeso->begin(); it != listaCrescrenteNosPorPeso->end(); it++){
    //     if(divisivel){
    //         if(contadorLista % int(pontoCorte) == 0){
    //             matrizCluster[contadorLinhaMatriz].push_back(*it);
    //             matrizMenorMaiorCluster[contadorLinhaMatriz][0] = getNodeByRotulo(*it)->getWeight();
    //             matrizMenorMaiorCluster[contadorLinhaMatriz][1] = getNodeByRotulo(*it)->getWeight();
    //             matrizMenorMaiorCluster[contadorLinhaMatriz][2] = matrizMenorMaiorCluster[contadorLinhaMatriz][1] - matrizMenorMaiorCluster[contadorLinhaMatriz][0];
    //             // cout << "Inseriu o numero " << *it << " no cluster " << contadorLinhaMatriz + 1 << endl;
    //             // cout << "Novo menor: " << matrizMenorMaiorCluster[contadorLinhaMatriz][0] << " Novo maior: " << matrizMenorMaiorCluster[contadorLinhaMatriz][1] << endl;
    //             listaDecrescrenteNosPorGrau->remove(*it);
    //             contadorLinhaMatriz++;
    //         }
    //     } else {
    //         if(contadorLista == int(round(pontoCorte * contadorNaoDivisivel))){
    //             matrizCluster[contadorLinhaMatriz].push_back(*it);
    //             matrizMenorMaiorCluster[contadorLinhaMatriz][0] = getNodeByRotulo(*it)->getWeight();
    //             matrizMenorMaiorCluster[contadorLinhaMatriz][1] = getNodeByRotulo(*it)->getWeight();
    //             matrizMenorMaiorCluster[contadorLinhaMatriz][2] = matrizMenorMaiorCluster[contadorLinhaMatriz][1] - matrizMenorMaiorCluster[contadorLinhaMatriz][0];
    //             // cout << "Inseriu o numero " << *it << " no cluster " << contadorLinhaMatriz + 1 << endl;
    //             // cout << "Novo menor: " << matrizMenorMaiorCluster[contadorLinhaMatriz][0] << " Novo maior: " << matrizMenorMaiorCluster[contadorLinhaMatriz][1] << endl;
    //             listaDecrescrenteNosPorGrau->remove(*it);
    //             contadorLinhaMatriz++;
    //             contadorNaoDivisivel++;
    //         }
    //     }
    //     contadorLista++;
    // }
}

void Graph::atualizaSolucao(int clusters, Node *noAtual, list<int> matrizCluster[], int matrizMenorMaiorCluster[][3], stack<int> *pilhaReservaVertices) {
    // Deve mudar totalmente

    // int pesoNoLeve, pesoNoPesado;
    // int menorDiferenca[2];
    // bool semDiferenca, atualizouMenor, atualizouMaior, encontrouCluster, colocouNaPilha;

    // menorDiferenca[0] = int(INFINITY);
    // semDiferenca = false;
    // atualizouMenor = false;
    // atualizouMaior = false;
    // encontrouCluster = false;
    // colocouNaPilha = false;
    // // cout << "CHEGOU AQUI 2" << endl;
    // for(int i = 0; i < clusters; i++){
    //     pesoNoLeve = matrizMenorMaiorCluster[i][0];
    //     pesoNoPesado = matrizMenorMaiorCluster[i][1];
    //     // cout << "CHEGOU AQUI 3" << endl;
    //     if(noAtual->getWeight() < pesoNoLeve){
    //         //Condicional que garante a conexidade entre os elementos dos clusters
    //         if(pesoNoPesado - noAtual->getWeight() < menorDiferenca[0] && mantemConexidade(matrizCluster[i],noAtual)){

    //             //Condicional que garante que os clusters terão no minimo dois elementos
    //             if(menorDiferenca[0] == int(INFINITY) || matrizCluster[i].size() == 1 || matrizCluster[menorDiferenca[1]].size() > 1 ){
    //                 menorDiferenca[0] = pesoNoPesado - noAtual->getWeight();
    //                 menorDiferenca[1] = i;
    //                 atualizouMenor = true;
    //                 atualizouMaior = false;
    //                 encontrouCluster = true;
    //                 if(colocouNaPilha){
    //                     pilhaReservaVertices->pop();
    //                     colocouNaPilha = false;
    //                 }
    //             }

    //         }
    //             //Condicional que garante a conexidade entre os elementos dos clusters
    //     } else if(noAtual->getWeight() > pesoNoPesado && mantemConexidade(matrizCluster[i],noAtual)){
    //         if(noAtual->getWeight() - pesoNoLeve < menorDiferenca[0]){
    //             //Condicional que garante que os clusters terão no minimo dois elementos
    //             if(menorDiferenca[0] == int(INFINITY) || matrizCluster[i].size() == 1 || matrizCluster[menorDiferenca[1]].size() > 1 ) {
    //                 menorDiferenca[0] = noAtual->getWeight() - pesoNoLeve;
    //                 menorDiferenca[1] = i;
    //                 atualizouMaior = true;
    //                 atualizouMenor = false;
    //                 encontrouCluster = true;
    //                 if (colocouNaPilha) {
    //                     pilhaReservaVertices->pop();
    //                     colocouNaPilha = false;
    //                 }
    //             }
    //         }
    //             //Condicional que garante a conexidade entre os elementos dos clusters
    //     } else if(mantemConexidade(matrizCluster[i],noAtual)){
    //         //Condicional que garante que os clusters terão no minimo dois elementos
    //         if(menorDiferenca[0] == int(INFINITY) || matrizCluster[i].size() == 1 || matrizCluster[menorDiferenca[1]].size() > 1 ) {
    //             menorDiferenca[0] = 0;
    //             menorDiferenca[1] = i;
    //             semDiferenca = true;
    //             encontrouCluster = true;
    //             if (colocouNaPilha) {
    //                 pilhaReservaVertices->pop();
    //                 colocouNaPilha = false;
    //             }
    //             break;
    //         }

    //     } else if(!encontrouCluster){
    //         // cout << "Nao encontrou conexidade" << endl;
    //         if(!colocouNaPilha){
    //             colocouNaPilha = true;
    //             pilhaReservaVertices->push(noAtual->getIdRotulo());
    //         }
    //     }
    // }
    // // cout << "CHEGOU AQUI 4" << endl;
    // if(encontrouCluster){
    //     // cout << "ENCONTROU UM CLUSTER PARA ELE ->  " << noAtual->getIdRotulo() << endl;
    //     matrizCluster[menorDiferenca[1]].push_back(noAtual->getIdRotulo());
    //     // cout << "O CLUSTER QUE ELE ENTROU FOI: " << menorDiferenca[1] + 1 << endl;
    //     if(!semDiferenca){
    //         if(atualizouMenor){
    //             matrizMenorMaiorCluster[menorDiferenca[1]][0] = noAtual->getWeight();
    //         } else{
    //             matrizMenorMaiorCluster[menorDiferenca[1]][1] = noAtual->getWeight();
    //         }
    //     }
    //     matrizMenorMaiorCluster[menorDiferenca[1]][2] = matrizMenorMaiorCluster[menorDiferenca[1]][1] - matrizMenorMaiorCluster[menorDiferenca[1]][0];
    // }
}

// Precisa ser modificada para atualizar conforme um índice da lista de candidatos
void Graph::atualizaListaCandidatos(list<int> *listaCandidatos, int indice, stack<int> *pilhaReservaVertices) {
    // Deve mudar em partes

    removeElementoLista(listaCandidatos, indice);
    
    if (listaCandidatos->empty() && !pilhaReservaVertices->empty()) {
        // cout << "A lista acabou e a pilha nao esta vazia!" << endl;
        while (!pilhaReservaVertices->empty()) {
            listaCandidatos->push_front(pilhaReservaVertices->top());
            // cout << "LISTA ATUALMENTE3: " << endl;
            // for (auto it = listaCandidatos->begin(); it != listaCandidatos->end(); it++) {
            //     cout << *it << " ";
            // }
            // cout << endl;
            pilhaReservaVertices->pop();
        }
    }
}

void Graph::testeVector() {
    list<vector<int>> menorMaiorPesosClusters; //[0] = menorPeso, [1] = maiorPeso, [2] = diferença entre pesos
    cout << "Tamanho da lista de vetores: " << menorMaiorPesosClusters.size() << endl;

    menorMaiorPesosClusters.resize(2);

    cout << "Tamanho novo da lista de vetores: " << menorMaiorPesosClusters.size() << endl;

    cout << "TAMANHO DO VETOR: " << menorMaiorPesosClusters.back().size() << endl;

    menorMaiorPesosClusters.front().push_back(10);

    cout << menorMaiorPesosClusters.front().front() << endl;

}

void Graph::gerarClusters(list<list<int>> *listaClusters, list<list<int>>::iterator itListaClusters){
    Node *noAtual = this->first_node;
    for(itListaClusters = listaClusters->begin(); itListaClusters != listaClusters->end(); itListaClusters++){
        itListaClusters->push_back(noAtual->getIdRotulo()); //popula cada cluster
        noAtual = noAtual->getNextNode();
    }
}

void Graph::conhecerVizinhanca(list<int> *listaVerticesVizinhos, list<int>::iterator  itElementosCluster, list<list<int>>::iterator itListaClusters){

    Node *noAtual;
    Edge *arestaAtual;

    for(itElementosCluster = itListaClusters->begin(); itElementosCluster != itListaClusters->end(); itElementosCluster++){
        cout << "Tamanho da lista do cluster" << itListaClusters->size() << endl;
        noAtual = getNodeByRotulo(*itElementosCluster);
        arestaAtual = noAtual->getFirstEdge();
        //Percorre as arestas do elemento atual da lista de elementos de um cluster
        while(arestaAtual != nullptr){
            //verifica se o elemento ja foi inserido na lista de vizinhos dos elementos do cluster
            if(!contidoNaLista(arestaAtual->getTargetIdRotulo(),*listaVerticesVizinhos)){
                listaVerticesVizinhos->push_back(arestaAtual->getTargetIdRotulo());
            }
            arestaAtual = arestaAtual->getNextEdge();
        }
    }
}

void Graph::buscarConexidade(list<int> *listaVerticesVizinhos, list<int> *idListaCandidatos, list<int> *pesoListaCandidatos, list<int>::iterator itPesoListaCandidatos, list<int>::iterator itIdListaCandidatos, list<int>::iterator  itElementosCluster, list<list<int>>::iterator itListaClustersAux,list<list<int>>::iterator itListaClusters,int posicaoClusterAlvo){

    int pesoLeveClusterAtual, pesoPesadoClusterAtual, pesoLeveClusterAlvo, pesoPesadoClusterAlvo, novoPesoLeve, novoPesoPesado;

    for(itElementosCluster = itListaClustersAux->begin(); itElementosCluster != itListaClustersAux->end(); itElementosCluster++){
        //Verifica se o elemento pertence a lista de vertices vizinhos do cluster atual
        if(contidoNaLista(*itElementosCluster,*listaVerticesVizinhos)){
            //Recebe os pesos minimos e maximos de cada cluster
            pesoLeveClusterAtual = itListaClusters->front();
            pesoPesadoClusterAtual = itListaClusters->back();
            pesoLeveClusterAlvo = itListaClustersAux->front();
            pesoPesadoClusterAlvo = itListaClustersAux->back();

            //Seleciona o menor peso minimo e o maior peso maximo
            novoPesoLeve = min(pesoLeveClusterAtual,pesoLeveClusterAlvo);
            novoPesoPesado = max(pesoPesadoClusterAtual,pesoPesadoClusterAlvo);

            //Caso a lista esteja vazia ou o novo peso seja maior q o ultimo, já insere automaticamente no final
            if(pesoListaCandidatos->empty() || novoPesoPesado - novoPesoLeve > pesoListaCandidatos->back()){
                pesoListaCandidatos->push_back(novoPesoPesado - novoPesoLeve);
                idListaCandidatos->push_back(posicaoClusterAlvo);
                //caso contrario, insere ordenamente na lista de pesos
            } else {
                for(itPesoListaCandidatos = pesoListaCandidatos->begin(), itIdListaCandidatos = idListaCandidatos->begin(); itPesoListaCandidatos != pesoListaCandidatos->end(); itPesoListaCandidatos++, itIdListaCandidatos++){
                    if(novoPesoPesado - novoPesoLeve < *itPesoListaCandidatos){
                        pesoListaCandidatos->insert(itPesoListaCandidatos,novoPesoPesado - novoPesoLeve);
                        idListaCandidatos->insert(itIdListaCandidatos,posicaoClusterAlvo);
                    }
                }
            }
        }
    }
}

void Graph::algoritmoGuloso(int cluster) {
    cout << "Algoritmo Guloso" << endl;
    cout << "CLUSTER: " << cluster << endl;

    list<list<int>> listaClusters(this->order); //Lista responsavel por armazenar as listas que armazenam os membros de cada cluster;
    list<list<int>>::iterator itListaClusters, itListaClustersAux; //Iterator da lista de Clusters;

    list<int> idListaCandidatos, pesoListaCandidatos; //Listas que armazenam a posicao dos clusters candidatos e o peso atualizado caso ocorra o merge
    list<int>::iterator itIdListaCandidatos;
    list<int>::iterator itPesoListaCandidatos;

//    list<int> listaCrescenteNosPorPeso;
//    list<int>::iterator  itListaCrescente;
//    ordenarCrescentementeNosPorPeso(&listaCrescenteNosPorPeso);

    cout << "Tamanho da lista de Clusters antes " << listaClusters.size() << endl;

    //Funcao responsavel por dividir cada nó em um cluster
    gerarClusters(&listaClusters,itListaClusters);

    cout << "Tamanho da lista Cluters depois: " << listaClusters.size() << endl;

    list<int> listaVerticesVizinhos;

    list<int>::iterator  itElementosCluster, itElementosClusterAux, itGuardaUltimaInsercao;
    int posicaoClusterAlvo;
    int pesoLeveClusterAtual, pesoPesadoClusterAtual, pesoLeveClusterAlvo, pesoPesadoClusterAlvo, novoPesoLeve, novoPesoPesado;
    int idClusterEscolhido, pesoClusterEscolhido;

    //Loop principal, responsável por tornar a lista de clusters do tamanho certo que a instancia pede
    while(listaClusters.size() > cluster){
        //Loop responsável por percorrer cada lista da lista de clusters, para realizar o merge no final
        for(itListaClusters = listaClusters.begin(); itListaClusters != listaClusters.end(); itListaClusters++){
            //esse for é temporario, será apagado no trabalho final
            for(auto it2 = listaClusters.begin(); it2 != listaClusters.end(); it2++) {
                cout << "Tamanho do cluster: " << it2->size() << endl;
                cout << "MEMBROS DO CLUSTER: " << endl;
                for(auto it = it2->begin(); it != it2->end(); it++){
                    cout << *it << " ";
                }
                cout << endl;
            }

            //Primeiramente percorremos a lista de elementos do cluster para descobrir seus vertices vizinhos
            conhecerVizinhanca(&listaVerticesVizinhos,itElementosCluster,itListaClusters);

            posicaoClusterAlvo = 1;

            //Novamente percorremos a lista de clusters, dessa vez em busca dos clusters conexos ao que estamos atualmente
            for(itListaClustersAux = listaClusters.begin(); itListaClustersAux != listaClusters.end(); itListaClustersAux++){
                //verifica se nao eh o cluster que estamos atualmente
                if(itListaClustersAux != itListaClusters){
                    //percorre os elementos do cluster para encontrar conexidade
                    //talvez dê para fazer essa funcao pegar o for e if anterior
                    buscarConexidade(&listaVerticesVizinhos,&idListaCandidatos,&pesoListaCandidatos,itPesoListaCandidatos,itIdListaCandidatos,itElementosCluster,itListaClustersAux, itListaClusters,posicaoClusterAlvo);
                }
                posicaoClusterAlvo++;
            }

            cout << "Tamanho da lista de pesos dos candidatos: " << pesoListaCandidatos.size() << endl;
            cout << "Tamanho da lista de ids dos candidatos: " << idListaCandidatos.size() << endl;

            cout << "Lista de Candidatos: " << endl;
            for(itPesoListaCandidatos = pesoListaCandidatos.begin(), itIdListaCandidatos = idListaCandidatos.begin(); itPesoListaCandidatos != pesoListaCandidatos.end(); itPesoListaCandidatos++, itIdListaCandidatos++){
                cout << "ID: " << *itIdListaCandidatos << " PESO: " << *itPesoListaCandidatos << endl;
            }

            if(!idListaCandidatos.empty()){
                //Aqui teria a iteração para escolher de acordo com o alfa
                idClusterEscolhido = idListaCandidatos.front();
                pesoClusterEscolhido = pesoListaCandidatos.front();

                //Loop que avança até o iterator da lista selecionada
                itListaClustersAux = listaClusters.begin();
                for(int i = 1; i < idClusterEscolhido; i++){
                    itListaClustersAux++;
                }

                //Criar funcao inserirOrdenamente (talvez essa funcao possa ate ser usada em outro lugar do codigo acima)
                //Aqui inserimos cada elemento do cluster escolhido no cluster atual de forma ordenada
                itGuardaUltimaInsercao = itListaClusters->begin();
                for(itElementosClusterAux = itListaClustersAux->begin(); itElementosClusterAux != itListaClustersAux->end(); itElementosClusterAux++){
                    if(*itElementosClusterAux > itListaClusters->back()){
                        itListaClusters->push_back(*itElementosClusterAux);
                    } else {
                        for(itElementosCluster = itGuardaUltimaInsercao; itElementosCluster != itListaClusters->end(); itElementosCluster++){
                            if(*itElementosClusterAux < *itElementosCluster){
                                itListaClusters->insert(itElementosCluster,*itElementosClusterAux);
                                itGuardaUltimaInsercao = itElementosCluster;
                                break;
                            }
                        }
                    }
                }

                listaClusters.erase(itListaClustersAux);

                pesoListaCandidatos.clear();
                idListaCandidatos.clear();
            }

            if(listaClusters.size() == cluster){
                break;
            }
        }

    }
    //criar funcao registrarResultado (Nela vamos imprimir na tela e também anotar no TXT
    int soma = 0;
    for(itListaClusters = listaClusters.begin(); itListaClusters != listaClusters.end(); itListaClusters++) {
        cout << "Tamanho do cluster: " << itListaClusters->size() << endl;
        for(auto it = itListaClusters->begin(); it != itListaClusters->end(); it++){
            cout << *it << " ";
        }
        cout << endl;
        cout << "Diferenca no CLuster: " <<itListaClusters->back() - itListaClusters->front() << endl;
        soma = soma + itListaClusters->back() - itListaClusters->front();
    }
    cout << "SOMA = " << soma << endl;
}

solucao Graph::algoritmoGulosoRandomizado(int clusters, float alfa, int numIter) {
    list<int> *melhorSolucao = nullptr;
    int menorDiferenca = (int) INFINITY;
    
    srand(time(NULL));

    for (int i = 0; i < numIter; i++) {
        list<int> *matrizCluster = new list<int>[clusters];
        int matrizMenorMaiorCluster[clusters][3];
        stack<int> pilhaReservaVertices;

        int diferencaSolucaoAtual = 0;

        list<int> listaCrescrenteNosPorPeso; //Lista que armazena os ID's ROTULO dos Vertices
        list<int> listaCandidatos; //Lista que armazena os ID's ROTULO dos Vertices
        ordenarCrescentementeNosPorPeso(&listaCrescrenteNosPorPeso);
        ordenarDecrescentementeNosPorGrau(&listaCandidatos);

        inicializacaoClusters(clusters, &listaCrescrenteNosPorPeso, &listaCandidatos, matrizCluster, matrizMenorMaiorCluster);
    
        do
        {
            int indice = 0;
            int limiteSuperior = alfa * listaCandidatos.size();

            if (limiteSuperior != 0) {
                indice = rand() % limiteSuperior;
            }
            
            Node *noAtual = getNodeByRotulo(getElementoLista(listaCandidatos, indice));

            atualizaSolucao(clusters, noAtual, matrizCluster, matrizMenorMaiorCluster, &pilhaReservaVertices);

            atualizaListaCandidatos(&listaCandidatos, indice, &pilhaReservaVertices);

        } while (!listaCandidatos.empty());

        // Calculando gap da solução
        for (int j = 0; j < clusters; j++) {
            diferencaSolucaoAtual += matrizMenorMaiorCluster[j][2];
        }

        // Comparando com a melhor solução
        if (diferencaSolucaoAtual < menorDiferenca) {
            if (melhorSolucao != nullptr) {
                delete [] melhorSolucao;
            }

            menorDiferenca = diferencaSolucaoAtual;
            melhorSolucao = matrizCluster;
        } else {
            delete [] matrizCluster;
        }

        // ================= EXIBIÇÃO DE RESULTADOS ====================

        // for(int i = 0; i < clusters; i++){
        //     cout << "Membros do Cluster " << i+1 << endl;
        //     for(auto it = melhorSolucao[i].begin(); it != melhorSolucao[i].end(); it++){
        //         cout << *it << " ";
        //     }
        //     cout << endl;
        // }

        // cout << "DIFERENCA DOS CLUSTERS: " << endl;
        // for(int i = 0; i < clusters; i++){
        //     cout << "Menor peso do Cluster " << i+1 << " = " << matrizMenorMaiorCluster[i][0] << endl;
        //     cout << "Maior peso do Cluster " << i+1 << " = " << matrizMenorMaiorCluster[i][1] << endl;
        //     cout << "Diferenca do Cluster " << i+1 << " = " << matrizMenorMaiorCluster[i][2] << endl;
        // }

        // Checagem de conexidade
        // for (int i = 0; i < clusters; i++) {
        //     list<int> clusterAtual = melhorSolucao[i];
        //     Graph *subgrafoCluster = subgrafoVerticeInduzido(clusterAtual);

            // Exibindo lista de adjacência do cluster (NÃO SERÁ MANTIDO NO TRABALHO FINAL)
            // for (Node *noAtual = subgrafoCluster->getFirstNode(); noAtual != nullptr; noAtual = noAtual->getNextNode()) {
            //     cout << noAtual->getIdRotulo() << " -> ";
            //     for (Edge *arestaAtual = noAtual->getFirstEdge(); arestaAtual != nullptr; arestaAtual = arestaAtual->getNextEdge()) {
            //         cout << arestaAtual->getTargetIdRotulo() << " ";
            //     }
            //     cout << endl;
            // }

            // Verificando conexidade do cluster
        //     cout << "Cluster" << i + 1 << " - Conexo: ";
        //     cout << boolalpha << subgrafoCluster->ehConexo() << endl;
        // }

        // cout << "GAP: " << diferencaSolucaoAtual << endl;

        // ================= FIM EXIBIÇÃO DE RESULTADOS ====================
    }

    solucao structMelhorSolucao = {
        .diferenca = menorDiferenca,
        .clusters = melhorSolucao
    };

    return structMelhorSolucao;
}

void Graph::inicializaVetoresReativo(float probabilidades[], float medias[], int numElementos) {
    float probabilidadeInicial = (float) (1.0 / numElementos) * 100;

    // cout << "Probabilidades: ";
    for (int i = 0; i < numElementos; i++) {
        probabilidades[i] = probabilidadeInicial;
        // cout << probabilidades[i] << " ";
        medias[i] = INFINITY;
    }
    cout << endl;
}

void Graph::atualizaProbabilidadesReativo(float probabilidades[], float medias[], int menorDiferenca, int numElementos) {
    float qi[10] = {0.0};
    float somatorioQi = 0;

    // cout << "Qi: ";
    for (int i = 0; i < numElementos; i++) {
        qi[i] = powf(menorDiferenca / medias[i], 10);
        somatorioQi += qi[i];
        // cout << qi[i] << " ";
    }
    // cout << endl;

    // cout << "Probabilidades: ";
    for (int i = 0; i < numElementos; i++) {
        probabilidades[i] = (qi[i] / somatorioQi) * 100;
        // cout << probabilidades[i] << " ";
    }
    // cout << endl;
}

float Graph::escolheAlfaReativo(float probabilidades[], float alfas[], int numElementos) {
    int numeroAleatorio = rand() % 101;  // Números aleatórios de 0 a 100
    float probabilidadeAcumulada = 0.0;

    for (int i = 0; i < numElementos; i++) {
        probabilidadeAcumulada += probabilidades[i];

        if (numeroAleatorio <= probabilidadeAcumulada) {
            // cout << "Alfa: " << alfas[i] << endl;
            return alfas[i];
        }
    }
    // cout << "ALFA QUE SOBROU";

    return alfas[numElementos - 1];
}

void Graph::atualizarMediasReativo(float medias[], int acumuladorDiferencas[], int contadorDiferencasPorAlfa[], int diferenca, float alfas[], float alfa, int numElementos) {
    // cout << "Medias: ";
    for (int i = 0; i < numElementos; i++) {
        if (alfas[i] == alfa) {
            acumuladorDiferencas[i] += diferenca;
            contadorDiferencasPorAlfa[i] += 1;
            medias[i] = (float) acumuladorDiferencas[i] / contadorDiferencasPorAlfa[i];
            break;
        }
    }

    // for (int i = 0; i < numElementos; i++) {
    //     cout << medias[i] << " ";
    // }
    // cout << endl;
}

solucao Graph::algoritmoGulosoRandomizadoReativo(int clusters, float alfas[], int numIter, int bloco) {
    // cout << "Algoritmo Guloso Randomizado Reativo" << endl;
    // cout << "CLUSTERS: " << clusters << endl;

    list<int> *melhorSolucao = nullptr;
    int menorDiferenca = (int) INFINITY;

    int numElementos = 10;
    int acumuladorDiferencas[10] = {0};
    int contadorDiferencasPorAlfa[10] = {0};
    float probabilidades[10];
    float medias[10];
    inicializaVetoresReativo(probabilidades, medias, numElementos);
    
    srand(time(NULL));

    for (int i = 0; i < numIter; i++) {
        if (i % bloco == 0 && i != 0) {
            atualizaProbabilidadesReativo(probabilidades, medias, menorDiferenca, numElementos);
        }

        list<int> *matrizCluster = new list<int>[clusters];
        int matrizMenorMaiorCluster[clusters][3];
        stack<int> pilhaReservaVertices;

        int diferencaSolucaoAtual = 0;

        list<int> listaCrescrenteNosPorPeso; //Lista que armazena os ID's ROTULO dos Vertices
        list<int> listaCandidatos; //Lista que armazena os ID's ROTULO dos Vertices
        ordenarCrescentementeNosPorPeso(&listaCrescrenteNosPorPeso);
        ordenarDecrescentementeNosPorGrau(&listaCandidatos);

        inicializacaoClusters(clusters, &listaCrescrenteNosPorPeso, &listaCandidatos, matrizCluster, matrizMenorMaiorCluster);
        float alfa = escolheAlfaReativo(probabilidades, alfas, numElementos);

        do
        {
            int indice = 0;
            int limiteSuperior = alfa * listaCandidatos.size();

            if (limiteSuperior != 0) {
                indice = rand() % limiteSuperior;
            }
            
            Node *noAtual = getNodeByRotulo(getElementoLista(listaCandidatos, indice));

            atualizaSolucao(clusters, noAtual, matrizCluster, matrizMenorMaiorCluster, &pilhaReservaVertices);

            atualizaListaCandidatos(&listaCandidatos, indice, &pilhaReservaVertices);

        } while (!listaCandidatos.empty());

        // Calculando gap da solução
        for (int j = 0; j < clusters; j++) {
            diferencaSolucaoAtual += matrizMenorMaiorCluster[j][2];
        }

        atualizarMediasReativo(medias, acumuladorDiferencas, contadorDiferencasPorAlfa, diferencaSolucaoAtual, alfas, alfa, numElementos);

        // Comparando com a melhor solução
        if (diferencaSolucaoAtual < menorDiferenca) {
            if (melhorSolucao != nullptr) {
                delete [] melhorSolucao;
            }

            menorDiferenca = diferencaSolucaoAtual;
            melhorSolucao = matrizCluster;
        } else {
            delete [] matrizCluster;
        }

        // cout << "Atual: " << diferencaSolucaoAtual << endl; 
        // cout << "Melhor: " << menorDiferenca << endl;
    }

    solucao structMelhorSolucao = {
        .diferenca = menorDiferenca,
        .clusters = melhorSolucao
    };

    return structMelhorSolucao;
}