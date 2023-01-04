#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menutex.h"
#include "data.h"
#include "functions.h"
#define STRING_SIZE 200

/**
 * TextualMode:Controla o menu principal do Modo Textual da aplicação
 * \param headFixed: Cabeça da listas de listas
 * \param headCountry: Cabeça da lista das cidades
 */
void TextualMode(nodeF *headFixed, node_country *headCountry)
{
  int mainOp=0;
  printfMainMenu(&mainOp);

 while(mainOp!=5){

  if(mainOp==1)
   filtrationOption(headFixed, &headCountry);

  else if(mainOp==2)
   tempHist(headCountry, headFixed);

  else if(mainOp==3)
   AnaliseTemp(headCountry, headFixed);

  printfMainMenu(&mainOp);

  }

}

//-----------------------------------------------------Filtragem de dados--------------------------------------------
/**
 * filtrationOption: Controla o menu da Filtração
 * \param headFixed: Cabeça da lista por lista para percorrer e eliminar nódulos
 * \param headCountry: Cabeça da lista dos países para percorrer e eliminar nódulos
 */
 void filtrationOption(nodeF *headFixed, node_country **headCountry)
 {
   int inf=0;
   int inf1=4;
   int monthI=0;
   int monthF=0;
   int month=0;
   int year=0;

   printfFilter(&inf, &inf1, &month, &year, &monthI, &monthF);

   while(inf!=4){

     if(inf==1)
     {
       *headCountry = clearcountrylist(*headCountry);
       headFixed = clearcitylistH(headFixed);
       headFixed = LoadCityFile();
       *headCountry=LoadCountryFile();
     }

     if(inf==2)
     {
       FilterCity_opt2(headFixed, month, year);
       *headCountry = FilterCountry_opt2(*headCountry, month, year);
     }

     if(inf==3)
     {
       *headCountry=FilterCountry_opt3(*headCountry, monthI, monthF);
       FilterCity_opt3(headFixed,monthI,monthF);
     }

     if((inf==2 && inf1==3) || (inf==3 && inf1==2))
     {
       *headCountry = FilterCountry_opt2(*headCountry, month, year);
       *headCountry=FilterCountry_opt3(*headCountry, monthI, monthF);
       FilterCity_opt2(headFixed, month, year);
       FilterCity_opt3(headFixed,monthI,monthF);
     }

     inf=0;
     inf1=0;
     printfFilter(&inf, &inf1, &month, &year, &monthI, &monthF);
   }


 }

 /**
  * FilterCity_opt2: Filtra os dados da lista de cidades
  * \param headFixed: Cabeça da listas de listas
  * \param month/year: Mês e ano escolhido pelo utilizador
  */
 void FilterCity_opt2(nodeF *headFixed, int month, int year)
 {
   nodeV *auxV=NULL;
   nodeF *auxF=NULL;
   nodeV *tmp=NULL;

   auxF=headFixed;

   while(auxF!=NULL){
     auxV=auxF->headV;
     //Se for menor que o ano ou mes então o nódulos é para eliminar
     while(auxV!=NULL && (auxV->dataV.dt.ano<year || (auxV->dataV.dt.ano==year && auxV->dataV.dt.mes<month)))
     {
       tmp=auxV->next;
       free(auxV);
       auxV=tmp;
     }
     auxV->headF=auxF;  //auxV passa a ser a nova cabeça da lista vertical
     auxF->headV=auxV;
     auxF=auxF->next;
   }

 }
 /**
  * FilterCountry_opt2: Filtra os dados da lista de cidades
  * \param headFixed: Cabeça da listas de listas
  * \param month/year: Mês e ano escolhido pelo utilizador
  */
 node_country* FilterCountry_opt2(node_country *head, int month, int year)
 {
   node_country *aux=NULL;
   node_country *temp=NULL;
   aux=head;

   while(aux!=NULL && (aux->countryload.dt.ano<year || (aux->countryload.dt.ano==year && aux->countryload.dt.mes<month)))
   {
     temp=aux->next;
     free(aux->countryload.pais);
     free(aux);
     aux=temp;
   }

   head=aux;

   return head;
 }
 /**
  * FilterCountry_opt3: Filtra os dados da lista de cidades
  * \param headFixed: Cabeça da listas de listas
  * \param monthI/monthF: Mês Inicial e mês Final escolhido pelo utilizador
  */
 node_country* FilterCountry_opt3(node_country *head, int monthI, int monthF)
 {
   node_country *aux=NULL;
   node_country *temp=NULL;
   int enable=0;

   aux=head;
   while(aux!=NULL){
     if(monthI>aux->countryload.dt.mes || monthF<aux->countryload.dt.mes) //condição para eliminar módulo
     {
       temp=aux;
       aux=aux->next;
       enable=1;
       head=remove_free_node(head,&temp);  //funcao para remover um nódulo
     }
     if(enable==0){
       aux=aux->next; //se nao for para eliminar avança
     }
    enable=0;
   }
   return head;
 }

 /**
  * FilterCity_opt3:
  * \param headFixed: Cabeça da listas de listas
  * \param monthI/monthF: Mês Inicial e mês Final escolhido pelo utilizador
  */

 void FilterCity_opt3(nodeF *head, int monthI, int monthF)
 {
   nodeF *aux=NULL;
   nodeV *auxV=NULL;
   nodeV *tempV=NULL;
   int enable=0;

   aux=head;
   while(aux!=NULL){
     auxV=aux->headV;
     while(auxV!=NULL){
       if(monthI>auxV->dataV.dt.mes || monthF<auxV->dataV.dt.mes)
       {
         tempV=auxV;
         auxV=auxV->next;
         enable=1;
         aux->headV=remove_free_nodeV(aux->headV,&tempV);
       }
       if(enable==0){
         auxV=auxV->next;
       }
      enable=0;
     }
     aux=aux->next;
   }
 }

 /**
  * remove_free_node: Remover nós
  * \param headFixed: recebe cabeça da lista
  * \param node: Nó a remover
  */

 node_country* remove_free_node(node_country* head, node_country** node){
   if(head==NULL || *node==NULL) return head;

   if(head==(*node)){  //Se o nó a remover for a cabeça da lista, a cabeça da lista passa para a posiçao seguinte
     head=head->next;

     if(head!=NULL)
     {
       head->prev=NULL;
     }
     (*node)->next=NULL;
   }

   else if((*node)->next==NULL) //se quiser se eliminar o ultimo nodulo da lista
   {
     (*node)->prev->next=NULL;
     (*node)->prev=NULL;
   }

   else //se quiser eliminar um nodulo a meio da lista
   {
     (*node)->prev->next=(*node)->next;
     (*node)->next->prev=(*node)->prev;
     (*node)->next=NULL;
     (*node)->prev=NULL;
   }
   free((*node)->countryload.pais); //libertar a memória desse nó
   free(*node);
   *node=NULL;
   return head; //retornar a head da lista
 }

 /**
  * remove_free_nodeV: Remove nós da lista das cidades
  * \param head: Cabeça da listas
  * \param monthI/monthF: Nó a eliminar
  */
 nodeV* remove_free_nodeV(nodeV* head, nodeV** node){
   if(head==NULL || *node==NULL) return head;

   if(head==(*node)){
     head=head->next;

     if(head!=NULL)
     {
       head->prev=NULL;
     }
     (*node)->next=NULL;
   }

   else if((*node)->next==NULL)
   {
     (*node)->prev->next=NULL;
     (*node)->prev=NULL;
   }

   else
   {
     (*node)->prev->next=(*node)->next;
     (*node)->next->prev=(*node)->prev;
     (*node)->next=NULL;
     (*node)->prev=NULL;
   }
   free(*node);
   *node=NULL;
   return head;
 }

 //----------------------------Análise da temperatura global---------------------//
 /**
  * AnaliseGlobalTemp: Analisa da temperatura global
  * \param headFixed: Cabeça da listas de listas
  */
 void AnaliseGlobalTemp(nodeF *headFixed, node_country *headCountry)
 {
   int n_months=0;
   char countryName[100]={0};
   char cityName[100]={0};
   int inf=0;
   float vector_average[300][12]={{0}};

   printGlobal(&n_months, countryName, cityName, &inf);

   while(inf!=4)
   {
     if(inf==1)
       GlobalTempC(headCountry, vector_average);
   }
 }

 void GlobalTempC(node_country *head, float vector_average[][12])
 {
   calculateAveragePerYear(head,vector_average);
 }

 void calculateAveragePerYear(node_country *head, float vector_average[300][12])
 {
   node_country *aux=NULL;
   aux=head;
   float sum=0;
   int i=0;
   int j=0;
   int lastmonth=head->countryload.dt.mes;
   int lastyear=head->countryload.dt.ano;
   int n_medicoes=0;

   while(aux!=NULL){
    while(aux->countryload.dt.ano==lastyear){
      while(aux->countryload.dt.mes == lastmonth){

        sum+=aux->countryload.temperatura;
        n_medicoes++;
        aux=aux->next;
      }
      lastmonth=aux->countryload.dt.mes;
      vector_average[i][j]=(sum)/n_medicoes;
      i++;
    }
    lastyear=aux->countryload.dt.ano;
    j++;
   }


 }

