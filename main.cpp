#include <bits/stdc++.h>
#define vi std::vector <int>
#define solu std::pair <vi, vi>
 
 
using namespace std;
using namespace std::chrono;
 
template <class dado>
class ListaTabu{
 
    public:
 
        set <dado> lista_movimentos;
 
        queue <dado> ordem_chegada;
 
        int tamanho_maximo;
 
        ListaTabu(int tamanho_maximo){
 
            this->tamanho_maximo = tamanho_maximo;
 
        }
 
        bool tabu(dado movimento){
 
            return lista_movimentos.count(movimento);
 
        }
 
 
        void adicionar_movimento(dado movimento){
 
            if (lista_movimentos.size() == tamanho_maximo) {
 
                lista_movimentos.erase(ordem_chegada.front());
                ordem_chegada.pop();
            }
 
            int size = lista_movimentos.size();
 
            lista_movimentos.insert(movimento);
            if (size < lista_movimentos.size()) ordem_chegada.push(movimento);
           
        }
};
 
bool comp(int a, int b){
    return a > b;
 
}
 
 
solu first_fit_decr(vector <int> &tamanhos_itens, int capacidade_maxima, vector <int> &carga){
 
    sort(tamanhos_itens.begin(), tamanhos_itens.end(), comp); // Ordena os tamanhos dos itens de forma decrescente
 
    vector <int> itens_e_pacotes(tamanhos_itens.size()); // Solução inicial
 
    carga.reserve(tamanhos_itens.size()); // Reserva o espaço no vetor para o push_back não realizar realocação de memoria
 
    carga.push_back(0);
 
    solu solucao;
 
    int flag;
 
    for (int i = 0; i < tamanhos_itens.size(); i++){
 
        flag = 0;
 
        for (int j = 0; j < carga.size() and flag != 1; j++){
 
            if (carga[j] + tamanhos_itens[i] <= capacidade_maxima){
 
                carga[j] += tamanhos_itens[i];
 
                itens_e_pacotes[i] = j;
 
                flag = 1;
            }
        }
        if (flag == 0){
           
            carga.push_back(0);
            i--;
        }
    }
 
    solucao.first = itens_e_pacotes;
    solucao.second = carga;
 
    return solucao;
}
 
 
solu melhor_vizinho(solu &solucao, ListaTabu<tuple<int, int,int>> &lista_tabu,
                            vi &tamanho_itens, int capacidade,
                            solu &melhor_solucao){
 
    int size = solucao.first.size();
 
    solu solucao_ = solucao;
 
    int flag = 0;
 
    int caixas_melhor_solucao = 0;
    int caixas_solucao_atual = 0;
 
    int baixa_ocupacao_solu = 0;
    int baixa_ocupacao_melhor = 0;
 
    for (int i = 0; i < size; i++){
 
        caixas_melhor_solucao = max(melhor_solucao.first[i], caixas_melhor_solucao);
 
        caixas_solucao_atual = max(solucao.first[i], caixas_solucao_atual);
 
        if (melhor_solucao.second[i] < (capacidade / 2.0)) baixa_ocupacao_melhor++;
       
        if (solucao.second[i] < (capacidade / 2.0)) baixa_ocupacao_solu++;
 
    }
 
    caixas_melhor_solucao++;
    caixas_solucao_atual++;
 
    tuple <int, int, int> movimento, melhor_mov = {0, 0, 0};
 
    for (int i = 0; i < size and !flag; i++){
 
        for (int j = 0; j < solucao.second.size() and !flag; j++){
 
            if (j != solucao.first[i] and
                solucao.second[j] + tamanho_itens[i] < capacidade){
               
                movimento = {1, i, j};
 
                flag = (solucao.second[solucao.first[i]] == tamanho_itens[i]);
 
                if (!lista_tabu.tabu(movimento) or caixas_solucao_atual - flag < caixas_melhor_solucao) {
 
                    melhor_mov = movimento;
 
                }
            }
        }
    }
 
    if (melhor_mov == make_tuple(0, 0, 0)){
 
        lista_tabu.adicionar_movimento(melhor_mov);
 
        int i = get<1>(melhor_mov);
        int j = get<2>(melhor_mov);
 
        solucao_.second[j] = solucao.second[j] + tamanho_itens[i];
        solucao_.second[solucao.first[i]] -= tamanho_itens[i];
        solucao_.first[i] = j;
 
        return solucao_;
 
    }
 
    int maior_diff = -1;
 
    for (int i = 0; i < size; i++){
 
        for (int j = i + 1; j < size; j++){
 
            if (solucao.first[i] != solucao.first[j]){
 
                int x = solucao.second[solucao.first[i]];
                int y = solucao.second[solucao.first[j]];
 
                int a = tamanho_itens[i];
                int b = tamanho_itens[j];
 
                if (x - a + b <= capacidade and y - b + a <= capacidade){
                   
                    movimento = {2, i, j};
 
                    int diff = ((x - a + b) >= (capacidade / 2.0)) + ((y - b + a) >= (capacidade / 2.0));
 
                    if ((!lista_tabu.tabu(movimento) or baixa_ocupacao_solu - diff < baixa_ocupacao_melhor) and diff > maior_diff){
 
                            melhor_mov = movimento;
 
                            maior_diff = diff;
                    }
                }
            }
        }
    }
 
    lista_tabu.adicionar_movimento(melhor_mov);
 
    int i = get<1>(melhor_mov);
    int j = get<2>(melhor_mov);
 
    int x = solucao.second[solucao.first[i]];
    int y = solucao.second[solucao.first[j]];
 
    int a = tamanho_itens[i];
    int b = tamanho_itens[j];
 
    solucao_.second[solucao.first[i]] = x - a + b;
    solucao_.second[solucao.first[j]] = y - b + a;
 
    swap(solucao_.first[i], solucao_.first[j]);
 
    return solucao_;
 
}
 
 
vi busca_tabu(vi &itens, int capacidade, vector <int> &cargas){
 
    solu solucao, melhor_solucao;
 
    solucao = first_fit_decr(itens, capacidade, cargas);
 
    melhor_solucao = solucao;
 
    ListaTabu <tuple<int, int, int>> lista_tabu(30);
 
    int melhor_iter = 0;
 
    int num_iter = 10;
 
    for (int i = 0; i < num_iter; i++){
 
        solucao = melhor_vizinho(solucao, lista_tabu, itens, capacidade, melhor_solucao);
 
        int size = solucao.first.size();
 
        int caixas_melhor_solucao = 0;
        int caixas_solucao_atual = 0;
 
        int baixa_ocupacao_solu = 0;
        int baixa_ocupacao_melhor = 0;
 
        for (int i = 0; i < size; i++){
 
            caixas_melhor_solucao = max(melhor_solucao.first[i], caixas_melhor_solucao);
 
            caixas_solucao_atual = max(solucao.first[i], caixas_solucao_atual);
 
            if (melhor_solucao.second[i] < (capacidade / 2.0)) baixa_ocupacao_melhor++;
           
            if (solucao.second[i] < (capacidade / 2.0)) baixa_ocupacao_solu++;
 
        }
 
        if (make_pair(caixas_solucao_atual, baixa_ocupacao_solu) < make_pair(caixas_melhor_solucao, baixa_ocupacao_melhor)){
 
            melhor_solucao = solucao;
 
            melhor_iter = i;
 
        }
 
    }
 
    cargas = melhor_solucao.second;
 
    return melhor_solucao.first;
 
}
 
 
 
