#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "main.h"
#include "volumen.h"
#include <string.h>
#include <math.h>



Mix_Chunk* sonidos[8];

// ---------------------
// Funciones de sonido
// ---------------------

void inicializarSonidos() {
    // Inicializa el sistema de audio
  switch(modo){
      case 0:

            musicaMenu = Mix_LoadMUS("snd/balatro-theme.ogg");
            if (musicaMenu == NULL) {
                printf("Error al cargar música: %s\n", Mix_GetError());
            } else{
                Mix_VolumeMusic(15);
                Mix_PlayMusic(musicaMenu, -1);
            }
        break;
    case 1:
    case 3:
        for(int j=0; j<8;j++)
        {
            frecuencia[j]=(double)(rand() % 700); //No me gustan los agudos asi que max 1000
            sonidos[j] = generarTono(frecuencia[j], duracionInicial);
        }

    break;
     case 2:
        for(int i=0;i<8;i++)
            sonidos[i] = generarTono(frecuencia[i],duracionInicial);
        break;

}
}

void liberarSonidos() {
    if (musicaMenu != NULL) {
        Mix_HaltMusic();
        Mix_FreeMusic(musicaMenu);
        musicaMenu = NULL;
    }
    for(int i = 0; i < 8; i++) {
        if(sonidos[i]) {
            free(sonidos[i]->abuf);
            free(sonidos[i]);
            sonidos[i] = NULL;
        }
    }
    Mix_CloseAudio();
}


void reproducirSonido(int idNota) {
    if(idNota < 0 || idNota >= 8) return;

    int notaFinal = idNota;
    // Lógica de notas según cantidadNotas
    if(cantidadNotas == 3) {
        if(idNota == 3) { // amarillo
            notaFinal = rand() % 8; // cualquier nota aleatoria
        }
    } else if(cantidadNotas >= 5 && cantidadNotas <= 8) {
        if(idNota < 4) {
            int limite = cantidadNotas; // solo se permiten notas dentro de la cantidad actual
            int aguda = idNota + 4;
            if(aguda >= limite) aguda = idNota; // si la nota aguda supera el límite, usar la normal
            notaFinal = (rand() % 2 == 0) ? idNota : aguda; // alterna entre normal y aguda
        }
    }


    if(sonidos[notaFinal]) {
        Mix_PlayChannel(idNota, sonidos[notaFinal], 0);
        Mix_Volume(idNota, MIX_MAX_VOLUME / 3);
        printf("Reproduciendo sonido %d (%0.2f)\n", notaFinal, frecuencia[notaFinal]);
    }
}




// Generar un tono en memoria
Mix_Chunk* generarTono(double frecuencia, int duracion_ms) {
    int samples = (int)(SAMPLE_RATE * (duracion_ms / 1000.0));
    Sint16* buffer = (Sint16*)malloc(sizeof(Sint16) * samples);

    for(int i = 0; i < samples; i++)
        buffer[i] = (Sint16)(VOLUMEN * sin(2.0 * M_PI * frecuencia * i / SAMPLE_RATE));

    Mix_Chunk* chunk = (Mix_Chunk*)malloc(sizeof(Mix_Chunk));
    chunk->allocated = 1;
    chunk->abuf = (Uint8*)buffer;
    chunk->alen = samples * sizeof(Sint16);
    chunk->volume = MIX_MAX_VOLUME;

    return chunk;
}