//-------------------------------Historico temperaturas

/**
 * tempHist: Menu principal do histórico de temperaturas
 * \param headCountry: Cabeça da listas dos países
 * \param headFixed: cabeça da lista da cidade
 */
void tempHist(node_country *headCountry, nodeF *headFixed)
{
  int period=0;
  int inf=0;
  char countryName[STRING_SIZE];
  char cityName[STRING_SIZE];

  printHist(&period,&inf,countryName,cityName);

  while(inf!=4)
  {
  if(inf==1)
    optGlobal(period, headCountry);

  else if(inf==2)
    optPais(period, headCountry, countryName);

  else if(inf==3)
    optCityF(period,headFixed,cityName);

  printHist(&period,&inf,countryName,cityName);
}

}
/**
 * OptPais: Histórico da temperatura de um país
 * \param period: perido de análise defenido pelo o utilizador
 * \param pais_introduzido: nome do país que o utilizador quer analisar
 */

void optPais(int period, node_country *headCountry,char pais_introduzido[])
{
  node_country *aux=NULL;
  float min=40.0;
  float max=-30.0;
  float media=0.0;
  int initial_year= 0;
  int n_medicoes=0;

  aux=headCountry;
  initial_year= headCountry->countryload.dt.ano;

  while(aux != NULL){
    while(aux!=NULL && (aux->countryload.dt.ano) < (initial_year+period)){
      if(strcmp(aux->countryload.pais,pais_introduzido)==0){
        if(aux->countryload.temperatura > (max)){
          max = aux->countryload.temperatura;
        }

        if(aux->countryload.temperatura < (min)){
          min = aux->countryload.temperatura;
        }
        media+=aux->countryload.temperatura;
        n_medicoes++;
      }

      aux=aux->next;
     }
     media=media/n_medicoes;
     min=50;
     max=-30;
     media=0.0;
     n_medicoes=0;
     initial_year=initial_year+period;
  }

}

