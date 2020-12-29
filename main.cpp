#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <deque>
#include <stack>
#include <climits>
#include <string.h>
#include <stdbool.h>
/*
Aluno: �gor Ferrazza Capeletti
Matr�cula: 1802440007

Descri��o: O programa representa uma estrutura simplificada de uma rede social com v�rias opera��es, entre elas:
    Adicionar perfis; adicionar amigos para um perfil; remover amigos de um perfil; buscar algum perfil na rede;
    buscar um perfil dentro da lista de amigos de um perfil; buscar um perfil dentro da lista de amigos dos seus amigos;
    buscar um perfil dentro da lista de amigos dos seus amigos, limitando por um n�vel maximo a busca; buscar amigos
    em comum entre dois perfis; sugerir amigos para um perfil; calcular a distancia entre dois perfis; verificar os
    grupos de amizades de toda rede; fundir dois ou mais perfis em um s�; detectar perfis falsos; e tamb�m,
    detectar perfis influenciadores.
OBS: Cada fun��o tem uma breve descri��o de como foram feitos os algoritmos e opera��es.*/

using namespace std;

//estrutura do perfil(n� do grafo)
struct no{
    int id;             //id do perfil
    char *nome;         //ponteiro de caracteres
    bool visitado;      //armazenar se o perfil foi visitado ou n�o(para algumas opera��es funcionarem)
    bool ciclo;         //armazena se o perfil saindo dele consegue voltar novamente at� ele
    int distancia;      //armazena a dist�ncia do perfil
    int anterior;       //define o perfil anterior a esse(seu pai)
    vector<struct no*> *amigos;     //lista dos amigos do perfil
    vector<int> *custo;             //lista de valores de cada aresta com seus amigos
};
typedef struct no Perfil;


//estrutura do Grafo
struct grafo{
    int m;   //n�mero de arestas do grafo(amigos)
    vector<Perfil*> listaPerfis;    //lista dos perfis do grafo
};
typedef struct grafo Grafo;

//fun��o para criar grafo vazio
Grafo *criaGrafo(){
    Grafo *redeSocial= (Grafo*)malloc(sizeof(Grafo));
    redeSocial->m= 0;
    return redeSocial;
}

//fun��o para imprimir amigos de um determinado perfil
void imprimeAmigos(vector<Perfil*> *amigos){ //percorre a lista de amigos de um determinado perfil
    for(int i=0; i < amigos->size(); i++){
        printf("%s ", amigos->at(i)->nome);
    }
}

//fun��o para imprimir todos os perfis da rede social com os amigos de cada perfil
void imprimeRedeSocial(Grafo *redeSocial){
    printf("\n\nPerfis da rede social:");
    for(int i=0; i < redeSocial->listaPerfis.size(); i++){  //percorre todos os perfis(n�) do grafo
        printf("\n\n%s!", redeSocial->listaPerfis.at(i)->nome);
        printf("\nAmigos:");
        imprimeAmigos(redeSocial->listaPerfis.at(i)->amigos);   //chama a fun��o para imprimir os amigos de cada perfil
    }
}

/*Fun��o para verificar se perfil j� existe na rede social(grafo)
Solu��o: Percorre todo o vetor de perfis do grafo comparando os ids*/
bool existePerfil(Grafo *redeSocial, int numCompara){
    for(int i=0; i < redeSocial->listaPerfis.size(); i++){ //percorre toda lista de perfis do grafo
        if(redeSocial->listaPerfis[i]->id == numCompara){   //compara cada id do perfil do grafo com o id recebido
            return true;    //se for iguais os ids, ent�o j� existe este perfil, e retorna
        }
    }
    return false;
}

/*1)-Fun��o para adicionar perfil na rede social(no grafo)
Primeiro verifica se existe o perfil, chamando uma fun��o.
Se n�o existir vai alocar mem�ria din�nica de tamanho da estrutura Perfil, depois seta os outros atributos
e adiciona o perfil na ultima posi��o da rede social(grafo)*/
Perfil *addPerfil(Grafo *redeSocial, int numId, char *nome){
    if(!existePerfil(redeSocial, numId)){//chama a fun��o para verificar se existe esse perfil no grafo, se n�o existe, vai prosseguir as opera��es
        Perfil *novo= (Perfil*)malloc(sizeof(Perfil));  //aloca dinamicamente mem�ria do tamanho de uma estrutura de perfil
        novo->id= numId;
        novo->nome= nome;
        novo->custo= new vector<int>;   //cria um vetor vazio
        novo->visitado= false;
        novo->ciclo= false;
        novo->distancia= INT_MAX;
        novo->amigos= new vector<Perfil*>;  //cria vetor vazio
        redeSocial->listaPerfis.push_back(novo);    //adiciona o perfil na lista de perfis do grafo
        //printf("\nPerfil %s foi adicionado a rede social!", novo->nome);
        return novo;
    }else{
        //printf("\nErro ao adicionar, perfil %i ja existe, nao adicionado!", numId);
        return NULL;
    }
}

//Fun��o para remover perfil(n�) na rede social(Grafo)
Grafo *removePerfil(Grafo *redeSocial, int idPerfil){
    vector<Perfil*>::iterator it;
    for(it= redeSocial->listaPerfis.begin(); it!= redeSocial->listaPerfis.end(); it++){
        if((*it)->id == idPerfil){
            redeSocial->listaPerfis.erase(it);
            printf("\nPerfil id=%d removido!",idPerfil);
            return redeSocial;
        }
    }
}

