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

Graph* Graph::subgrafoVerticeInduzido(list<int> listaNosIdRotulo) {
    Graph *subgrafo = new Graph(0, this->directed, true, this->weighted_node); 

    // Percorre lista de rotulos do subgrafo
    for (auto it = listaNosIdRotulo.begin(); it != listaNosIdRotulo.end(); it++) {
        Node *noGrafoOriginal = getNodeByRotulo(*it);
        subgrafo->insertNode(noGrafoOriginal->getIdRotulo(), noGrafoOriginal->getWeight());  // Insere o nó no subgrafo
    }

    for (Node *noSubgrafo = subgrafo->getFirstNode(); noSubgrafo != nullptr; noSubgrafo = noSubgrafo->getNextNode()) {
        Node *noGrafoOriginal = getNodeByRotulo(noSubgrafo->getIdRotulo());

        // Percorre as arestas do nó 
        for (Edge *arestaAtual = noGrafoOriginal->getFirstEdge(); arestaAtual != nullptr; arestaAtual = arestaAtual->getNextEdge()) {
            if (contidoNaLista(arestaAtual->getTargetIdRotulo(), listaNosIdRotulo)) {
                int pesoAresta = 1;
                subgrafo->insertEdge(noSubgrafo->getIdRotulo(), arestaAtual->getTargetIdRotulo(), pesoAresta);  // Insere a aresta
            }
        }
    }

    return subgrafo;
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

    // cout << "Lista Crescente por Peso: " << endl;

    //Imprime a lista para verificarmos se está ordenando corretamente. NÃO ESTARÁ PRESENTE NO TRABALHO FINAL!
    // for(auto it = listaCrescrenteNosPorPeso->begin(); it!=listaCrescrenteNosPorPeso->end(); it++){
    //     cout << *it << " ";
    // }
    // cout << endl;
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

    // cout << "Lista Decrescente por grau: " << endl;

    //Imprime a lista para verificarmos se está ordenando corretamente. NÃO ESTARÁ PRESENTE NO TRABALHO FINAL!
    // for(auto it = listaDecrescrenteNosPorGrau->begin(); it != listaDecrescrenteNosPorGrau->end(); it++){
    //     cout << *it << " ";
    // }
    // cout << endl;
}

void Graph::inicializacaoClusters(int clusters, list<int> *listaCrescrenteNosPorPeso, list<int> *listaDecrescrenteNosPorGrau, list<int> matrizCluster[], int matrizMenorMaiorCluster[][3]) {
    float pontoCorte = float(this->order) / float(clusters);
    bool divisivel;

    if(this->order % clusters == 0){
        divisivel = true;
    } else {
        divisivel = false;
    }

    int contadorLista = 1;
    int contadorLinhaMatriz = 0;
    int contadorNaoDivisivel = 1;

    for(auto it = listaCrescrenteNosPorPeso->begin(); it != listaCrescrenteNosPorPeso->end(); it++){
        if(divisivel){
            if(contadorLista % int(pontoCorte) == 0){
                matrizCluster[contadorLinhaMatriz].push_back(*it);
                matrizMenorMaiorCluster[contadorLinhaMatriz][0] = getNodeByRotulo(*it)->getWeight();
                matrizMenorMaiorCluster[contadorLinhaMatriz][1] = getNodeByRotulo(*it)->getWeight();
                matrizMenorMaiorCluster[contadorLinhaMatriz][2] = matrizMenorMaiorCluster[contadorLinhaMatriz][1] - matrizMenorMaiorCluster[contadorLinhaMatriz][0];
                // cout << "Inseriu o numero " << *it << " no cluster " << contadorLinhaMatriz + 1 << endl;
                // cout << "Novo menor: " << matrizMenorMaiorCluster[contadorLinhaMatriz][0] << " Novo maior: " << matrizMenorMaiorCluster[contadorLinhaMatriz][1] << endl;
                listaDecrescrenteNosPorGrau->remove(*it);
                contadorLinhaMatriz++;
            }
        } else {
            if(contadorLista == int(round(pontoCorte * contadorNaoDivisivel))){
                matrizCluster[contadorLinhaMatriz].push_back(*it);
                matrizMenorMaiorCluster[contadorLinhaMatriz][0] = getNodeByRotulo(*it)->getWeight();
                matrizMenorMaiorCluster[contadorLinhaMatriz][1] = getNodeByRotulo(*it)->getWeight();
                matrizMenorMaiorCluster[contadorLinhaMatriz][2] = matrizMenorMaiorCluster[contadorLinhaMatriz][1] - matrizMenorMaiorCluster[contadorLinhaMatriz][0];
                // cout << "Inseriu o numero " << *it << " no cluster " << contadorLinhaMatriz + 1 << endl;
                // cout << "Novo menor: " << matrizMenorMaiorCluster[contadorLinhaMatriz][0] << " Novo maior: " << matrizMenorMaiorCluster[contadorLinhaMatriz][1] << endl;
                listaDecrescrenteNosPorGrau->remove(*it);
                contadorLinhaMatriz++;
                contadorNaoDivisivel++;
            }
        }
        contadorLista++;
    }
}

