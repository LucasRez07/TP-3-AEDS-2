#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct Data{ //Crio a struct data
    int ano, mes, dia;
} Data;

typedef struct Veiculo{ //Crio a struct veículo
    int id, ano, cilindros;
    double cilindrada, consumoCidade, consumoEstrada, co2;
    bool turbo;
    char marca[50], modelo[50], categoria[50], transmissao[50], tracao[50];
    char combustivel[5][50];
    Data dataRegistro;
} Veiculo;

Data parseData(char* s){ //Crio o parseData para converter a string data lida do arquivo para uma variável do tipo Data
    Data data;
    sscanf(s, "%d-%d-%d", &data.ano, &data.mes, &data.dia);
    return data;
}

void formatData(Data d, char* buffer){ //formatData para printar a data do jeito que deve ser
    sprintf(buffer, "%02d/%02d/%04d", d.dia, d.mes, d.ano);
}

void parseCombustivel(char* c, Veiculo* veic){
    int i = 0, j = 0; //Crio os índices da matriz

    for(int k=0; c[k] != '\0'; k++){ //Percorre a string até encontrar o caractere terminador, o \0
        
        if(c[k] == ';'){ //Se o caractere atual for ; significa que achei o separador entre os combustíveis
            veic->combustivel[i][j] = '\0'; //Fecho essa string com \0
            i++; //Vou para o próximo combustível
            j = 0; //Começo a ler os caracteres da posição inicial
        }

        else{
            veic->combustivel[i][j] = c[k]; //Caso contrário avanço para o próximo caractere
            j++;
        }
    }
    veic->combustivel[i][j] = '\0'; //Quando sai do loop fecho o último combustível
    i++;

    for(; i<5; i++){
        veic->combustivel[i][0] = '\0'; //Limpa as posições que sobraram da matriz
    }
}

void formatCombustivel(Veiculo v, char* buffer){
    buffer[0] = '\0'; //Inicializo o buffer para conseguir usar o strcat
    bool continua = true; //Para parar o loop quando necessário

    for(int i=0; i<5 && continua; i++){
        if(v.combustivel[i][0] == '\0'){
            continua = false; //Se a posição i da matriz está vazia significa que chagou ao fim dos combustíveis e encerro o loop
        }

        else{
            if(i>0){
                strcat(buffer, ","); //Caso contrário, a posição é um combustível, se não for o primeiro, coloco uma , antes para separar do combustível anterior
            }
            strcat(buffer, v.combustivel[i]); //Concateno o combustível atual no buffer
        }
    }
}

Veiculo* parseVeiculo(char* s){
    Veiculo* veic = (Veiculo*)malloc(sizeof(Veiculo)); //Aloco memória para caber um veiculo
    
    char* token; //Guarda o pedaço do strtok e para cada atributo transformo parte da string total em seu determinado tipo

    token = strtok(s, ",");
    veic->id = atoi(token);

    token = strtok(NULL, ","); //Uso o NULL nas demais para continuar separando até o separador de onde o tok anterior parou
    strcpy(veic->marca, token);

    token = strtok(NULL, ",");
    strcpy(veic->modelo, token);

    token = strtok(NULL, ",");
    veic->ano = atoi(token);

    token = strtok(NULL, ",");
    strcpy(veic->categoria, token);

    token = strtok(NULL, ",");
    parseCombustivel(token, veic); //Chamo o parse para preencher combustivel diretamente

    token = strtok(NULL, ",");
    veic->cilindros = atoi(token);

    token = strtok(NULL, ",");
    veic->cilindrada = atof(token);

    token = strtok(NULL, ",");
    strcpy(veic->transmissao, token);

    token = strtok(NULL, ",");
    strcpy(veic->tracao, token);

    token = strtok(NULL, ",");
    veic->consumoCidade = atof(token);

    token = strtok(NULL, ",");
    veic->consumoEstrada = atof(token);

    token = strtok(NULL, ",");
    veic->co2 = atof(token);

    token = strtok(NULL, ",");
    veic->turbo = (strcmp(token, "true") == 0); 

    token = strtok(NULL, ",");
    veic->dataRegistro = parseData(token); //Chamo o parse para retornar a data preenchida

    return veic;
}

void formatVeiculo(Veiculo v, char* buffer){
    char buffCombustivel[100]; //Crio três buffers temporários para guardar os resultados dos atributos que não consigo colocar diretamente no format
    char buffData[20];
    char buffTurbo[6];

    formatCombustivel(v, buffCombustivel); //Escrevo combustível do jeito que tem que ser com o format e gravo no seu determinado buffer
    formatData(v.dataRegistro, buffData); //Faço o mesmo com a data

    if(v.turbo){
        strcpy(buffTurbo, "true"); 
    }
    else{
        strcpy(buffTurbo, "false");
    }

     sprintf(buffer, "[%d ## %s ## %s ## %d ## %s ## [%s] ## %d ## %.1lf ## %s ## %s ## %.2lf ## %.2lf ## %.1lf ## %s ## %s]",
        v.id, v.marca, v.modelo, v.ano, v.categoria, buffCombustivel, v.cilindros, v.cilindrada,
        v.transmissao, v.tracao, v.consumoCidade, v.consumoEstrada, v.co2,
        buffTurbo, buffData); //Monto o resto do format normalmente
}

