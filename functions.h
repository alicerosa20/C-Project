#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STRING_SIZE 200

int program_mode(int, char *[]);
void openfiles(FILE**);
nodeF* LoadCityFile();
node_country* LoadCountryFile();
void print_listcity(nodeF*);
node_country* CreateNewNode2(char[STRING_SIZE]);
void InsertTailCountry(node_country**, node_country*, node_country**);
void print_listcountry(node_country*);
nodeF* clearcitylistH(nodeF*);
node_country* clearcountrylist(node_country*);
void calc_min(int*, node_country*);
void calc_max(int*, node_country*);
node_country** vetor_size(node_country** , int, int , int*);
node_country* removehead(node_country*, node_country**);
node_country** create_vetor(node_country**, int , int , int* , node_country*);
node_country* insertnode_vetor(node_country*, node_country *, node_country*);
node_country* findposition(node_country*, node_country *, node_country**);
void print_vetor(node_country**, int);
void clean_vetor(node_country**, int);
node_country* listconvert(node_country**, int, node_country*);
// void InsertInList(nodeF**,nodeF**,nodeV**, nodeV*,nodeF*,char[STRING_SIZE]);
 nodeF* createNodeF(char [], char []);
 nodeV* createNodeV();
#endif // FUNCTIONS_H_INCLUDED
