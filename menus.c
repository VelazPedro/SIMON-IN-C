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

#define ANCHO 1000
#define LARGO 500
#define MAX 25
#define MAX_PUNTAJES 100

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

void pantallaInicio(SDL_Renderer* renderer, TTF_Font* fontGrande)
{
    SDL_Color fondos[] = {
        {180, 220, 255, 255},
        {240, 240, 200, 255},
        {200, 255, 200, 255},
        {255, 200, 200, 255}
    };
    int fondoIndex = 0, totalFondos = 4;
    Uint32 ultimoCambio = SDL_GetTicks(), delayCambio = 2000;

    // Colores del texto
    SDL_Color colorTexto = {76, 40, 130, 255};

    bool corriendo = true;
    SDL_Event e;

    // Crear las texturas de texto
    SDL_Surface* surfTitulo = TTF_RenderText_Solid(fontGrande, "SIMON DICE", colorTexto);
    SDL_Texture* texTitulo = SDL_CreateTextureFromSurface(renderer, surfTitulo);

    SDL_Surface* surfSub = TTF_RenderText_Solid(fontGrande, "GRUPO PIANO", colorTexto);
    SDL_Texture* texSub = SDL_CreateTextureFromSurface(renderer, surfSub);

    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);

    // Obtener tamaños de los textos
    int tw1 = surfTitulo->w, th1 = surfTitulo->h;
    int tw2 = surfSub->w, th2 = surfSub->h;

    SDL_FreeSurface(surfTitulo);
    SDL_FreeSurface(surfSub);

    // Márgenes del rectángulo
    int margenX = 20;
    int margenY = 10;

    while (corriendo)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                corriendo = false;
            if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
                corriendo = false; // salir al presionar algo
        }

        // Actualizar fondo animado
        Uint32 ahora = SDL_GetTicks();
        if (ahora - ultimoCambio > delayCambio) {
            fondoIndex = (fondoIndex + 1) % totalFondos;
            ultimoCambio = ahora;
        }

        // Dibujar fondo
        SDL_SetRenderDrawColor(renderer,
            fondos[fondoIndex].r,
            fondos[fondoIndex].g,
            fondos[fondoIndex].b,
            255);
        SDL_RenderClear(renderer);

        // Rectángulo detrás del título, ajustado al texto y centrado
        SDL_Rect rectTitulo = {
            w/2 - tw1/2 - margenX,            // izquierda
            h/2 - th1/2 - margenY - 30,       // arriba, sube 30px
            tw1 + 2*margenX,                  // ancho
            th1 + 2*margenY                    // alto
        };
        SDL_SetRenderDrawColor(renderer, 0, 120, 255, 180);
        SDL_RenderFillRect(renderer, &rectTitulo);

        // Renderizar texto SIMON DICE
        SDL_Rect dstTitulo = {
            w/2 - tw1/2,
            h/2 - th1/2 - 30,                 // sube 30 px
            tw1,
            th1
        };
        SDL_RenderCopy(renderer, texTitulo, NULL, &dstTitulo);

        // Renderizar subtítulo GRUPO PIANO
        SDL_Rect dstSub = {
            w/2 - tw2/2,
            h/2 + th1 + 30 - 30,              // sube 30 px
            tw2,
            th2
        };
        SDL_RenderCopy(renderer, texSub, NULL, &dstSub);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(texTitulo);
    SDL_DestroyTexture(texSub);
}