//Fun��o de buscar perfil no grafo a partir do numId
Perfil *buscaPerfil(Grafo *redeSocial, int numId){
    for(int i=0; i < redeSocial->listaPerfis.size(); i++){ //percorre toda lista de perfis do grafo
        if(redeSocial->listaPerfis[i]->id == numId){    //faz compara��o entre os ids
            return redeSocial->listaPerfis[i];  //se compara��o for verdadeira, perfil foi encontrado e retorna ele
        }
    }
    return NULL;
}

/*2)-Fun��o de adicionar amigo em um perfil
Solu��o: Como n�o tem nomes nos ids dos arquivos das listas da rede social,
coloquei meu nome como padr�o adicionando sempre o id nofinal do meu nome.
Aqui busca o perfil principal e o perfil do amigo no grafo, se n�o encontrar algum deles,
vai ser criado o perfil que n�o existir.
Depois o perfilAmigo ser� adicionado na ultima posi��o do vetor de amigos do perfilPrincipal.*/
void addAmigo(Grafo *redeSocial, int perfilPrincipal, int perfilAmigo){
    char padrao[5] = "Igor";
    char *nomeP= (char*)malloc(20*sizeof(char)); //recebe a comcatena��o do char padr�o mais o id do perfil principal
    char *nomeA= (char*)malloc(20*sizeof(char)); //recebe a comcatena��o do char padr�o mais o id do perfil amigo
    snprintf (nomeP, 20, "%s%d", padrao, perfilPrincipal); //concatena para string as duas variaveis
    snprintf (nomeA, 20, "%s%d", padrao, perfilAmigo); //concatena para string as duas variaveis
    Perfil *principal = buscaPerfil(redeSocial, perfilPrincipal); //busca o perfil se ja existe no grafo
    Perfil *amigoAdd = buscaPerfil(redeSocial, perfilAmigo); //busca o perfil se ja existe no grafo
    if(principal == NULL){
        principal= addPerfil(redeSocial, perfilPrincipal, nomeP);
    }
    if(amigoAdd == NULL){
        amigoAdd= addPerfil(redeSocial, perfilAmigo, nomeA);
    }
    principal->amigos->push_back(amigoAdd); //adiciona o perfil amigo na ultima posi��o da lista de amigos do perfil principal
    redeSocial->m+= 1;  //adiciona uma aresta no grafo
    //printf("\nPerfil %s foi adicionado como amigo do perfil %s", amigoAdd->nome, principal->nome);
}

/*2)-Fun��o que cria amizade entre dois perfis
Solu��o: Adiciona o perfil2 como amigo do perfil1 e depois faz o contrario, nas duas vezes chama a fun��o addAmigo*/
void addAmizade(Grafo *redeSocial, int idPerfil1, int idPerfil2){
    printf("\nAdiciona amizade entre os perfis %i e %i:",idPerfil1, idPerfil2);
    addAmigo(redeSocial, idPerfil1, idPerfil2); //adiciona perfil 2 como amigo do perfil 1
    addAmigo(redeSocial, idPerfil2, idPerfil1); //adiciona perfil 1 como amigo do perfil 2
    printf("\n--->Amizade entre o perfil id=%i e perfil id=%i realizada!\n", idPerfil1, idPerfil2);
}

/*/Fun��o para buscar amigos de um determinado Perfil
Solu��o: chama a fun��o de buscar o perfil no grafo e retorna a lista de amigos do perfil encontrado*/
vector<Perfil*> *buscaListaAmigos(Grafo *redeSocial, int idPerfil){
    Perfil *principal = buscaPerfil(redeSocial, idPerfil); //busca o perfil no grafo
    if(principal != NULL){
        return principal->amigos;   //retorna a lista de amigos se o perfil existir no grafo
    }else{
        return NULL;
    }
}

/*3)-Fun��o para remover amigo de um perfil
Solu��o: percorre a lista de amigos do perfilPrincipal, comparando o id do perfil amigo a ser
removido com o id dos amigos do perfilPrincipal, quando encontrar o perfil amigo,
remove da lista de amigos do perfilPrincipal*/
void removeAmigo(Grafo *redeSocial, int perfilPrincipal, int perfilAmigo){
    Perfil *principal= buscaPerfil(redeSocial, perfilPrincipal); //recebe o perfil
    vector<Perfil*>::iterator it;
    for(it= principal->amigos->begin(); it != principal->amigos->end(); it++){ //percorre a lista de amigos do perfil
        if((*it)->id == perfilAmigo){ //compara o id dos perfis com o id do amigo a ser retirado
            printf("\nAmigo %s removido do perfil %s!", (*it)->nome, principal->nome);
            principal->amigos->erase(it); //se encontrou o perfil dentro dos amigos, remove da lista de amigos do perfil
            break;
        }
    }
    redeSocial->m -=1; //diminui uma aresta no grafo
}

/*3)-Fun��o para desfazer amizade entre dois perfis
Solu��o: remove o perfil2 de amigo do perfil1 e depois faz o contr�rio, chamando a fun��o remover amigo nas duas vezes*/
void removeAmizade(Grafo *redeSocial, int idPerfil1, int idPerfil2){
    printf("\n\nDesfaz amizade entre os perfis %i e %i!", idPerfil1, idPerfil2);
    removeAmigo(redeSocial, idPerfil1, idPerfil2); //remove o perfil 2 da lista de amigos do perfil 1
    removeAmigo(redeSocial, idPerfil2, idPerfil1); //remove o perfil 1 da lista de amigos do perfil 2
    printf("\nAmizade desfeita entre os perfis %i e %i!\n", idPerfil1, idPerfil2);
}

