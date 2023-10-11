#include <iostream>
#include <string>
#include <vector>
#include <queue>
#define DEBUG if(1)
#define INFINITY 100000


void prim(std::vector<std::vector<int>> graph, int s, int pre[], int cost[]){
    int len_v = graph[0].size();
    int pas[len_v];
    for (int i=0; i<len_v; ++i){
        cost[i]=INFINITY;
        pre[i]=-1;
        pas[i]=0;
    }
    cost[s]=0;
    pre[s]=s;
    std::priority_queue<std::pair<int, int>> h = std::priority_queue<std::pair<int, int>>();
    h.push({0,s});
    while (!h.empty()){
        // int vert_c = h.top().first;
        int vert_v = h.top().second;
        h.pop();
        if (pas[vert_v]==1){continue;}
        else{pas[vert_v]=1;}
        for (int z=0; z<len_v; ++z){
            if (pas[z]==0 && graph[vert_v][z]!=0 && cost[z]>graph[vert_v][z]){
                // DEBUG{std::cout<<"HERE\n";}
                cost[z]=graph[vert_v][z];
                h.push({-cost[z],z});
                pre[z]=vert_v;
            }
        }
    }
}


int main(int argv, char** argc){
    for (int index_arg=0; index_arg<argv; index_arg++){
        // DEBUG{std::cout<<"AQUUI TA INDO!! "<<argc[index_arg]<<index_arg<<"\n";}
        if (argc[index_arg][0]=='-' && argc[index_arg][1]=='f'){
            FILE* file = fopen(argc[index_arg+1],"r");
            if (file==NULL){
                std::cout<<"File not found.\n";
                return 0;
            }
            // DEBUG{std::cout<<"NAMEFILE="<<argc[index_arg+1]<<"\n";}
            int n, m;
            fscanf(file, "%d %d", &n, &m);
            // std::cin>>n>>m;
            std::vector<std::vector<int>> graph = std::vector<std::vector<int>>(n, std::vector<int>(n,0));
            for (int i=0; i<m; ++i){
                int a, b, w;
                fscanf(file, "%d %d %d", &a, &b, &w);
                graph[a-1][b-1]=w;
                graph[b-1][a-1]=w;
            }

            int init = 1;
            for (int index_arg2=0; index_arg2<argv; index_arg2++){
                if (argc[index_arg2][0]=='-' && argc[index_arg2][1]=='i'){
                    init = std::stoi(argc[index_arg2+1])-1;
                    break;
                }
            }
            int previews[n];
            int cost_weight[n];
            prim(graph, init, previews, cost_weight);

            int order = 0;
            for (int index_arg3=0; index_arg3<argv; index_arg3++){
                if (argc[index_arg3][0]=='-' && argc[index_arg3][1]=='s'){order=1;break;}
            }
            if (order==0){
                // std::cout<<"RESULT:\n";
                int t=0;
                for (int l=0; l<n; ++l){
                    t+=cost_weight[l];
                    // std::cout << "(" << previews[l]<<","<<cost_weight[l]<<") ";
                    // std::cout<<"\n";
                }
                std::cout<<t<<"\n";
            }
            else if (order==1){
                // std::cout<<"ERROR 1\n";
                std::priority_queue<std::pair<int, int>> res = std::priority_queue<std::pair<int, int>>();
                // std::cout<<"ERROR 2\n";
                int t = 0;
                for (int l=0; l<n; ++l){
                    t+=cost_weight[l];
                    res.push({-cost_weight[l],previews[l]});
                }
                for (int k=0; k<n; ++k){
                    std::cout << "(" << res.top().second<<","<<-res.top().first<<") ";
                    // std::cout<<"\n";
                    res.pop();
                }
                std::cout<<"\n";
            }

            for (int index_arg4=0; index_arg4<argv; index_arg4++){
                if (argc[index_arg4][0]=='-' && argc[index_arg4][1]=='o'){
                    FILE *savefile = fopen(argc[index_arg4+1],"w");
                    
                    int order = 0;
                    for (int index_arg3=0; index_arg3<argv; index_arg3++){
                        if (argc[index_arg3][0]=='-' && argc[index_arg3][1]=='s'){order=1;break;}
                    }
                    if (order==0){
                        // std::cout<<"RESULT:\n";
                        int t=0;
                        for (int l=0; l<n; ++l){
                            t+=cost_weight[l];
                            fprintf(savefile, "(%d,%d) ", previews[l], cost_weight[l]);
                        }
                        fprintf(savefile, "\n");
                    }
                    else if (order==1){
                        // std::cout<<"ERROR 1\n";
                        std::priority_queue<std::pair<int, int>> res = std::priority_queue<std::pair<int, int>>();
                        // std::cout<<"ERROR 2\n";
                        int t = 0;
                        for (int l=0; l<n; ++l){
                            t+=cost_weight[l];
                            res.push({-cost_weight[l],previews[l]});
                        }
                        for (int k=0; k<n; ++k){
                            fprintf(savefile, "(%d,%d) ", res.top().second, -res.top().first);
                            res.pop();
                        }
                        fprintf(savefile, "\n");
                    }
                    fclose(savefile);
                }
            }
            fclose(file);

        }
        if (argc[index_arg][0]=='-' && argc[index_arg][1]=='h'){
            std::cout<<"Para executar um algoritmo esteja branch \"main\", tenha o g++ e execute os seguintes passos:\n\n";
            std::cout<<"1º: Acesse a pasta do algoritmo;\n\n";
            std::cout<<"2º: Digite sem as aspas a palavra \"make\" para compilar.\n\n";
            std::cout<<"3º: Digite sem as aspas a palavra \"./<nome_do_algoritmo> <parametros>\" para executar.\n\n";
            std::cout<<"Em caso de dúvida digite -h como parâmetro.\n\n";
            std::cout<<"Direcione a saída para um arquivo com o parâmetro -o e nome_do_arquivo.dat\n";
            std::cout<<"Indique um arquivo que contém o grafo com o parâmetro -f e nome_do_arquivo.dat\n";
            std::cout<<"Mostre a solução em ordem crescente com o parâmetro -s\n";
            std::cout<<"Defina o vértice inicial com o parâmetro -i.\n\n";
        }
    }
}
