#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#define MAX_LINE 305

typedef struct{
  int n, m;
  int *d, *best;
  int ***adj;
}GRAFO;

void freeGRAFO(GRAFO *g){
  int j, i = -1;
  while(++i < g->n){
    j = -1;
    while(++j < g->d[i]){
      free(g->adj[i][j]);
    }
    free(g->adj[i]);
  }
  free(g->adj);
  free(g->d);
  free(g->best);
  free(g);
}

void merge(int **v, int comeco, int meio, int fim, int **arrAux){
  int i = comeco, j = meio + 1, k = comeco;
  while(i <= meio && j <= fim){
    if(v[i][1] < v[j][1]){
      arrAux[k++] = v[i++];
    }else{
      arrAux[k++] = v[j++];
    }
  }
  while(i <= meio)
    arrAux[k++] = v[i++];
  while(j <= fim)
    arrAux[k++] = v[j++];
  for(i = comeco; i <= fim; i++)
    v[i] = arrAux[i];
}

void mergeSort(int **v, int comeco, int fim, int **arrAux){
  if(comeco < fim){
    int meio = (fim+comeco)/2;
    mergeSort(v, comeco, meio, arrAux);
    mergeSort(v, meio+1, fim, arrAux);
    merge(v, comeco, meio, fim, arrAux);
  }
}

GRAFO* readGRAFO(FILE *inFile){
  GRAFO *g = (GRAFO*) malloc(sizeof(GRAFO));
  int **arestas, **arrAux;
  char line[MAX_LINE];
  int u, v, j, i = -1;
  if(inFile != NULL)
    fscanf(inFile, "%d %d", &(g->n), &(g->m));
  else
    scanf("%d %d", &(g->n), &(g->m));
  arestas = (int**) malloc(g->m * sizeof(int*));
  arrAux = (int**) malloc((g->n - 1) * sizeof(int*));
  g->adj = (int***) malloc(g->n * sizeof(int**));
  g->d = (int*) malloc(g->n * sizeof(int));
  g->best = (int*) malloc(g->n * sizeof(int));
  i = -1;
  while(++i < g->n){
    g->d[i] = 0;
  }
  i = -1;
  while(++i < g->m){
    arestas[i] = (int*) malloc(3 * sizeof(int));
    if(inFile != NULL)
      fscanf(inFile, "\n%[^\n]", line);
    else
      scanf("\n%[^\n]", line);
    if(sscanf(line, "%d %d %d", &arestas[i][0], &arestas[i][1], &arestas[i][2]) == 2)
      arestas[i][2] = 1;
    u = --arestas[i][0];
    v = --arestas[i][1];
		g->d[u]++;
    g->d[v]++;
  }
  if(inFile != NULL)
  	fclose(inFile);
  i = -1;
  while(++i < g->n){
    g->adj[i] = (int**) malloc(g->d[i] * sizeof(int*));
    j = -1;
    while(++j < g->d[i]){
      g->adj[i][j] = (int*) malloc(2 * sizeof(int));
    }
    g->d[i] = 0;
  }
  i = -1;
  while(++i < g->m){
    u = arestas[i][0];
    v = arestas[i][1];
    j = (g->d[u])++;
    g->adj[u][j][0] = v;
    g->adj[u][j][1] = arestas[i][2];
    j = (g->d[v])++;
    g->adj[v][j][0] = u;
    g->adj[v][j][1] = arestas[i][2];
    free(arestas[i]);
  }
  free(arestas);
  i = -1;
  while(++i < g->n){
    mergeSort(g->adj[i], 0, g->d[i] - 1, arrAux);
  }
  free(arrAux);
  return g;
}

void halmDFS(GRAFO *g, int **best, int v, int *ham, int hamParc2, int **con, int cam){
  int aux, w, u = v, i = -1;
  if(cam < g->n){
    while(++i < g->d[u]){
      v = g->adj[u][i][0];
      if(con[v][0] == -1){
        con[v][0] = u;
        con[v][1] = g->adj[u][i][1];
        con[u][2] = con[u][1] + con[v][1];
        hamParc2 -= g->best[v];
        if(u == g->adj[v][0][0]){
          con[v][2] = g->best[v];
        }else{
          con[v][2] = g->adj[v][0][1] + con[v][1];
        }
        if((hamParc2 + con[u][2] + con[v][2]) / 2 <= *ham){
          halmDFS(g, best, v, ham, hamParc2 + con[u][2], con, cam + 1);
        }
        con[v][0] = -1;
        hamParc2 += g->best[v];
      }
    }
  }else{
    while(++i < g->d[u]){
      v = g->adj[u][i][0];
      if(v == 0){
        w = g->adj[u][i][1];
        aux = con[0][1] + w;
        con[u][2] = con[u][1] + w;
        hamParc2 += con[u][2] + aux;
        if(hamParc2 / 2 < *ham){
          *ham = hamParc2 / 2;
          best[0][0] = u;
          best[0][1] = w;
          u = con[u][0];
          i = 0;
          while(++i < g->n){
            best[i][0] = con[i][0];
            best[i][1] = con[i][1];

          }
        }
      }
    }
  }
}

