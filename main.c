#include "./src/tp2.h"
#include <stdio.h>

// make run citiesFile="./entryFiles/cidades1.txt"

int main(int argc, char *argv[]) {
  char *citiesFile;

  // argc != 5 -> debug VSCODE
  if (argc != 2 && argc != 5) {
    printf("Por favor insira o argumento, no formato \"./main "
           "{PATH_TO_FOLDER}/cidades.txt\"\n");
    printf("OBS: Se estiver usando make run, passe os argumentos no "
           "formato:\"make run citiesFile=\"{PATH_TO_FOLDER}/cidades.txt\"\n");
  } else {
    citiesFile = argv[1];
    startTP2(citiesFile);
  }
  return 0;
}