/**
 * optGlobal: Opção da temperatura global
 * \param period: perido de análise defenido pelo o utilizador
 * \param headCountry: nome do país que o utilizador quer analisar
 */

void optGlobal(int period, node_country *headCountry)
{
  node_country *aux=NULL;
  float min=40.0;
  float max=-30.0;
  float media=0.0;
  int initial_year= 0;
  int n_medicoes=0;

  aux=headCountry;
  initial_year= headCountry->countryload.dt.ano;

  while(aux != NULL){
    while(aux!=NULL && (aux->countryload.dt.ano) < (initial_year+period)){

      if(aux->countryload.temperatura > (max)){
        max = aux->countryload.temperatura;
      }

      if(aux->countryload.temperatura < (min)){
        min = aux->countryload.temperatura;
      }
      media+=aux->countryload.temperatura;
      n_medicoes++;
      aux=aux->next;
     }
     media=media/n_medicoes;
     printf("%d-%d | %.2f | %.2f | %.2f |\n",initial_year, (initial_year+period), max, min,media);
     min=50;
     max=-30;
     media=0.0;
     n_medicoes=0;
     initial_year=initial_year+period;
  }
  // media=(float)sum/period;
}

/**
 * optCityF: Opção da temperatura por cidade
 * \param period: perido de análise defenido pelo o utilizador
 * \param headCity: nome do país que o utilizador quer analisar
 * \param cidadeIntroduzida: nome da cidade introduzida
 */

