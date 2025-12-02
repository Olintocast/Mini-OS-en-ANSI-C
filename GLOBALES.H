#ifndef GLOBALES_H
#define GLOBALES_H

/* CONSTANTES */
#define WIDTH 640
#define HEIGHT 480

/* Constantes de ventana */
#define APP_X1 150
#define APP_Y1 80
#define APP_X2 500
#define APP_Y2 350

/* Constantes de la barra superior */
#define APP_BARRA_X1 APP_X1
#define APP_BARRA_Y1 APP_Y1
#define APP_BARRA_X2 APP_X2
#define APP_BARRA_Y2 (APP_Y1 + 13)

/* Constantes del boton para cerrar las ventanas */
#define APP_CERRAR_X1 (APP_X2 - 20)
#define APP_CERRAR_Y1 APP_Y1
#define APP_CERRAR_X2 APP_X2
#define APP_CERRAR_Y2 APP_BARRA_Y2

/* Constantes para el menu archivo */
#define MENU_ARCHIVO_X1 APP_X1
#define MENU_ARCHIVO_Y1 APP_Y1
#define MENU_ARCHIVO_X2 (APP_X1 + 60)
#define MENU_ARCHIVO_Y2 (APP_Y1 + 20)

#define APP_TEXT_X (APP_X1 + 10)
#define APP_TEXT_Y (APP_BARRA_Y2 + 10)
#define APP_STATUS_Y (APP_Y2 - 10)

#define MAX_LINEAS 10
#define MAX_CHARS_POR_LINEA 40
#define ESPACIADO_Y 15
#define FILENAME_DEFAULT "SIN_TITULO.TXT"
#define MAX_FILENAME_LEN 30
#define MAX_FILES 10

#define ACCION_GUARDAR 0
#define ACCION_ABRIR 1

/* Estructuras */
typedef struct{
    int x1, y1, x2, y2;
    int color;
} Elemento;

typedef struct {
    char texto[MAX_CHARS_POR_LINEA + 5];
} Cadenas;

typedef struct {
    char nombres[MAX_FILES][MAX_FILENAME_LEN];
    int count;
} IndiceArchivos;

/* Enums */
enum EstadoPC{ APAGADO, INICIO, ESCRITORIO };
enum EstadoApps { NINGUNA, CALCULADORA, PAINT, BLOC_DE_NOTAS, CONFIGURACION, SISTEMAS, MINIJUEGO1 };

enum EstadoPC PC_pantalla = APAGADO;
enum EstadoApps APP_activa = NINGUNA;

/* Prototipos de funciones */
void verificar_Actividad(void);
void mostrar_Protector_Pantalla(void);
void barra_Superior_Apps(const char *titulo);
void guardar_y_ocultar_mouse(void);
void restaurar_mouse(void);
void apagar_PC(Elemento *B_encendido);

/* --- SISTEMA DE SONIDO --- */
void reproducir_efecto(int tipo) {
    int i;
    switch(tipo) {
        case 0: /* Clic mecanico (para el boton de encendido / Interacciones) */
            sound(1000); delay(30); nosound();
            break;
            
        case 1: /* Melodia de inicio (Ascendente estilo retro) */
            for(i = 200; i <= 800; i += 100) {
                sound(i); delay(40);
            }
            nosound();
            break;

        case 2: /* Melodia de apagado (Descendente) */
            for(i = 800; i >= 200; i -= 100) {
                sound(i); delay(40);
            }
            nosound();
            break;

        case 3: /* Abrir Aplicacion (Blip agudo) */
            sound(2000); delay(50); nosound();
            delay(30);
            sound(2500); delay(50); nosound();
            break;

        case 4: /* Error / Accion invalida */
            sound(150); delay(150); nosound();
            break;

        case 5: /* Exito (Para juegos o calculos) */
            sound(523); delay(60); /* Do */
            sound(659); delay(60); /* Mi */
            sound(784); delay(60); /* Sol */
            nosound();
            break;
    }
}

#endif
