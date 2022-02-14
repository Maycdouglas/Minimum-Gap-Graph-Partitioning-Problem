#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <utility>
#include <tuple>
#include <iomanip>
#include <cstdlib>
#include <chrono>
#include "Graph.h"
#include "Node.h"

using namespace std;

void exportarGrafo(Graph* graph, ofstream& output_file) {

    string grafo, arestaDOT;

    if(graph->getDirected()) //verifica se o grafo é direcionado
    {
        grafo = "strict digraph G {\n";
        arestaDOT = " -> ";
    }
    else {
        grafo = "strict graph G {\n";
        arestaDOT = " -- ";
    }

    Node *noAtual = graph->getFirstNode();
    Edge *arestaAtual;

    while(noAtual != nullptr){
        arestaAtual = noAtual->getFirstEdge();
        while (arestaAtual != nullptr) {
            if(!arestaAtual->getRetorno()){
                grafo += "\t" + to_string(noAtual->getIdRotulo()) + arestaDOT + to_string(arestaAtual->getTargetIdRotulo());
                grafo += "\n";
            }
            arestaAtual = arestaAtual->getNextEdge();
        }
        noAtual = noAtual->getNextNode();
    }

    grafo += "}";

    output_file << grafo;

}

Graph* leituraInstancia(ifstream& input_file, int directed, int weightedEdge, int weightedNode){

    //Variaveis para auxiliar na criacao dos nos no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;
    //int numEdges;
    int pesoAresta;

    //Pegando a ordem do grafo
    input_file >> order;

    //Criando o grafo
    Graph* graph = new Graph(0, directed, weightedEdge, weightedNode);


    //Leitura de arquivo
    if(weightedEdge){
        while(input_file >> idNodeSource >> idNodeTarget >> pesoAresta) {
            graph->insertEdge(idNodeSource, idNodeTarget, pesoAresta);
        }
    } else{
        while(input_file >> idNodeSource >> idNodeTarget >> pesoAresta) {
            graph->insertEdge(idNodeSource, idNodeTarget, 0);
        }
    }


    return graph;
}

Graph* leituraInstancia2(ifstream& input_file, int directed, int weightedEdge, int weightedNode, int *cluster){

    //Variaveis para auxiliar na criacao dos nos no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;
    string descarte;
    string descarte2;
    int contador = 0;
    float nodeWeight;
    int numEdges;
    string aresta;
    string idOrigem;
    string idAlvo;
    int posicaoVirgula;
    int posicaoParanteses;

    //Criando o grafo
    Graph* graph = new Graph(0, directed, weightedEdge, weightedNode);

    while(contador < 10){
        if(contador == 3){
            input_file >> *cluster;
        } else {
            input_file >> descarte;
        }
        contador++;
    }

    contador = 0;

    while(contador < 6){
        if(contador == 1){
            input_file >> order;
        } else {
            input_file >> descarte;
        }
        contador++;
    }

    contador = 0;

    while(contador < order){
        input_file >> idNodeSource;
        contador++;
    }

    contador = 0;

    while(contador < 4) {
        input_file >> descarte;
        contador++;
    }

    contador = 0;

    while(contador < order){
        input_file >> idNodeSource >> nodeWeight;
        graph->insertNode(idNodeSource,nodeWeight);
        contador++;
    }

    contador = 0;

    while(contador < 7){
        if(contador == 2){
            input_file >> numEdges;
        } else {
            input_file >> descarte;
        }
        contador++;
    }

    contador = 0;

    while(contador < numEdges / 2){
        input_file >> aresta;
        posicaoParanteses = aresta.find(")");
        posicaoVirgula = aresta.find(",");

        idNodeSource = stoi(aresta.substr(1,posicaoVirgula - 1));
        idNodeTarget = stoi(aresta.substr(posicaoVirgula + 1,posicaoParanteses - posicaoVirgula - 1));

        graph->insertEdge(idNodeSource,idNodeTarget,0);

        contador++;
    }

    return graph;
}

int menu(){

    int selecao;

    cout << "MENU" << endl;
    cout << "----" << endl;
    cout << "[1] Gerar grafo" << endl;
    cout << "[2] Algoritmo Guloso" << endl;
    cout << "[3] Algoritmo Guloso Randomizado" << endl;
    cout << "[4] Algoritmo Guloso Randomizado Reativo" << endl;
    cout << "[0] Sair" << endl;

    cin >> selecao;

    return selecao;

}

string montarStringSaidaClusters(solucao solucaoAlgoritmo, int clusters) {
    string outputString;

    for (int i = 0; i < clusters; i++) {
        outputString += "Membros do Cluster " + to_string(i + 1) + "\n";

        list<int> cluster = solucaoAlgoritmo.clusters[i];
        for (auto it = cluster.begin(); it != cluster.end(); it++) {
            outputString += to_string(*it) + " ";
        }
        outputString += "\n";
    }

    outputString += "\n";

    return outputString;
}

string montarStringSaida(solucao solucaoAlgoritmo, int clusters, int numExecucao, int tempoDuracao) {
    string outputString = "Execucao: " + to_string(numExecucao);
    outputString += " - Gap: " + to_string(solucaoAlgoritmo.diferenca);
    outputString += " - Tempo de processamento: " + to_string(tempoDuracao) + " segundos\n\n";
    outputString += montarStringSaidaClusters(solucaoAlgoritmo, clusters);

    return outputString;
}