Veiculo* lerCsv(char* caminho, int* n){

    FILE* arquivo = fopen(caminho, "r");

    Veiculo* aux = (Veiculo*)malloc(500*sizeof(Veiculo));
    char linhas[500];
    fgets(linhas, sizeof(linhas), arquivo); //Pulo a linha inicial do arquivo
    int i = 0; //Contador para auxiliar na atribuição dos veículos

    while(fgets(linhas, sizeof(linhas), arquivo) != NULL){ //Leio até o fim do arquivo
        Veiculo* tmp = parseVeiculo(linhas); //Chamo o parse para uma variável temporária para que o malloc da função receba o free aqui
        aux[i] = *tmp;
        free(tmp);
        i++; 
    }

    fclose(arquivo); //Fecho o arquivo
    *n = i; //Aqui atribuo o i, que será a quantidade de carros, no n
    return aux;
}

typedef struct Celula{
    Veiculo mod;
    struct Celula* prox;
}Celula;

Celula* novaCel(Veiculo m){
    Celula* nova = (Celula*)malloc(sizeof(Celula));
    nova->mod = m;
    nova->prox = NULL;
    return nova;
}

typedef struct Lista{
    Celula* primeiro;
    Celula* ultimo;
}Lista;

void start(Lista* l){
    Veiculo aux;
    l->primeiro = novaCel(aux);
    l->ultimo = l->primeiro;
}

void inserirInicio(Lista* l, Veiculo a){

    Celula* tmp = novaCel(a);
    tmp->prox = l->primeiro->prox;
    l->primeiro->prox = tmp;

    if(l->primeiro == l->ultimo){
        l->ultimo = tmp;
    }

}

void inserirFim(Lista* l, Veiculo a){

    l->ultimo->prox = novaCel(a);
    l->ultimo = l->ultimo->prox;

}

void inserirPos(Lista* l, Veiculo a, int pos, int tam){

    if(pos < 0 || pos > tam){
        exit(1);
    }

    else if(pos == 0){
        inserirInicio(l, a);
    }

    else if(pos == tam){
        inserirFim(l, a);
    }

    else{

        Celula* i = l->primeiro;

        for(int j=0; j<pos; j++, i = i->prox);

        Celula* tmp = novaCel(a);
        tmp->prox = i->prox;
        i->prox = tmp;

    }

}

Veiculo removerInicio(Lista* l){
    
    Celula* tmp = l->primeiro;
    l->primeiro = l->primeiro->prox;
    Veiculo resp = l->primeiro->mod;
    tmp->prox = NULL;

    return resp;

}

Veiculo removerFim(Lista* l){
    
    Celula* i = NULL;

    for(i = l->primeiro; i->prox != l->ultimo; i = i->prox);

    Veiculo resp = l->ultimo->mod;
    l->ultimo = i;
    l->ultimo->prox = NULL;

    return resp;

}

Veiculo removerPos(Lista* l, int tam, int pos){
    
    Veiculo resp;

    if(l->primeiro == l->ultimo || pos < 0 || pos > tam){
        exit(1);
    }

    else if(pos == 0){
        resp = removerInicio(l);
    }

    else if(pos == tam-1){
        resp = removerFim(l);
    }

    else{
        Celula* i = l->primeiro;

        for(int j=0; j<pos; j++, i = i->prox);

        Celula* tmp = i->prox;
        resp = tmp->mod;
        i->prox = tmp->prox;
    }

    return resp;

}

void mostrar(Lista* l){

    Celula* i = l->primeiro->prox;
    
    while(i != NULL){

        char* resposta = (char*)malloc(1000*sizeof(char));
        formatVeiculo(i->mod, resposta);
        printf("%s\n", resposta);
        free(resposta);
        i = i->prox;

    }

}

char viraMinuscula(char c){ //Nesta função vejo se o caractere está dentro do alfabeto maiúsculo, se sim, transformo a letra em minúscula
    if(c >= 'A' && c <= 'Z'){
        c += 32;
    }
    return c;
}

int comparaMinuscula(char *a, char* b){
    int k = 0;

    while(a[k] != '\0' && b[k] != '\0'){ //Vai olhar até o fim de uma das strings ou das duas
        char ca = viraMinuscula(a[k]); //Pega o caractere de cada uma
        char cb = viraMinuscula(b[k]);

        if(ca != cb){ //Se forem diferentes não tem mais motivo pra continuar comparando
            return ca - cb;
        }
        k++;
    }
    return a[k] - b[k]; //Se as strings forem iguais até aqui, o tamanho decide qual é a menor
}

int main(){
    int id = 0, n;
    Lista lista;
    start(&lista);

     //Veiculo* cars = lerCsv("veiculos.csv", &n); //Mando o caminho, um do linux e um do windows, o que eu for usar eu descomento
     Veiculo* cars = lerCsv("/tmp/veiculos.csv", &n);

    while(id != -1){
        scanf("%d", &id); //Leio os ids
            for(int i=0; i<n; i++){
                if(id == cars[i].id){ //Se eu encontrar o id adiciono na lista
                   inserirFim(&lista, cars[i]);
                    i = n; //Dou i = n para sair de uma vez da pesquisa
                }
            }
    }

    for(Celula* i = lista.primeiro->prox; i != lista.ultimo; i = i->prox){ //Faço o seleção na lista usando o atributo modelo do veículo
        Celula* menor = i;
        for(Celula* j = i->prox; j != NULL; j = j->prox){
            if(comparaMinuscula(menor->mod.modelo, j->mod.modelo) > 0){
                menor = j;
            }
        }
        Veiculo aux = menor->mod;
        menor->mod = i->mod;
        i->mod = aux;
    }

    mostrar(&lista); //Mostro a lista ordenada

    free(cars); //Dou o último free necessários
}