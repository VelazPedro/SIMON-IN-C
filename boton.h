#ifndef DIBUJOS_H_INCLUDED
#define DIBUJOS_H_INCLUDED

#define PI 3.14159265
#include <SDL2/SDL.h>
#include <stdbool.h>

bool botonDetectar(SDL_Event* , int, int, int, int);
void botonPintar(SDL_Renderer*, SDL_Rect, SDL_Color, SDL_Color colorHover, bool);
void mostrarBoton(SDL_Renderer*, int, SDL_Color[], SDL_Color[], SDL_Texture*,SDL_Color*);
void dibujarSector(SDL_Renderer*, int, int, int, int, int, SDL_Color);
void botonSector(SDL_Renderer*, int id, int centroX, int, int, SDL_Color[], SDL_Color[], int);

#endif // DIBUJOS_H_INCLUDED
