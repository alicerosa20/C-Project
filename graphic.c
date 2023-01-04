#include "graphic.h"
#include "datagraphic.h"
#include "data.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const char myName2[] = "Beatriz Pereira";
const char myNumber2[] = "IST190029";
const char myName[] = "Alice Rosa";
const char myNumber[] = "IST190007";

const char scale0[] = "Temperature Scale:";
const char scale1[] = "T < 10";
const char scale2[] = "10 < = T < 15";
const char scale3[] = "15 < = T < 20";
const char scale4[] = "20 < = T < 25";
const char scale5[] = "T > = 25";
const char year0[] = "Year:";
const int colors[3][5] = {{16, 61, 40, 255, 255},{74, 173,187, 85, 5},{168,214, 17, 5, 12}};



/**
 * RenderPoints: renders the board
 * \param _board 2D array with integers representing board colors
 * \param _board_pos_x number of positions in the board (x axis)
 * \param _board_pos_y number of positions in the board (y axis)
 * \param _square_size_px size of each square
 * \param _board_size_px size of the board
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderPoints(int _board[][MAX_BOARD_POS], int _board_pos_x, int _board_pos_y,
        int _board_size_px[], int _square_size_px, SDL_Renderer *_renderer, nodeF* head, int ano)
{
  nodeF* auxF= NULL;
  nodeV* auxV= NULL;
  int n_medicoes =0;
  float soma = 0;
  int ano_atual=0;
  float media_temperatura= 400;



    int clr=-1, x_corner, y_corner, circleX, circleY, circleR;
    // corner of the board
    x_corner = (TABLE_SIZE_X)/2; // divide a largura do tabuleiro em dois para separar em Oeste e Este com a origem no centro do mapa
    y_corner = (TABLE_SIZE_Y)/2; // divide a altura do tabuleiro em dois para separar em Norte e Sul com a origem no centro do mapa

                auxF = head;

                while(auxF != NULL){

                  auxV = auxF->headV;

                  if(auxF->dataF.latitude.direcao == 'N'){
                    _board_pos_y = auxF->dataF.latitude.angular;}

                  if(auxF->dataF.latitude.direcao == 'S'){ // Se a latitude tiver direção Sul, a sua coordenada será negativa neste referencial
                    _board_pos_y = -(auxF->dataF.latitude.angular);}

                  if(auxF->dataF.longitude.direcao == 'W'){ // Se a longitude tiver direção Oeste, a sua coordenada é negativa neste referêncial
                    _board_pos_x = -(auxF->dataF.longitude.angular); }

                  else{ // Se não lê norte, sul ou oeste só poderá ler Este, cuja coordenada de longitude é positiva neste referencial
                    _board_pos_x = auxF->dataF.longitude.angular; }

                    while(auxV != NULL){

                      ano_atual = auxV->dataV.dt.ano; // lê o ano de cada node da lista

                      if(ano_atual == ano){
                        n_medicoes++; // Se o ano lido for igual ao ano que se está a analisar, soma-se uma medição de temperatura e soma-se esse valor de temperatura à soma
                        soma += auxV->dataV.temperatura;
                      }

                      if(n_medicoes!=0) // se o número de medições for diferente de zero, calcula-se a média de temperaturas para o ano em causa
                      media_temperatura = soma/n_medicoes;
                      else {
                        media_temperatura=400; // caso contrário, o valor da média é redefinido para um valor irrelevante para a escala de temperatura e a cor do circulo passa a -1. logo, não será impresso nenhuma bola
                        clr=-1;
                      }
                      auxV = auxV->next;
                    }
                    // Escala de cores de acordo com a temperatura média no ano em estudo
                    if(media_temperatura==400) clr=-1;
                    else{
                    if (media_temperatura < 10){
                      clr= 0;
                    }
                    else if (media_temperatura >= 10 && media_temperatura <15){
                      clr= 1;
                    }
                    else if (media_temperatura >= 15 && media_temperatura <20){
                      clr= 2;
                    }
                    else if (media_temperatura >= 20 && media_temperatura <25){
                      clr= 3;
                    }
                    else if(media_temperatura >= 25){
                      clr= 4;
                    }
                  }

                    circleX = x_corner + (_board_pos_x)*3;
                    circleY = y_corner - (_board_pos_y)*3;
                    circleR = 5;
                    // draw a circle
                    if(clr != -1){
                      filledCircleRGBA(_renderer, circleX, circleY, circleR, colors[0][clr], colors[1][clr], colors[2][clr]);
                    } // só imprime a bola se a cor for diferente de -1
                    n_medicoes=0;
                    soma=0;
                    media_temperatura=-54.3; // redefinição dos valores das medições, soma e média de modo a repetir tudo sem influênciar os dados que serão analisados posteriormente

                auxF= auxF->next;
              }

}

/**
 * filledCircleRGBA: renders a filled circle
 * \param _circleX x pos
 * \param _circleY y pos
 * \param _circleR radius
 * \param _r red
 * \param _g gree
 * \param _b blue
 */
