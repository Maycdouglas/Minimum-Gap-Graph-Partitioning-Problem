
#ifndef MGGPP_GRAPH_H
#define MGGPP_GRAPH_H
#include "Node.h"
#include "Edge.h" // Include of the Edge class
#include <fstream>
#include <stack>
#include <list>
#include <queue>

using namespace std;

struct solucao
{
    int diferenca;
    list<int> *clusters;
};

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
    void atualizaSolucao(int clusters, Node *noAtual, list<int> matrizCluster[], int matrizMenorMaiorCluster[][3], stack<int> *pilhaReservaVertices);
    void atualizaListaCandidatos(list<int> *listaCandidatos, int indice, stack<int> *pilhaReservaVertices);
    solucao algoritmoGuloso(int clusters);
    bool mantemConexidade(list<int> listaCluster,Node *noAtual);
    solucao algoritmoGulosoRandomizado(int clusters, float alfa, int numIter);
    solucao algoritmoGulosoRandomizadoReativo(int clusters, float alfas[], int numIter, int bloco);
    float** floyd();

private:
    //Auxiliar methods
    Graph* subgrafoVerticeInduzido(list<int> listaNosIdRotulo);
    bool contidoNaLista(int elemento, list<int> lista);
    int getElementoLista(list<int> lista, int indice);
    void removeElementoLista(list<int> *lista, int indice);
    bool ehConexo();
    void inicializaVetoresReativo(float probabilidades[], float medias[], int numElementos);
    void atualizaProbabilidadesReativo(float probabilidades[], float medias[], int menorDiferenca, int numElementos);
    float escolheAlfaReativo(float probabilidades[], float alfas[], int numElementos);
    void atualizarMediasReativo(float medias[], int acumuladorDiferencas[], int contadorDiferencasPorAlfa[], int diferenca, float alfas[], float alfa, int numElemetos);
};
#endif //MGGPP_GRAPH_H