void Graph::atualizaSolucao(int clusters, Node *noAtual, list<int> matrizCluster[], int matrizMenorMaiorCluster[][3], stack<int> *pilhaReservaVertices) {
    int pesoNoLeve, pesoNoPesado;
    int menorDiferenca[2];
    bool semDiferenca, atualizouMenor, atualizouMaior, encontrouCluster, colocouNaPilha;

    menorDiferenca[0] = int(INFINITY);
    semDiferenca = false;
    atualizouMenor = false;
    atualizouMaior = false;
    encontrouCluster = false;
    colocouNaPilha = false;
    // cout << "CHEGOU AQUI 2" << endl;
    for(int i = 0; i < clusters; i++){
        pesoNoLeve = matrizMenorMaiorCluster[i][0];
        pesoNoPesado = matrizMenorMaiorCluster[i][1];
        // cout << "CHEGOU AQUI 3" << endl;
        if(noAtual->getWeight() < pesoNoLeve){
            //Condicional que garante a conexidade entre os elementos dos clusters
            if(pesoNoPesado - noAtual->getWeight() < menorDiferenca[0] && mantemConexidade(matrizCluster[i],noAtual)){

                //Condicional que garante que os clusters terão no minimo dois elementos
                if(menorDiferenca[0] == int(INFINITY) || matrizCluster[i].size() == 1 || matrizCluster[menorDiferenca[1]].size() > 1 ){
                    menorDiferenca[0] = pesoNoPesado - noAtual->getWeight();
                    menorDiferenca[1] = i;
                    atualizouMenor = true;
                    atualizouMaior = false;
                    encontrouCluster = true;
                    if(colocouNaPilha){
                        pilhaReservaVertices->pop();
                        colocouNaPilha = false;
                    }
                }

            }
                //Condicional que garante a conexidade entre os elementos dos clusters
        } else if(noAtual->getWeight() > pesoNoPesado && mantemConexidade(matrizCluster[i],noAtual)){
            if(noAtual->getWeight() - pesoNoLeve < menorDiferenca[0]){
                //Condicional que garante que os clusters terão no minimo dois elementos
                if(menorDiferenca[0] == int(INFINITY) || matrizCluster[i].size() == 1 || matrizCluster[menorDiferenca[1]].size() > 1 ) {
                    menorDiferenca[0] = noAtual->getWeight() - pesoNoLeve;
                    menorDiferenca[1] = i;
                    atualizouMaior = true;
                    atualizouMenor = false;
                    encontrouCluster = true;
                    if (colocouNaPilha) {
                        pilhaReservaVertices->pop();
                        colocouNaPilha = false;
                    }
                }
            }
                //Condicional que garante a conexidade entre os elementos dos clusters
        } else if(mantemConexidade(matrizCluster[i],noAtual)){
            //Condicional que garante que os clusters terão no minimo dois elementos
            if(menorDiferenca[0] == int(INFINITY) || matrizCluster[i].size() == 1 || matrizCluster[menorDiferenca[1]].size() > 1 ) {
                menorDiferenca[0] = 0;
                menorDiferenca[1] = i;
                semDiferenca = true;
                encontrouCluster = true;
                if (colocouNaPilha) {
                    pilhaReservaVertices->pop();
                    colocouNaPilha = false;
                }
                break;
            }

        } else if(!encontrouCluster){
            // cout << "Nao encontrou conexidade" << endl;
            if(!colocouNaPilha){
                colocouNaPilha = true;
                pilhaReservaVertices->push(noAtual->getIdRotulo());
            }
        }
    }
    // cout << "CHEGOU AQUI 4" << endl;
    if(encontrouCluster){
        // cout << "ENCONTROU UM CLUSTER PARA ELE ->  " << noAtual->getIdRotulo() << endl;
        matrizCluster[menorDiferenca[1]].push_back(noAtual->getIdRotulo());
        // cout << "O CLUSTER QUE ELE ENTROU FOI: " << menorDiferenca[1] + 1 << endl;
        if(!semDiferenca){
            if(atualizouMenor){
                matrizMenorMaiorCluster[menorDiferenca[1]][0] = noAtual->getWeight();
            } else{
                matrizMenorMaiorCluster[menorDiferenca[1]][1] = noAtual->getWeight();
            }
        }
        matrizMenorMaiorCluster[menorDiferenca[1]][2] = matrizMenorMaiorCluster[menorDiferenca[1]][1] - matrizMenorMaiorCluster[menorDiferenca[1]][0];
    }
}