/*4)-Fun��o de buscar perfil no grafo a partir do nome
Solu��o: Percorre toda a lista de perfis do grafo e retorna o perfil que tiver o mesmo nome do perfil a ser buscado.*/
Perfil *buscaNomePerfil(Grafo *redeSocial, char *nome){
    for(int i=0; i < redeSocial->listaPerfis.size(); i++){ //percorre toda lista de perfis do grafo
        if(!strcmp(nome,redeSocial->listaPerfis[i]->nome)){ //compara as strings do nome recebido na fun��o com o nome dos perfis
            //printf("\nPerfil %s de id=%i encontrado na rede social!", redeSocial->listaPerfis[i]->nome, redeSocial->listaPerfis[i]->id);
            return redeSocial->listaPerfis[i]; //retorna o perfil se os dois nomes s�o iguais na compara��o
        }
    }
    //printf("\nPerfil %s n�o encontrado!", nome);
    return NULL;
}

/*5)-Fun��o de buscar um amigo dentro da lista de amigos do perfil
Solu��o: Busca o perfil dentro do grafo, depois compara o nome do amigo a ser buscado com o nome de todos
os amigos do perfil, se encontrar retorna o perfil amigo.*/
Perfil *buscaPerfilAmigos(Grafo *redeSocial, int idPerfil, char *nomeAmigo){
    if(!existePerfil(redeSocial,idPerfil)){
        printf("\nPerfil de id=%i nao existe na rede!", idPerfil);
        return NULL;
    }
    vector<Perfil*> *amigos= buscaListaAmigos(redeSocial, idPerfil); //recebe a lista de amigos do perfil
    for(int i=0; i < amigos->size(); i++){ //percorre toda lista de amigos
        if(!strcmp(amigos->at(i)->nome, nomeAmigo)){ //compara os nomes dos perfis com o nome do perfil a ser buscado
            //printf("\nAmigo %s encontrado na lista de amigos do perfil id=%d!",amigos->at(i)->nome, idPerfil);
            return amigos->at(i); //retorna o perfil amigo se encontrado
        }
    }
    //printf("\nAmigo %s n�o encontrado na lista de amigos do perfil id=%d!", nomeAmigo, idPerfil);
    return NULL;
}

/*6)-Buscar um perfil dentre os amigos dos amigos atrav�s do nome do perfil(busca em largura)
Solu��o: Vai ser feita uma busca em largura, percorrendo todos os amigos de cada perfil e
comparando com o nome do perfil a ser buscado.*/
Perfil *buscaPerfilAmigosAmigos(Grafo *redeSocial, int idPerfil, char *nome){
    for(int i=0; i < redeSocial->listaPerfis.size(); i++){ //percorre e seta todos os Perfis do grafo como nenhum visitado
        redeSocial->listaPerfis.at(i)->visitado = false;
    }
    deque<Perfil*> listaPerfis; //cria uma fila
    Perfil *aux;
    int i;
    aux = buscaPerfil(redeSocial, idPerfil); //busca o perfil inicial
    if(aux == NULL) return NULL; //se perfil n�o existir, fun��o acaba
    listaPerfis.push_back(aux); //adiciona o perfil inicial na ultima posi��o da fila(que � a primeira)
    while(!listaPerfis.empty()){ //vai continuar enquanto a fila n�o estiver vazia
        aux = listaPerfis.front();  //perfil aux recebe o perfil da primeira posi��o da fila
        listaPerfis.pop_front();    //retira o primeiro elemento da fila
        aux->visitado = true;       //marca como visitado o perfil aux no grafo, para n�o ser mais adicionado futuramente na fila
        //printf("\n-> Perfil %s", aux->nome);
        if(!strcmp(aux->nome, nome)){ //compara o nome do perfil a ser buscado com o nome do perfil aux
            printf("\nPerfil %s encontrado na lista de amigos dos amigos do perfil id=%i",aux->nome, idPerfil);
            return aux; //se os nomes dos perfis forem iguais, vai retornar o perfil que estava sendo buscado
        }
        for(i=0; i < aux->amigos->size(); i++){ //percorre toda a lista de amigos do perfil aux
            if(aux->amigos->at(i)->visitado == false){ //se os amigos do perfil aux ainda n�o foram visitados no grafo, ent�o vai adicionar esses amigos dentro da fila
                listaPerfis.push_back(aux->amigos->at(i)); //adiciona para a ultima posi��o da fila os perfis amigos de aux que ainda n�o foram visitados
                aux->amigos->at(i)->visitado= true; //depois de adicionado cada perfil a fila, seta eles como visitados
            }
        }
    }
    printf("\nPerfil %s n�o encontrado nos amigos dos amigos do perfil id=%i!",nome, idPerfil);
    return NULL;
}

//setar todos os perfis como n�o visitados
void resetaPerfisVisitados(Grafo *redeSocial){
    for(int i=0; i < redeSocial->listaPerfis.size(); i++){ //percorrer todos os perfis dos grafo
        redeSocial->listaPerfis.at(i)->visitado= false; //setar como todos n�o visitados
        redeSocial->listaPerfis.at(i)->distancia= INT_MAX; //setar como distancia de todos infinito(valor maximo de int)
    }
}