void optCityF(int period, nodeF* headCity ,char cidadeIntroduzida[]){
    nodeF* aux=NULL;
    nodeV* headV=NULL;
    int cidade_encontrada=0;
    aux=headCity;
    while(aux!=NULL){
      if(strcmp(cidadeIntroduzida,aux->dataF.cidade)==0){ //se a cidade introduzida for igual ao nome da cidade no nodulo
          headV=aux->headV; //aponta para a cabeça da lista vertical
          cidade_encontrada=1;
          aux=NULL;
      }
      if(aux!=NULL)
      aux=aux->next;
    }
    if(cidade_encontrada==0) printf("\nCidade não encontrada.");
    else optCityV(period,headV); //eliminar nódulo
  }

  /**
   * optCityV: Opção da temperatura global
   * \param period: perido de análise defenido pelo o utilizador
   * \param headCountry: nome do país que o utilizador quer analisar
   */

void optCityV(int period, nodeV *head)
{
    nodeV *aux=NULL;
    float min=40.0;
    float max=-30.0;
    float media=0.0;
    int initial_year= 0;
    int n_medicoes=0;

    aux=head;
    initial_year= head->dataV.dt.ano;

    while(aux != NULL){
      while(aux!=NULL && (aux->dataV.dt.ano) < (initial_year+period)){

        if(aux->dataV.temperatura > (max)){
          max = aux->dataV.temperatura;
        }

        if(aux->dataV.temperatura < (min)){
          min = aux->dataV.temperatura;
        }
        media+=aux->dataV.temperatura;
        n_medicoes++;
        aux=aux->next;
       }
       media=media/n_medicoes;
       printf("%d-%d | %.2f | %.2f | %.2f |\n",initial_year, (initial_year+period), max, min,media);
       min=50;
       max=-30;
       media=0.0;
       n_medicoes=0;
       initial_year=initial_year+period;
    }
  }

//---------------------------Análise de temperatura por ano

/**
 * optGlobal: Análise da temperatura por ano
 */
void AnaliseTemp(node_country *headCountry, nodeF *headFixed)
{
  int inf=0;
  int year=0;
  int N=0;
  int exi=0;
  char tmp[20]="\0";

  printAnalise(&inf, &year, &N);

  while(exi!=1)
  {
    if(inf==1){
      AnaliseCountry(headCountry,year,N);
    printf("Para sair clique em 1: ");
    fgets(tmp,20, stdin);
    sscanf(tmp,"%d",&exi);
    }

    if(inf==2){
      AnaliseCity(headFixed, year, N);
      printf("Para sair clique em 1: ");
      fgets(tmp,20, stdin);
      sscanf(tmp,"%d",&exi);
    }
  }
  printAnalise(&inf, &year, &N);
}

//---opt 1

/**
 * AnaliseCountry: Análise da temperatura de um país num determinado ano
 * \param headCountry: nome do país que o utilizador quer analisar
 * \param year: Ano que é para analisar
 * \param N: Número de anos mais quentes/frios/extremos
 */

void AnaliseCountry(node_country *headCountry, int year, int N)
{
  CountryInfo *data=NULL;
  node_country *pointerYear=NULL;
  int n_countries=0;
  float average=0.0;
  float variation=0.0;
  int i=0;

  data = SaveCountryName(year, headCountry, &pointerYear, &n_countries); //salvar o nome do país

  for(i=0; i<n_countries; i++) //para cada país
  {
    SaveAverage(data, pointerYear, year, i, &variation, &average); //calcular a media e a variacao de temp
    data[i].tempMed=average; //passar o valor para o vector de estruturas
    data[i].variacao=variation;

  }

  OrganizeTemp(data,N,n_countries); //organiza por temperaturas
}

/**
 * SaveCountryName: Salvar o nome das cidades analisadas naquele ano
 * \param year: perido de análise defenido pelo o utilizador
 * \param head: nome do país que o utilizador quer analisar
 * \param temp:Ponteiro que aponta para o nó com o ano a analisar
 * \param n_countries: número de países
 */

CountryInfo* SaveCountryName(int year, node_country *head, node_country **temp, int *n_countries){

  CountryInfo *data=NULL;
  int n_diferenca=1;
  node_country* aux=NULL;
  int i=0;

  aux=head;
  while(aux!=NULL && aux->countryload.dt.ano!=year){ //Encontra o ano inserido

    aux=aux->next;
  }
  *temp=aux;

  while(aux!=NULL && aux->countryload.dt.ano==year){  //Coloca o nome dos paises numa estrutura
    for(i=0;i<(*n_countries);i++){
      if(strcmp(data[i].pais,aux->countryload.pais)==0){
        n_diferenca=0;
      }
    }
    if(n_diferenca==1){ //se o país for diferente incrementa o número do país e alloca memoria para o vetor
      (*n_countries)++;
      data = (CountryInfo*)realloc(data, (*n_countries)*sizeof(CountryInfo));
      strcpy(data[(*n_countries)-1].pais,aux->countryload.pais);
    }
    n_diferenca=1;
    aux=aux->next;
  }

  return data;

}

