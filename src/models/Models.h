#ifndef MODELS_H
#define MODELS_H

/**
 * Boolean type
 */
typedef int bool;
#define true 1
#define false 0

typedef struct Edge Edge;

struct Edge {
  int src;
  int dest;
  int originalSrc;
  int originalDest;
  int weight;
  Edge *next;
};

typedef struct {
  int id;
  bool visited;
  Edge *head;
} AdjList;

typedef struct {
  int V;
  AdjList *array;
} Graph;

#endif