/*7)-Buscar um perfil pelo nome dentro dos amigos dos amigos de um perfil inicial, tendo como limite um n�vel m�ximo(busca em largura):
Solu��o: Vai ser feita uma busca em largura, percorrendo todos os amigos de cada perfil e comparando
com o id do perfil a ser buscado mas com a condi��o de n�o passar do n�vel m�ximo, cada n�vel ser�
aumentado quando visitar os amigos de cada perfil, quando chegar at� este n�vel e n�o ser encontrado,
finaliza e retorna nulo.*/
Perfil *buscaPerfilAmigosNivel(Grafo *redeSocial, int idPerfil, char *nomePerfilBusca, int nivelMax){
    printf("\nPerfil inicial id= %i, perfil a ser buscado %s, ate nivel maximo= %i.",idPerfil, nomePerfilBusca, nivelMax);
    resetaPerfisVisitados(redeSocial); //chama a fun��o de cima para resetar os perfis

    deque<Perfil*> listaPerfis; //cria uma fila
    Perfil *aux;
    int i;
    aux = buscaPerfil(redeSocial, idPerfil); //busca um determinado perfil para ser o inicial
    aux->distancia= 0;

    if(aux == NULL) return NULL;
    listaPerfis.push_back(aux); //add o perfil aux para a lista

    while(!listaPerfis.empty()){ //continua enquanto a fila n�o estiver vazia
        aux = listaPerfis.front(); //aux recebe o perfil da primeira posi��o da fila
        listaPerfis.pop_front(); //retira o primeiro elemento da fila
        aux->visitado = true; //aux seta como perfil j� visitado
        //printf("\n-> Perfil %s, acessando nivel %i", aux->nome, aux->distancia);
        if(!strcmp(aux->nome, nomePerfilBusca)){ //compara o nome do perfil aux com o nome do perfil a ser buscado
            printf("\nPerfil %s encontrado na lista de amigos dos amigos do perfil id=%i no nivel %i.",aux->nome, idPerfil, aux->distancia);
            return aux; //se os nomes forem iguais, retorna o perfil encontrado
        }
        for(i=0; i < aux->amigos->size(); i++){ //percorre toda lista de amigos do perfil aux
            //se amigos do perfil aux n�o forem visitados e a distancia do perfil aux for menor que o n�vel m�ximo, ent�o adiciona os amigos
            //quando a distancia do perfil aux for igual ao nivelMax, a fun��o vai parar de acrescentar perfis na fila
            if(aux->amigos->at(i)->visitado == false && aux->distancia < nivelMax){
                aux->amigos->at(i)->distancia= aux->distancia+1; //adiciona a distancia do perfil aux +1 na distancia do perfil amigo que ainda n�o havia sido visitado
                listaPerfis.push_back(aux->amigos->at(i)); //adiciona o perfil amigo na lista
                aux->amigos->at(i)->visitado= true; //seta o perfil como j� visitado
            }
        }
    }
    printf("\nPerfil %s nao encontrado nos amigos dos amigos ate nivel %i!", nomePerfilBusca, nivelMax);
    return NULL;
}

/*8)-Fun��o para verificar amigos em comuns entre dois perfis:
Solu��o: Primeiramente vai ser percorrida toda a lista de amigos do perfil1 e
em cada vez que percorrer um amigo do perfil1, vai pegar um amigo do perfil1 e comparar com todos os amigos do perfil2.
Os perfis que forem iguais na lista de amigos do perfil1 e perfil2 ser�o armazenados em outra lista que ser� retornada.*/
vector<Perfil*> *comparaAmigos(Grafo *redeSocial, int idPerfil1, int idPerfil2){
    vector<Perfil*> *comum = new vector<Perfil*>; //recebe lista de perfis em comum entre os dois perfis
    if(!existePerfil(redeSocial, idPerfil1) && !existePerfil(redeSocial, idPerfil1)){
        return comum;
    }
    vector<Perfil*> *amigosP1= buscaListaAmigos(redeSocial, idPerfil1); //recebe lista de amigos do perfil 1
    vector<Perfil*> *amigosP2= buscaListaAmigos(redeSocial, idPerfil2); //recebe lista de amigos do perfil 2

    //compara cada amigo da lista do perfil1 como todos os amigos do perfil 2
    for(int i=0; i < amigosP1->size(); i++){ //percorre a lista de amigos do perfil 1
        for(int j=0; j < amigosP2->size(); j++){ //percorre a lista de amigos do perfil 2
            if(amigosP1->at(i)->id == amigosP2->at(j)->id){ //se um id de amigos do perfil 1 for igual ao id de algum dos amigos do perfil 2, add  o perfil igual na lista comum
                comum->push_back(amigosP1->at(i)); //adiciona o perfil no final da lista, quando igual nos dois perfis
            }
        }
    }
    return comum;
}

