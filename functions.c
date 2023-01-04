#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#define STRING_SIZE 200

/*
 * Decide se o programa corre em modo gráfico ou modo textual
 * \param argc variável que guarda o número de argumentos
 * \param argv variável que guarda o argumento introduzido
  */
int program_mode(int argc, char *argv[])
{
  int i=0;
  char t[2];
  char g[2];
  int  conf=1;

  strcpy(t,"-t");
  strcpy(g,"-g");

  for(i=0; i<argc; i++)
  {
    conf = strcmp(argv[i], g); // Procura o comando g nos argumentos e caso encontre retorna 0; O programa entra no modo gráfico
    if(conf==0)
    {
      return 1;
    }
    conf = strcmp(argv[i], t); // Procura o comando t nos argumentos e caso encontre retorna 1; O programa entra no modo textual
    if(conf==0)
    {
      return 0;
    }

  }

  printf("Impossível abrir o programa\n");
  exit(EXIT_FAILURE);
}

//----------------------------- Country file ----------------------------------
/*
 * Lê o ficheiro com os dados dos países
 * \param *headcountry cabeça da lista de países
 */
node_country* LoadCountryFile()
{
  int minimo=2020; // guarda o menor ano
  int maximo=0; // guarda o maior ano
  char _str[STRING_SIZE];
  node_country *NodeCountry = NULL;
  node_country *tailcountry = NULL; // Tail da lista de países
  node_country **vetor_anos= NULL; // vetor que tem em cada posição os diferentes anos existentes no ficheiro. Em cada ano os dados de cada país estão organizados numa lista por mês e dia
  node_country *headCountry = NULL;
  int sizeofvector=0; // tamanho do vetor de anos
  FILE *f2 = NULL;


  f2= fopen("tempcountries.csv", "r");

  if(f2==NULL){
    printf("Não foi possível abrir o ficheiro.");
    exit(EXIT_FAILURE);
  }

  while(fgets(_str, STRING_SIZE, f2) != NULL){
      NodeCountry= CreateNewNode2(_str);
    if( NodeCountry != NULL){
      InsertTailCountry(&headCountry, NodeCountry,&tailcountry);
    }
  }
   calc_min(&minimo, headCountry);
   calc_max(&maximo, headCountry);
   vetor_anos = create_vetor(vetor_anos,minimo, maximo,&sizeofvector,headCountry); // cria o vetor de listas
   headCountry= listconvert(vetor_anos, sizeofvector,headCountry); // converte o vetor de listas numa lista ordenada por anos
   fclose(f2);
   return headCountry;
}

/*
 * Cria um novo node para a lista de países
 */
node_country* CreateNewNode2 (char str[STRING_SIZE])
{
  char tmp_country[STRING_SIZE];
  char *pointer=NULL;
  char find[3];
  node_country *new_node=NULL;
  strcpy(find, ",,");

  pointer = strstr(str, find); // se alguma das linhas do ficheiro não tiver algum dos dados a sua informação não é passada para a estrutura

  if(pointer == NULL){
    new_node = (node_country*)malloc(sizeof(node_country));

    if(new_node == NULL)
    {
      printf("Memory not allocated\n");
      exit(EXIT_FAILURE);
    }
    sscanf(str,"%d-%d-%d,%f,%f,%[^\n]", &(new_node->countryload.dt.ano),&(new_node->countryload.dt.mes),&(new_node->countryload.dt.dia),
    &(new_node->countryload.temperatura), &(new_node->countryload.incerteza), tmp_country);

    if(new_node == NULL){
      printf("Erro ao reservar memória\n");
      exit(EXIT_FAILURE);
    }

    new_node->countryload.pais=(char*)malloc((strlen(tmp_country)+1)*sizeof(char)); // alocação de memória para o nome do país

    if (new_node->countryload.pais == NULL){
      printf("Erro ao reservar memória\n");
      exit(EXIT_FAILURE);}
      tmp_country[strlen(tmp_country)-1]='\0';
      strcpy(new_node->countryload.pais, tmp_country);
      tmp_country[0]='\0';

      if(new_node->countryload.dt.ano == 0){
        free(new_node);
      }

      new_node->next=NULL;
      new_node->prev=NULL;
      return new_node;
    }
    return NULL;
  }

  /*
   * Calcula o ano minimo
   * \param minimo pointeiro para o menor ano
   * \param head cabeça da lista de países
   */
  void calc_min(int *minimo, node_country* head){

    node_country *aux= NULL;

    aux = head;
    while( aux!= NULL){
      if(aux->countryload.dt.ano < (*minimo)){
        *minimo = aux->countryload.dt.ano; // se ao percorrer a lista encontrar um valor menor que o minimo atual, substitui-o
      }
      aux = aux-> next;
    }
  }

  /*
   * Calcula o ano máximo
   * \param maximo pointeiro para o maior ano
   * \param head cabeça da lista de países
   */
  void calc_max(int *maximo, node_country* head){

    node_country *aux= NULL;

    aux = head;
    while( aux!= NULL){
      if(aux->countryload.dt.ano > (*maximo)){
        *maximo = aux->countryload.dt.ano; // se ao percorrer a lista encontrar um valor maior que o maximo atual, substitui-o
      }
      aux = aux-> next;
    }

  }