/**
 * SaveAverage: Calcular a média e a variação
 * \param data: estrutura com os dados de temperatura, variacao e nome da cidade
 * \param pointer: pointer que aponta para o país a analisar
 * \i: valor do index da estrutura
 * \param variation: Variacao do país
 * \param average: média do país
 */


void SaveAverage(CountryInfo *data, node_country *pointer, int year, int i, float *variation, float *average)
{
  int n_medicoes=0;
  float maxTemp=-300.0;
  float minTemp=300.0;
  float sum=0.0;

  while(pointer!=NULL && pointer->countryload.dt.ano == year){ //enquanto o ano do nó for igual ao ano a analisar

    if(strcmp(pointer->countryload.pais, data[i].pais) == 0) //se encontrar um nódulo com o mesmo nome do pais
    {
      sum+=pointer->countryload.temperatura;
      n_medicoes++;

      if(pointer->countryload.temperatura>maxTemp){
        maxTemp=pointer->countryload.temperatura;
      }

      if(pointer->countryload.temperatura<minTemp){
        minTemp=pointer->countryload.temperatura;
      }
    }
    pointer=pointer->next;
  }
    *average=(sum)/n_medicoes;
    *variation=maxTemp-minTemp;

}

/**
 * OrganizeTemp: Organizar o vector de estruturas por ordem decrescente de temperatura usando booble sort
 * \param data: estrutura com as informações sobre a temperatura e variacao
 * \param N: N países a imprimir
 * \param n_countries: número de países
 */

void OrganizeTemp(CountryInfo *data, int N, int n_countries)
{
  int i=0, j=0;
  float aux_average=0;
  float aux_variation=0;
  char aux_name[100];
  float max=0;
  int savemax=0;
  int k=0;

  for(i=n_countries; i>0; i--){
  for(j=1; j<i; j++){
      if(data[j-1].tempMed < data[j].tempMed){ //Se a temperatura seguinte for superior a anterior troca posiçoes
          aux_average=data[j-1].tempMed;
          data[j-1].tempMed=data[j].tempMed;
          data[j].tempMed=aux_average;

          aux_variation=data[j-1].variacao;
          data[j-1].variacao=data[j].variacao;
          data[j].variacao=aux_variation;

          strcpy(aux_name,data[j-1].pais);
          strcpy(data[j-1].pais,data[j].pais);
          strcpy(data[j].pais,aux_name);

      }
   }
 }


 i=0;
 system("clear");
 printf("******** Por País *********\n");

 printf("%d País(es) Mais Quente(s)\tMais Frio(s)\tMais Extremo(s)\n",N);
  while(i<N)
  {
    for(k=0;k<n_countries;k++)
    {
      if(data[k].variacao>max){
         max=data[k].variacao;
         savemax=k;
       }
    }
    printf("%d:\t|%s|\t\t|%s|\t\t|%s|\n",i+1,data[i].pais,data[(n_countries-1)-i].pais, data[savemax].pais);
    data[savemax].variacao=0;
    max=0;
    k=-2;
    i++;
  }

}

// --- opt2
/**
 * AnaliseCity: Salvar o nome das cidades analisadas naquele ano
 * \param year: ano a analisar
 * \param headCity: nome da cidade que o utilizador quer analisar
 * \param N:Ponteiro que aponta para o nó com o ano a analisar
 */

void AnaliseCity( nodeF *headCity, int year, int N)
{
  CityInfo *data=NULL;
  nodeV *pointerYearV=NULL;
  int n_cities=0;

  data = SaveCityName(year, headCity, &n_cities);

  SaveAverage2(data, pointerYearV,headCity, year);


  OrganizeTempCity(data,N,n_cities);
}