void pantallaIngresoNombre(SDL_Renderer* renderer, TTF_Font* font){
    bool enPantalla = true;
    SDL_Event e;
    char buffer[32] = "";
    int len = 0;

    SDL_Color fondos[] = { {180,220,255,255}, {240,240,200,255}, {200,255,200,255}, {255,200,200,255} };
    int fondoIndex = 0, totalFondos = 4;
    Uint32 ultimoCambio = SDL_GetTicks(), delayCambio = 2000;

    SDL_StartTextInput();

    while(enPantalla){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT) exit(0);

            if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_RETURN && len > 0){
                    buffer[len] = '\0';
                    strcpy(nombreJugador, buffer);
                    enPantalla = false;
                }
                else if(e.key.keysym.sym == SDLK_BACKSPACE && len > 0){
                    len--; buffer[len] = '\0';
                }
            }
            if(e.type == SDL_TEXTINPUT && len + strlen(e.text.text) < 32){
                strcat(buffer, e.text.text);
                len = strlen(buffer);
            }
        }

        Uint32 ahora = SDL_GetTicks();
        if(ahora - ultimoCambio > delayCambio){
            fondoIndex = (fondoIndex + 1) % totalFondos;
            ultimoCambio = ahora;
        }

        SDL_SetRenderDrawColor(renderer, fondos[fondoIndex].r, fondos[fondoIndex].g, fondos[fondoIndex].b, 255);
        SDL_RenderClear(renderer);

        // Texto "Ingrese su nombre:"
        SDL_Texture* titulo = crearTextoMenu(renderer, font, "Ingrese su nombre:");
        int w, h;
        SDL_QueryTexture(titulo, NULL, NULL, &w, &h);
        SDL_Rect rectTitulo = {ANCHO/2 - w/2, 100, w, h};
        SDL_RenderCopy(renderer, titulo, NULL, &rectTitulo);
        SDL_DestroyTexture(titulo);

        // Texto que escribe el usuario
        char textoBuffer[32];
        strcpy(textoBuffer, len > 0 ? buffer : "_");
        SDL_Texture* textoNombre = crearTextoMenu(renderer, font, textoBuffer);
        SDL_QueryTexture(textoNombre, NULL, NULL, &w, &h);
        SDL_Rect rectNombre = {ANCHO/2 - w/2, 200, w, h};
        SDL_RenderCopy(renderer, textoNombre, NULL, &rectNombre);
        SDL_DestroyTexture(textoNombre);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_StopTextInput();
}



void menuPrincipal(SDL_Renderer* renderer, TTF_Font* font, TTF_Font* fontdos, SDL_Window* ventana, SDL_Rect botones[], SDL_Color colores[], SDL_Color colores_claros[], char nombres[4][10], int activos[]){
    bool enMenu=true; SDL_Event e;

    SDL_Color fondos[]={ {180,220,255,255},{240,240,200,255},{200,255,200,255},{255,200,200,255} };
    int fondoIndex=0,totalFondos=4;
    Uint32 ultimoCambio=SDL_GetTicks(), delayCambio=1500;

    TTF_Font* fontpeque = TTF_OpenFont("fnt/FUENTECOMIC.ttf", 12);
    if (!fontpeque) {
        printf("Error cargando fuente: %s\n", TTF_GetError());
        return;
    }

    char textoTitulo[50]; // aumentar el tamaño del buffer si es necesario
    snprintf(textoTitulo, sizeof(textoTitulo), "Bienvenido, %s", nombreJugador);
    SDL_Texture* titulo=crearTextoMenu(renderer,font,textoTitulo);
    SDL_Texture* Schober=crearTextoMenu(renderer,font,"Modo SCHONBER");
    SDL_Texture* Mozart=crearTextoMenu(renderer,font,"Modo MOZART");
    SDL_Texture* Cheat=crearTextoMenu(renderer,font,"Modo CHEAT");
    SDL_Texture* opciones=crearTextoMenu(renderer,font,"OPCIONES");
    SDL_Texture* instructivo=crearTextoMenu(renderer,fontpeque,"INSTRUCTIVO");

    SDL_Rect rectTitulo={ANCHO/2-150,20,300,50};
    SDL_Rect rectSchober={ANCHO/2-200,120,400,40};
    SDL_Rect rectMozart={ANCHO/2-200, 220, 400, 40};
    SDL_Rect rectCheat={ANCHO/2-200, 320, 400, 40};
    SDL_Rect rectOpciones={ANCHO/2-200,420,400,40};
    SDL_Rect rectInstructivo={20,250,200,40};
//Creo las 4 texturas y rectangulos en donde va a ir el texto de las opciones
    while(enMenu){
        while(SDL_PollEvent(&e)){
            if(e.type==SDL_QUIT) return;

            if(e.type==SDL_MOUSEBUTTONDOWN && e.button.button==SDL_BUTTON_LEFT){
                int x=e.button.x; int y=e.button.y;

                if(x>=rectSchober.x && x<=rectSchober.x+rectSchober.w && y>=rectSchober.y && y<=rectSchober.y+rectSchober.h){
                    enMenu=false;
                    modo=1;
                    inicializarSonidos();
                }
                if(x>=rectMozart.x && x<=rectMozart.x+rectMozart.w && y>=rectMozart.y && y<=rectMozart.y+rectMozart.h){
                    modo=2;
                    modoMozart(renderer,font,ventana);
                    enMenu=false;
                }
                if(x>=rectCheat.x && x<=rectCheat.x+rectCheat.w && y>=rectCheat.y && y<=rectCheat.y+rectCheat.h){
                    modo=3;
                    inicializarSonidos();
                    enMenu=false;

                }
                if(x>=rectOpciones.x && x<=rectOpciones.x+rectOpciones.w && y>=rectOpciones.y && y<=rectOpciones.y+rectOpciones.h){
                    mostrarOpciones(renderer,font,ventana);
                }
                if(x>=rectInstructivo.x && x<=rectInstructivo.x+rectInstructivo.w && y>=rectInstructivo.y && y<=rectInstructivo.y+rectInstructivo.h){
                    mostrarInstructivo(renderer,font,ventana);
                }
            }
            if(e.type==SDL_KEYDOWN && e.key.keysym.sym==SDLK_RETURN)
                enMenu=false;
        }  //Les doy interaccion

        Uint32 ahora=SDL_GetTicks();
        if(ahora-ultimoCambio>delayCambio){
            fondoIndex=(fondoIndex+1)%totalFondos;
            ultimoCambio=ahora;
        } //Fondo animado

        SDL_SetRenderDrawColor(renderer,fondos[fondoIndex].r,fondos[fondoIndex].g,fondos[fondoIndex].b,255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,titulo,NULL,&rectTitulo);
        SDL_RenderCopy(renderer,Schober,NULL,&rectSchober);
        SDL_RenderCopy(renderer,Mozart,NULL,&rectMozart);
        SDL_RenderCopy(renderer,Cheat,NULL,&rectCheat);
        SDL_RenderCopy(renderer,opciones,NULL,&rectOpciones);
        SDL_RenderCopy(renderer,instructivo,NULL,&rectInstructivo);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
        //Rendereo todo con un pequenio delay para darle mas emocion
    }

    SDL_DestroyTexture(titulo);
    SDL_DestroyTexture(Schober);
    SDL_DestroyTexture(Mozart);
    SDL_DestroyTexture(Cheat);
    SDL_DestroyTexture(opciones);
    SDL_DestroyTexture(instructivo);
//libero recursos
    juego(botones,colores,colores_claros,nombres,activos,font,fontdos,renderer,ventana);
}


