#include "functions.h"
#include "graphic.h"
#include "data.h"
#include "menutex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>




int main(int argc, char *argv[])
{
  int i=0;
  node_country *headCountry = NULL;
  nodeF *headFixed=NULL;

  headFixed=LoadCityFile();


  headCountry=LoadCountryFile();


   i = program_mode(argc,argv); // Indica se o programa vai funcionar em modo textual e em modo gráfico
   if(i==0){
     printf("Modo textual\n");
     TextualMode(headFixed, headCountry);
   }

   if(i==1){
      graphics(headFixed);
   }

       headCountry = clearcountrylist(headCountry);
       headFixed = clearcitylistH(headFixed);
   return EXIT_SUCCESS;
}