//---------------------- Ordenação do Country file -----------------------------

/*
 * Calcula o tamanho do vetor
 * \param vetor_anos vetor que tem em cada posição uma lista para cada ano
 */
  node_country** vetor_size(node_country** vetor_anos, int minimo, int maximo, int* sizeofvector){

    *sizeofvector = (maximo - minimo)+1; // O tamanho do vetor obtem-se fazendo a diferença entre o ano de maior valor e o ano de menor valor, pois existem tantas posições como anos
    vetor_anos = (node_country**)calloc((*sizeofvector), sizeof(node_country*));

    return vetor_anos;
  }

  /*
   * Remove a cabeça da lista desordenada para analisar e colocar na posição correta do vetor
   */
  node_country* removehead(node_country* head, node_country** node){

    (*node)=head;
    head = head->next; // a cabeça da lista passa a ser no node seguinte
    if(head!=NULL) {
      (*node)->next = NULL; // A antiga cabeça da lista fica sem ligação à nova cabeça
      head->prev=NULL;
    }
    return head;
  }

  /*
   * Função que retorna o vetor de anos
   */
  node_country** create_vetor(node_country** vetor_anos, int minimo, int maximo, int* sizeofvector, node_country* head){

    int index_posicao=0; // índice da posição do nó no vetor
    node_country* posicao= NULL;

    node_country* node = NULL;
    node_country* tail= NULL;
    vetor_anos = vetor_size(vetor_anos, minimo, maximo, &(*sizeofvector));
    if(vetor_anos == NULL){
      printf("Vetor vazio durante a escrita");
      return vetor_anos;
    }
    while(head!=NULL){
      head = removehead(head,&node);
      index_posicao= (node->countryload.dt.ano) - minimo; // A posição (índice) do vetor onde a head removida é inserida é calculada fazendo a diferença entre o ano lido e o mínimo
      if(vetor_anos[index_posicao]==NULL){
        vetor_anos[index_posicao]= node; // se ainda não existir nenhum nó nessa posição, o vetor nesse índice passa a ser igual ao node que está a ser inserido
      }
      else{
        posicao= findposition(vetor_anos[index_posicao], node,&tail); // se já existir algum node nessa posição, calcula a posição correta para inserir de forma ordenada em relação aos restantes
        if(posicao == NULL){
          if(tail == NULL){
            printf("Tail= NULL.");
          }
          tail->next=node; // insere manualmente a tail da lista
          node->prev=tail;
        }
        else{
          vetor_anos[index_posicao] = insertnode_vetor(posicao,vetor_anos[index_posicao],node); // insere o node no vetor
        }
      }
    }

    return vetor_anos;
  }

  /*
  * Função que insere o nó da lista no vetor
  \param posicao variável do tipo da estrutura que indica a posição do vetor em que o nó será inserido
  \param node nó a inserir
  */
  node_country* insertnode_vetor(node_country* posicao, node_country *head, node_country* node){

    if(head == NULL){
      return node;
    }
    if(head==posicao){
      head->prev=node;
      node->next=head;
      return node;
    }
    node->next=posicao;
    node->prev= posicao->prev;
    node->prev->next=node;
    node->next->prev=node;
    return head;
    // Não insere na tail da lista
  }

  /*
  * Função que encontra a posição correta para inserir de forma ordenada o nó na lista
  */
  node_country* findposition(node_country* head, node_country *node, node_country** tail){

    node_country* aux= NULL;
    node_country* tmp= NULL;
    *tail = NULL;

    aux= head;
    // Os nós inseridos em cada lista têm todos o mesmo ano, logo, serão ordenados por mês e dia
    while(aux != NULL){
      if(node->countryload.dt.mes < aux->countryload.dt.mes){ // ordenação da lista por data
        return aux;
      }
      if(node->countryload.dt.mes == aux->countryload.dt.mes){
        if(node->countryload.dt.dia < aux->countryload.dt.dia)
        return aux;
      }
      tmp = aux;
      aux= aux->next;
    }

    *tail= tmp;
    return aux;
  }


  /*
  * Função que converte o vetor de listas numa só lista ordenada
  */
  node_country* listconvert(node_country** vetor_anos, int sizeofvector,node_country* head){

    // Une a tail de cada posição à head da posição seguinte
    int i=0;
    node_country *aux = NULL;
    node_country *tail= NULL;

    head=vetor_anos[0]; // inicializa-se a head na posição zero do vetor

    for(i=0; i<sizeofvector-1; i++){
      aux= vetor_anos[i];
      while(aux->next != NULL){
        aux= aux->next;
      }
      while(vetor_anos[i+1]== NULL && i<sizeofvector){ // procura a próxima posição não nula do vetor, pois poderão existir anos sem dados
        i++;
      }
      tail = aux;
      tail->next = vetor_anos[i+1];
      vetor_anos[i+1]->prev= tail;
    }
    return head;
  }

  /*
  * Função que insere os novos nós na tail da lista
  */
  void InsertTailCountry(node_country** head, node_country* new_node, node_country **tail)
  {
    // Lista vazia?
    if ( *head == NULL)
    {
      *head = new_node;
      *tail = new_node;
      return;
    }

    (*tail)->next = new_node;
    new_node->prev = (*tail);
    *tail=new_node;

  }

  /*
  * Função que limpa e liberta a memória utilizada na lista de países
  */
  node_country* clearcountrylist(node_country *head){

    node_country *aux= NULL;
    node_country *tmp=NULL;

    aux = head;

    if(aux == NULL){
      printf("Variável auxiliar vazia.\n");
    }

    while(aux != NULL){
      tmp = aux->next;
      free(aux->countryload.pais);
      free(aux);
      aux = tmp;
    }
    head=NULL;
    return head;
  }

