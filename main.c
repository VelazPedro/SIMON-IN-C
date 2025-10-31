/*
Apellido: Cuda, Federico José
Dni: 47126739
Entrega: No

Apellido: Velaz, Pedro Ezequiel
DNI: 45148037
Entrega: Si

Apellido: Monardo, Pablo Nicolás
DNI: 40429974
Entrega: No
*/

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "boton.h"
#include "main.h"
#include "volumen.h"
#include <time.h>
#include <string.h>
#include <math.h>


Mix_Music* musicaMenu = NULL; // música del menú de opciones
char nombreJugador[32] = ""; // nombre del jugador
int cantidadNotas=3; // se explica solo
int duracionInicial = 2000;  // duración inicial de cada nota en ms (mínimo 2000)
float porcentajeReducir = 0.03; // 3% de reducción por cada nota extra
int modo = 0;
double frecuencia[8]={0};
int secuenciaM[8]={0};


int main(int argc, char* argv[])
{
    SDL_Window* ventana = SDL_CreateWindow("Simon GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                           ANCHO, LARGO, SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS);
    SDL_Renderer* renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED); //Inicializo ventana y render

    // Inicialización mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Error inicializando SDL_mixer: %s\n", Mix_GetError());
        return 1;
    }

    // Inicializar SDL_ttf
    if (TTF_Init() < 0) {
        printf("Error inicializando SDL_ttf: %s\n", TTF_GetError());
        return 1;
    }

    // Cargar fuentes
    TTF_Font* font = TTF_OpenFont("fnt/FUENTECOMIC.ttf", 24);
    if (!font) {
        printf("Error cargando fuente: %s\n", TTF_GetError());
        return 1;
    }
    TTF_Font* fontdos = TTF_OpenFont("fnt/FUENTELETRA.ttf", 24);
    if (!fontdos) {
        printf("Error cargando fuente: %s\n", TTF_GetError());
        return 1;
    }
    TTF_Font* fontdosgrande = TTF_OpenFont("fnt/FUENTELETRA.ttf", 40);
    if (!fontdosgrande) {
        printf("Error cargando fuente: %s\n", TTF_GetError());
        return 1;
    }
    // Inicializar sonidos
    inicializarSonidos();

    SDL_Rect botones[4] = {
        {400, 150, 100, 100}, // Rojo
        {520, 150, 100, 100}, // Azul
        {400, 270, 100, 100}, // Verde
        {520, 270, 100, 100}  // Amarillo
    };

    SDL_Color colores[4] = {
        {180, 0, 0, 255}, //Rojo
        {0, 0, 180, 255}, //Azul
        {0, 180, 0, 255}, //Verde
        {255, 180, 0, 255} //Amarillo
    };

    SDL_Color colores_claros[4] = {
        {200, 100, 100, 255},
        {100, 100, 200, 255},
        {100, 200, 100, 255},
        {200, 255, 150, 255}
    };

    char nombres[4][10] = {"Rojo","Azul","Verde","Amarillo"};
    int activo[4] = {0,0,0,0};

    srand(time(NULL));
    pantallaInicio(renderer,fontdosgrande);
    pantallaIngresoNombre(renderer, font);
    menuPrincipal(renderer, font, fontdos, ventana, botones, colores, colores_claros, nombres, activo);

    // Liberar recursos
    liberarSonidos();
    TTF_CloseFont(font);
    TTF_CloseFont(fontdos);
    TTF_Quit();
    SDL_Quit();
    pausarYfinalizarPrograma(ventana, renderer);
    return 0;
}




// Texto y botones
SDL_Texture* crearTextoMenu(SDL_Renderer* renderer, TTF_Font* font, char texto[]) {
    char buffer[128];
    sprintf(buffer,"%s",texto);
    SDL_Color color={68,31,2,255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, buffer,color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);
    return texture;
}

SDL_Texture* crearTextoPuntaje(SDL_Renderer* renderer, TTF_Font* font, int correcto) {
    char buffer[64];
    sprintf(buffer,"Puntaje: %d",correcto);
    SDL_Color color={0,0,0,255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, buffer,color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);
    return texture;
} //No es una funcion generica para crear texto porque una se tendra que actualizar constantemente con el puntaje y la otra solo printear la textura



