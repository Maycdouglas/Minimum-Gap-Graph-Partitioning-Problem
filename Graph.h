
#ifndef MGGPP_GRAPH_H
#define MGGPP_GRAPH_H
#include "Node.h"
#include "Edge.h" // Include of the Edge class
#include <fstream>
#include <stack>
#include <list>
#include <queue>

using namespace std;

class Graph{

    //Atributes
private:
    int order;
    int number_edges;
    bool directed;
    bool weighted_edge;
    bool weighted_node;
    Node* first_node; //lista de nós do grafo
    Node* last_node;

public:
    //Constructor
    Graph(int order, bool directed, bool weighted_edge, bool weighted_node);
    //Destructor
    ~Graph();
    //Getters
    int getOrder();
    int getNumberEdges();
    bool getDirected();
    bool getWeightedEdge();
    bool getWeightedNode();
    Node* getFirstNode();
    Node* getLastNode();
    //Other methods
    void insertNode(int id_rotulo, float peso);
    void insertEdge(int id_rotulo, int target_id_rotulo, float weight);
    void removeNode(int id);
    bool searchNode(int id);
    Node* getNode(int id);
    Node* getNodeByRotulo(int id);

    //Nossos Metodos
    void montarCabecalhoGrafoDOT(string *grafo, string *arestaDOT);
    void montarArestaGrafoDOT(string *grafo, string *arestaDOT, int idRotuloNoAtual, int idRotuloNoAlvo, float pesoAresta, bool retorno);
    void ordenarCrescentementeNosPorPeso(list<int> *listaCrescrenteNosPorPeso);
    void ordenarDecrescentementeNosPorGrau(list<int> *listaDecrescrenteNosPorGrau);
    void inicializacaoClusters(int clusters, list<int> *listaCrescrenteNosPorPeso, list<int> *listaDecrescrenteNosPorGrau, list<int> matrizCluster[], int matrizMenorMaiorCluster[][3]);
    void atualizaSolucao(int clusters, Node *noAtual, list<int> matrizCluster[], int matrizMenorMaiorCluster[][3]);
    void atualizaListaCandidatos(list<int> *listaCandidatos, int indice);
    void algoritmoGuloso(int cluster);
    void algoritmoGuloso2(int clusters, float alfa, int numIter);
    void algoritmoGulosoRandomizado(int cluster, float alfa, int numIter);
    void algoritmoGulosoRandomizadoReativo(int cluster);
    float** floyd();

private:
    //Auxiliar methods
    Graph* subgrafoVerticeInduzido(list<int> listaNosIdRotulo);
    bool contidoNaLista(int elemento, list<int> lista);
    bool ehConexo();
};
#endif //MGGPP_GRAPH_H