void selecionar(int selecao, Graph* graph, ofstream& output_file, int cluster){

    switch (selecao) {

        //Gerar Grafo
        case 1:{
            cout << "Caso 1" << endl;
            Node *noAtual = graph->getFirstNode();
            Edge *arestaAtual;
            while(noAtual != nullptr ){
                arestaAtual = noAtual->getFirstEdge();
                while(arestaAtual != nullptr){
                    arestaAtual = arestaAtual->getNextEdge();
                }
                noAtual = noAtual->getNextNode();
            }

            exportarGrafo(graph,output_file);
            break;
        }
        //Algoritmo Guloso
        case 2:{
            cout << "Caso 2" << endl;
            output_file << "Algoritmo Guloso\n";
            string outputString;

            solucao solucaoGuloso = graph->algoritmoGuloso(cluster);

            cout << "Gap: " << solucaoGuloso.diferenca << endl;
            outputString += "Gap: " + to_string(solucaoGuloso.diferenca) + "\n";

            outputString += montarStringSaidaClusters(solucaoGuloso, cluster);

            output_file << outputString;

            delete [] solucaoGuloso.clusters;

            break;
        }

        //Algoritmo Guloso Randomizado
        case 3:{
            cout << "Caso 3" << endl;
            output_file << "Algoritmo Guloso Randomizado\n";

            float alfas[3] = {0.1, 0.2, 0.3};

            for (int i = 0; i < 3; i++) {
                float alfa = alfas[i];
                
                string tituloAlfa = "ALFA: " + to_string(alfa);
                cout << tituloAlfa << endl;
                output_file << tituloAlfa + "\n";

                cout << "----------------------------------" << endl;

                for (int execucao = 1; execucao <= 30; execucao++) {
                    auto tempoInicial = chrono::high_resolution_clock::now();

                    solucao solucaoGulosoRandomizado = graph->algoritmoGulosoRandomizado(cluster, alfa, 1000);
                    
                    auto tempoFinal = chrono::high_resolution_clock::now();
                    auto duracaoEmSegundos = chrono::duration_cast<chrono::seconds>(tempoFinal - tempoInicial);
                    
                    string outputString = montarStringSaida(solucaoGulosoRandomizado, cluster, execucao, duracaoEmSegundos.count());
                    cout << outputString;
                    output_file << outputString;

                    delete [] solucaoGulosoRandomizado.clusters;
                }

                cout << "----------------------------------" << endl;
            }

            break;
        }

        //Algoritmo Guloso Randomizado Reativo
        case 4:{
            cout << "Caso 4" << endl;
            output_file << "Algoritmo Guloso Randomizado Reativo\n";

            float alfas[10] = {0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50};
            
            cout << "----------------------------------" << endl;

            for (int execucao = 1; execucao <= 30; execucao++) {
                auto tempoInicial = chrono::high_resolution_clock::now();

                solucao solucaoReativo = graph->algoritmoGulosoRandomizadoReativo(cluster, alfas, 4000, 250);
                
                auto tempoFinal = chrono::high_resolution_clock::now();
                auto duracaoEmSegundos = chrono::duration_cast<chrono::seconds>(tempoFinal - tempoInicial);
                
                string outputString = montarStringSaida(solucaoReativo, cluster, execucao, duracaoEmSegundos.count());

                cout << outputString;
                output_file << outputString;

                delete [] solucaoReativo.clusters;
            }

            cout << "----------------------------------" << endl;
            break;
        }

        default:
        {
            cout << " Error!!! invalid option!!" << endl;
        }

    }
}

int mainMenu(ofstream& output_file, Graph* graph, int cluster){

    int selecao = 1;

    while(selecao != 0){
        //system("clear");
        selecao = menu();

        if(selecao != 0){
            if(output_file.is_open())
                selecionar(selecao, graph, output_file, cluster);

            else
                cout << "Unable to open the output_file" << endl;
        }

        output_file << endl;

    }

    return 0;
}

int main(int argc, char const *argv[]) {

    //Verifica se todos os parâmetros do programa foram inseridos
    if (argc != 6) {

        cout << "ERROR: Expecting: ./<program_name> <input_file> <output_file> <directed> <weighted_edge> <weighted_node> " << endl;
        return 1;

    }

    string program_name(argv[0]);
    string input_file_name(argv[1]);

    //SIMPLESMENTE NÃO SEI O QUE ESTÁ ACONTECENDO NESSE BLOCO DE CÓDIGO
    string instance;
    if(input_file_name.find("v") <= input_file_name.size()){
        string instance = input_file_name.substr(input_file_name.find("v"));
        cout << "Running " << program_name << " with instance " << instance << " ... " << endl;
    }

    //Abrindo arquivo de entrada
    ifstream input_file;
    ofstream output_file;
    input_file.open(argv[1], ios::in);
    output_file.open(argv[2], ios::out | ios::trunc);

    Graph* graph;
    int cluster;

    if(input_file.is_open()){

        graph = leituraInstancia2(input_file, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), &cluster);

    }else
        cout << "Unable to open " << argv[1];


    mainMenu(output_file, graph, cluster);

    //Fechando arquivo de entrada
    input_file.close();

    //Fechando arquivo de saída
    output_file.close();



    return 0;
}
