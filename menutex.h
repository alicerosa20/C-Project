#ifndef MENUTEX_H_INCLUDED
#define MENUTEXT_H_INCLUDED
#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STRING_SIZE 200

void TextualMode(nodeF*, node_country*);
void filtrationOption(nodeF*, node_country**);
void FilterCity_opt2(nodeF*, int, int);
node_country* FilterCountry_opt2(node_country*,int, int);
node_country* FilterCountry_opt3(node_country*, int, int);
void FilterCity_opt2(nodeF*, int, int);
void FilterCity_opt3(nodeF*, int, int);
void printfFilter(int *, int *, int *, int *, int *, int *);
void printfMainMenu(int *);
void Actlistcity(nodeF*);
void Actlistcountry(node_country*);
void resetcountrylist(node_country*);
void resetcitylist(nodeF*);
void tempHist(node_country *, nodeF*);
void printHist(int*, int*, char[STRING_SIZE], char[STRING_SIZE]);
void optGlobal(int, node_country*);
void calculateMaxMin(node_country*,float*,float*);
void optCityF(int, nodeF*, char[STRING_SIZE]);
void optCityV(int, nodeV*);
node_country* remove_free_node(node_country* , node_country**);
nodeV* remove_free_nodeV(nodeV* , nodeV** );
void optPais(int,node_country*,char[]);
void AnaliseTemp(node_country*, nodeF*);
void AnaliseCountry(node_country*, int, int);
void printAnalise(int*, int*, int*);
CountryInfo* SaveCountryName(int, node_country*, node_country**, int*);
void SaveAverage(CountryInfo*, node_country*, int, int, float*, float*);
void OrganizeTemp(CountryInfo *, int , int);
void AnaliseCity(nodeF *, int, int);
CityInfo* SaveCityName(int , nodeF *, int *);
void SaveAverage2(CityInfo *, nodeV *, nodeF *, int );
void OrganizeTempCity(CityInfo *, int , int );
void printGlobal(int*, char[], char[], int*);
void GlobalTempC(node_country*, float[][12]);
void calculateAveragePerYear(node_country*, float[][12]);

#endif