/**
 * SaveCityName: Salvar o nome das cidades analisadas naquele ano
 * \param year: perido de análise defenido pelo o utilizador
 * \param headF: nome da cidade que o utilizador quer analisar
 * \param temp:Ponteiro que aponta para o nó com o ano a analisar
 * \param n_cities: número de cidades
 */

CityInfo* SaveCityName(int year, nodeF *headF, int *n_cities){

  CityInfo *data=NULL;
  nodeF* auxF=NULL;
  auxF=headF;
  while(auxF!=NULL){ //Encontra o ano inserido
    (*n_cities)++;
    data = (CityInfo*)realloc(data, (*n_cities)*sizeof(CityInfo));
    strcpy(data[(*n_cities)-1].city,auxF->dataF.cidade);
    auxF=auxF->next;
  }
  return data;
}

/**
 * SaveAverage2: Calcular a média e a variação
 * \param data: estrutura com os dados de temperatura, variacao e nome da cidade
 * \param pointerV: pointer que aponta para a informação da cidade a analisar
 * \param pointerF: pointer que aponta para a cidade a analisar
 * \param year: Variacao do país
 */

void SaveAverage2(CityInfo *data, nodeV *pointerV, nodeF *pointerF, int year)
{
  int n_medicoes=0;
  int i=0;
  float maxTemp=-300.0;
  float minTemp=300.0;
  float sum=0.0;
  nodeV* auxV=NULL;
  nodeF* auxF=NULL;

  auxF=pointerF;

 while(auxF!=NULL){
  auxV=auxF->headV;
  while(auxV!=NULL && auxV->dataV.dt.ano!=year){ //Encontra o ano
    auxV=auxV->next;
  }
  while(auxV!=NULL && auxV->dataV.dt.ano==year){  //Calcula a média e a variaçao

    sum+=auxV->dataV.temperatura;
    n_medicoes++;

    if(auxV->dataV.temperatura>maxTemp){
      maxTemp=auxV->dataV.temperatura;
    }

    if(auxV->dataV.temperatura<minTemp){
      minTemp=auxV->dataV.temperatura;
    }
    auxV=auxV->next;

  }
  data[i].tempMed=(sum)/n_medicoes;
  data[i].variacao=maxTemp-minTemp;
  maxTemp=-300.0;
  minTemp=300.0;
  sum=0.0;
  n_medicoes=0;

  auxF=auxF->next;
  i++;
}
}

/**
 * OrganizeTemp: Organizar o vector de estruturas por ordem decrescente de temperatura usando booble sort
 * \param data: estrutura com as informações sobre a temperatura e variacao
 * \param N: N cidades a imprimir
 * \param n_cities: número de cidades
 */

void OrganizeTempCity(CityInfo *data, int N, int n_cities)
{
  int i=0, j=0;
  float aux_average=0;
  float aux_variation=0;
  char aux_name[100];
  float max=0;
  int savemax=0;
  int k=0;

  for(i=n_cities; i>0; i--){
  for(j=1; j<i; j++){
      if(data[j-1].tempMed < data[j].tempMed){
          aux_average=data[j-1].tempMed;
          data[j-1].tempMed=data[j].tempMed;
          data[j].tempMed=aux_average;

          aux_variation=data[j-1].variacao;
          data[j-1].variacao=data[j].variacao;
          data[j].variacao=aux_variation;

          strcpy(aux_name,data[j-1].city);
          strcpy(data[j-1].city,data[j].city);
          strcpy(data[j].city,aux_name);

      }
   }
 }


 i=0;
 system("clear");
 printf("******** Por País *********\n");

 printf("%d Cidade(s) Mais Quente(s)\tMais Fria(s)\tMais Extrema(s)\n",N);
  while(i<N)
  {
    for(k=0;k<n_cities;k++)
    {
      if(data[k].variacao>max){
         max=data[k].variacao;
         savemax=k;
       }
    }
    printf("%d:\t|%s|\t\t|%s|\t\t|%s|\n"
    ,i+1,data[i].city,data[(n_cities-1)-i].city, data[savemax].city);
    data[savemax].variacao=0;
    max=0;
    k=-2;
    i++;
  }

}

//---------------------------------------------------Printf menus----------------------------------------------------