// Menú opciones
void mostrarOpciones(SDL_Renderer* renderer, TTF_Font* font, SDL_Window* ventana) {
    bool enOpciones = true;
    SDL_Event e;

    SDL_Color fondos[] = { {180,220,255,255}, {240,240,200,255}, {200,255,200,255}, {255,200,200,255} };
    int fondoIndex = 0, totalFondos = 4;
    Uint32 ultimoCambio = SDL_GetTicks(), delayCambio = 1500;

    SDL_Rect botonMasNotas    = {ANCHO/2+100,150,50,50};
    SDL_Rect botonMenosNotas  = {ANCHO/2-150,150,50,50};
    SDL_Rect botonVolver      = {20,20,100,40};

    while(enOpciones){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT)
                exit(0);

            if(e.type==SDL_MOUSEBUTTONDOWN && e.button.button==SDL_BUTTON_LEFT){
                int x=e.button.x, y=e.button.y;

                // Aumentar / Disminuir cantidad de notas
                if(x>=botonMasNotas.x && x<=botonMasNotas.x+botonMasNotas.w &&
                   y>=botonMasNotas.y && y<=botonMasNotas.y+botonMasNotas.h &&
                   cantidadNotas<8) {
                        cantidadNotas++;
                }
                if(x>=botonMenosNotas.x && x<=botonMenosNotas.x+botonMenosNotas.w &&
                   y>=botonMenosNotas.y && y<=botonMenosNotas.y+botonMenosNotas.h &&
                   cantidadNotas>3 ) {
                        cantidadNotas--;
                } //Calculo la cantidad de notas (para modo 1 y 3)

                // Ajustar duracionInicial según cantidad de notas
                duracionInicial = 2000; // base inicial
                duracionInicial = (int)(duracionInicial * pow(1.0f - porcentajeReducir, cantidadNotas - 3));
                if(duracionInicial > 2000) duracionInicial = 2000;

                // Botón Volver
                if(x >= botonVolver.x && x <= botonVolver.x + botonVolver.w &&
                   y >= botonVolver.y && y <= botonVolver.y + botonVolver.h) {
                        enOpciones = false;
                }
                if(modo==2)
                {
                    duracionInicial = 2000 -( (cantidadNotas-3)*porcentajeReducir*100); //Esto hace que en el modo Mozart la duracion se adapte en funcion de la frecuencia dada y no solo personalizable
                }
            }
        }

        // Fondo animado
        Uint32 ahora = SDL_GetTicks();
        if(ahora - ultimoCambio > delayCambio){
            fondoIndex = (fondoIndex + 1) % totalFondos;
            ultimoCambio = ahora;
        }

        SDL_SetRenderDrawColor(renderer,fondos[fondoIndex].r,fondos[fondoIndex].g,fondos[fondoIndex].b,255);
        SDL_RenderClear(renderer);

        // Título
        SDL_Texture* titulo = crearTextoMenu(renderer,font,"Opciones de sonido");
        int w,h; SDL_QueryTexture(titulo,NULL,NULL,&w,&h);
        SDL_Rect rectTitulo = {ANCHO/2 - w/2, 50, w, h};
        SDL_RenderCopy(renderer,titulo,NULL,&rectTitulo);
        SDL_DestroyTexture(titulo);

        // Texto cantidad notas
        char buffer[64];
        sprintf(buffer,"Notas: %d", cantidadNotas);
        SDL_Texture* textoNotas = crearTextoMenu(renderer,font,buffer);
        SDL_Rect rectNotas = {ANCHO/2-50,150,100,50};
        SDL_RenderCopy(renderer,textoNotas,NULL,&rectNotas);
        SDL_DestroyTexture(textoNotas);

        // Texto duración
        sprintf(buffer,"Duracion: %d ms", duracionInicial);
        SDL_Texture* textoDuracion = crearTextoMenu(renderer,font,buffer);
        SDL_Rect rectDuracion = {ANCHO/2-75,250,150,50};
        SDL_RenderCopy(renderer,textoDuracion,NULL,&rectDuracion);
        SDL_DestroyTexture(textoDuracion);

        // Botones visuales
        SDL_SetRenderDrawColor(renderer,150,150,150,255);
        SDL_RenderFillRect(renderer,&botonMasNotas);
        SDL_RenderFillRect(renderer,&botonMenosNotas);

        // Botón Volver
        SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
        SDL_RenderFillRect(renderer, &botonVolver);
        SDL_Texture* textoVolver = crearTextoMenu(renderer, font, "Volver");
        SDL_Rect rectTextoVolver = {botonVolver.x + 10, botonVolver.y + 5, 80, 30};
        SDL_RenderCopy(renderer, textoVolver, NULL, &rectTextoVolver);
        SDL_DestroyTexture(textoVolver);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}
