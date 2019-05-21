#include "./models/Models.h"
#include "./utility/DataHandlers.h"
#include <stdio.h>
#include <stdlib.h>

void addIfNotExists(int **array, int *size, int value) {
  int i;
  bool exists = false;
  for (i = 0; i < *size; i++) {
    if ((*array)[i] == value)
      exists = true;
  }
  if (!exists) {
    (*array)[*size] = value;
    *size += 1;
    *array = realloc(*array, (*size + 1) * sizeof(int));
  }
}

int compareEdges(const void *a, const void *b) {
  const Edge *edgeA = (const Edge *)a;
  const Edge *edgeB = (const Edge *)b;
  return (edgeA->weight - edgeB->weight);
}

int compareNumbers(const void *a, const void *b) {
  return (*(int *)a - *(int *)b);
}

void partitionEdges(Edge *edges, int neg, Edge **A, Edge **B, int *nva, int *nea, int *nvb, int *neb) {
  int i, median, *va = malloc(sizeof(int)), *vb = malloc(sizeof(int));
  *A = malloc(sizeof(Edge));
  *B = malloc(sizeof(Edge));
  *nva = *nea = *nvb = *neb = 0;

  // Ordena as arestas
  qsort(edges, neg, sizeof(Edge), compareEdges);

  // Pega a mediana
  if (neg % 2 != 0) {
    median = edges[neg / 2].weight;
  } else {
    median = (edges[(neg / 2)].weight + edges[(neg / 2) - 1].weight) / 2;
  }

  for (i = 0; i < neg; i++) {
    Edge edge = edges[i];
    if (edge.weight <= median) {
      (*B)[*neb] = edge;
      *neb += 1;
      addIfNotExists(&vb, nvb, edge.src);
      *B = realloc(*B, (*neb + 1) * sizeof(Edge));
    } else {
      (*A)[*nea] = edge;
      *nea += 1;
      addIfNotExists(&va, nva, edge.src);
      *A = realloc(*A, (*nea + 1) * sizeof(Edge));
    }
  }
}

void DFS(Graph *G, int startingV, int *numberOfVertex, AdjList **vertexConnected) {
  int i;
  AdjList *vertex = findVertex(G, startingV);
  // Mark the current node as visited
  vertex->visited = true;
  (*vertexConnected)[*numberOfVertex] = *vertex;
  *numberOfVertex += 1;
  *vertexConnected = realloc(*vertexConnected, (*numberOfVertex + 1) * sizeof(AdjList));

  // Recur for all the vertices adjacent
  // to this vertex
  Edge *edge = vertex->head;
  while (edge) {
    AdjList *vertexDest = findVertex(G, edge->dest);
    if (!vertexDest->visited)
      DFS(G, edge->dest, numberOfVertex, vertexConnected);
    edge = edge->next;
  }
  return;
}

Graph *getConnectedComponents(Edge *B, int *nnc, int neb, int nvb) {
  int i, *vb, auxNv = 0;
  *nnc = 0;
  int *vertexIds = malloc(sizeof(int)), numberOfVertex = 0;

  for (i = 0; i < neb; i++) {
    Edge edge = B[i];
    addIfNotExists(&vertexIds, &numberOfVertex, edge.src);
  }

  // Ordena os ids
  qsort(vertexIds, numberOfVertex, sizeof(int), compareNumbers);

  Graph *GB = createGraph(nvb, true, vertexIds);
  Graph *F = malloc(sizeof(Graph));

  for (i = 0; i < neb; i++) {
    Edge edge = B[i];
    duplicateEdge(GB, edge);
  }

  for (i = 0; i < GB->V; i++) {
    if (!GB->array[i].visited) {
      auxNv = 0;
      AdjList *arrayOfVertex = malloc(sizeof(AdjList));
      DFS(GB, GB->array[i].id, &auxNv, &arrayOfVertex);
      F[*nnc] = *createGraph(auxNv, false, NULL);
      F[*nnc].array = arrayOfVertex;
      *nnc += 1;
      F = realloc(F, (*nnc + 1) * sizeof(Graph));
    }
  }
  return F;
}

bool found(int value, int *array, int size) {
  int i;
  bool exists = false;
  for (i = 0; i < size; i++) {
    if (array[i] == value)
      exists = true;
  }
  return exists;
}