void printfMainMenu(int *inf){

  char tmp[20];

  system("clear");
  printf("\n\n\t*************** Menu ***************\n");
  printf("\t* 1.      Filtragem de dados       *\n");
  printf("\t************************************\n");
  printf("\t* 2.   Histórico de temperaturas   *\n");
  printf("\t************************************\n");
  printf("\t* 3.     Análise da temperatura    *\n");
  printf("\t************************************\n");
  printf("\t* 4. Análise da temperatura global *\n");
  printf("\t************************************\n");
  printf("\t* 5.            Sair               *\n");
  printf("\t************************************\n");
  printf("\tEscolha uma das seguintes opções: ");
  fgets(tmp,20, stdin);
  sscanf(tmp,"%d",&(*inf));
  while(*inf!=1 && *inf!=2 && *inf!=3 && *inf!=4 && *inf!=5){
    printf("\r\n\tOpção inválida. Insira outra opção: ");
    fgets(tmp,20, stdin);
    sscanf(tmp,"%d",&(*inf));
  }
  system("clear");

}

void printfFilter(int *inf, int *inf1, int *month, int *year, int *monthI, int *monthF){

  char tmp[20];
  system("clear");

  printf("\n\n\t********* Filtragem de Dados ********\n");
  printf("\t* 1.        Limpa critérios         *\n");
  printf("\t*************************************\n");
  printf("\t* 2. Escolhe intervalo para análise *\n");
  printf("\t*************************************\n");
  printf("\t* 3.    Escolhe mês para análise    *\n");
  printf("\t*************************************\n");
  printf("\t* 4.           Retornar             *\n");
  printf("\t*************************************\n");
  printf("\n\tEscolha uma das seguintes opções: ");
  fgets(tmp,20, stdin);
  sscanf(tmp,"%d",&(*inf));
  tmp[0]='\0';
  while(*inf!=1 && *inf!=2 && *inf!=3 && *inf!=4){
    printf("\n\tOpção inválida. Insira outra opção: ");
    fgets(tmp,20, stdin);
    sscanf(tmp,"%d",&(*inf));
    tmp[0]='\0';
  }
  if(*inf == 2 || *inf == 3)
  {
    printf("\n\tDeseja escolher mais algum critério?\n");
    printf("\n\tSe sim insira o número do critério/Se não insira o número 0: ");
    fgets(tmp,20,stdin);
    sscanf(tmp,"%d",&(*inf1));
    tmp[0]='\0';
    while(*inf1!=0 && *inf1!=2 && *inf1!=3)
    {
      printf("\n\tOpção inválida. Insira outra opção: ");
      fgets(tmp,20,stdin);
      sscanf(tmp,"%d",&(*inf1));
      tmp[0]='\0';
    }
  }

  if(*inf==2 || *inf1==2)
  {
    printf("\n\tInsira o mês e o ano a partir do qual quer considerar(MM/YYYY): ");
    fgets(tmp,20,stdin);
    sscanf(tmp,"%d/%d",&(*month),&(*year));
    tmp[0]='\0';

    while((*month<1 || *month>12) || (*year<1743 || *year>2013) )
      {
        printf("\n\tData não é válida. Insira novamente: ");
        fgets(tmp,20,stdin);
        sscanf(tmp,"%d/%d",&(*month),&(*year));
        tmp[0]='\0';
      }

  }

  if(*inf==3 || *inf1==3)
  {
    printf("\n\tInsira o mês inicial e o mês final que quer considerar para análise(MM-MM): ");
    fgets(tmp, 20, stdin);
    sscanf(tmp, "%d-%d", &(*monthI), &(*monthF));
    tmp[0]='\0';
       while((*monthI<1 || *monthI>12) || (*monthF<1 || *monthF>12))
        {
          printf("\n\tData não é válida. Insira novamente: ");
          fgets(tmp, 20, stdin);
          sscanf(tmp, "%d-%d", &(*monthI), &(*monthF));
          tmp[0]='\0';
        }
  }

}