//--------------------------- City File ---------------------------------

/*
* Função que descarrega e lê o ficheiro com as informações das cidades. Cria uma lista de listas
*/
nodeF* LoadCityFile()
{
  char tmp_city[STRING_SIZE];
  char str[STRING_SIZE];
  char tmp_country[STRING_SIZE];
  char *pointer;
  char find[3];
  nodeV *newNodeV=NULL;
  nodeF *newNodeF=NULL;
  char lastcity[STRING_SIZE] = {0};
  strcpy(find, ",,");
  FILE* f1=NULL;
  float longitude=0.0;
  float latitude=0.0;
  char direcao_Long;
  char direcao_Lat;
  nodeF *tailFixed=NULL;
  nodeV *tailVar=NULL;
  nodeF *headFixed=NULL;

  f1 = fopen("tempcities.csv", "r");

    if(f1==NULL){
    printf("Não foi possível abrir o ficheiro.");
     exit(EXIT_FAILURE);
    }

    fgets(str, STRING_SIZE, f1);
    while(fgets(str, STRING_SIZE, f1) != NULL){

      pointer = strstr(str, find); // se encontrar ",," significa que há dados em falta nessa linha e esta é ignorada, i.e, não é escrita num novo nó

      if(pointer == NULL)
      {
        newNodeV=createNodeV();
         sscanf(str,"%d-%d-%d,%f,%f,%[^,],%[^,],%f%c,%f%c", &(newNodeV->dataV.dt.ano),&(newNodeV->dataV.dt.mes),&(newNodeV->dataV.dt.dia),
         &(newNodeV->dataV.temperatura), &(newNodeV->dataV.incerteza), tmp_city, tmp_country, &latitude, &direcao_Lat, &longitude,
         &direcao_Long);

          // lista de listas é constituída por uma lista horizontal com os dados fixos e uma lista vertical para cada node da horizontal que contém os dados variáveis
            if(strcmp(lastcity, tmp_city)!=0) // compara a cidade anterior guardada com a cidade atual e se forem diferentes cria um novo node na lista horizontal
            {
              newNodeF= createNodeF(tmp_city, tmp_country);

              // variáveis auxiliares que guardam o ângulo e direção das coordenadas geográficas (latitude e longitude)
              newNodeF->dataF.latitude.angular=latitude;
              newNodeF->dataF.latitude.direcao=direcao_Lat;
              newNodeF->dataF.longitude.angular=longitude;
              newNodeF->dataF.longitude.direcao=direcao_Long;

              if(headFixed==NULL)
              {
                headFixed=newNodeF;
                tailFixed=newNodeF;
              }
                else{
                  lastcity[0]='\0';
                  tailFixed->next=newNodeF;
                  newNodeF->prev = tailFixed;
                  tailFixed = newNodeF;
               }

            strcpy(lastcity, tmp_city); // nova last city

            newNodeV->headF=tailFixed;

            tailFixed->headV=newNodeV;
            tailVar=newNodeV;
          }
            else
            {
                tailVar->next=newNodeV;
                newNodeV->prev=tailVar;
                tailVar= newNodeV;
           }

           tmp_city[0]='\0';
           tmp_country[0]='\0';
      }
    }

    fclose(f1);
    return headFixed;
  }

  /*
  * Função que cria um novo nó Fixo para a lista horizontal
  */
  nodeF* createNodeF(char tmp_city[], char tmp_country[])
  {
    nodeF* newNodeF=NULL;

    newNodeF = (nodeF*)malloc(sizeof(nodeF));

    if(newNodeF == NULL)
    {
      printf("Memory not allocated\n");
      exit(EXIT_FAILURE);
    }

    newNodeF->dataF.cidade=(char*)malloc((strlen(tmp_city)+1)*sizeof(char)); // alocação de memória para o nome da cidade

    if (newNodeF->dataF.cidade == NULL){
      printf("Erro ao reservar memória\n");
      exit(EXIT_FAILURE);}

      strcpy(newNodeF->dataF.cidade, tmp_city);

      newNodeF->dataF.pais=(char*)malloc((strlen(tmp_country)+1)*sizeof(char)); // alocação de memória para o nome do país

      if (newNodeF->dataF.pais == NULL){
        printf("Erro ao reservar memória\n");
        exit(EXIT_FAILURE);}

        strcpy(newNodeF->dataF.pais, tmp_country);


        newNodeF->next=NULL;
        newNodeF->prev=NULL;

        return newNodeF;

      }

  /*
  * Função que cria um novo node variável para a lista vertical
  */
nodeV* createNodeV()
  {
    nodeV* newNodeV=NULL;

    newNodeV = (nodeV*)malloc(sizeof(nodeV));

      if(newNodeV == NULL)
      {
        printf("Memory not allocated\n");
        exit(EXIT_FAILURE);
      }

    newNodeV->next=NULL;
    newNodeV->prev=NULL;

    return newNodeV;
  }

/*
* Função que limpa e liberta a memória utilizada na lista horizontal
*/
nodeF* clearcitylistH(nodeF *head){

    nodeF *auxF= NULL;
    nodeV *auxV=NULL;
    nodeF *tmpF=NULL;
    nodeV *tmpV=NULL;
    auxF=head;

    while(auxF!=NULL)
    {
      auxV=auxF->headV;
        while(auxV!=NULL){
          tmpV=auxV->next;
          free(auxV);
          auxV=tmpV;
      }

     tmpF=auxF->next;
     free(auxF->dataF.cidade);
     free(auxF->dataF.pais);
     free(auxF);
     auxF=tmpF;
    }
    head=NULL;
    return head;
}
