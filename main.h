#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "boton.h"
#include <time.h>
#include <string.h>

#include <math.h>

#define ANCHO 1000
#define LARGO 500
#define MAX 25
//globales
extern char nombreJugador[32];
extern int cantidadNotas;
extern int duracionInicial;
extern float porcentajeReducir;
extern int modo;
extern Mix_Music* musicaMenu;
double frecuencia[8];
int secuenciaM[8];

// funciones
void pantallaInicio(SDL_Renderer*,TTF_Font*);
void pantallaIngresoNombre(SDL_Renderer*, TTF_Font*);
void pausarYfinalizarPrograma(SDL_Window*, SDL_Renderer*);
SDL_Texture* crearTextoPuntaje(SDL_Renderer*, TTF_Font*, int);
SDL_Texture* crearTextoMenu(SDL_Renderer*, TTF_Font*, char[]);
void menuPrincipal(SDL_Renderer*, TTF_Font*, TTF_Font*, SDL_Window*, SDL_Rect[], SDL_Color[], SDL_Color[], char[4][10], int[]);
void juego(SDL_Rect[], SDL_Color[], SDL_Color[], char nombres[4][10], int[], TTF_Font*,TTF_Font*, SDL_Renderer*,SDL_Window*);
void mostrarOpciones(SDL_Renderer*, TTF_Font*, SDL_Window*);
void menuderrota(SDL_Renderer*, TTF_Font*, SDL_Window*, int,const char*);
const char* tablapuntaje(int);
void mostrarPuntajes(SDL_Renderer*, TTF_Font*, SDL_Window*, const char*);
void modoMozart(SDL_Renderer*, TTF_Font*, SDL_Window*);
void mostrarInstructivo(SDL_Renderer*, TTF_Font*, SDL_Window*);
void encontrado(FILE*);
#endif //MAIN_H_INCLUDED