/*9)- Fun��o para sugerir amigos para um perfil
Solu��o: Vai pegar toda a lista dos amigos de meus amigos e vai comparar com todos os perfis da minha lista de
amigos, os perfis que s�o amigos de meus amigos e n�o s�o meus amigos, v�o ficar nesta lista de sugestao de amigos.*/
vector<Perfil*> *sugerirAmigos(Grafo *redeSocial, int idPerfil){
    vector<Perfil*> *amigosSugerir= new vector<Perfil*>; //vetor dos amigos que ser�o sugeridos
    vector<Perfil*> *amigos= buscaListaAmigos(redeSocial, idPerfil); //vetor com a lista dos amigos do perfil
    resetaPerfisVisitados(redeSocial); //resetar todos os perfis como n�o visitados
    Perfil *setaPerfilVisitado= buscaPerfil(redeSocial, idPerfil); //buscar o perfil que receber� sugest�es
    setaPerfilVisitado->visitado = true;  //setar o perfil como visitado para ele n�o ser sugerido como amigo

    //percorrer a lista de amigos do perfil
    for(int i=0; i < amigos->size(); i++){
        vector<Perfil*> *listaAmigos_PerfilAmigo= buscaListaAmigos(redeSocial, amigos->at(i)->id); //recebe a lista de amigos de um amigo do perfil

        for(int j=0; j < listaAmigos_PerfilAmigo->size(); j++){

            /*pega 1 perfil amigo da minha lista e compara com a lista de amigos de um amigo meu
            se for diferente o id deles, vai pegar o amigo do meu amigo e comparar com todos os meus amigos novamente*/
            if(amigos->at(i)->id != listaAmigos_PerfilAmigo->at(j)->id){
                for(int x=0; x < amigos->size(); x++){
                    //se esse amigo do meu amigo estiver na minha lista, vou setar ele como visitado
                    if(listaAmigos_PerfilAmigo->at(j)->id == amigos->at(x)->id){
                        listaAmigos_PerfilAmigo->at(j)->visitado= true;
                    }
                }
                /*se o amigo de meu amigo n�o for visitado � porque ele n�o esta na minha lista
                e ser� adicionado na ultima posi��o da minha lista de amigos,
                depois de adicionado a minha lista, marca como visitado para n�o ser adicionado novamente*/
                if(listaAmigos_PerfilAmigo->at(j)->visitado == false){
                    amigosSugerir->push_back(listaAmigos_PerfilAmigo->at(j));
                    listaAmigos_PerfilAmigo->at(j)->visitado= true;
                }
            }
        }
    }
    //percorre a lista de amigos sugeridos se n�o for vazia e imprime os perfis, depois retorna a lista
    if(amigosSugerir->size() > 0){
        printf("\nAmigos Sugeridos para o perfil %d s�o:", idPerfil);
        for(int i=0; i < amigosSugerir->size(); i++){
            //printf("\nPerfil %d",amigosSugerir->at(i)->id);
        }
        return amigosSugerir;
    }else{
        //printf("\nNenhum amigo para sugerir!");
    return NULL;
    }
}

/*10)-Calcular dist�ncia entre Perfis
Solu��o: Pega o perfil1 como inicial e vai acessando todos os amigos dos seus amigos at�
chegar no perfil a ser buscado.
Perfil inicial, a dist�ncia � zero, cada amigo do perfil inicial(aux)
ter� a dist�ncia atual do perfil +1. Quando chegar ao perfil procurado vai exibir a distancia e retornar ela.*/
int distanciaEntrePerfis(Grafo *redeSocial, int idPerfil1, int idPerfil2){
    resetaPerfisVisitados(redeSocial);
    printf("\nPerfil inicial id= %i, perfil final id= %i:",idPerfil1, idPerfil2);

    deque<Perfil*> listaPerfis;
    Perfil *aux;
    int i;
    aux = buscaPerfil(redeSocial, idPerfil1); //busca primeiro perfil
    aux->distancia= 0;

    if(aux == NULL) return NULL;
    listaPerfis.push_back(aux); //add perfil a lista

    //aux sempre vai recebendo os perfis na primeira posi��o da lista, e comparando se � o perfil a ser buscado
    while(!listaPerfis.empty()){ //s� para quando a lista for vazia
        aux = listaPerfis.front();
        listaPerfis.pop_front();
        aux->visitado = true;
        //printf("\n-> Perfil %s, distancia = %i", aux->nome, aux->distancia);
        if(aux->id == idPerfil2){
            printf("\nDistancia do perfil id=%i at� perfil id=%i ficou = %i",idPerfil1, idPerfil2, aux->distancia);
            return aux->distancia;
        }
        //vai percorrer os amigos de aux, verificando se n�o foram visitados, se n�o visitados ainda,
        //recebem a distancia do perfil aux +1 e s�o adicionados na ultima posi��o na lista
        for(i=0; i < aux->amigos->size(); i++){
            if(aux->amigos->at(i)->visitado == false){
                aux->amigos->at(i)->distancia= aux->distancia+1;
                listaPerfis.push_back(aux->amigos->at(i));
                aux->amigos->at(i)->visitado= true;
            }
        }
    }
    printf("\nN�o foi encontrada distancia entre os dois perfis.");
    return NULL;
}

//busca por profundidade no grafo
int dfs(Grafo *redeSocial, int idPrincipal, int idBusca){
    resetaPerfisVisitados(redeSocial);

    stack<Perfil*> pilha;
    Perfil *aux;
    int i;
    aux= buscaPerfil(redeSocial, idPrincipal);

    if(aux == NULL){ return 0;}
    pilha.push(aux);

    while(!pilha.empty()){
        aux= pilha.top();
        pilha.pop();
        aux->visitado = true;

        if(aux->id == idBusca){
            return 1;
        }

        for(i=0; i < aux->amigos->size(); i++){
            if(aux->amigos->at(i)->visitado == false){
                pilha.push(aux->amigos->at(i));
            }
        }
    }
    return 0;
}

/*11)-Verificar grupos de amizades
Solu��o: Primeiro verifica se o grafo � conexo,
*/
vector<vector<Perfil*>*> *gruposAmizade(Grafo *redeSocial){
    vector<vector<Perfil*>*> *listaFinal= new vector<vector<Perfil*>*>; //recebe a lista final dos componentes conexos
    vector<Perfil*> *perfisConexos;     //recebe os perfis tempor�rios que s�o de um componente conexo
}

