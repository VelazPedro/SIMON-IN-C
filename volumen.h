#ifndef VOL_H_INCLUDED
#define VOL_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <math.h>
#define SAMPLE_RATE 44100
#define VOLUMEN 4000

double frecuencia[8];

extern Mix_Chunk* sonidos[8];
extern const char* archivosSonidos[8];
Mix_Chunk* tonos[8];
Mix_Chunk* generarTono(double, int);
void inicializarTonos(int);
void liberarTonos(int);
void inicializarSonidos();
void modoSchonber();
void liberarSonidos();
void reproducirSonido(int idNota);


#endif