void juego(SDL_Rect botones[], SDL_Color colores[], SDL_Color colores_claros[],
           char nombres[4][10], int activos[], TTF_Font* font,TTF_Font* fontdos, SDL_Renderer* renderer,SDL_Window* ventana) {
    SDL_Event e;
    int corriendo = 1, ce = 0, correcto = 0, ultimoCorrecto = 0;
    int vecorden[MAX] = {0};
    bool jugando = false;
    SDL_Texture* textoCorrectos = crearTextoPuntaje(renderer, font, 0);
    int centroX = ANCHO / 2;
    int centroY = LARGO / 2;
    int radio = 150;
    const char* rutaArchivo;
    Mix_HaltMusic(); //Paro la musica mientras estoy jugando
    while(corriendo) {
        if(!jugando) {
            if(modo == 2){
                    vecorden[ce] = secuenciaM[ce]; ce++;
            } else {
                vecorden[ce] = rand() % 4;
                ce++;
                };

            // Mostrar todos los cuadrantes sin resaltar
            mostrarBoton(renderer, 4, colores, colores_claros, textoCorrectos,NULL);
            SDL_RenderPresent(renderer);

            // Mostrar secuencia de colores del Simon
            for(int j = 0; j < ce; j++) {
                int boton = vecorden[j];
                // Mostrar botón iluminado
                reproducirSonido(boton);
                SDL_Color* fondoExtra = (modo == 3) ? &colores[boton] : NULL;
                mostrarBoton(renderer, boton, colores, colores_claros, textoCorrectos,fondoExtra);
                SDL_RenderPresent(renderer);

                // Mantener iluminado por duracionInicial / 4
                Uint32 holdTime = SDL_GetTicks();
                while(SDL_GetTicks() - holdTime < duracionInicial / 4) {
                    SDL_Event e_tmp;
                    while(SDL_PollEvent(&e_tmp)) {
                        if(e_tmp.type == SDL_QUIT) {
                            SDL_DestroyTexture(textoCorrectos);
                            return;
                        }
                    }
                }

                // Apagar botón
                mostrarBoton(renderer, 4, colores, colores_claros, textoCorrectos,fondoExtra);
                SDL_RenderPresent(renderer);

                // Delay entre notas de 150 ms y sin que se cuelgue el programa como con sdl_delay
                Uint32 pauseTime = SDL_GetTicks();
                while(SDL_GetTicks() - pauseTime < 150) {
                    SDL_Event e_tmp;
                    while(SDL_PollEvent(&e_tmp)) {
                        if(e_tmp.type == SDL_QUIT) {
                            SDL_DestroyTexture(textoCorrectos);
                            return;
                        }
                    }
                }
            }
        correcto = 0;
        jugando = true;
    }

        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) corriendo = 0;

            if(jugando) {
                for(int i = 0; i < 4; i++) {
                    if(botonDetectar(&e, i, centroX, centroY, radio+5)) {
                        if(vecorden[correcto] == i) {
                            activos[i] = 1;

                            // Mostrar sector iluminado al presionar
                            SDL_Color* fondoExtra = (modo == 3) ? &colores[i] : NULL;
                            if(modo != 3)
                                mostrarBoton(renderer, i, colores, colores_claros, textoCorrectos,fondoExtra);
                            printf("Color presionado: %s \n", nombres[i]);
                            reproducirSonido(i);
                            SDL_RenderPresent(renderer);

                            // Mantener iluminado por duracionInicial / 4
                            Uint32 holdTime = SDL_GetTicks();
                            while(SDL_GetTicks() - holdTime < duracionInicial / 4) {
                                SDL_Event e_tmp;
                                while(SDL_PollEvent(&e_tmp)) {
                                    if(e_tmp.type == SDL_QUIT) {
                                        SDL_DestroyTexture(textoCorrectos);
                                        return;
                                    }
                                }
                                SDL_Delay(1);
                            }

                            mostrarBoton(renderer, 4, colores, colores_claros, textoCorrectos,fondoExtra);
                            SDL_RenderPresent(renderer);

                            correcto++;
                            if(correcto > ultimoCorrecto) {
                                SDL_DestroyTexture(textoCorrectos);
                                textoCorrectos = crearTextoPuntaje(renderer, font, correcto);
                                ultimoCorrecto = correcto;
                                    }
                            }else {
                            printf("Perdiste!\n");
                            // Limpiar la pantalla antes de pasar a menuderrota
                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                            SDL_RenderClear(renderer);
                            SDL_RenderPresent(renderer);
                            // Llamar a la pantalla de derrota
                            rutaArchivo=tablapuntaje(ultimoCorrecto);
                            menuderrota(renderer, fontdos, ventana, ultimoCorrecto,rutaArchivo);
                            // Liberar texturas y recursos del juego
                            if (textoCorrectos) {
                                SDL_DestroyTexture(textoCorrectos);
                                textoCorrectos = NULL;
                            }
                            return;

                        }
                    }
                }

                if(correcto == ce) {
                    SDL_DestroyTexture(textoCorrectos);
                    textoCorrectos = crearTextoPuntaje(renderer, font, correcto);
                    jugando = false;

                    Uint32 waitTime = SDL_GetTicks();
                    while(SDL_GetTicks() - waitTime < duracionInicial / 2) {
                        SDL_Event e_tmp;
                        while(SDL_PollEvent(&e_tmp)) {
                            if(e_tmp.type == SDL_QUIT) {
                                SDL_DestroyTexture(textoCorrectos);
                                return;
                            }
                        }
                        SDL_Delay(1);
                    }
                }
            }
        }
    }

    SDL_DestroyTexture(textoCorrectos);
}

const char* tablapuntaje(int correcto)
{
    const char* rutaArchivo="puntajes.csv";
    char mododejuego[10];
    switch(modo){
case 1:
    strcpy(mododejuego,"SCHONBER");
    break;
case 2:
    strcpy(mododejuego,"MOZART");
    break;
case 3:
    strcpy(mododejuego,"CHEAT");
    break;
    }
    // Abrir archivo CSV para agregar puntaje, o crearlo si todavia no esta abrido
    FILE *archivo = fopen("puntajes.csv", "a"); // "a" para agregar al final
    if (!archivo) {
        printf("No se pudo abrir puntajes.csv");
        exit(1);
            }

    // Guardar en formato CSV: nombre,puntaje,mododejuego
    fprintf(archivo, "\"%s\",%d, %9s\n", nombreJugador, correcto,mododejuego);
    fclose(archivo);
    printf("Puntaje guardado: %s - %d - %9s\n", nombreJugador, correcto,mododejuego);

    return rutaArchivo;
}
// ---------------------
void pausarYfinalizarPrograma(SDL_Window* window, SDL_Renderer* renderer){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
//selfexplanative