void filledCircleRGBA(SDL_Renderer * _renderer, int _circleX, int _circleY, int _circleR, int _r, int _g, int _b)
{
    int off_x = 0;
    int off_y = 0;
    float degree = 0.0;
    float step = M_PI / (_circleR*8);

    SDL_SetRenderDrawColor(_renderer, _r, _g, _b, 255);

    while (_circleR > 0)
    {
        for (degree = 0.0; degree < M_PI/2; degree+=step)
        {
            off_x = (int)(_circleR * cos(degree));
            off_y = (int)(_circleR * sin(degree));
            SDL_RenderDrawPoint(_renderer, _circleX+off_x, _circleY+off_y);
            SDL_RenderDrawPoint(_renderer, _circleX-off_y, _circleY+off_x);
            SDL_RenderDrawPoint(_renderer, _circleX-off_x, _circleY-off_y);
            SDL_RenderDrawPoint(_renderer, _circleX+off_y, _circleY-off_x);
        }
        _circleR--;
    }
}

/*
 * RenderTable: Draws the table where the game will be played, namely:
 * -  some texture for the background
 * -  the right part with the IST logo and the student name and number
 * -  the grid for game board with squares and seperator lines
 * \param _board_pos_x number of positions in the board (x axis)
 * \param _board_pos_y number of positions in the board (y axis)
 * \param _board_size_px size of the board
 * \param _font font used to render the text
 * \param _img surfaces with the table background and IST logo (already loaded)
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderTable( int _board_pos_x, int _board_pos_y, int _board_size_px[],
        TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer ,int ano)
{
    SDL_Color black = { 0, 0, 0 }; // black
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest;
    int height=0, board_size, square_size_px, max_pos;
    char ano_string[5]="\0";
    int i=0;
      int circleX=0, circleY=0, circleR=0;
    // set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );

    // clear the window
    SDL_RenderClear( _renderer );

    tableDest.x = tableSrc.x = 0;
     tableDest.y = tableSrc.y = 0;
     tableSrc.w = _img[0]->w;
     tableSrc.h = _img[0]->h;
     tableDest.w = TABLE_SIZE_X;
     tableDest.h = TABLE_SIZE_Y;

    // draws the table texture
    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);

    sprintf(ano_string, "%d",ano);

    // Renders the temperature scale
    height = RenderText(TABLE_SIZE_X+3*MARGIN, height, scale0, _font, &black, _renderer);
    height += RenderText(TABLE_SIZE_X+3*MARGIN, height, scale1, _font, &black, _renderer);
    height += RenderText(TABLE_SIZE_X+3*MARGIN, height, scale2, _font, &black, _renderer);
    height += RenderText(TABLE_SIZE_X+3*MARGIN, height, scale3, _font, &black, _renderer);
    height += RenderText(TABLE_SIZE_X+3*MARGIN, height, scale4, _font, &black, _renderer);
    height += RenderText(TABLE_SIZE_X+3*MARGIN, height, scale5, _font, &black, _renderer);

    for(i=0;i<5; i++){
    circleX = 1200;
    circleY = 39 + i*(27.5);
    circleR = 5;
    filledCircleRGBA(_renderer, circleX, circleY, circleR, colors[0][i], colors[1][i], colors[2][i]);}

    height += RenderText(TABLE_SIZE_X+3*MARGIN, height, year0, _font, &black, _renderer);

    height += RenderText(TABLE_SIZE_X+3*MARGIN, height, ano_string, _font, &black, _renderer);


    // compute and adjust the size of the table and squares
    max_pos = MAX(_board_pos_x, _board_pos_y);
    board_size = (int)(BOARD_SIZE_PER*TABLE_SIZE_X);
    square_size_px = (board_size - (max_pos+1)*SQUARE_SEPARATOR) / max_pos;
    _board_size_px[0] = _board_pos_x*(square_size_px+SQUARE_SEPARATOR)+SQUARE_SEPARATOR;
    _board_size_px[1] = _board_pos_y*(square_size_px+SQUARE_SEPARATOR)+SQUARE_SEPARATOR;


    // destroy everything
    SDL_DestroyTexture(table_texture);
    // return for later use
    return square_size_px;
}

/**
 * RenderLogo function: Renders the IST logo on the app window
 * \param x X coordinate of the Logo
 * \param y Y coordinate of the Logo
 * \param _logoIST surface with the IST logo image to render
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderLogo(int x, int y, SDL_Surface *_logoIST, SDL_Renderer* _renderer)
{
    SDL_Texture *text_IST;
    SDL_Rect boardPos;

    // space occupied by the logo
    boardPos.x = x;
    boardPos.y = y;
    boardPos.w = _logoIST->w;
    boardPos.h = _logoIST->h;

    // render it
    text_IST = SDL_CreateTextureFromSurface(_renderer, _logoIST);
    SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

    // destroy associated texture !
    SDL_DestroyTexture(text_IST);
    return _logoIST->h;
}

/**
 * RenderText function: Renders some text on a position inside the app window
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string with the text to be written
 * \param _font TTF font used to render the text
 * \param _color color of the text
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer)
{
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    SDL_Rect solidRect;

    solidRect.x = x;
    solidRect.y = y;
    // create a surface from the string text with a predefined font
    text_surface = TTF_RenderText_Blended(_font,text,*_color);
    if(!text_surface)
    {
        printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    // create texture
    text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
    // obtain size
    SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // render it !
    SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);
    // clear memory
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    return solidRect.h;
}

/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _font font that will be used to render the text
 * \param _img surface to be created with the table background and IST logo
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */
void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer)
{
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height);
    *_renderer = CreateRenderer(width, height, *_window);

    // load the table texture
    _img[0] = IMG_Load("mapa_fixe.jpeg");
    if (_img[0] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("FreeSerif.ttf", 16);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitSDL: Initializes the SDL2 graphic library
 */
void InitSDL()
{
    // init SDL library
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */
void InitFont()
{
    // Init font library
    if(TTF_Init()==-1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */
SDL_Window* CreateWindow(int width, int height)
{
    SDL_Window *window;
    // init window
    window = SDL_CreateWindow( "Warming Up", WINDOW_POSX, WINDOW_POSY, width, height, 0 );
    // check for error !
    if ( window == NULL )
    {
        printf("Failed to create window : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    return window;
}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *renderer;
    // init renderer
    renderer = SDL_CreateRenderer( _window, -1, 0 );

    if ( renderer == NULL )
    {
        printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // set size of renderer to the same as window
    SDL_RenderSetLogicalSize( renderer, width, height );

    return renderer;
}

/**
 * Função que calcula o ano máximo e o ano mínimo dos dados fornecidos
 */
 void calc_max_min(int *maximo, nodeF* head, int *minimo){

      nodeV *auxV= NULL;
      nodeF* auxF = NULL;

      auxF = head;

      while( auxF != NULL){
        auxV = auxF->headV;

        if(auxV->dataV.dt.ano < (*minimo)){
          *minimo= auxV->dataV.dt.ano;
        }
        while(auxV->next != NULL){
          auxV = auxV->next;
        }

    if(auxV->dataV.dt.ano > (*maximo)){
      *maximo = auxV->dataV.dt.ano;
    }
     auxF = auxF-> next;
   }
 }

 /**
  * Counter infinito do valor do ano em estudo desde o mínimo até ao máximo
  */
 void atualiza_ano(int minimo, int maximo, int *ano){

    if( (*ano)> maximo){
      (*ano)= minimo;
    }
      (*ano)++;

 }

 //--------------------------

 /**
  * Função base do modo gráfico, onde todas as funções são chamadas
  */
 void graphics( nodeF* headFixed)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *serif = NULL;
    SDL_Surface *imgs[2];
    SDL_Event event;
    int delay = 300;
    int quit = 0;
    int width = (TABLE_SIZE_X + LEFT_BAR_SIZE);
    int height = TABLE_SIZE_Y;
    int square_size_px = 0, board_size_px[2] = {0};
    int board_pos_x = -8, board_pos_y = 38;
    int board[MAX_BOARD_POS][MAX_BOARD_POS] = {{0}};
    int minimo=3000;
    int maximo=0;


    calc_max_min(&maximo, headFixed, &minimo);
    int ano=minimo;

    // initialize graphics
    InitEverything(width, height, &serif, imgs, &window, &renderer);

    while( quit == 0 )
    {
        // while there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT )
            {
                quit=1;
            }
            else if ( event.type == SDL_KEYDOWN )
            {
                switch ( event.key.keysym.sym )
                {
                    case SDLK_q:
                        quit=1; break;
                    default:
                        break;
                }
            }



        }
        // render game table
        square_size_px = RenderTable( board_pos_x, board_pos_y, board_size_px, serif, imgs, renderer,ano);
        // render board
        RenderPoints(board, board_pos_x, board_pos_y, board_size_px, square_size_px, renderer, headFixed, ano);
        atualiza_ano(minimo, maximo, &ano);
        // render in the screen all changes above
        SDL_RenderPresent(renderer);
        // add a delay
        SDL_Delay( delay );
    }

    // free memory allocated for images and textures and closes everything including fonts
    TTF_CloseFont(serif);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}
