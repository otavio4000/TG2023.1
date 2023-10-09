#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#define MAX_LINE 305

typedef struct{
  int n, m;
  int *d;
  int **adj;
  int **w;
}GRAFO;

void freeGRAFO(GRAFO *g){
  int i = -1;
  while(++i < g->n){
    free(g->adj[i]);
    free(g->w[i]);
  }
  free(g->d);
  free(g->adj);
  free(g->w);
  free(g);
}

typedef struct{
  int *heap;
  int *pos;
  int n;
} HEAP;

void freeHEAP(HEAP *h){
  free(h->heap);
  free(h->pos);
  free(h);
}

void swap(HEAP* h, int a, int b){
  int temp = h->heap[a];
  h->heap[a] = h->heap[b];
  h->heap[b] = temp;
  temp = h->pos[h->heap[a]];
  h->pos[h->heap[a]] = h->pos[h->heap[b]];
  h->pos[h->heap[b]] = temp;
}
/**
** como a heap se inicia com todas as prioridades infinitas,
** basta colocar o vertice inicial na frente, tambem foi criado
** um vetor pos que vai mostrar qual a posição do vertice i na heap
** isso é util pois se a posião for maior que n, o vétice ja saiu da 
** heap e ja foi percorrido, logo não será atualizado.
**/
HEAP *inicHEAP(int n, int first){
  int i = -1, temp = first;
  HEAP *h = (HEAP*) malloc(sizeof(HEAP));
  h->n = n;
  h->pos = (int*) malloc(n * sizeof(int));
  h->heap = (int*) malloc(n * sizeof(int));
  while(++i < n){
    h->pos[i] = i;
    h->heap[i] = i;
  }
  swap(h, 0, first);
  return h;
}

/**
** retorna se a heap não está vazia, se n = 0, retorna 0 (FALSE)
**/
int notEmpty(HEAP* h){
  return h->n;
}

/**
** retorna se a heap não está vazia
**/
int inHeap(HEAP* h, int i){
  return h->pos[i] < h->n;
}

/**
** retorna vertice com menor prioridade e faz o heapfy
**/
int pop(HEAP* h, int *prio){
  int filhoD, filho = 1, index = 0;
  swap(h, 0, --(h->n));
  while(filho < h->n){
    filhoD = filho|1;
    if(filhoD < h->n && prio[h->heap[filhoD]] < prio[h->heap[filho]])
      filho = filhoD;
    if(prio[h->heap[index]] <= prio[h->heap[filho]])
      break;
    swap(h, index, filho);
    index = filho;
    filho = (index<<1);
  }
  return h->heap[h->n];
}

/**
** Atualiza a prioridade de index, como a prioridade do vértice
** só diminui, então o vértice só pode subrir na heap, nunca descer
**/
void update(HEAP* h, int p, int *prio){
  int index = h->pos[p], pai = index>>1;
  while(index && prio[h->heap[index]] < prio[h->heap[pai]]){
    swap(h, index, pai);
    index = pai;
    pai = index>>1;;
  }
}

GRAFO* readGRAFO(FILE *inFile){
  GRAFO *g = (GRAFO*) malloc(sizeof(GRAFO));
  char line[MAX_LINE];
  int i = -1, j;
  int **listAdj;
  if(inFile != NULL)
    fscanf(inFile, "%d %d", &(g->n), &(g->m));
  else
    scanf("%d %d", &(g->n), &(g->m));
  listAdj = (int**) malloc(g->m * sizeof(int*));
  g->adj = (int**) malloc(g->n * sizeof(int*));
  g->w = (int**) malloc(g->n * sizeof(int*));
  g->d = (int*) malloc(g->n * sizeof(int));
  while(++i < g->n){
    g->d[i] = 0;
  }
  i = -1;
  while(++i < g->m){
    listAdj[i] = (int*) malloc(3 * sizeof(int));
    if(inFile != NULL)
      fscanf(inFile, "\n%[^\n]", line);
    else
      scanf("\n%[^\n]", line);
    if(sscanf(line, "%d %d %d", &listAdj[i][0], &listAdj[i][1], &listAdj[i][2]) == 2)
      listAdj[i][2] = 1;
    g->d[--listAdj[i][0]]++;
    g->d[--listAdj[i][1]]++;
  }
  if(inFile != NULL)
  	fclose(inFile);
  i = -1;
  while(++i < g->n){
    g->adj[i] = (int*) malloc(g->d[i] * sizeof(int));
    g->w[i] = (int*) malloc(g->d[i] * sizeof(int));
    g->d[i] = 0;
  }
  i = -1;
  while(++i < g->m){
    g->adj[listAdj[i][0]][g->d[listAdj[i][0]]] = listAdj[i][1];
    g->w[listAdj[i][0]][g->d[listAdj[i][0]]++] = listAdj[i][2];
    g->adj[listAdj[i][1]][g->d[listAdj[i][1]]] = listAdj[i][0];
    g->w[listAdj[i][1]][g->d[listAdj[i][1]]++] = listAdj[i][2];
    free(listAdj[i]);
  }
  free(listAdj);
  return g;
}