int fitness(vector<int>& lista, vector<int>& tamanhos, int capacidade) {
 
    // Determina o número de bins utilizados
 
    int numerocaixas=0;
 
    for(int i=0; i<lista.size(); i++)
    {
        numerocaixas=max(numerocaixas,lista[i]);
 
    }
 
    numerocaixas++;
 
    vector<int> carga(numerocaixas, 0);
 
    // Calcula a carga de cada bin
 
    for (int i = 0; i < lista.size(); ++i) {
 
        carga[lista[i]] += tamanhos[i];
 
    }
 
    int poucousadas=0;
 
    // Calcula a penalidade por bins que excedem a capacidade
 
    int penalidade = 0;
 
    for (int load : carga) {
 
        if(load<(capacidade * 0.5))
        {
 
            poucousadas+= (capacidade * 0.5) - load;
 
        }
 
        if (load > capacidade) {
 
            penalidade += (load - capacidade);  // Penaliza excesso
 
        }
 
    }
 
    return numerocaixas + penalidade + poucousadas;
 
}
 
void mutacao(vector<int>& solucao, vector<int>& tamanhos, vector<int>& carga, int capacidade) {
 
    double taxa = 0.05;  // 5% de chance de mutação
 
    int maior=0;
 
    for(int i=0; i<solucao.size(); i++)
    {
        maior=max(maior,solucao[i]);
 
    }
 
    int aleatorio=rand() % 100;
 
    if (aleatorio < taxa * 100) {
 
        int item = rand() % solucao.size();
 
        int novacaixa = rand() % (maior + 1);
 
        if (carga[novacaixa] + tamanhos[item] <= capacidade) {
 
            solucao[item] = novacaixa;  // Realoca o item em uma nova caixa
 
        }
 
    }
 
}
 
 
vector<vector<int>> populacaoinicial(int quantidade, vector<int>& tamanhos, int capacidade) {
 
    vector<vector<int>> populacao;
 
    vector <int> cargas;
 
    vector<int> solucao = busca_tabu(tamanhos, capacidade, cargas);
 
    for (int i = 0; i < quantidade; ++i) {
 
        if (i > 0) {
 
            mutacao(solucao, tamanhos, cargas, capacidade);  // Aplica mutação para gerar soluções variadas
 
        }
 
        populacao.push_back(solucao);
 
    }
 
    return populacao;
 
}
 
 
 