void menuderrota(SDL_Renderer* renderer, TTF_Font* font, SDL_Window* ventana, int puntaje, const char* rutaArchivo)
{
    SDL_Color colorBoton = {128,128,0,255};

    SDL_Rect botonSalir = { (ANCHO/2) - 200, LARGO-90, 180, 60 };
    SDL_Rect botonPuntajes = { (ANCHO/2) + 10, LARGO-90, 180, 60 };
    int frasenum;
    char frase[256];

    // Elegir frase aleatoria
    frasenum = rand() % 14;
    switch(frasenum){
        case 0: strcpy(frase,"Esto es lo que pasa cuando no te rindes, Tokai Teio"); break;
        case 1: strcpy(frase,"Sabias que si no cerras las puertas, se quedan abiertas"); break;
        case 2: strcpy(frase,"A la grande le puse cuca"); break;
        case 3: strcpy(frase,"Yo sabiendo que de una correccion calaz me papea: entrego solo"); break;
        case 4: strcpy(frase,"Algun laburito? Si saben de algo me avisan"); break;
        case 5: strcpy(frase,"Vendo motorola G4 64gbs medio rota la pantalla por constitucion info al dm"); break;
        case 6: strcpy(frase,"Pepe pecas pica papas con un pico, con un pico pica papas Pepe pecas"); break;
        case 7: strcpy(frase,"Arriba arriba abajo abajo izquierda derecha izquierda derecha B A inicio"); break;
        case 8: strcpy(frase,"El honor ha muerto, pero vere que puedo hacer. -- Kaladin Bendito por la tormenta"); break;
        case 9: strcpy(frase,"La vida se trata de determinacion, el resultado es secundario"); break;
        case 10: strcpy(frase,"Huevo, talento y mucha cafeina. -- Yo el domingo que se tiene que entregar el tp"); break;
        case 11: strcpy(frase,"Sabias que la salchicha tiene un 33% de sal? El resto es solo chicha."); break;
        case 12: strcpy(frase,"Un 3 a las 10, es un 10 a las 3. -- Socrates, a Platon"); break;
        case 13: strcpy(frase,"River el mas grande, lejos"); break;
    } //No hace falta leer esto

    // Fuente grande para el titulo
    TTF_Font* fontGrande = TTF_OpenFont("fnt/FUENTECOMIC.ttf", 40);
    SDL_Texture* textoPerdiste = crearTextoMenu(renderer, fontGrande, "Perdiste!");
    int tw, th;
    SDL_QueryTexture(textoPerdiste, NULL, NULL, &tw, &th);
    SDL_Rect rectPerdiste = { (ANCHO - tw)/2, 20, tw, th };

    // Word wrap de frase
    SDL_Texture* lineasTexto[20];
    int numLineas = 0;
    int start = 0;
    int len = strlen(frase);
    int maxCharsPorLinea = 40;
    char linea[256];

    while(start < len && numLineas < 20){
        int end = start + maxCharsPorLinea;
        if(end >= len) end = len;
        else {
            int espacio = end;
            while(espacio > start && frase[espacio] != ' ') espacio--;
            if(espacio > start) end = espacio;
        }
        int lineLen = end - start;
        if(lineLen >= sizeof(linea)) lineLen = sizeof(linea)-1;
        strncpy(linea, frase+start, lineLen);
        linea[lineLen]='\0';
        lineasTexto[numLineas++] = crearTextoMenu(renderer, font, linea);
        start = end;
        while(start < len && frase[start]==' ') start++;
    }

    // Música
    if(musicaMenu) { Mix_FreeMusic(musicaMenu); musicaMenu=NULL; }
    musicaMenu = Mix_LoadMUS("snd/derrota.mp3");
    if(musicaMenu) { Mix_VolumeMusic(15); Mix_PlayMusic(musicaMenu,0); }

    // Fondo animado
    SDL_Color fondos[] = { {180,220,255,255},{240,240,200,255},{200,255,200,255},{255,200,200,255} };
    int fondoIndex = 0;
    const int totalFondos = sizeof(fondos)/sizeof(fondos[0]);
    Uint32 ultimoCambio = SDL_GetTicks();
    const Uint32 delayCambio = 1500;

    SDL_Texture* textoPuntaje = crearTextoPuntaje(renderer, font, puntaje);

    bool mostrando = true;
    SDL_Event e;

    while(mostrando){
        // --- Manejo eventos ---
        while(SDL_PollEvent(&e)){
            if(e.type==SDL_QUIT) mostrando=false;
            if(e.type==SDL_MOUSEBUTTONDOWN){
                int mx=e.button.x, my=e.button.y;
                if(mx>=botonSalir.x && mx<=botonSalir.x+botonSalir.w &&
                   my>=botonSalir.y && my<=botonSalir.y+botonSalir.h){
                    mostrando=false;
                }
                if(mx>=botonPuntajes.x && mx<=botonPuntajes.x+botonPuntajes.w &&
                   my>=botonPuntajes.y && my<=botonPuntajes.y+botonPuntajes.h){
                    mostrarPuntajes(renderer,font,ventana,rutaArchivo);
                }
            }
        }

        // --- Animación fondo ---
        Uint32 ahora = SDL_GetTicks();
        if(ahora-ultimoCambio>delayCambio){
            fondoIndex = (fondoIndex+1)%totalFondos;
            ultimoCambio = ahora;
        }

        SDL_SetRenderDrawColor(renderer, fondos[fondoIndex].r, fondos[fondoIndex].g, fondos[fondoIndex].b, 255);
        SDL_RenderClear(renderer);

        // --- Renderizar puntaje ---
        if(textoPuntaje){ SDL_Rect dst={10,10,150,30}; SDL_RenderCopy(renderer,textoPuntaje,NULL,&dst); }

        // "¡Perdiste!"
        SDL_RenderCopy(renderer, textoPerdiste,NULL,&rectPerdiste);

        // Frase motivacional
        int yOffset=0;
        for(int i=0;i<numLineas;i++){
            SDL_QueryTexture(lineasTexto[i],NULL,NULL,&tw,&th);
            SDL_Rect rectLinea={ (ANCHO-tw)/2, (LARGO/2 - 50)+yOffset, tw, th };
            SDL_RenderCopy(renderer,lineasTexto[i],NULL,&rectLinea);
            yOffset+=th+5;
        }

        // --- Botones ---
        SDL_SetRenderDrawColor(renderer,colorBoton.r,colorBoton.g,colorBoton.b,colorBoton.a);
        SDL_RenderFillRect(renderer,&botonSalir);
        SDL_RenderFillRect(renderer,&botonPuntajes);
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderDrawRect(renderer,&botonSalir);
        SDL_RenderDrawRect(renderer,&botonPuntajes);

        SDL_Texture* txtSalir = crearTextoMenu(renderer,font,"Salir");
        SDL_Texture* txtPuntajes = crearTextoMenu(renderer,font,"Puntajes");
        SDL_QueryTexture(txtSalir,NULL,NULL,&tw,&th);
        SDL_Rect dstS = { botonSalir.x + (botonSalir.w-tw)/2, botonSalir.y + (botonSalir.h-th)/2, tw, th };
        SDL_RenderCopy(renderer,txtSalir,NULL,&dstS);
        SDL_DestroyTexture(txtSalir);

        SDL_QueryTexture(txtPuntajes,NULL,NULL,&tw,&th);
        SDL_Rect dstP = { botonPuntajes.x + (botonPuntajes.w-tw)/2, botonPuntajes.y + (botonPuntajes.h-th)/2, tw, th };
        SDL_RenderCopy(renderer,txtPuntajes,NULL,&dstP);
        SDL_DestroyTexture(txtPuntajes);

        // --- Presentar todo ---
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // --- Limpieza ---
    SDL_DestroyTexture(textoPerdiste);
    for(int i=0;i<numLineas;i++) SDL_DestroyTexture(lineasTexto[i]);
    if(textoPuntaje) SDL_DestroyTexture(textoPuntaje);
    TTF_CloseFont(fontGrande);
    if(musicaMenu){
    Mix_HaltMusic();
    Mix_FreeMusic(musicaMenu);
    musicaMenu=NULL;
    }
}
void mostrarPuntajes(SDL_Renderer* renderer, TTF_Font* font,
                     SDL_Window* ventana, const char* rutaArchivo) {

    // --- Fondo animado ---
    SDL_Color fondos[] = { {180,220,255,255},{240,240,200,255},
                           {200,255,200,255},{255,200,200,255} };
    int fondoIndex = 0;
    const int totalFondos = sizeof(fondos)/sizeof(fondos[0]);
    Uint32 ultimoCambio = SDL_GetTicks();
    const Uint32 delayCambio = 1500; // ms


    char nombres[MAX_PUNTAJES][50];
    int puntajes[MAX_PUNTAJES];
    char mododejuego[MAX_PUNTAJES][10];
    int count = 0;

    FILE* f = fopen(rutaArchivo, "r");
    if(f) {
        char line[128];
        while(fgets(line, sizeof(line), f) && count < MAX_PUNTAJES) {
            sscanf(line, "\"%[^\"]\",%d,%9s", nombres[count], &puntajes[count],mododejuego[count]);
            count++;
        }
        fclose(f);
    }

for(int i = 0; i < count-1; i++){
    for(int j = i+1; j < count; j++){
        if(puntajes[j] > puntajes[i]){
            // Intercambiar puntajes
            int tmpP = puntajes[i]; puntajes[i] = puntajes[j]; puntajes[j] = tmpP;

            // Intercambiar nombres
            char tmpN[50]; strcpy(tmpN, nombres[i]); strcpy(nombres[i], nombres[j]); strcpy(nombres[j], tmpN);

            // Intercambiar modos
            char tmpM[10]; strcpy(tmpM, mododejuego[i]); strcpy(mododejuego[i], mododejuego[j]); strcpy(mododejuego[j], tmpM);
        }
    }
}


    int maxMostrar = (count < 10) ? count : 10;

    int ancho, alto;
    SDL_GetWindowSize(ventana, &ancho, &alto);

    bool mostrando = true;
    SDL_Event e;

    while(mostrando) {
        Uint32 ahora = SDL_GetTicks();
        if(ahora - ultimoCambio > delayCambio) {
            fondoIndex = (fondoIndex + 1) % totalFondos;
            ultimoCambio = ahora;
        }
        SDL_SetRenderDrawColor(renderer, fondos[fondoIndex].r,
                                           fondos[fondoIndex].g,
                                           fondos[fondoIndex].b,
                                           fondos[fondoIndex].a);
        SDL_RenderClear(renderer);

    //Texto titulo y encabezado
        SDL_Surface* surfTitulo = TTF_RenderText_Solid(font, "PUNTAJES", (SDL_Color){0,0,0,255});
        SDL_Texture* texTitulo = SDL_CreateTextureFromSurface(renderer, surfTitulo);
        SDL_Rect rectTitulo = { (ancho - surfTitulo->w)/2, 20, surfTitulo->w, surfTitulo->h };
        SDL_FreeSurface(surfTitulo);
        SDL_RenderCopy(renderer, texTitulo, NULL, &rectTitulo);
        SDL_DestroyTexture(texTitulo);


        SDL_Surface* surfEncabezado = TTF_RenderText_Solid(font, "NOMBRE - PUNTUACION - MODO", (SDL_Color){0,0,0,255});
        SDL_Texture* texEncabezado = SDL_CreateTextureFromSurface(renderer, surfEncabezado);
        SDL_Rect rectEncabezado = { (ancho - surfEncabezado->w)/2, rectTitulo.y + rectTitulo.h + 10,
                                    surfEncabezado->w, surfEncabezado->h };
        SDL_FreeSurface(surfEncabezado);
        SDL_RenderCopy(renderer, texEncabezado, NULL, &rectEncabezado);
        SDL_DestroyTexture(texEncabezado);

        // Centrar texto abajo del encabezado
        for(int i=0; i<maxMostrar; i++) {
            char texto[64];
            snprintf(texto, sizeof(texto), "%s - %d - %s", nombres[i], puntajes[i],mododejuego[i]);

            SDL_Surface* surf = TTF_RenderText_Solid(font, texto, (SDL_Color){0,0,0,255});
            SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);

            SDL_Rect rect;
            rect.w = surf->w;
            rect.h = surf->h;
            rect.x = (ancho - rect.w)/2;
            rect.y = rectEncabezado.y + rectEncabezado.h + 10 + i*rect.h;

            SDL_FreeSurface(surf);
            SDL_RenderCopy(renderer, tex, NULL, &rect);
            SDL_DestroyTexture(tex);
        }

        SDL_RenderPresent(renderer);

        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT || e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
                mostrando = false;
        }

        SDL_Delay(16);
    }
}
void mostrarInstructivo(SDL_Renderer* renderer, TTF_Font* font, SDL_Window* ventana)
{
    SDL_SetRenderDrawColor(renderer, 255,255,255,255); // fondo blanco
    SDL_RenderClear(renderer);
    Mix_PauseMusic();
    const char* instructivo =
    "INSTRUCTIVO SIMON MUSICAL\n"
    "\n"
    "- El juego consiste en repetir secuencias de luces y sonidos.\n"
    "- Cada color corresponde a un boton: Rojo (0), Azul (1), Verde (2), Amarillo (3).\n"
    "- Modo Schober: secuencias aleatorias crecientes.\n"
    "- Modo Mozart: reproduce melodias desde un archivo de texto.\n"
    "  El usuario debe colocar el archivo en la carpeta 'snd'.\n"
    "  Y ademas, click derecho en ProjecTopicos (workspace), add file y anadir el archivo de snd.\n"
    "  Luego se solicitara el nombre exacto del archivo con su extension.\n"
    "  Formato del archivo: primera fila = secuencia de botones (0-3),\n"
    "  segunda fila = frecuencia de cada nota en Hz (todos los elementos separados por un espacio).\n"
    "  La cantidad de notas se detecta automaticamente (minimo 3, maximo 8)\n"
    "  y se ajusta la duracion de cada nota segun la cantidad.\n"
    "- Modo Cheat: activa funcionalidades especiales.\n"
    "- opciones: ajustes del juego.\n"
    "\n"
    "Presione cualquier tecla o clic para volver.";

    SDL_Event e;
    bool salir = false;
// Array de texturas de líneas
SDL_Texture* lineasTexto[100];
int numLineas = 0;

// Copiamos el texto para poder usar strtok
char temp[1024];
strcpy(temp, instructivo);
char* lineaPtr = strtok(temp, "\n"); // separar por saltos de línea

while(lineaPtr != NULL && numLineas < 100){
    // Crear textura directamente para cada línea
    lineasTexto[numLineas++] = crearTextoMenu(renderer, font, lineaPtr);

    // Siguiente línea
    lineaPtr = strtok(NULL, "\n");
}

    // Calcular tamaño del rectángulo central
    int rectAncho = 600;
    int rectAlto = numLineas * 30 + 20; // 30 px por linea + padding
    int rectX = (ANCHO - rectAncho)/2;
    int rectY = (LARGO - rectAlto)/2;
    SDL_Rect rect = { rectX, rectY, rectAncho, rectAlto };

    // Fondo del rectángulo (semi-transparente)
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_RenderFillRect(renderer, &rect);

    // Dibujar cada línea de texto dentro del rectángulo
    int y = rectY + 10;
    for(int i = 0; i < numLineas; i++){
        SDL_Rect dst = { rectX + 10, y, rectAncho - 20, 25 }; // margen 10px
        SDL_RenderCopy(renderer, lineasTexto[i], NULL, &dst);
        y += 30;
    }

    SDL_RenderPresent(renderer);

    // esperar que el usuario toque algo
    while(!salir){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
                {
                salir = true;
                Mix_ResumeMusic();
                }
            if(e.type == SDL_QUIT)
                exit(0);
        }
        SDL_Delay(10);
    }

    // libero recursos antes de que pan me mate
    for(int i=0; i<numLineas; i++){
        SDL_DestroyTexture(lineasTexto[i]);
    }
}
void modoMozart(SDL_Renderer* renderer, TTF_Font* font, SDL_Window* ventana)
{
    bool enPantalla = true;
    SDL_Event e;
    char buffer[32] = "";
    int len = 0;
    char nombreArchivo[40];
    char ruta[20] = "snd/";
    SDL_Color fondos[] = { {180,220,255,255}, {240,240,200,255}, {200,255,200,255}, {255,200,200,255} };
    int fondoIndex = 0, totalFondos = 4;
    Uint32 ultimoCambio = SDL_GetTicks(), delayCambio = 2000;
    bool huboError = false;
    SDL_StartTextInput();

    while(enPantalla){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT) exit(0);

            if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_RETURN && len > 0){
                    buffer[len] = '\0';
                    strcpy(nombreArchivo, buffer);
                    strcat(ruta,nombreArchivo);
                    // Intentar abrir archivo
                    FILE* archivo = fopen(ruta, "r");
                    if(archivo == NULL){
                        huboError = true; // mostrar mensaje de error y reiniciar buffer
                        len = 0;
                        buffer[0] = '\0';
                    } else {
                        encontrado(archivo);
                        enPantalla = false; // archivo correcto, salir del bucle
                        fclose(archivo);
                    }
                }
                else if(e.key.keysym.sym == SDLK_BACKSPACE && len > 0){
                    len--; buffer[len] = '\0';
                }
            }

            if(e.type == SDL_TEXTINPUT && len + strlen(e.text.text) < 32){
                strcat(buffer, e.text.text);
                len = strlen(buffer);
            }
        }

        Uint32 ahora = SDL_GetTicks();
        if(ahora - ultimoCambio > delayCambio){
            fondoIndex = (fondoIndex + 1) % totalFondos;
            ultimoCambio = ahora;
        }

        SDL_SetRenderDrawColor(renderer, fondos[fondoIndex].r, fondos[fondoIndex].g, fondos[fondoIndex].b, 255);
        SDL_RenderClear(renderer);

        // texto título
        SDL_Texture* titulo = crearTextoMenu(renderer, font, "Ingrese el nombre del archivo ('Nombre.csv'):");
        int w, h;
        SDL_QueryTexture(titulo, NULL, NULL, &w, &h);
        SDL_Rect rectTitulo = {ANCHO/2 - w/2, 100, w, h};
        SDL_RenderCopy(renderer, titulo, NULL, &rectTitulo);
        SDL_DestroyTexture(titulo);

        // texto del buffer del usuario
        char textoBuffer[32];
        strcpy(textoBuffer, len > 0 ? buffer : "_");
        SDL_Texture* textoNombre = crearTextoMenu(renderer, font, textoBuffer);
        SDL_QueryTexture(textoNombre, NULL, NULL, &w, &h);
        SDL_Rect rectNombre = {ANCHO/2 - w/2, 200, w, h};
        SDL_RenderCopy(renderer, textoNombre, NULL, &rectNombre);
        SDL_DestroyTexture(textoNombre);

        // mostrar mensaje de error si hubo error al abrir el archivo
        if(huboError){
            SDL_Texture* errorTex = crearTextoMenu(renderer, font, "Error: archivo no encontrado. Intente nuevamente.");
            SDL_QueryTexture(errorTex, NULL, NULL, &w, &h);
            SDL_Rect rectError = {ANCHO/2 - w/2, 300, w, h};
            SDL_RenderCopy(renderer, errorTex, NULL, &rectError);
            SDL_DestroyTexture(errorTex);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}
void encontrado(FILE* archivo){

    char linea[256];
    cantidadNotas=0;
    // leer secuencia y copiarlas globalmente
    if(fgets(linea, sizeof(linea), archivo) != NULL){
        char* token = strtok(linea, " \r\n");
        while(token != NULL && cantidadNotas < 8){
            secuenciaM[cantidadNotas++] = atoi(token);
            token = strtok(NULL, " \r\n");
        }
    }

    // leer frecuencias y copiarlas globalmente
    if(fgets(linea, sizeof(linea), archivo) != NULL){
        char* token = strtok(linea, " \r\n");
        int i = 0;
        while(token != NULL && i < cantidadNotas){
            frecuencia[i++] = atof(token);
            token = strtok(NULL, " \r\n");
        }
    }


    fclose(archivo);
    inicializarSonidos();
}