int **dijkstra(GRAFO *g, int start){
  int v, i = g->n;
  int **distPre = (int**) malloc(2 * sizeof(int*));
  distPre[0] = (int*) malloc(g->n * sizeof(int));
  distPre[1] = (int*) malloc(g->n * sizeof(int));
  while(i--){
    distPre[0][i] = INT_MAX;
    distPre[1][i] = -1;
  }
  distPre[0][start] = 0;
  HEAP *h = inicHEAP(g->n, start);
  while(notEmpty(h)){
    start = pop(h, distPre[0]);
    i = -1;
    while(++i < g->d[start]){
      v = g->adj[start][i];
      if(inHeap(h, v) && distPre[0][v] - g->w[start][i] > distPre[0][start]){
        distPre[0][v] = distPre[0][start] + g->w[start][i];
        distPre[1][v] = start;
        update(h, v, distPre[0]);
      }
    }
  }
  return distPre;
  freeHEAP(h);
}

int readArgs(int argc, char *argv[], FILE **inFile, FILE **outFile, int *start, int *end, int *show){
  int i = 0;
  *start = 0;
  *end = 0;
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
        printf("Este programa tem o objetivo de calcular a \
distância mínima entre pares de vertices de um grafo \
não direcionado com vertices começando de 1 até n.\n\
Se o vértice inial não for definido, ele será definido como 1, \
se o vértice final não for definido, ele calculará a distância partindo \
do vértice inicial até todos os outros demais. Se o argumento s for \
passado, ele mostrará também o caminho.\n\n\
Argumentos do programa:\n\
-h : mostra o help (Quando h é selecionado, invalida outros argumentos.)\n\
-o <arquivo> : redireciona a saida para o \"arquivo\"\n\
-f <arquivo> : indica o \"arquivo\" que contém o grafo de entrada\n\
-s : mostra a solução (caminho(s))\n\
-i : vértice inicial (Se não especificado i = 1)\n\
-l : vértice final (Se não especificado, calculará de todos)\n");
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
      case 'i':
        sscanf(argv[++i], "%d", start);
        break;
      case 'l':
        sscanf(argv[++i], "%d", end);
        break;
      default:
        printf("Comando não reconhecido, use o comando -h para ajudar.\n");
        return 0;
    }
  }
  if(!(*start)){
    printf("Nenhum vertice inicial definido, assumiremos i = 1\n");
    *start = 1;
  }
  return 1;
}

int main(int argc, char *argv[]) {
  int **distPre, *reverse, j, i, show, start, end;
  FILE *outFile, *inFile;
  GRAFO *g;
  if(!readArgs(argc, argv, &inFile, &outFile, &start, &end, &show)){
    return 0;
  }
  g = readGRAFO(inFile);
  distPre = dijkstra(g, start - 1);
  if(show){
    reverse = (int*) malloc(g->n * sizeof(int));
    if(end){
      j = 0;
      i = end - 1;
      do{
        reverse[j++] = i;
        i = distPre[1][i];
      }while(i != -1);
      if(outFile != NULL){
        fprintf(outFile, "%d ( ", distPre[0][end - 1]);
      }else{
        printf("%d ( ", distPre[0][end - 1]);
      }
      while(--j){
        if(outFile != NULL){
          fprintf(outFile, "%d -> ", reverse[j] + 1);
        }else{
          printf("%d -> ", reverse[j] + 1);
        }
      }
      if(outFile != NULL){
        fprintf(outFile, "%d)\n", reverse[j] + 1);
        fclose(outFile);
      }else{
        printf("%d)\n", reverse[j] + 1);
      }
    }else{
      while(++end <= g->n){
        if(end == start)
        	continue;
        j = 0;
        i = end - 1;
        do{
          reverse[j++] = i;
          i = distPre[1][i];
        }while(i != -1);
        if(outFile != NULL){
          fprintf(outFile, "%d:%d ( ", end, distPre[0][end - 1]);
        }else{
          printf("%d:%d ( ", end, distPre[0][end - 1]);
        }
        while(--j){
          if(outFile != NULL){
            fprintf(outFile, "%d -> ", reverse[j] + 1);
          }else{
            printf("%d -> ", reverse[j] + 1);
          }
        }
        if(outFile != NULL){
          fprintf(outFile, "%d)\n", reverse[j] + 1);
        }else{
          printf("%d)\n", reverse[j] + 1);
        }
      }
      if(outFile != NULL)
      	fclose(outFile);
    }
    free(reverse);
  }else{
    if(end){
      if(outFile != NULL){
        fprintf(outFile, "%d\n", distPre[0][end - 1]);
        fclose(outFile);
      }else{
        printf("%d\n", distPre[0][end - 1]);
      }
    }else{
      end = 0;
      while(++end <= g->n){
      	if(end == start)
        	continue;
        if(outFile != NULL){
          fprintf(outFile, "%d:%d ", end, distPre[0][end - 1]);
        }else{
          printf("%d:%d ", end, distPre[0][end - 1]);
        }
      }
      if(outFile != NULL){
        fprintf(outFile, "\n");
        fclose(outFile);
      }else{
        printf("\n");
      }
    }
  }
  free(distPre[0]);
  free(distPre[1]);
  free(distPre);
  freeGRAFO(g);
  return 0;
}