Graph *populateNewGraph(Edge *A, int nva, int nea, Edge *B, int nvb, int neb, int nnc, Graph *F) {
  int index, j,
      countVertexGraph = 0,
      *vertexInA = malloc(sizeof(int)),
      sizeVertexInA = 0,
      sizeByComponent[nnc];

  int **vertexByComponent = (int **)malloc(nnc * sizeof(int *));
  for (index = 0; index < nnc; index++)
    vertexByComponent[index] = malloc(sizeof(int));

  for (index = 0; index < nnc; index++) {
    sizeByComponent[index] = 0;
    countVertexGraph++;
    for (j = 0; j < F[index].V; j++) {
      addIfNotExists(&vertexByComponent[index], &(sizeByComponent)[index], F[index].array[j].id);
    }
  }

  for (index = 0; index < nea; index++) {
    for (j = 0; j < nnc; j++) {
      if (!found(A[index].src, vertexByComponent[j], sizeByComponent[j])) {
        addIfNotExists(&vertexInA, &sizeVertexInA, A[index].src);
      } else {
        A[index].src = j - nnc;
      }

      if (found(A[index].dest, vertexByComponent[j], sizeByComponent[j])) {
        A[index].dest = j - nnc;
      }
    }
  }
  countVertexGraph += sizeVertexInA;

  int ids[countVertexGraph];
  for (index = 0; index < countVertexGraph; index++) {
    if (index < nnc) {
      ids[index] = index - nnc;
    } else {
      ids[index] = vertexInA[index - nnc];
    }
  }

  Graph *newGraph = createGraph(countVertexGraph, true, ids);
  for (index = 0; index < nea; index++) {
    duplicateEdge(newGraph, A[index]);
  }
  return newGraph;
}

void MBST(Graph *G, Edge *edges, int neg, Graph *mbst) {
  // ncc -> Número de componentes conectados
  // nva -> Número de vértices em A
  // nvb -> Número de vértices em B
  // nea -> Número de arestas em A
  // neb -> Número de arestas em B
  int nnc, nva, nea, nvb, neb, auxIndex;
  // A -> Arestas maiores que a mediana dos pesos
  // B -> Arestas menores ou iguais a mediana dos pesos
  Edge *A, *B;
  // F -> Vetor de Componentes conectados em B
  // NG -> Merge de A e F
  Graph *F, *NG;
  if (neg == 2) {
    addEdge(mbst, edges[0].originalSrc, edges[0].originalDest, edges[0].weight, true, true, 0, 0);
    return;
  } else {
    partitionEdges(edges, neg, &A, &B, &nva, &nea, &nvb, &neb);
    F = getConnectedComponents(B, &nnc, neb, nvb);
    // Se B só tem um componente conectado
    // e este possui todos os vértices de G (is spanning tree)
    if (nnc == 1 && F[0].V == G->V) {
      MBST(G, B, neb, mbst);
    } else {
      // Cria o novo grafo, com os vértices de A + 1 vértice
      // para cada componente conectado de B
      NG = populateNewGraph(A, nva, nea, B, nvb, neb, nnc, F);
      // Armazena as arestas B na MBST
      for (auxIndex = 0; auxIndex < neb; auxIndex++) {
        addEdge(mbst, B[auxIndex].originalSrc, B[auxIndex].originalDest, B[auxIndex].weight, false, true, 0, 0);
      }
      edges = getEdges(NG, &neg);
      MBST(NG, edges, neg, mbst);
    }
  }
}

void startTP2(char *citiesFile) {
  // Monta o grafo
  Graph *graphTelescopes = getGraphFromFile(citiesFile);

  // Busca a Minimum Bottleneck Spanning Tree
  Graph *mbst = createGraph(graphTelescopes->V, false, NULL);
  int numberOfEdges, i, maxWeight = -1;
  Edge *edges = getEdges(graphTelescopes, &numberOfEdges);
  MBST(graphTelescopes, edges, numberOfEdges, mbst);
  edges = getEdges(mbst, &numberOfEdges);

  for (i = 0; i < numberOfEdges; i++) {
    if (edges[i].weight > maxWeight)
      maxWeight = edges[i].weight;
  }

  printf("%d", maxWeight);
}