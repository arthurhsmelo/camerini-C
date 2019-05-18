#include "DataHandlers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define earthRadiusKm 6371.0

double deg2rad(double deg) { return (deg * M_PI / 180); }

int distanceEarthKm(double lat1d, double lon1d, double lat2d, double lon2d) {
  double lat1r, lon1r, lat2r, lon2r, u, v;
  lat1r = deg2rad(lat1d);
  lon1r = deg2rad(lon1d);
  lat2r = deg2rad(lat2d);
  lon2r = deg2rad(lon2d);
  u = sin((lat2r - lat1r) / 2);
  v = sin((lon2r - lon1r) / 2);
  return (int)2.0 * earthRadiusKm *
         asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}

Edge *newEdge(int src, int dest, int weight) {
  Edge *edge = (Edge *)malloc(sizeof(Edge));
  edge->src = src;
  edge->dest = dest;
  edge->weight = weight;
  edge->next = NULL;
  return edge;
}

Graph *createGraph(int V, bool withIds, int *ids) {
  Graph *graph = (Graph *)malloc(sizeof(Graph));
  graph->V = V;

  graph->array = (AdjList *)malloc(V * sizeof(AdjList));

  // Inicializa as V listas de adjacência
  int i;
  for (i = 0; i < V; ++i) {
    graph->array[i].head = NULL;
    graph->array[i].visited = false;
    if (withIds) {
      graph->array[i].id = ids[i];
    } else {
      graph->array[i].id = i;
    }
  }

  return graph;
}

AdjList *findVertex(Graph *graph, int id) {
  int v;
  for (v = 0; v < graph->V; ++v) {
    if (graph->array[v].id == id) {
      return &graph->array[v];
    }
  }
}

void addEdge(Graph *graph, int src, int dest, int weight) {
  Edge *edge = newEdge(src, dest, weight);
  AdjList *vertexSrc = findVertex(graph, src);
  edge->next = vertexSrc->head;
  vertexSrc->head = edge;

  // Cria a aresta dest -> src, pois é não direcionado
  edge = newEdge(dest, src, weight);
  AdjList *vertexDest = findVertex(graph, dest);
  edge->next = vertexDest->head;
  vertexDest->head = edge;
}

void duplicateEdge(Graph *graph, Edge edge) {
  addEdge(graph, edge.src, edge.dest, edge.weight);
}

Edge *getEdges(Graph *graph, int *neg) {
  Edge *edges = malloc(sizeof(Edge));
  int vIndex;
  *neg = 0;
  for (vIndex = 0; vIndex < graph->V; ++vIndex) {
    Edge *edge = graph->array[vIndex].head;
    while (edge) {
      edges[*neg] = *edge;
      *neg += 1;
      edges = realloc(edges, (*neg + 1) * sizeof(Edge));
      edge = edge->next;
    }
  }
  return edges;
}

void printGraph(Graph *graph) {
  int v;
  for (v = 0; v < graph->V; ++v) {
    Edge *pCrawl = graph->array[v].head;
    printf("\n Graph Adjacency List \n%d ", graph->array[v].id);
    while (pCrawl) {
      printf(" - [%d -> %d, %d]", pCrawl->src, pCrawl->dest, pCrawl->weight);
      pCrawl = pCrawl->next;
    }
    printf("\n");
  }
}

Graph *getGraphFromFile(char *fileName) {
  FILE *uFile = fopen(fileName, "r");
  int numberOfCities, index, indexEdge;

  fscanf(uFile, "%d", &numberOfCities);

  Graph *graphTelescopes = createGraph(numberOfCities, false, NULL);

  // Preenche uma matriz com todas as lat/lon
  double cities[numberOfCities][2];
  for (index = 0; index < numberOfCities; index++) {
    fscanf(uFile, "%lf", &cities[index][0]);
    fscanf(uFile, "%lf", &cities[index][1]);
  }

  // Percorre as cidades, criando as arestas
  for (index = 0; index < numberOfCities; index++) {
    for (indexEdge = index + 1; indexEdge < numberOfCities; indexEdge++) {
      addEdge(graphTelescopes, index, indexEdge,
              distanceEarthKm(cities[index][0], cities[index][1],
                              cities[indexEdge][0], cities[indexEdge][1]));
    }
  }

  printGraph(graphTelescopes);

  fclose(uFile);
  return graphTelescopes;
}