void printHist(int *period, int *inf, char countryName[STRING_SIZE], char cityName[STRING_SIZE])
{
  char tmp[20];

 while(*period<1 || *period>270){
   printf("\t\nEscolha o período de amostragem em T anos: ");
   fgets(tmp,20,stdin);
   sscanf(tmp,"%d",&(*period));
   tmp[0]='\0';
 }

  printf("\n\t*********** Histórico de Temperaturas *********\n");
  printf("\t* 1.    Global: Aplica-se a todos os países   *\n");
  printf("\t***********************************************\n");
  printf("\t* 2.    Por País: Aplica-se a um único país   *\n");
  printf("\t***********************************************\n");
  printf("\t* 3. Por cidade: Aplica-se a uma única cidade *\n");
  printf("\t***********************************************\n");
  printf("\t* 4.                 Retornar                 *\n");
  printf("\t***********************************************\n");
  printf("\n\tEscolha uma das seguintes opções: ");
  fgets(tmp,20, stdin);
  sscanf(tmp,"%d",&(*inf));
  tmp[0]='\0';
  while(*inf!=1 && *inf!=2 && *inf!=3 && *inf!=4){
    printf("\n\tOpção inválida. Insira outra opção: ");
    fgets(tmp,20, stdin);
    sscanf(tmp,"%d",&(*inf));
    tmp[0]='\0';
  }

  if(*inf==2)
  {
    printf("\n\tInsira o nome do país: ");
    fgets(countryName,STRING_SIZE,stdin);
    countryName[strlen(countryName)-1]='\0';
  }

  if(*inf==3)
  {
    printf("\n\tInsira o nome da cidade: ");
    fgets(cityName,STRING_SIZE,stdin);
    cityName[strlen(cityName)-1]='\0';
  }
}

void printAnalise(int *inf, int *year, int *N){

  char tmp[20]={0};
  system("clear");

  while(*year<1 || *year>2013){
    printf("\n\tEscolha o ano de análise: ");
    fgets(tmp,20,stdin);
    sscanf(tmp,"%d",&(*year));
    tmp[0]='\0';
  }

  while(*N<1 || *N>20){
    printf("\n\tEscolha N Países/Cidades para análise (1-20): ");
    fgets(tmp,20,stdin);
    sscanf(tmp,"%d",&(*N));
    tmp[0]='\0';
  }

  printf("\n\t*********** Análise da temperatura por ano *********\n");
  printf("\t* 1.             Análise por país                  *\n");
  printf("\t****************************************************\n");
  printf("\t* 2.            Análise por cidade                 *\n");
  printf("\t****************************************************\n");
  printf("\t* 3.                Retornar                       *\n");
  printf("\t****************************************************\n");
  printf("\n\tInsira a sua opção: ");
  fgets(tmp,20, stdin);
  sscanf(tmp,"%d",&(*inf));
  tmp[0]='\0';
  while(*inf!=1 && *inf!=2 && *inf!=3){
    printf("\n\tOpção inválida. Insira outra opção: ");
    fgets(tmp,20, stdin);
    sscanf(tmp,"%d",&(*inf));
    tmp[0]='\0';

  }

}

void printGlobal(int *n_months, char countryName[], char cityName[], int *inf)
{
  char tmp[100]={0};
  system("clear");

  while(*n_months<1 || *n_months>12){
    printf("Insira o número de meses que pretende considerar: \n");
    fgets(tmp,20,stdin);
    sscanf(tmp,"%d",&(*n_months));
    tmp[0]='\0';
  }

  printf("\n\t*********** Análise da Temperatura Global *********\n");
  printf("\t* 1.        Aumento da temperatura global         *\n");
  printf("\t***********************************************\n");
  printf("\t* 2.    Por País: Aplica-se a um único país   *\n");
  printf("\t***********************************************\n");
  printf("\t* 3. Por cidade: Aplica-se a uma única cidade *\n");
  printf("\t***********************************************\n");
  printf("\t* 4.                 Retornar                 *\n");
  printf("\t***********************************************\n");
  printf("\n\tEscolha uma das seguintes opções: ");
  fgets(tmp,20, stdin);
  sscanf(tmp,"%d",&(*inf));
  tmp[0]='\0';
  while(*inf!=1 && *inf!=2 && *inf!=3 && *inf!=4){
    printf("\n\tOpção inválida. Insira outra opção: ");
    fgets(tmp,20, stdin);
    sscanf(tmp,"%d",&(*inf));
    tmp[0]='\0';
  }

  if(*inf==2)
  {
    printf("\n\tInsira o nome do país: ");
    fgets(countryName,STRING_SIZE,stdin);
    countryName[strlen(countryName)-1]='\0';
  }

  if(*inf==3)
  {
    printf("\n\tInsira o nome da cidade: ");
    fgets(cityName,STRING_SIZE,stdin);
    cityName[strlen(cityName)-1]='\0';
  }

}