vector<int> avaliarpopulacao(vector<vector<int>>& populacao, vector<int>& tamanhos, int capacidade) {
 
    vector<int> fitpopulacao;
 
    for (int i=0; i<populacao.size(); i++) {
 
        int fit = fitness(populacao[i], tamanhos, capacidade);
 
        fitpopulacao.push_back(fit);
 
    }
 
    return fitpopulacao;
 
}
 
 
 
vector<int> torneio(vector<vector<int>>& populacao, vector<int>& fitpopulacao, int tamanhotorneio) {
 
    int melhor = rand() % populacao.size();
 
    for (int i = 1; i < tamanhotorneio; ++i) {
 
        int competidor = rand() % populacao.size();
 
        if (fitpopulacao[competidor] < fitpopulacao[melhor]) {
 
            melhor = competidor;
 
        }
 
    }
 
    return populacao[melhor];  // Retorna o melhor indivíduo do torneio
 
}
 
 
 
vector<int> crossover(vector<int>& pai1, vector<int>& pai2) {
 
    vector<int> filho(pai1.size());
 
    int corte = rand() % pai1.size();  // Ponto onde vai cortar para cruzar entre os 2 pais
 
    for (int i = 0; i < corte; ++i) {
 
        filho[i] = pai1[i];  // Copia parte do pai1
 
    }
 
    for (int i = corte; i < pai2.size(); ++i) {
 
        filho[i] = pai2[i];  // Copia parte do pai2
 
    }
 
    return filho;
 
}
 
 
 
 
void evoluirPopulacao(vector<vector<int>>& populacao, vector<int>& tamanhos, int capacidade) {
 
    int tp = populacao.size();
 
    vector<int> fitpop = avaliarpopulacao(populacao, tamanhos, capacidade);
    vector<vector<int>> novapopulacao;
 
    // Elitismo: Manter a melhor solução
 
    int melhor=0;
 
    for(int i=1; i<fitpop.size(); i++)
    {
 
        if(fitpop[i]<fitpop[melhor])
        {
            melhor=i;
 
        }
 
    }
 
    novapopulacao.push_back(populacao[melhor]);
 
    while (novapopulacao.size() < tp) {
 
        // Seleciona dois pais
 
        vector<int> pai1 = torneio(populacao, fitpop, 3);
 
        vector<int> pai2 = torneio(populacao, fitpop, 3);
        // Aplica crossover
 
        vector<int> filho = crossover(pai1, pai2);
 
        int numerocaixas=0;
 
        for(int i=0; i<filho.size(); i++)
        {
            numerocaixas=max(numerocaixas,filho[i]);
        }
 
        vector<int> carga(numerocaixas + 1, 0);
 
        // Calcula a carga de cada bin
 
        for (int i = 0; i < filho.size(); ++i) {
 
            carga[filho[i]] += tamanhos[i];
        }
        // Aplica mutação
 
        mutacao(filho, tamanhos, carga, capacidade);
        // Adiciona à nova população
 
        novapopulacao.push_back(filho);
    }
    populacao = novapopulacao;
 
}
 
 
int main(){
 
    int t;
 
    int cont = 0;
    int n;
    cin>>n;
    int capacidade;
    cin>>capacidade;
    vector<int> tamanhos;
    tamanhos.reserve(n);
    for(int i=0; i<n; i++)
    {
        int x;
        cin>>x;
        tamanhos.push_back(x);
    }
    auto start = high_resolution_clock::now();
    // População inicial
    vector<vector<int>> populacao = populacaoinicial(100, tamanhos, capacidade);
 
    // Executa o Algoritmo Genético por 100 iterações
    for (int iter = 0; iter < 100; iter++) {
        evoluirPopulacao(populacao, tamanhos, capacidade);
    }
 
    // Avalia a população final
    vector<int> fitpopulacao = avaliarpopulacao(populacao, tamanhos, capacidade);
 
    // Encontra a melhor solução
    int melhor = 0;
    for (int i = 1; i < fitpopulacao.size(); i++) {
        if (fitpopulacao[i] < fitpopulacao[melhor]) {
            melhor = i;
        }
    }
 
    auto stop = high_resolution_clock::now();
   
    int caixas_usadas = *max_element(populacao[melhor].begin(), populacao[melhor].end()) + 1;
    cout << "Quantidade de caixas usadas: " << caixas_usadas << endl;
   
    auto duration = duration_cast<milliseconds>(stop - start);
 
    cout << "Tempo usado em milissegundos: " << duration.count() << " milisegundos" << endl;
 
 
}
 
 