/*12)-Fus�o de perfis
Solu��o: Deixa o ultimo perfil da lista sendo o que vai ser fundido, depois pega cada perfil da lista menos o �ltimo
e compara os amigos em comuns com o perfil definitivo. Ap�s isto, pega os amigos que sobraram da lista do perfil aux
e adiciona esses amigos no perfil definitivo. Depois percorre toda a lista de perfis do grafo e recebe os perfis
que tem o perfil aux como amigo deles, deleta a amizade vice e versa do perfil aux com esses perfis encontrados
e adiciona o perfil definitivo como amigo desses perfis encontrados.*/
Perfil *fusaoPerfis(Grafo *redeSocial, vector<Perfil*> listaPerfis){
    printf("\nTamanho da lista de perfis para fundir= %i", listaPerfis.size());
    int ult= (listaPerfis.size())-1;
    if(ult < 1){ //se lista for menor que dois perfis para fundir vai terminar execu��o
        printf("\nLista de perfis menor que 2, nenhuma a��o realizada!");
        return NULL;
    }

    Perfil *resBusca; //receber� futuramente a lista dos amigos de um perfil, no final da fun��o
    Perfil *definitivo = buscaPerfil(redeSocial, listaPerfis[ult]->id); //perfil definitivo vai ser o perfil que recebera a fus�o dos outros perfis
    Perfil *aux;    //receber� cada perfil que sera fundido com o definitivo
    vector<Perfil*> *amigosComum; //lista de amigos em comum entre dois perfis(o definitivo e o que estiver comparando)

    //percorre toda lista dos perfis a serem fundidos menos o �ltimo, que � o definitivo
    for(int i=0; i < ult; i++){
        aux= buscaPerfil(redeSocial, listaPerfis[i]->id); //busca perfil(da lista) dentro do grafo
        amigosComum = comparaAmigos(redeSocial, definitivo->id, aux->id); //compara os amigos entre o perfil definitivo e um da lista de perfis a serem fundidos

        //percorre a lista de amigos em comum, pega cada elemento da lista em comum e compara com todos os amigos
        //do perfil aux, os mesmos amigos que estiverem na lista amigosComum e na lista de amigos do perfil aux
        //esses perfis v�o ser deletados da lista de amigos do perfil aux
        if(amigosComum != NULL){
            for(int x=0; x < amigosComum->size(); x++){
                for(int xx=0; xx < aux->amigos->size(); xx++){
                    if(aux->amigos->at(xx)->id == amigosComum->at(x)->id){
                        removeAmigo(redeSocial, aux->id, amigosComum->at(x)->id);
                    }
                }
            }
        }
        //percorre a lista de amigos do perfil aux e adiciona os amigos que sobraram, como amigos do perfil definitivo
        if(aux->amigos->size() > 0){
            for(int y=0; y < aux->amigos->size(); y++){
                addAmigo(redeSocial, definitivo->id, aux->amigos->at(y)->id);
            }
        }
        //percorre toda a lista de perfis do grafo, encontrando os perfis que tem como amigo o perfil aux
        for(int z=0; z < redeSocial->listaPerfis.size(); z++){
            resBusca= buscaPerfilAmigos(redeSocial, redeSocial->listaPerfis[z]->id, aux->nome);
            if(resBusca != NULL){
                removeAmigo(redeSocial, redeSocial->listaPerfis[z]->id, aux->id); //remove o perfil aux da lista de amigos do perfil encontrado
                removeAmigo(redeSocial, aux->id, redeSocial->listaPerfis[z]->id); //remove o perfil encontrado da lista de amigos do perfil aux
                addAmigo(redeSocial, redeSocial->listaPerfis[z]->id, definitivo->id); //adiciona o perfil definitivo como amigo do perfil encontrado
            }
        }
    }
    printf("\nPerfis da lista foram unidos para o perfil %s", definitivo->nome);
    return definitivo;
}

/*13)- Detectar perfis falsos
Solu��o: Primeiro pega a quantidade total de arestas do grafo e divide pelo total de perfis do grafo.
Esse valor � a m�dia de amigos por perfil do grafo, depois fiz uma conta para dar o limiar.
Ap�s o limiar definido, vai ser percorrido todos os perfis do grafo, os perfis que tiverem o
o n�mero de amigos menor ou igual ao limiar entram na lista de perfis falsos.*/
vector<Perfil*> *buscaPerfisFalsos(Grafo *redeSocial){
    int mediaAmigos= 0;
    int limite= 0;
    vector<Perfil*> *listaPerfisFalsos= new vector<Perfil*>; //vai receber os perfis falsos
    vector<Perfil*> *listaAmigos;

    //recebe o numero de arestas do grafo(n� de amigos totais) e divide pela quantidade de perfis do grafo
    mediaAmigos= redeSocial->m / redeSocial->listaPerfis.size();
    printf("\n\nTotal de amigos da rede social = %i", redeSocial->m);
    printf("\nTotal de perfis na rede social = %i", redeSocial->listaPerfis.size());
    printf("\nMedia de amigos por perfil ficou em %i", mediaAmigos);

    //limiar para perfis falsos: (metade do valor da m�dia)
    limite= mediaAmigos * 0.5;
    printf("\nPerfis com numero de amigos <= %i serao considerados perfis falsos!", limite);
    //printf("\n\nLista de perfis considerados falsos na rede social:");

    //percorre todo o grafo e adiciona a lista de perfis falsos os perfis que tiverem amigos menor que o limiar estabelecido
    for(int i=0; i < redeSocial->listaPerfis.size(); i++){
        listaAmigos= buscaListaAmigos(redeSocial, redeSocial->listaPerfis.at(i)->id);
        if(listaAmigos->size() <= limite){
            listaPerfisFalsos->push_back(redeSocial->listaPerfis.at(i));
            //printf("\nPerfil %s;", redeSocial->listaPerfis.at(i)->nome);
        }
    }
    if(listaPerfisFalsos->size() <= 0){
        printf("\nNenhum perfil falso!");
        return NULL;
    }
    return listaPerfisFalsos;
}

