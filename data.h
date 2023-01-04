#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

typedef struct {
	int dia;
	int mes;
	int ano;
} data;

typedef struct {
	float angular;
	char direcao;
} geo_coord;


typedef struct {
	data dt;
	float temperatura;
	float incerteza;
	char *pais;
} dados_temp2;

typedef struct{
	int flag;
} flagcountry;


typedef struct node2 {
	dados_temp2 countryload;
	flagcountry flagCountry;
	struct node2 *next;
	struct node2 *prev;
} node_country;

typedef struct{
	float temperatura;
	float incerteza;
	data dt;
} dataV;

struct nodeF;

typedef struct{
	int flag;
}flagCity;

typedef struct nodeV{
	dataV dataV;
	flagCity flagCity;
	struct nodeF *headF;
	struct nodeV *next;
	struct nodeV *prev;
} nodeV;

typedef struct{
	char *cidade;
	char *pais;
	geo_coord latitude;
	geo_coord longitude;
} dataF;

typedef struct{
	char pais[100];
	float tempMed;
	float variacao;
} CountryInfo;

typedef struct{
	char city[100];
	float tempMed;
	float variacao;
} CityInfo;

typedef struct nodeF{
	dataF dataF;
	nodeV *headV;
	struct nodeF *next;
	struct nodeF *prev;
} nodeF;




#endif // DATA_H_INCLUDED