// Precisa ser modificada para atualizar conforme um índice da lista de candidatos
void Graph::atualizaListaCandidatos(list<int> *listaCandidatos, int indice, stack<int> *pilhaReservaVertices) {
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

int Graph::algoritmoGulosoRandomizado(int clusters, float alfa, int numIter) {
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

    delete [] melhorSolucao;

    return menorDiferenca;
}

int Graph::algoritmoGuloso(int clusters) {
    return this->algoritmoGulosoRandomizado(clusters, 0, 1);
}

// void Graph::algoritmoGuloso(int cluster) {
//
//     cout << "Algoritmo Guloso" << endl;
//     cout << "CLUSTER: " << cluster << endl;
//     list<int> listaCrescrenteNosPorPeso; //Lista que armazena os ID's ROTULO dos Vertices
//     list<int> listaDecrescrenteNosPorGrau; //Lista que armazena os ID's ROTULO dos Vertices
//     ordenarCrescentementeNosPorPeso(&listaCrescrenteNosPorPeso);
//     ordenarDecrescentementeNosPorGrau(&listaDecrescrenteNosPorGrau);
//
//     list<int> matrizCluster[cluster]; //As linhas correspondem aos clusters. A primeira coluna é a lista de vertices do cluster, a segunda coluna é o menor peso e terceira coluna o maior peso
//     int matrizMenorMaiorCluster[cluster][3];
//     float pontoCorte = float(this->order) / float(cluster);
//
//     cout << "IMPRIMINDO OS ADJACENTES DE CADA VERTICE " << endl;
//     Node *noTesteOrigem = this->first_node;
//     Edge *arestaTeste;
//     while(noTesteOrigem != nullptr){
//         arestaTeste = noTesteOrigem->getFirstEdge();
//         cout << "VIZINHOS DO NOH " << noTesteOrigem->getIdRotulo() << endl;
//         while(arestaTeste != nullptr) {
//             cout << arestaTeste->getTargetIdRotulo() << " ";
//             arestaTeste = arestaTeste->getNextEdge();
//         }
//         cout << endl;
//         noTesteOrigem = noTesteOrigem->getNextNode();
//     }
//
//     cout << "Ponto de corte: " << pontoCorte << endl;
//     cout << "Ordem: " << this->order << endl;
//     cout << "Cluster: " << cluster << endl;
//
//     bool divisivel;
//
//     if(this->order % cluster == 0){
//         divisivel = true;
//     } else {
//         divisivel = false;
//     }
//
//     int contadorLista = 1;
//     int contadorLinhaMatriz = 0;
//     int contadorNaoDivisivel = 1;
//
//     for(auto it = listaCrescrenteNosPorPeso.begin(); it != listaCrescrenteNosPorPeso.end(); it++){
//         if(divisivel){
//             if(contadorLista % int(pontoCorte) == 0){
//                 matrizCluster[contadorLinhaMatriz].push_back(*it);
//                 matrizMenorMaiorCluster[contadorLinhaMatriz][0] = getNodeByRotulo(*it)->getWeight();
//                 matrizMenorMaiorCluster[contadorLinhaMatriz][1] = getNodeByRotulo(*it)->getWeight();
//                 matrizMenorMaiorCluster[contadorLinhaMatriz][2] = matrizMenorMaiorCluster[contadorLinhaMatriz][1] - matrizMenorMaiorCluster[contadorLinhaMatriz][0];
//                 cout << "Inseriu o numero " << *it << " no cluster " << contadorLinhaMatriz + 1 << endl;
//                 cout << "Novo menor: " << matrizMenorMaiorCluster[contadorLinhaMatriz][0] << " Novo maior: " << matrizMenorMaiorCluster[contadorLinhaMatriz][1] << endl;
//                 listaDecrescrenteNosPorGrau.remove(*it);
//                 contadorLinhaMatriz++;
//             }
//         } else {
//             if(contadorLista == int(round(pontoCorte * contadorNaoDivisivel))){
//                 matrizCluster[contadorLinhaMatriz].push_back(*it);
//                 matrizMenorMaiorCluster[contadorLinhaMatriz][0] = getNodeByRotulo(*it)->getWeight();
//                 matrizMenorMaiorCluster[contadorLinhaMatriz][1] = getNodeByRotulo(*it)->getWeight();
//                 matrizMenorMaiorCluster[contadorLinhaMatriz][2] = matrizMenorMaiorCluster[contadorLinhaMatriz][1] - matrizMenorMaiorCluster[contadorLinhaMatriz][0];
//                 cout << "Inseriu o numero " << *it << " no cluster " << contadorLinhaMatriz + 1 << endl;
//                 cout << "Novo menor: " << matrizMenorMaiorCluster[contadorLinhaMatriz][0] << " Novo maior: " << matrizMenorMaiorCluster[contadorLinhaMatriz][1] << endl;
//                 listaDecrescrenteNosPorGrau.remove(*it);
//                 contadorLinhaMatriz++;
//                 contadorNaoDivisivel++;
//             }
//         }
//         contadorLista++;
//     }
//
//     Node *noAtual;
//     int pesoNoLeve, pesoNoPesado;
//     int menorDiferenca[2];
//     bool semDiferenca, atualizouMenor, atualizouMaior, encontrouCluster, colocouNaPilha;
//     stack<int> pilhaVertices;
//
//     cout << "CHEGOU AQUI" << endl;
//
//     while(!listaDecrescrenteNosPorGrau.empty()){
//         cout << "LISTA ATUALMENTE: " << endl;
//         for(auto it = listaDecrescrenteNosPorGrau.begin(); it != listaDecrescrenteNosPorGrau.end(); it++){
//             cout << *it << " ";
//         }
//         cout << endl;
//
//         noAtual = getNodeByRotulo(listaDecrescrenteNosPorGrau.front());
//         menorDiferenca[0] = int(INFINITY);
//         semDiferenca = false;
//         atualizouMenor = false;
//         atualizouMaior = false;
//         encontrouCluster = false;
//         colocouNaPilha = false;
//         cout << "CHEGOU AQUI 2" << endl;
//         for(int i = 0; i < cluster; i++){
//             pesoNoLeve = matrizMenorMaiorCluster[i][0];
//             pesoNoPesado = matrizMenorMaiorCluster[i][1];
//             cout << "CHEGOU AQUI 3" << endl;
//             if(noAtual->getWeight() < pesoNoLeve){
//                 //Condicional que garante a conexidade entre os elementos dos clusters
//                 if(pesoNoPesado - noAtual->getWeight() < menorDiferenca[0] && mantemConexidade(matrizCluster[i],noAtual)){
//
//                     //Condicional que garante que os clusters terão no minimo dois elementos
//                     if(menorDiferenca[0] == int(INFINITY) || matrizCluster[i].size() == 1 || matrizCluster[menorDiferenca[1]].size() > 1 ){
//                         menorDiferenca[0] = pesoNoPesado - noAtual->getWeight();
//                         menorDiferenca[1] = i;
//                         atualizouMenor = true;
//                         atualizouMaior = false;
//                         encontrouCluster = true;
//                         if(colocouNaPilha){
//                             pilhaVertices.pop();
//                             colocouNaPilha = false;
//                         }
//                     }
//
//                 }
//                    //Condicional que garante a conexidade entre os elementos dos clusters
//             } else if(noAtual->getWeight() > pesoNoPesado && mantemConexidade(matrizCluster[i],noAtual)){
//                 if(noAtual->getWeight() - pesoNoLeve < menorDiferenca[0]){
//                     //Condicional que garante que os clusters terão no minimo dois elementos
//                     if(menorDiferenca[0] == int(INFINITY) || matrizCluster[i].size() == 1 || matrizCluster[menorDiferenca[1]].size() > 1 ) {
//                         menorDiferenca[0] = noAtual->getWeight() - pesoNoLeve;
//                         menorDiferenca[1] = i;
//                         atualizouMaior = true;
//                         atualizouMenor = false;
//                         encontrouCluster = true;
//                         if (colocouNaPilha) {
//                             pilhaVertices.pop();
//                             colocouNaPilha = false;
//                         }
//                     }
//                 }
//                    //Condicional que garante a conexidade entre os elementos dos clusters
//             } else if(mantemConexidade(matrizCluster[i],noAtual)){
//                 //Condicional que garante que os clusters terão no minimo dois elementos
//                 if(menorDiferenca[0] == int(INFINITY) || matrizCluster[i].size() == 1 || matrizCluster[menorDiferenca[1]].size() > 1 ) {
//                     menorDiferenca[0] = 0;
//                     menorDiferenca[1] = i;
//                     semDiferenca = true;
//                     encontrouCluster = true;
//                     if (colocouNaPilha) {
//                         pilhaVertices.pop();
//                         colocouNaPilha = false;
//                     }
//                     break;
//                 }
//
//             } else if(!encontrouCluster){
//                 cout << "Nao encontrou conexidade" << endl;
//                 if(!colocouNaPilha){
//                     colocouNaPilha = true;
//                     pilhaVertices.push(noAtual->getIdRotulo());
//                 }
//             }
//         }
//         cout << "CHEGOU AQUI 4" << endl;
//         if(encontrouCluster){
//             cout << "ENCONTROU UM CLUSTER PARA ELE ->  " << noAtual->getIdRotulo() << endl;
//             matrizCluster[menorDiferenca[1]].push_back(noAtual->getIdRotulo());
//             cout << "O CLUSTER QUE ELE ENTROU FOI: " << menorDiferenca[1] + 1 << endl;
//             if(!semDiferenca){
//                 if(atualizouMenor){
//                     matrizMenorMaiorCluster[menorDiferenca[1]][0] = noAtual->getWeight();
//                 } else{
//                     matrizMenorMaiorCluster[menorDiferenca[1]][1] = noAtual->getWeight();
//                 }
//             }
//             matrizMenorMaiorCluster[menorDiferenca[1]][2] = matrizMenorMaiorCluster[menorDiferenca[1]][1] - matrizMenorMaiorCluster[menorDiferenca[1]][0];
//         }
//
//         cout << "CHEGOU AQUI 5" << endl;
//
//         listaDecrescrenteNosPorGrau.pop_front();
//         cout << "LISTA ATUALMENTE2: " << endl;
//         for(auto it = listaDecrescrenteNosPorGrau.begin(); it != listaDecrescrenteNosPorGrau.end(); it++){
//             cout << *it << " ";
//         }
//         cout << endl;
//         if(listaDecrescrenteNosPorGrau.empty() && !pilhaVertices.empty()){
//             cout << "A lista acabou e a pilha nao esta vazia!" << endl;
//             while(!pilhaVertices.empty()){
//                 listaDecrescrenteNosPorGrau.push_front(pilhaVertices.top());
//                 cout << "LISTA ATUALMENTE3: " << endl;
//                 for(auto it = listaDecrescrenteNosPorGrau.begin(); it != listaDecrescrenteNosPorGrau.end(); it++){
//                     cout << *it << " ";
//                 }
//                 cout << endl;
//                 pilhaVertices.pop();
//             }
//         }
//     }
//
//     cout << "CHEGOU AQUI 6" << endl;
//
//     for(int i = 0; i < cluster; i++){
//         cout << "Membros do Cluster " << i+1 << endl;
//         for(auto it = matrizCluster[i].begin(); it != matrizCluster[i].end(); it++){
//             cout << *it << " ";
//         }
//         cout << endl;
//     }
//
//     int somaGaps = 0;
//     cout << "DIFERENCA DOS CLUSTERS: " << endl;
//     for(int i = 0; i < cluster; i++){
//         somaGaps += matrizMenorMaiorCluster[i][2];
//         cout << "Diferenca do Cluster " << i+1 << " = " << matrizMenorMaiorCluster[i][2] << endl;
//     }
//
//     cout << "RESULTADO (GAP): " << somaGaps << endl;
//
//     for (int i = 0; i < cluster; i++) {
//         list<int> clusterAtual = matrizCluster[i];
//         Graph *subgrafoCluster = subgrafoVerticeInduzido(clusterAtual);
//
//         // Exibindo lista de adjacência do cluster (NÃO SERÁ MANTIDO NO TRABALHO FINAL)
//         for (Node *noAtual = subgrafoCluster->getFirstNode(); noAtual != nullptr; noAtual = noAtual->getNextNode()) {
//             cout << noAtual->getIdRotulo() << " -> ";
//             for (Edge *arestaAtual = noAtual->getFirstEdge(); arestaAtual != nullptr; arestaAtual = arestaAtual->getNextEdge()) {
//                 cout << arestaAtual->getTargetIdRotulo() << " ";
//             }
//             cout << endl;
//         }
//
//         // Verificando conexidade do cluster
//         cout << "Cluster" << i + 1 << " - Conexo: ";
//         cout << boolalpha << subgrafoCluster->ehConexo() << endl;
//     }
// }

//Funcao que verifica se a inserção de um novo elemento no cluster manterá a conexidade do cluster
bool Graph::mantemConexidade(list<int> listaCluster,Node *noAtual) {
    Node *noAlvo;
    // cout << "=== ESTOU NA FUNCAO MANTEM CONEXIDADE "<< endl;
    // cout << "O noh atual eh: " << noAtual->getIdRotulo() << endl;
    for (auto it = listaCluster.begin(); it != listaCluster.end(); it++) {
        noAlvo = getNodeByRotulo(*it);
        // cout << "O noh alvo eh: " << noAlvo->getIdRotulo() << endl;
        if(noAtual->searchEdge(noAlvo->getId())){
            return true;
        }
    }
    return false;
}

// void Graph::algoritmoGulosoRandomizado(int cluster, float alfa, int numIter) {
//     cout << "Algoritmo Guloso Randomizado" << endl;
//     cout << "CLUSTER: " << cluster << endl;
//     list<int> listaCrescrenteNosPorPeso; //Lista que armazena os ID's ROTULO dos Vertices
//     list<int> listaDecrescrenteNosPorGrau; //Lista que armazena os ID's ROTULO dos Vertices
//     ordenarCrescentementeNosPorPeso(&listaCrescrenteNosPorPeso);
// }

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

int Graph::algoritmoGulosoRandomizadoReativo(int clusters, float alfas[], int numIter, int bloco) {
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

    delete [] melhorSolucao;

    return menorDiferenca;
}

bool Graph::ehConexo() {
    float **matrizVertices = this->floyd();

    // Verificando conexidade
    bool conexo = true;
    for (int linha = 0; linha < this->order; linha++) {
        for (int coluna = 0; coluna < this->order; coluna++) {
            if (matrizVertices[linha][coluna] == INFINITY) {
                conexo = false;
                break;
            }
        }

        if (conexo)
            break;
    }

    // Desalocando matriz
    for (int linha = 0; linha < this->order; linha++)
        delete [] matrizVertices[linha];  // Desaloca cada linha da matriz

    delete [] matrizVertices;  // Desaloca o vetor externo

    return conexo;
}

// void Graph::floyd(int idRotuloInicial, int idRotuloFinal, float **matrizVertices){
float** Graph::floyd() {
    // Alocando matriz de vértices
    float **matrizVertices = new float*[this->order];
    for (int j = 0; j < this->order; j++)
        matrizVertices[j] = new float[this->order];  // Alocando linhas da matriz

    // Node *noAtual, *noAlvo, *noInicial = getNodeByRotulo(idRotuloInicial), *noFinal = getNodeByRotulo(idRotuloFinal);
    Node *noAtual, *noAlvo;
    Edge *arestaAtual;
    // int linha, coluna, idInicial = noInicial->getId(), idFinal = noFinal->getId();
    int linha, coluna;
    list <int> caminho;
    list<int>::iterator it;

    //Clausula de segurança para grafos que não possuem peso nas arestas
    if(!this->getWeightedEdge()){
        cout << "O grafo precisa ter peso nas arestas!" << endl;
        return nullptr;
    }

    //Clausula de segurança para nós que não existem no grafo
    // if(noInicial == nullptr || noFinal == nullptr){
    //     cout << "O no escolhido nao esta presente no grafo!" << endl;
    //     return;
    // }

    //Clausula de segurança para nós que não possuem Out Degree
    // if(noInicial->getOutDegree() < 1){
    //     cout << "O no escolhido possui grau de saida igual a zero!" << endl;
    //     return;
    // }

    //Loop responsável por montar a matriz inicial
    for(linha = 0; linha < this->order; linha++) {
        noAtual = getNode(linha + 1);
        for(coluna = 0; coluna < this->order; coluna++) {
            noAlvo = getNode(coluna + 1);
            if(linha == coluna) {  //responsável por colocar o valor 0 na distância entre o vértice e ele mesmo
                matrizVertices[linha][coluna] = 0;
            } else if(noAtual->searchEdge(noAlvo->getId())){ //verifica se existe uma aresta entre os dois vertices
                arestaAtual = noAtual->hasEdgeBetween(noAlvo->getId());
                //Clausula de segurança para caso exista aresta com peso negativo
                if(arestaAtual->getWeight() < 0){
                    cout << "Algoritmo interrompido por ter aresta com peso negativo!" << endl;
                    return nullptr;
                }

                matrizVertices[linha][coluna] = arestaAtual->getWeight(); //insere a distancia entre os dois vertices na matriz

                //caso exista aresta entre o nó inicial e o nó final, a lista caminho é atualizada
                // if(linha == idInicial - 1 && coluna == idFinal - 1){
                //     caminho.push_front(idInicial);
                //     caminho.push_back(idFinal);
                // }
            } else {
                matrizVertices[linha][coluna] = INFINITY; //quando não houver arestas entre os vértices, o valor INFINITO é usado
            }
        }
    }

    //Loop responsável por atualizar a matriz com os caminhos obtidos
    for(int k = 0; k < this->order; k++) {
        for(linha = 0; linha < this->order; linha++) {
            noAtual = getNode(linha + 1);
            for(coluna = 0; coluna < this->order; coluna++) {
                noAlvo = getNode(coluna + 1);
                //verifica se os indices da matriz são diferentes entre si para poderem ser atualizados se possível
                if(linha != k && coluna != k && linha != coluna){
                    //verifica se o vértice não está numa linha ou coluna que é impossível sofrer alterações no momento
                    if(matrizVertices[linha][k] != INFINITY && matrizVertices[k][coluna] != INFINITY){
                        //verifica se o caminho atual na matriz é maior que o novo caminho proposto
                        if(matrizVertices[linha][coluna] > matrizVertices[linha][k] + matrizVertices[k][coluna]){
                            matrizVertices[linha][coluna] = matrizVertices[linha][k] + matrizVertices[k][coluna];
                            //verifica se atualizou o caminho entre os nós escolhidos pelo usuario
                            // if(linha == idInicial - 1 && coluna == idFinal - 1){
                            //     //se o caminho estiver vazio, realiza a primeira inserção
                            //     if(caminho.empty()){
                            //         caminho.push_front(k + 1);
                            //         caminho.push_back(idFinal);
                            //     } else{
                            //         it = caminho.end();
                            //         it--;
                            //         caminho.insert(it,k + 1); //adiciona o novo vertice no caminho na penultima posição
                            //     }
                            // }
                        }
                    }
                }
            }
        }
    }

    return matrizVertices;
}