/*14)-Detectar perfis influenciadores
Solu��o: Primeiro pega a quantidade total de arestas do grafo e divide pelo total de perfis do grafo.
Esse valor � a m�dia de amigos por perfil do grafo, depois fiz uma conta para dar o limiar.
Ap�s o limiar definido, vai ser percorrido todos os perfis do grafo, os perfis que tiverem o
o n�mero de amigos maior ou igual ao limiar entram na lista de perfis influenciadores.*/
vector<Perfil*> *buscaPerfisInfluenciadores(Grafo *redeSocial){
    int mediaAmigos= redeSocial->m / redeSocial->listaPerfis.size();
    int limite= 0;
    vector<Perfil*> *listaPerfisInfluenciadores= new vector<Perfil*>;
    vector<Perfil*> *listaAmigos;
    printf("\n\nTotal de amigos da rede social = %i", redeSocial->m);
    printf("\nTotal de perfis na rede social = %i", redeSocial->listaPerfis.size());
    printf("\nMedia de amigos por perfil ficou em %i", mediaAmigos);

    //limiar para perfis influenciadores: (pega o valor da m�dia e soma com a metade do valor da m�dia)
    limite= mediaAmigos + (mediaAmigos*0.5);

    printf("\nPerfis com numero de amigos >= %i serao considerados perfis influenciadores!", limite);
    //printf("\n\nLista de perfis considerados influenciadores na rede social:");
    for(int i=0; i < redeSocial->listaPerfis.size(); i++){
        listaAmigos= buscaListaAmigos(redeSocial, redeSocial->listaPerfis.at(i)->id);
        if(listaAmigos->size() >= limite){
            listaPerfisInfluenciadores->push_back(redeSocial->listaPerfis.at(i));
            //printf("\nPerfil %s;", redeSocial->listaPerfis.at(i)->nome);
        }
    }
    if(listaPerfisInfluenciadores->size() <= 0){
        printf("\nNenhum perfil influenciador!");
        return NULL;
    }
    return listaPerfisInfluenciadores;
}

//fun��o para ler o arquivo e salvar dados no grafo
void lerArquivo(Grafo *redeSocial, char nomeArquivo[]){
    int idPerfil, idAmigo, cont=0, i=0;
    char texto[10];

    FILE *arquivoLer= fopen(nomeArquivo, "r");
    while(fgets(texto, 10, arquivoLer)!= NULL){
        cont++;
    }
    fclose(arquivoLer);

    FILE *arquivo= fopen(nomeArquivo, "r");
    while(i < cont){
        fscanf(arquivo, "%i %i", &idPerfil, &idAmigo);
        //se um perfil n�o existir no grafo, dentro dessa fun��o "addamigo" vai ser chamada a
        //fun��o para adicionar perfil na rede social
        addAmigo(redeSocial, idPerfil,  idAmigo);
        i++;
    }
    fclose(arquivo);
}

