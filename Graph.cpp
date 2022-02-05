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

void Graph::ordenarCrescentementeNosPorPeso(list<int> *listaCrescrenteNosPorPeso){
    cout << "Ordenar por Peso" << endl;
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

    //Imprime a lista para verificarmos se está ordenando corretamente. NÃO ESTARÁ PRESENTE NO TRABALHO FINAL!
    for(auto it = listaCrescrenteNosPorPeso->begin(); it!=listaCrescrenteNosPorPeso->end(); it++){
        cout << *it << " ";
    }
    cout << endl;
}

void Graph::ordenarDecrescentementeNosPorGrau(list<int> *listaDecrescrenteNosPorGrau){
    cout << "Ordenar por Grau" << endl;
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

    //Imprime a lista para verificarmos se está ordenando corretamente. NÃO ESTARÁ PRESENTE NO TRABALHO FINAL!
    for(auto it = listaDecrescrenteNosPorGrau->begin(); it != listaDecrescrenteNosPorGrau->end(); it++){
        cout << *it << " ";
    }
    cout << endl;
}

void Graph::algoritmoGuloso(int cluster) {

    cout << "Algoritmo Guloso" << endl;
    cout << "CLUSTER: " << cluster << endl;
    list<int> listaCrescrenteNosPorPeso; //Lista que armazena os ID's ROTULO dos Vertices
    list<int> listaDecrescrenteNosPorGrau; //Lista que armazena os ID's ROTULO dos Vertices
    ordenarCrescentementeNosPorPeso(&listaCrescrenteNosPorPeso);
    ordenarDecrescentementeNosPorGrau(&listaDecrescrenteNosPorGrau);

    list<int> matrizCluster[cluster][1]; //As linhas correspondem aos clusters. A primeira coluna é a lista de vertices do cluster, a segunda coluna é o menor peso e terceira coluna o maior peso
    int matrizMenorMaiorCluster[cluster][3];
    float pontoCorte = float(this->order) / float(cluster);

    cout << "Ponto de corte: " << pontoCorte << endl;
    cout << "Ordem: " << this->order << endl;
    cout << "Cluster: " << cluster << endl;

    bool divisivel;

    if(this->order % cluster == 0){
        divisivel = true;
    } else {
        divisivel = false;
    }

    int contadorLista = 1;
    int contadorLinhaMatriz = 0;
    int contadorNaoDivisivel = 1;

    for(auto it = listaCrescrenteNosPorPeso.begin(); it != listaCrescrenteNosPorPeso.end(); it++){
        if(divisivel){
            if(contadorLista % int(pontoCorte) == 0){
                matrizCluster[contadorLinhaMatriz][0].push_back(*it);
                matrizMenorMaiorCluster[contadorLinhaMatriz][0] = getNodeByRotulo(*it)->getWeight();
                matrizMenorMaiorCluster[contadorLinhaMatriz][1] = getNodeByRotulo(*it)->getWeight();
                matrizMenorMaiorCluster[contadorLinhaMatriz][2] = matrizMenorMaiorCluster[contadorLinhaMatriz][1] - matrizMenorMaiorCluster[contadorLinhaMatriz][0];
                cout << "Inseriu o numero " << *it << " no cluster " << contadorLinhaMatriz + 1 << endl;
                cout << "Novo menor: " << matrizMenorMaiorCluster[contadorLinhaMatriz][0] << " Novo maior: " << matrizMenorMaiorCluster[contadorLinhaMatriz][1] << endl;
                listaDecrescrenteNosPorGrau.remove(*it);
                contadorLinhaMatriz++;
            }
        } else {
            if(contadorLista == int(round(pontoCorte * contadorNaoDivisivel))){
                matrizCluster[contadorLinhaMatriz][0].push_back(*it);
                matrizMenorMaiorCluster[contadorLinhaMatriz][0] = getNodeByRotulo(*it)->getWeight();
                matrizMenorMaiorCluster[contadorLinhaMatriz][1] = getNodeByRotulo(*it)->getWeight();
                matrizMenorMaiorCluster[contadorLinhaMatriz][2] = matrizMenorMaiorCluster[contadorLinhaMatriz][1] - matrizMenorMaiorCluster[contadorLinhaMatriz][0];
                cout << "Inseriu o numero " << *it << " no cluster " << contadorLinhaMatriz + 1 << endl;
                cout << "Novo menor: " << matrizMenorMaiorCluster[contadorLinhaMatriz][0] << " Novo maior: " << matrizMenorMaiorCluster[contadorLinhaMatriz][1] << endl;
                listaDecrescrenteNosPorGrau.remove(*it);
                contadorLinhaMatriz++;
                contadorNaoDivisivel++;
            }
        }
        contadorLista++;
    }

    Node *noAtual;
    int pesoNoLeve, pesoNoPesado;
    int menorDiferenca[2];
    bool semDiferenca, atualizouMenor, atualizouMaior;

    while(!listaDecrescrenteNosPorGrau.empty()){
        cout << "Chegou aqui 1" << endl;
        noAtual = getNodeByRotulo(listaDecrescrenteNosPorGrau.front());
        menorDiferenca[0] = int(INFINITY);
        cout << "Chegou aqui 2" << endl;
        semDiferenca = false;
        atualizouMenor = false;
        atualizouMaior = false;
        for(int i = 0; i < cluster; i++){
            cout << "Chegou aqui 3" << endl;
            pesoNoLeve = matrizMenorMaiorCluster[i][0];
            pesoNoPesado = matrizMenorMaiorCluster[i][1];

            if(noAtual->getWeight() < pesoNoLeve){
                if(pesoNoPesado - noAtual->getWeight() < menorDiferenca[0]){
                    menorDiferenca[0] = pesoNoPesado - noAtual->getWeight();
                    menorDiferenca[1] = i;
                    atualizouMenor = true;
                    atualizouMaior = false;
                }
            } else if(noAtual->getWeight() > pesoNoPesado){
                if(noAtual->getWeight() - pesoNoLeve < menorDiferenca[0]){
                    menorDiferenca[0] = noAtual->getWeight() - pesoNoLeve;
                    menorDiferenca[1] = i;
                    atualizouMaior = true;
                    atualizouMenor = false;
                }
            } else {
                menorDiferenca[0] = 0;
                menorDiferenca[1] = i;
                semDiferenca = true;
                break;
            }
        }

        matrizCluster[menorDiferenca[1]][0].push_back(noAtual->getIdRotulo());
        if(!semDiferenca){
            if(atualizouMenor){
                matrizMenorMaiorCluster[menorDiferenca[1]][0] = noAtual->getWeight();
            } else{
                matrizMenorMaiorCluster[menorDiferenca[1]][1] = noAtual->getWeight();
            }
        }
        matrizMenorMaiorCluster[menorDiferenca[1]][2] = matrizMenorMaiorCluster[menorDiferenca[1]][1] - matrizMenorMaiorCluster[menorDiferenca[1]][0];

        cout << "Chegou aqui 4" << endl;
        listaDecrescrenteNosPorGrau.pop_front();
    }
    cout << "Chegou aqui 5" << endl;
    for(int i = 0; i < cluster; i++){
        cout << "Membros do Cluster " << i+1 << endl;
        for(auto it = matrizCluster[i][0].begin(); it != matrizCluster[i][0].end(); it++){
            cout << *it << " ";
        }
        cout << endl;
    }

    cout << "DIFERENCA DOS CLUSTERS: " << endl;
    for(int i = 0; i < cluster; i++){
        cout << "Diferenca do Cluster " << i+1 << " = " << matrizMenorMaiorCluster[i][2] << endl;
    }

}

void Graph::algoritmoGulosoRandomizado(int cluster) {
    cout << "Algoritmo Guloso Randomizado" << endl;
    cout << "CLUSTER: " << cluster << endl;
    list<int> listaCrescrenteNosPorPeso; //Lista que armazena os ID's ROTULO dos Vertices
    list<int> listaDecrescrenteNosPorGrau; //Lista que armazena os ID's ROTULO dos Vertices
    ordenarCrescentementeNosPorPeso(&listaCrescrenteNosPorPeso);
}

void Graph::algoritmoGulosoRandomizadoReativo(int cluster) {
    cout << "Algoritmo Guloso Randomizado Reativo" << endl;
    cout << "CLUSTER: " << cluster << endl;
    list<int> listaCrescrenteNosPorPeso; //Lista que armazena os ID's ROTULO dos Vertices
    list<int> listaDecrescrenteNosPorGrau; //Lista que armazena os ID's ROTULO dos Vertices
    ordenarCrescentementeNosPorPeso(&listaCrescrenteNosPorPeso);
}





