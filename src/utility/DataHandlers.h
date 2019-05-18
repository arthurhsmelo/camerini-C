#ifndef DATA_HANDLERS_H
#define DATA_HANDLERS_H
#include "../models/Models.h"

// Graus pra radianos
double deg2rad(double deg);

// Calcula a distância entre dois pontos(lat, long)
int distanceEarthKm(double lat1d, double lon1d, double lat2d, double lon2d);

// Cria um nó da lista de adjacência
Edge *newListEdge(int src, int dest, int weight);

// Encontra vertice pelo id
AdjList *findVertex(Graph *graph, int id);

// Cria o grafo, com V nós
Graph *createGraph(int V, bool withIds, int *ids);

// Adiciona as arestas src -> dest / dest -> src com o peso weight
void addEdge(Graph *graph, int src, int dest, int weight);

// Adiciona aresta já existente em outro grafo
void duplicateEdge(Graph *graph, Edge edge);

// Retorna as arestas de um grafo
Edge *getEdges(Graph *graph, int *neg);

// Utility para printar o grafo
void printGraph(Graph *graph);

// Monta o grafo, lendo do arquivo e populando a estrutura
Graph *getGraphFromFile(char *fileName);

#endif