int hamiltonian(GRAFO *g, int **best){
  int i, w, v = -1, ham = INT_MAX, hamParc2 = 0, is_pos = 1;
  int **con = (int**) malloc(g->n * sizeof(int*));
  while(++v < g->n){
    con[v] = (int*) malloc(3 * sizeof(int));
    con[v][0] = -1;
    if(g->d[v] < 2){
      is_pos = 0;
      break;
    }
    g->best[v] = g->adj[v][0][1] + g->adj[v][1][1];
    hamParc2 += g->best[v];
  }
  if(!is_pos){
    return -1;
  }
  hamParc2 -= g->best[0];
  i = -1;
  while(++i < g->d[0]){
    v = g->adj[0][i][0];
    w = g->adj[0][i][1];
    con[0][0] = v;
    con[0][1] = w;
    if(i == 0){
      con[0][2] = g->best[0];
    }else{
      con[0][2] = g->adj[0][0][1] + w;
    }
    con[v][0] = 0;
    con[v][1] = w;
    hamParc2 -= g->best[v];
    if(0 == g->adj[v][0][0]){
      con[v][2] = g->best[v];
    }else{
      con[v][2] = g->adj[v][0][1] + con[v][1];
    }
    if((hamParc2 + con[0][2] + con[v][2]) / 2 <= ham){
      halmDFS(g, best, v, &ham, hamParc2 + con[0][2], con, 2);
    }
    hamParc2 += g->best[v];
  }
  i = -1;
  while(++i < g->n){
    free(con[i]);
  }
  free(con);
  return ham;
}

int readArgs(int argc, char *argv[], FILE **inFile, FILE **outFile, int *show){
  int i = 0;
  *show = 0;
  *outFile = NULL;
  *inFile = NULL;
  while(++i < argc){
    if(argv[i][0] != '-'){
      printf("Argumento não reconhecido, use o comando -h para ajudar.\n");
      return 0;
    }
    switch(argv[i][1]){
      case 'h':
        printf("Este programa tem o objetivo de calcular o \
caminho hamiltoneano mínimo de um grafo \
não direcionado com vertices começando de 1 até n.\n\
Não é definido vértices iniciais nem finais. Se o argumento s for \
passado, ele mostrará também todas as arestas do caminho.\n\n\
Argumentos do programa:\n\
-h : mostra o help (Quando h é selecionado, invalida outros argumentos.)\n\
-o <arquivo> : redireciona a saida para o \"arquivo\"\n\
-f <arquivo> : indica o \"arquivo\" que contém o grafo de entrada\n\
-s : mostra arestas da arvore (caminho partindo de 1 e retornando)\n\
-i : vértice inicial (não hablitado)\n\
-l : vértice final (não hablitado)\n");
        return 0;
      case 'o':
        *outFile = fopen(argv[++i], "w");
        break;
      case 'f':
        *inFile = fopen(argv[++i], "r");
        if(*inFile == NULL){
          printf("arquivo de entrada não existe.\n");
          return 0;
        }
        break;
      case 's':
        *show = 1;
        break;
      default:
        printf("Comando não reconhecido, use o comando -h para ajudar.\n");
        return 0;
    }
  }
  return 1;
}

int main(int argc, char *argv[]) {
  int **best, tot, u, v, i = -1, show;
  FILE *outFile, *inFile;
  if(!readArgs(argc, argv, &inFile, &outFile, &show)){
    return 0;
  }
  GRAFO *g = readGRAFO(inFile);
  i = -1;
  best = (int**) malloc(g->n * sizeof(int*));
  while(++i < g->n){
    best[i] = (int*) malloc(2 * sizeof(int));
  }
  tot = hamiltonian(g, best);
  if(outFile != NULL){
    fprintf(outFile, "Hamiltoneano: %d\n", tot);
  }else{
    printf("Hamiltoneano: %d\n", tot);
  }
  if(show){
    u = 0;
    v = best[u][0];
    if(outFile != NULL){
      fprintf(outFile, "(%d,%d) %d\n", u + 1, v + 1, best[u][1]);
    }else{
      printf("(%d,%d) %d\n", u + 1, v + 1, best[u][1]);
    }
    while(v != 0){
      u = v;
      v = best[u][0];
      if(outFile != NULL){
        fprintf(outFile, "(%d,%d) %d\n", u + 1, v + 1, best[u][1]);
      }else{
        printf("(%d,%d) %d\n", u + 1, v + 1, best[u][1]);
      }
    } 
    if(outFile != NULL){
      fclose(outFile);
    }
  }
  i = -1;
  while(++i < g->n){
    free(best[i]);
  }
  freeGRAFO(g);
  free(best);
  return 0;
}