int main(){
    int escolha;
    char arquivoTeste[]={"teste.txt"};
    char arquivoLerFacebook[]={"arquivo_facebook.txt"};
    char arquivoLerOrkut[]={"arquivo_orkut.txt"};

    printf("Qual rede social deseja trabalhar? Apenas digite 1, 2 ou 3.\nSe Facebook= 1\nSe Orkut=2\nSe arquivo Teste=3\n");
    scanf("%i",&escolha);
    system("cls");

    Grafo *teste = new Grafo();

    if(escolha == 1){
        printf("Rede Social Facebook selecionada!");
        //Essa chamada de fun��o, dentro dela v�o ser chamadas as fun��es 1 e 2, adicionar perfis e adicionar amigos
        lerArquivo(teste, arquivoLerFacebook);
    }else if(escolha == 2){
        printf("Rede Social Orkut selecionada!");
        //Essa chamada de fun��o, dentro dela v�o ser chamadas as fun��es 1 e 2, adicionar perfis e adicionar amigos
        lerArquivo(teste, arquivoLerOrkut);
    }else if(escolha == 3){
        //Essa chamada de fun��o, dentro dela v�o ser chamadas as fun��es 1 e 2, adicionar perfis e adicionar amigos
        printf("Rede Social Teste selecionada!");
        lerArquivo(teste, arquivoTeste);
    }

    printf("\n\n");
    system("pause");
    system("cls");

    printf("\n\n1-Criacao de um perfil:");
    addPerfil(teste, 1,"Igor1");
    addPerfil(teste, 1,"Igor1");
    addPerfil(teste, 5,"Igor5");
    addPerfil(teste, 7,"Igor7");
    addPerfil(teste, 10,"Igor10");
    printf("\n\n");
    system("pause");
    system("cls");

    printf("\n\n2-Adicao de amigo em um perfil:");
    addAmizade(teste, 5, 1); //essa fun��o addAmiza faz a fun��o chama duas vezes a fun��o addAmigo
    addAmizade(teste, 10, 7);
    addAmigo(teste, 8, 9);
    printf("\n\n");
    system("pause");
    system("cls");

    //imprimeRedeSocial(teste);

    printf("\n\n3-Remove amigo de um perfil:");
    removeAmigo(teste, 8, 9);
    removeAmizade(teste, 50, 7);
    //imprimeRedeSocial(teste);
    printf("\n\n");
    system("pause");
    system("cls");

    printf("\n\n4-Busca um perfil na rede social pelo nome:");
    Perfil *buscar= buscaNomePerfil(teste,"Igor99");
    printf("\nPerfil %s encontrado!", buscar->nome);
    printf("\n\n");
    system("pause");
    system("cls");

    printf("\n\n5)-Funcao de buscar um amigo dentro da lista de amigos de um perfil:");
    Perfil *buscaAmigo1= buscaPerfilAmigos(teste, 15, "Igor100");
    if(buscaAmigo1 != NULL){
        printf(" Amigo %s encontrado!",buscaAmigo1->nome);
    }else{
        printf("Perfil nao encontrado!");
    }
    Perfil *buscaAmigo2= buscaPerfilAmigos(teste, 71, "Igor12");
    if(buscaAmigo2 != NULL){ printf(" Amigo %s encontrado!",buscaAmigo2->nome);}
    Perfil *buscaAmigo3= buscaPerfilAmigos(teste, 11, "Igor9");
    if(buscaAmigo3 != NULL){ printf(" Amigo %s encontrado!",buscaAmigo3->nome);}
    Perfil *buscaAmigo4= buscaPerfilAmigos(teste, 7, "Igor30");
    if(buscaAmigo4 != NULL){ printf(" Amigo %s encontrado!",buscaAmigo4->nome);}
    printf("\n\n");
    system("pause");
    system("cls");

    printf("\n\n6)-Buscar um perfil dentre os amigos dos amigos de um perfil, atraves do nome do perfil a ser buscado(busca em largura):");
    buscaPerfilAmigosAmigos(teste, 1, "Igor9");
    buscaPerfilAmigosAmigos(teste, 20, "Igor99");
    buscaPerfilAmigosAmigos(teste, 20, "Igor35");
    printf("\n\n");
    system("pause");
    system("cls");

    printf("\n\n7)-Buscar um perfil pelo nome dentro dos amigos dos amigos de um perfil inicial, tendo como limite um nivel maximo(busca em largura):");
    Perfil *buscaAmigoNivel= buscaPerfilAmigosNivel(teste, 20, "Igor99", 4);
    if(buscaAmigoNivel != NULL){ printf("\nPerfil %s encontrado nos amigos dos amigos por nivel!", buscaAmigoNivel->nome);}
    printf("\n\n");
    system("pause");
    system("cls");

    printf("\n\n8)-Funcao para verificar amigos em comuns entre dois perfis:");
    vector<Perfil*> *amigosComum1= comparaAmigos(teste, 11, 71);
    if(amigosComum1->size() > 0){
        printf("\nAmigos em comum recebidos:");
        for(int i=0; i < amigosComum1->size(); i++){
            printf("\nPerfil %s.",amigosComum1->at(i)->nome);
        }
    }else{
        printf("\nSem amigos em comum!");
    }
    printf("\n\n");
    system("pause");
    system("cls");

    printf("\n\n9)- Funcao de sugerir amigos para um perfil:");
    vector<Perfil*> *listaSugerir1= sugerirAmigos(teste, 20);
    printf("\nAmigos sugeridos para o perfil id=20:\n");
    for(int i=0; i < listaSugerir1->size(); i++){
        printf("%s! ",listaSugerir1->at(i)->nome);
    }
    printf("\n\n");
    system("pause");
    system("cls");

    printf("\n\n10)-Calcular distancia entre Perfis:");
    int dist= distanciaEntrePerfis(teste, 20, 6);
    if(dist > 0){
        printf("\nDistancia entre os dois perfis = %i", dist);
    }

    printf("\n\n");
    system("pause");
    system("cls");

    printf("\n\n12)-Fusao de perfis:");
    vector<Perfil*> perfisFundir;
    Perfil *fusao, *teste1, *teste2, *teste3;
    teste1= buscaPerfil(teste, 9);
    teste2= buscaPerfil(teste, 5);
    teste3= buscaPerfil(teste, 6);
    perfisFundir.push_back(teste1);
    perfisFundir.push_back(teste2);
    perfisFundir.push_back(teste3);
    fusao = fusaoPerfis(teste, perfisFundir);
    printf("\n\nPerfil final da fusao ficou o %s de id= %i", fusao->nome, fusao->id);
    printf("\nImpressao de amigos do Perfi %s:\n", fusao->nome);
    imprimeAmigos(fusao->amigos);
    Perfil *nove= buscaPerfil(teste, 9);
    printf("\nImpressao de amigos do Perfi %s:\n", nove->nome);
    imprimeAmigos(nove->amigos);
    Perfil *cinco= buscaPerfil(teste, 5);
    printf("\nImpressao de amigos do Perfi %s:\n", cinco->nome);
    imprimeAmigos(cinco->amigos);
    printf("\nPerfis que foram fundidos com o perfi %s serao deletados da rede social!",fusao->nome);
    if(perfisFundir.size() > 1){
        for(int i=0; i < perfisFundir.size()-1; i++){
            teste= removePerfil(teste, perfisFundir[i]->id);
        }
    }
    printf("\n\n");
    system("pause");
    system("cls");

    printf("\n\n13)-Detectar perfis falsos na rede social:");
    vector<Perfil*> *perfisFalsos= buscaPerfisFalsos(teste);
    if(perfisFalsos != NULL){
        printf("\nLista dos perfis falsos:");
        for(int i=0; i < perfisFalsos->size(); i++){
            printf("\nPerfil %s", perfisFalsos->at(i)->nome);
        }
    }
    printf("\n\n");
    system("pause");
    system("cls");

    printf("\n\n14)-Detectar Perfis Influenciadores:");
    vector<Perfil*> *perfisInfluenciadores= buscaPerfisInfluenciadores(teste);
    if(perfisInfluenciadores != NULL){
        printf("\nLista dos perfis influenciadores:");
        for(int i=0; i < perfisInfluenciadores->size(); i++){
            printf("\nPerfil %s", perfisInfluenciadores->at(i)->nome);
        }
    }
    printf("\n\n");
    return 0;
}
