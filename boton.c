#include "boton.h"
#include "main.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

//funciones matematicas para hacer el circulo del simon
void dibujarSector(SDL_Renderer* renderer, int cx, int cy, int radio, int angInicio, int angFin, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    for (int y = -radio; y <= radio; y++) {
        for (int x = -radio; x <= radio; x++) {
            if (x * x + y * y <= radio * radio) {
                double ang = atan2(y, x) * 180.0 / PI;
                if (ang < 0) ang += 360;

                // dibujar si está dentro del rango angular
                if (ang >= angInicio && ang < angFin) {
                    SDL_RenderDrawPoint(renderer, cx + x, cy + y);
                }
            }
        }
    }
}

void botonSector(SDL_Renderer* renderer, int id, int centroX, int centroY, int radio,
                 SDL_Color colores[], SDL_Color colores_claros[], int activo) {

    int angInicio, angFin;

    switch (id) {
        case 0: angInicio = 180; angFin = 270; break;
        case 1: angInicio = 270; angFin = 0; break;
        case 2: angInicio = 0;   angFin = 90; break;
        case 3: angInicio = 90;  angFin = 180; break;
        default: return;
    }
if(modo != 3)
{
    if (angInicio < angFin) {
        dibujarSector(renderer, centroX, centroY, radio, angInicio, angFin,
                      activo ? colores_claros[id] : colores[id]);
    } else {
        dibujarSector(renderer, centroX, centroY, radio, angInicio, 360,
                      activo ? colores_claros[id] : colores[id]);
        dibujarSector(renderer, centroX, centroY, radio, 0, angFin,
                      activo ? colores_claros[id] : colores[id]);
    }

} else{

    if (angInicio < angFin) {
        dibujarSector(renderer, centroX, centroY, radio, angInicio, angFin, colores[id]);
    } else {
        dibujarSector(renderer, centroX, centroY, radio, angInicio, 360, colores[id]);
        dibujarSector(renderer, centroX, centroY, radio, 0, angFin, colores[id]);
    }
                 }
                 }
void mostrarBoton(SDL_Renderer* renderer, int id, SDL_Color colores[], SDL_Color colores_claros[], SDL_Texture* textoCorrectos, SDL_Color* fondoExtra) {
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderClear(renderer);

    // mostrar puntaje constantemente
    SDL_Rect rectTexto = {10, 10, 150, 30};
    SDL_RenderCopy(renderer, textoCorrectos, NULL, &rectTexto);

    int centroX = ANCHO / 2;
    int centroY = LARGO / 2;
    int radio = 150;

    // si id no es 4 osea que no quiere solo dibujar los botones
    if (id != 4) {
        for (int i = 0; i < 4; i++) {
            bool activo = (i == id);
            botonSector(renderer, i, centroX, centroY, radio, colores, colores_claros, activo);
            if(fondoExtra!=NULL)
            {
                SDL_SetRenderDrawColor(renderer,fondoExtra->r,fondoExtra->g,fondoExtra->b,255);
                SDL_RenderClear(renderer);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(400); // se ilumina brevemente

        // volver a estado normal
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, textoCorrectos, NULL, &rectTexto);
    }

    // dibujar todos los cuadrantes normales
    for (int i = 0; i < 4; i++)
        botonSector(renderer, i, centroX, centroY, radio, colores, colores_claros, (i == id));

    SDL_RenderCopy(renderer, textoCorrectos, NULL, &rectTexto);
    SDL_RenderPresent(renderer);
}


bool botonDetectar(SDL_Event* e, int id, int centroX, int centroY, int radio) {
    if (e->type != SDL_MOUSEBUTTONDOWN || e->button.button != SDL_BUTTON_LEFT)
        return false;

    int x = e->button.x;
    int y = e->button.y;

    int dx = x - centroX;
    int dy = y - centroY;  // eje Y normal
    double distancia = sqrt(dx*dx + dy*dy);

    if (distancia > radio) return false; // fuera del círculo

    double angulo = atan2(dy, dx) * 180.0 / PI;
    if (angulo < 0) angulo += 360;  // normalizar 0-360°

    // Comparar según los ángulos de los sectores rotados
    switch (id) {
        case 0: return angulo >= 180 && angulo < 270;       // Rojo
        case 1: return angulo >= 270 || angulo < 0;         // Azul
        case 2: return angulo >= 0 && angulo < 90;          // Verde
        case 3: return angulo >= 90 && angulo < 180;        // Amarillo
        default: return false;
    }
}


void botonPintar(SDL_Renderer* renderer, SDL_Rect rect, SDL_Color colorNormal, SDL_Color colorHover, bool activo) {
    SDL_SetRenderDrawColor(renderer,
                           activo ? colorHover.r : colorNormal.r,
                           activo ? colorHover.g : colorNormal.g,
                           activo ? colorHover.b : colorNormal.b,
                           255);
    SDL_RenderFillRect(renderer, &rect);
}
