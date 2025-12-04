/* LIBRERIAS REQUERIDAS */
#include <graphics.h>
#include <mouse.h>
#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "globales.h"
#include "notas.h"
#include "sistemas.h"

/* Comentario aclaratorio: muchas funciones reciben por referencia estructuras 'globales' porque originalmente estaban inicializadas
en el main. Al crecer el proyecto tuvimos que hacerlas globales, pero no fue necesario cambiar la forma de acceder a ellas en todas las
funciones. Habria sido super tedioso. */

Elemento Iconos[6] = {
	{35, 35, 75, 80, WHITE}, /* Calculadora */
	{35, 100, 75, 145, WHITE}, /* Bloc de notas */
	{35, 165, 75, 210, WHITE}, /* Paint */
	{35, 230, 75, 275, WHITE}, /* Configuracion */
	{35, 295, 75, 340, WHITE}, /* Sistemas */
	{90, 35, 130, 80, WHITE} /* Minijuego 1 */
};

Elemento B_encendido = {575, 382, 600, 398, RED};

int mouse_X_escondido = 0, mouse_Y_escondido = 0;
int fondo_elegido = 1, protector_elegido = 0;
int colores_predefinidos[4] = {3, 9, 5, 12};
time_t ultima_actividad;
int mouse_X_viejo, mouse_Y_viejo;

/* PROTOTIPOS DE FUNCIONES */

/* Prototipos de las funciones de utilidad */
void guardar_y_ocultar_mouse(void);
void restaurar_mouse(void);
void inicializar_graficos(void);


/* Prototipos de las funciones generales del MiniOS */
void marco_Computadora(void);
void pantalla_Apagada(void);
void dibujar_Computadora(void);
void pantalla_Inicio(int color, const Elemento *B_iniciar);
void fondo(void);
void dibujar_Logo_Escritorio(void);
void escritorio(void);
void boton_Encendido(const Elemento *btn);
void encender_PC(const Elemento *B_iniciar, Elemento *B_encendido);
void apagar_PC(Elemento *B_encendido);
void mostrar_Protector_Pantalla(void);
void verificar_Actividad(void);


/* Prototipos de las funciones de iconos */
void icono_Calculadora(const Elemento *calculadora);
void icono_Bloc_De_Notas(const Elemento *notas);
void icono_Paint(const Elemento *paint);
void icono_Configuracion(const Elemento *config);
void icono_Sistemas(const Elemento *sistemas);
void icono_Minijuego1(const Elemento *minijuego1);


/* Prototipos de las funciones de las Apps */
void barra_Superior_Apps(const char *titulo);
void barra_Superior_Notas(char *titulo, int mostrar_menu_archivo);
void calculadora(Elemento *B_encendido);
void paint(Elemento *B_encendido);
void configuracion(Elemento *B_encendido);
void sistemas(void);
void minijuego1(Elemento *B_encendido);
void dibujar_Fecha_Hora(void);

/* Prototipos de las funciones de la calculadora */
void resetear_pantalla_calculadora(char *texto);

/* Prototipos de las funciones de la configuracion */
void dibujar_Configuracion(void);

int main(){
    /* Elementos */
    Elemento B_iniciar = {275, 230, 385, 255, WHITE};

    inicializar_graficos();

    /* Dibujo inicial de la computadora */
    dibujar_Computadora();

    mver(); /* Mostrar el puntero del raton */

	ultima_actividad = time(NULL);

    /* BUCLE PRINCIPAL DE EVENTOS */
	while(1){
		if(PC_pantalla != APAGADO && PC_pantalla != INICIO) verificar_Actividad();

		if(mclick()==1){
            guardar_y_ocultar_mouse();

            /* Clic en el boton de apagado */
            if(minlimit(1, B_encendido.x1, B_encendido.y1, B_encendido.x2, B_encendido.y2) == 1){
                if(PC_pantalla == APAGADO){
                    encender_PC(&B_iniciar, &B_encendido);
                } else{
                    apagar_PC(&B_encendido);
                }
            restaurar_mouse();
            }

            /* Manejo de pantalla de inicio */
            else if(PC_pantalla == INICIO){
                if(minlimit(1, B_iniciar.x1, B_iniciar.y1, B_iniciar.x2, B_iniciar.y2) == 1){
                    PC_pantalla = ESCRITORIO;
					escritorio();
                }
            }

            /* Manejo de los iconos de apps (clics) */
            else if(PC_pantalla == ESCRITORIO){
                if(minlimit(1, Iconos[0].x1, Iconos[0].y1, Iconos[0].x2, Iconos[0].y2) == 1){
                    reproducir_efecto(3); /* <--- Sonido */
                    APP_activa = CALCULADORA;
                    calculadora(&B_encendido);

					/* La siguiente logica se usa para cada app */
					
					/* Si durante el bucle de la app se presiono apagar (el boton) no se dibuja el escritorio al salir del mismo */
                    if(PC_pantalla != APAGADO){
                    	/* Al salir de la app se redibuja el escritorio */
						escritorio();
                        APP_activa = NINGUNA;
                    }
                    APP_activa = NINGUNA;
                }
                else if(minlimit(1, Iconos[1].x1, Iconos[1].y1, Iconos[1].x2, Iconos[1].y2) == 1){
                   reproducir_efecto(3); /* <--- Sonido */
                    APP_activa = BLOC_DE_NOTAS;
                    bloc_de_notas();

                    if(PC_pantalla != APAGADO){
						escritorio();
                        APP_activa = NINGUNA;
                        restaurar_mouse();
                    }
                    APP_activa = NINGUNA;
				}
				else if(minlimit(1, Iconos[2].x1, Iconos[2].y1, Iconos[2].x2, Iconos[2].y2) == 1){
					reproducir_efecto(3); /* <-- Sonido */
                    APP_activa = PAINT;
                    paint(&B_encendido);

					if(PC_pantalla != APAGADO){
						escritorio();
						APP_activa = NINGUNA;
						restaurar_mouse();
					}
					
					APP_activa = NINGUNA;
				}
				else if(minlimit(1, Iconos[3].x1, Iconos[3].y1, Iconos[3].x2, Iconos[3].y2) == 1){
				    reproducir_efecto(3); /* <-- Sonido */
                    APP_activa = CONFIGURACION;
                    configuracion(&B_encendido);

					if(PC_pantalla != APAGADO){
						escritorio();
						APP_activa = NINGUNA;
						restaurar_mouse();
					}
					
					APP_activa = NINGUNA;
				}
				else if(minlimit(1, Iconos[4].x1, Iconos[4].y1, Iconos[4].x2, Iconos[4].y2) == 1){
					reproducir_efecto(3); /* <-- Sonido */
                    sistemas(); 
                    
					escritorio();
					APP_activa = NINGUNA;
				}
				else if(minlimit(1, Iconos[5].x1, Iconos[5].y1, Iconos[5].x2, Iconos[5].y2) == 1){
					reproducir_efecto(3); /* <-- Sonido */
                    APP_activa = MINIJUEGO1;
                    minijuego1(&B_encendido);

					if(PC_pantalla != APAGADO){
						escritorio();
						APP_activa = NINGUNA;
						restaurar_mouse();
					}
				}
            }

            restaurar_mouse();

            /* Queda esperando hasta que se suelte el clic para solo detectar uno a la vez */
            while(mclick() == 1){
                delay(50);
            }
        }

        /* Escape para salir del bucle */
        if(kbhit()){
            if(getch() == 27) break;
        }
        /* Actualizamos la hora solo cuando la computadora esta encendida */
        if(PC_pantalla == ESCRITORIO && APP_activa == NINGUNA){
            dibujar_Fecha_Hora();
        }
        delay(30);
    }
return 0;
}

/*_________________________________________________________________________*/
/*                                                                         */
/* IMPLEMENTACION DE FUNCIONES                                             */
/*_________________________________________________________________________*/

/*--------------------------FUNCIONES DE UTILIDAD--------------------------*/

/* La siguientes 2 funciones las usamos para ocultar el mouse antes de dibujar y
volverlo a mostrar en la misma posicion después de dibujar. Dibujar sobre el cursor y acto seguido moverlo, borra pixeles */
/* Post-presentacion: Parece que la mayoria no lo identifico o no lo abordaron */

void guardar_y_ocultar_mouse(void){
	mouse_X_escondido = mxpos(1);
	mouse_Y_escondido = mypos(1);
    mocultar();
}

void restaurar_mouse(void){
    mver();
	msituar(1, mouse_X_escondido, mouse_Y_escondido);
}

void inicializar_graficos(void){
    int driver=DETECT, modo;
    initgraph(&driver, &modo, "c:\\TC20\\BIN");
    if (graphresult() != grOk) {
        printf("Error de inicializacion de graficos. Verigique que el archivo BGI/EGAVGA.BGI esta en la ruta.\n");
        exit(1);
    }
    mver(); /* Si no hay error se muestra el mouse */
}

/*---------------------------FUNCIONES DE DIBUJO---------------------------*/

void marco_Computadora(void){
    int color_Marco = LIGHTGRAY;
    int color_Base_Inferior = LIGHTGRAY;
    int color_Cuello_Base = DARKGRAY;

	/* Parte que une el monitor con su base */
    int cuello_base[] = {
        310, 401, 350, 401,
        370, 465, 290, 465
    };

    /* Bordes del monitor */
    setfillstyle(1, color_Marco);
    bar(10, 385, 30, 25); /* Izquierdo */
    bar(25, 10, 615, 30); /* Superior */
    bar(630, 22, 610, 385); /* Derecho */
    bar(615, 400, 25, 380); /* Inferior */

    /* Esquinas redondeadas */
    setcolor(color_Marco); /* Para el borde */
    fillellipse(22, 22, 12, 12); /* Sup izquierda */
    fillellipse(618, 22, 12, 12); /* Sup derecha */
    fillellipse(618, 388, 12, 12); /* Sub derecha */
    fillellipse(22, 388, 12, 12); /* Sub izquierda */

    /* Base (Se dibuja antes del cuello) */
    setcolor(color_Base_Inferior);
    setfillstyle(1, color_Base_Inferior);
    fillellipse(325, 480, 245, 30);

    /* Cuello */
    setcolor(color_Cuello_Base);
    setfillstyle(SOLID_FILL, color_Cuello_Base);
    fillpoly(4, cuello_base);
}

/* Dibuja la pantalla de inicio y el boton en verde */
void encender_PC(const Elemento *B_iniciar, Elemento *B_encendido){
    reproducir_efecto(0); /* Sonido similar al clic */
    PC_pantalla = INICIO;
    pantalla_Inicio(9, B_iniciar);
    
    /* Sonido de 'booteo' */
    reproducir_efecto(1); 
    
    B_encendido->color = GREEN;
    boton_Encendido(B_encendido); /* Se le pasa su estructura para dibujarse con las propiedades correctas (coordenadas y color) */
}

/* Dibuja la pantalla apagada y el boton en rojo */
void apagar_PC(Elemento *B_encendido){
 reproducir_efecto(2); /* Melodia de apagado */
    PC_pantalla = APAGADO;
    pantalla_Apagada();
    B_encendido->color = RED;
    boton_Encendido(B_encendido);
    reproducir_efecto(0); /* Clic final del boton */
}

/* Dibuja el boton usando la estructura que lo describe */
void boton_Encendido(const Elemento *btn){
    setcolor(btn->color);
    setfillstyle(SOLID_FILL, btn->color);
    bar(btn->x1, btn->y1, btn->x2, btn->y2);
    setcolor(WHITE);
    rectangle(btn->x1, btn->y1, btn->x2, btn->y2);
}

/* Dibuja la pantalla apagada */
void pantalla_Apagada(void){
    setfillstyle(SOLID_FILL, BLACK);
    bar(31, 31, 609, 379);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    setcolor(WHITE);
    outtextxy(195, 170, (char *)"SISTEMA APAGADO");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(200, 200, (char *)"PRESIONE BOTON PARA ENCENDER");
}

/* Dibuja la computadora en si co la pantalla apagada usando funciones ya vistas */
void dibujar_Computadora(void){
	marco_Computadora();
    pantalla_Apagada();
    boton_Encendido(&B_encendido);
}

/* Vamos a dejar de comentar funciones obvias jaja */
void pantalla_Inicio(int color, const Elemento *B_iniciar){
    /* Fondo */
    setcolor(color);
    setfillstyle(1, color);
    bar(31, 31, 609, 379);

    /* Boton de inicio */
    setfillstyle(1, B_iniciar->color);
    setcolor(B_iniciar->color);
    bar3d(B_iniciar->x1, B_iniciar->y1, B_iniciar->x2, B_iniciar->y2, 15, 1);

    /* Textos */
    settextstyle(3, 0, 2);
    setcolor(15);
    outtextxy(235, 160, (char *)"BIENVENIDO AL MINIOS");

    setcolor(2);
    outtextxy(300, 230, (char *)"INICIAR");
}

void barra_Tareas(void){
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(31, 355, 609, 379);
}

void dibujar_Fecha_Hora(void){
    time_t timer;
    struct tm *tm_info;
    char buffer[40];
	int x_pos = 480; /* Cerca del borde derecho */
	int y_pos = 360; /* Dentro de la barra de tareas */

	/* Static es para que la variable siga viviendo. La funcion se ejecuta por los bucles, asi evitamos que se resetee */
	static int ultimo_segundo = 0;

    /* Obtener el tiempo actual */
    time(&timer);
    tm_info = localtime(&timer);

	/* Si el segundo no ha cambiado salimos de la funcion para no redibujar */
	if (tm_info->tm_sec == ultimo_segundo) {
        return;
    }

    ultimo_segundo = tm_info->tm_sec; /* Si el segundo cambio se actualiza */

    sprintf(buffer, "%02d:%02d:%02d %02d/%02d/%02d",
        tm_info->tm_hour,
        tm_info->tm_min,
        tm_info->tm_sec,
        tm_info->tm_mday,
        tm_info->tm_mon + 1,
        tm_info->tm_year % 100);

    /* Limpiar el area */
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(x_pos - 5, y_pos - 2, 608, 378);

    /* Dibujar el texto */
    settextstyle(SMALL_FONT, HORIZ_DIR, 4);
    setcolor(WHITE);
    outtextxy(x_pos, y_pos, buffer);
}

void mostrar_Protector_Pantalla(void){
	int x1, y1, x2, y2, color;

    guardar_y_ocultar_mouse();

    /* Pantalla negra */
    setfillstyle(SOLID_FILL, BLACK);
    bar(31, 31, 609, 379);

    /* Dibujo de lineas aleatorias */
    while(!kbhit() && mclick() != 1){

		if(mxpos(1) != mouse_X_escondido && mypos(1) != mouse_Y_escondido) break;

		x1 = 33 + rand() % 568;
		y1 = 43 + rand() % 338;
		x2 = x1 + rand() % (568 - x1);
		y2 = y1 + 10;
		color = rand() % 15;

        setcolor(color);
        setfillstyle(SOLID_FILL, color);
        if(protector_elegido == 0){
			line(x1, y1, x2, y1);
		}
		else if(protector_elegido == 1){
			x2 = x1 + 10;
			bar(x1, y1, x2, y2);
		}
		else if(protector_elegido == 2){
			fillellipse(x1, y1, 5, 5);
		}

		delay(200);
    }

    /* Actualizamos la ultima actividad para reiniciar el contador */
	ultima_actividad = time(NULL);

    switch(APP_activa){
    	case NINGUNA:
    		escritorio();
    		break;

        case CALCULADORA:
            escritorio();
            calculadora(&B_encendido);
            break;

        case BLOC_DE_NOTAS:
            escritorio();
			bloc_de_notas();
            break;

        case PAINT:
            escritorio();
            paint(&B_encendido);
            break;

        case CONFIGURACION:
            escritorio();
            configuracion(&B_encendido);
            break;

        case MINIJUEGO1:
            escritorio();
            minijuego1(&B_encendido);
            break;
            
        default:
            escritorio();
            break;
    }
    
    restaurar_mouse();
}

/* Verifica si pasaron 60 segundos para mostrar el protector de pantalla */
void verificar_Actividad(void){
	int mouse_X_actual, mouse_Y_actual;
    time_t tiempo_actual;

	/* Se obtiene la posicion del cursor */
	mouse_X_actual = mxpos(1);
	mouse_Y_actual = mypos(1);

	/* Se verifica si se movio el mouse y se actualizan las variables */
	if(mouse_X_actual != mouse_X_viejo || mouse_Y_actual != mouse_Y_viejo){
        ultima_actividad = time(NULL);
		mouse_X_viejo = mouse_X_actual;
		mouse_Y_viejo = mouse_Y_actual;
    }

	/* Se verifica si se uso el teclado o el clic*/
    if(kbhit() || mclick() == 1){
        ultima_actividad = time(NULL);
    }

    /* Se obtiene el tiempo actual */
    tiempo_actual = time(NULL);

	/* Si la diferencia entre el tiempo actual y la ultima actividad es de 60
	segundos se muestra el protector */
	if(difftime(tiempo_actual, ultima_actividad) == 60){
        mostrar_Protector_Pantalla();
    }
}

/* Para el color se usa una variable global que la app de config. puede modificar */
void fondo(void){
	setfillstyle(SOLID_FILL, colores_predefinidos[fondo_elegido]);
	bar(31, 31, 609, 379);
}

void dibujar_Logo_Escritorio(void){
    /* Coordenadas del punto central del logo */
    int cx = 278;
    int cy = 163;
    int tamano_panel = 40;
    int espacio = 5;
    int profundidad = 4;

	/* Color del borde */
  	setcolor(BLACK);

    /* Panel 1 (Rojo, superior izquierdo) */
    setfillstyle(SOLID_FILL, RED);
	bar3d(cx, cy, cx + tamano_panel, cy + tamano_panel, profundidad, 1);

    /* Panel 2 (Verde, superior derecho) */
    setfillstyle(SOLID_FILL, GREEN);
    bar3d(cx + tamano_panel + espacio, cy, cx + (2*tamano_panel) + espacio, cy + tamano_panel, profundidad, 1);

    /* Panel 3 (Azul, inferior izquierdo) */
    setfillstyle(SOLID_FILL, BLUE);
    bar3d(cx, cy + tamano_panel + espacio, cx + tamano_panel, cy + (2*tamano_panel) + espacio, profundidad, 1);

    /* Panel 4 (Amarillo, inferior derecho) */
    setfillstyle(SOLID_FILL, YELLOW);
    bar3d(cx + tamano_panel + espacio, cy + tamano_panel + espacio, cx + (2*tamano_panel) + espacio, cy + (2*tamano_panel) + espacio, profundidad, 1);
}

/* Dibuja el escritorio con los iconos */
void escritorio(){
	fondo();
	dibujar_Logo_Escritorio();
    barra_Tareas();
    
    icono_Calculadora(&Iconos[0]);
	icono_Bloc_De_Notas(&Iconos[1]);
	icono_Paint(&Iconos[2]);
	icono_Configuracion(&Iconos[3]);
	icono_Sistemas(&Iconos[4]);
	icono_Minijuego1(&Iconos[5]);
}

/* DIBUJO DE ICONOS */
void icono_Calculadora(const Elemento *calculadora){
    int x = calculadora->x1; /* 35 */
    int y = calculadora->y1; /* 35 */
    int x_fin = calculadora->x2; /* 75 (Ancho 40) */
    int y_fin_icono = calculadora->y1 + 40; /* 75 (Alto 40) */
    int shadow_offset = 2;

    /* Sombra del icono */
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(x + shadow_offset, y + shadow_offset, x_fin + shadow_offset, y_fin_icono + shadow_offset);

    /* Fondo del icono */
    setfillstyle(SOLID_FILL, WHITE);
    bar(x, y, x_fin, y_fin_icono);
    setcolor(BLACK);
    rectangle(x, y, x_fin, y_fin_icono);
    
    /* Figura */
    /* Pantalla de la calculadora */
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(x + 5, y + 5, x_fin - 5, y + 15);
    setcolor(BLACK);
    rectangle(x + 5, y + 5, x_fin - 5, y + 15);

    /* Botones de la calculadora */
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(x + 5, y + 18, x + 15, y + 24);  /* Boton 1 */
    bar(x + 15, y + 18, x + 25, y + 24); /* Boton 2 */
    bar(x + 25, y + 18, x + 35, y + 24); /* Boton 3 */
    bar(x + 5, y + 26, x + 15, y + 32);  /* Boton 4 */
    bar(x + 15, y + 26, x + 25, y + 32); /* Boton 5 */
    bar(x + 25, y + 26, x + 35, y + 32); /* Boton 6 */

    /* Nombre de la app */
    settextstyle(SMALL_FONT, HORIZ_DIR, 4);
    setcolor(WHITE);
    outtextxy(x + 4, y_fin_icono + 3, "CALCUL");
}

void icono_Bloc_De_Notas(const Elemento *bloc){
 	int x = bloc->x1; /* 35 */
    int y = bloc->y1; /* 90 */
    int x_fin = bloc->x2; /* 75 (Ancho 40) */
    int y_fin_icono = bloc->y1 + 40; /* 130 (Alto 40) */
    int desplazamiento_sombra = 2;

    /* Sombra del icono */
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(x + desplazamiento_sombra, y + desplazamiento_sombra, x_fin + desplazamiento_sombra, y_fin_icono + desplazamiento_sombra);

    /* Fondo del icono */
    setfillstyle(SOLID_FILL, WHITE);
    bar(x, y, x_fin, y_fin_icono);
    setcolor(BLACK);
    rectangle(x, y, x_fin, y_fin_icono);

    /* Figura */
    /* Lineas azules (simulando texto) */
    setcolor(BLUE);
    line(x + 5, y + 8, x_fin - 5, y + 8);
    line(x + 5, y + 14, x_fin - 5, y + 14);
    line(x + 5, y + 20, x_fin - 5, y + 20);
    line(x + 5, y + 26, x_fin - 5, y + 26);
    line(x + 5, y + 32, x_fin - 5, y + 32);

    /* Nombre de la app */
    settextstyle(SMALL_FONT, HORIZ_DIR, 4);
    setcolor(WHITE);
    outtextxy(x + 2, y_fin_icono + 3, "NOTAS");
}

void icono_Paint(const Elemento *paint){
    int x = paint->x1; /* 35 */
    int y = paint->y1; /* 145 */
    int x_fin = paint->x2; /* 75 (Ancho 40) */
    int y_fin_icono = paint->y1 + 40; /* 185 (Alto 40) */
    int desplazamiento_sombra = 2;

    /* Sombra del icono */
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(x + desplazamiento_sombra, y + desplazamiento_sombra, x_fin + desplazamiento_sombra, y_fin_icono + desplazamiento_sombra);

    /* Fondo del icono */
	setfillstyle(SOLID_FILL, WHITE);
	bar(x, y, x_fin, y_fin_icono);
    setcolor(BLACK);
	rectangle(x, y, x_fin, y_fin_icono);

    /* Figura */
    /* Paleta (un ovalo marron) */
    setfillstyle(SOLID_FILL, BROWN);
    fillellipse(x + 20, y + 20, 18, 14);

    /* Circulos de colores */
    setfillstyle(SOLID_FILL, RED);
    fillellipse(x + 12, y + 15, 3, 3);
    setfillstyle(SOLID_FILL, GREEN);
    fillellipse(x + 20, y + 12, 3, 3);
    setfillstyle(SOLID_FILL, BLUE);
    fillellipse(x + 28, y + 15, 3, 3);

    /* Pincel (linea gruesa) */
    setcolor(DARKGRAY);
    setlinestyle(SOLID_LINE, 0, THICK_WIDTH); /* Linea gruesa */
    line(x + 25, y + 25, x + 35, y + 35);
	setlinestyle(SOLID_LINE, 0, NORM_WIDTH); /* Regresamos al otro estilo por seguridad */

    /* Titulo de la app */
    settextstyle(SMALL_FONT, HORIZ_DIR, 4);
	setcolor(WHITE);
	outtextxy(x + 2, y_fin_icono + 3, "PAINT");
}

void icono_Configuracion(const Elemento *config){
	int x = config->x1; /* 35 */
    int y = config->y1; /* 230 */
    int x_fin = config->x2; /* 75 (Ancho 40) */
    int y_fin_icono = config->y1 + 40; /* 270 (Alto 40) */
    int desplazamiento_sombra = 2;
    int centro_x = x + 20;
    int centro_y = y + 20;

    /* Sombra del icono */
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(x + desplazamiento_sombra, y + desplazamiento_sombra, x_fin + desplazamiento_sombra, y_fin_icono + desplazamiento_sombra);

    /* Fondo del icono */
	setfillstyle(SOLID_FILL, config->color);
	bar(x, y, x_fin, y_fin_icono);
    setcolor(BLACK);
	rectangle(x, y, x_fin, y_fin_icono);

    /* Figura de engranaje */
    /* Dientes del engranaje (fondo) */
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(centro_x - 3, y + 4, centro_x + 3, y + 9); /* Diente superior */
    bar(centro_x - 3, y_fin_icono - 9, centro_x + 3, y_fin_icono - 4); /* Diente inferior */
    bar(x + 4, centro_y - 3, x + 9, centro_y + 3); /* Diente izquierdo */
    bar(x_fin - 9, centro_y - 3, x_fin - 4, centro_y + 3); /* Diente derecho */

    /* Cuerpo principal del engranaje (circulo) */
    setfillstyle(SOLID_FILL, DARKGRAY);
    setcolor(DARKGRAY);
    fillellipse(centro_x, centro_y, 13, 13); /* Radio 13 */

    /* Agujero central */
    setfillstyle(SOLID_FILL, WHITE); /* Color del fondo del icono */
    setcolor(WHITE);
    fillellipse(centro_x, centro_y, 5, 5); /* Radio 5 */
    setcolor(BLACK); /* Restauramos color de borde */
    circle(centro_x, centro_y, 5); /* Borde del agujero */

    /* Titulo de la app */
    settextstyle(SMALL_FONT, HORIZ_DIR, 4);
    setcolor(WHITE);
	outtextxy(config->x1 + 1, y_fin_icono + 3, "CONFIG");
}

void icono_Sistemas(const Elemento *sistemas){
	int x = sistemas->x1;
    int y = sistemas->y1;
    int x_fin = sistemas->x2;
	int y_fin = sistemas->y1 + 40;
    int desplazamiento_sombra = 2;

    /* Sombra del icono */
    setfillstyle(SOLID_FILL, DARKGRAY);
	bar(x + desplazamiento_sombra, y + desplazamiento_sombra, x_fin + desplazamiento_sombra, y_fin + desplazamiento_sombra);

    /* Fondo del icono */
	setfillstyle(SOLID_FILL, WHITE);
	bar(x, y, x_fin, y_fin);
    setcolor(BLACK);
	rectangle(x, y, x_fin, y_fin);

	/* Icono */
	setfillstyle(SOLID_FILL, BLUE);
	bar(x + 5, y + 5, x_fin -5, y_fin - 5);
	setcolor(BLACK);
	rectangle(x + 5, y + 5, x_fin -5, y_fin - 5);

	setcolor(GREEN);
	line(x + 7, y + 10, x_fin - 7, y + 10);
	line(x + 7, y + 16, x_fin - 7, y + 16);
	line(x + 7, y + 23, x_fin - 7, y + 23);
	line(x + 7, y + 29, x_fin - 7, y + 29);

    /* Titulo de la app */
    settextstyle(SMALL_FONT, HORIZ_DIR, 4);
    setcolor(WHITE); /* Color de texto en la barra de tareas */
	outtextxy(sistemas->x1 + 2, y_fin + 3, "SISTEMA");
}

void icono_Minijuego1(const Elemento *minijuego1){
	int x = minijuego1->x1; /* 90 */
    int y = minijuego1->y1; /* 35 */
    int x_fin = minijuego1->x2; /* 130 (Ancho 40) */
    int y_fin_icono = minijuego1->y1 + 40; /* 75 (Alto 40) */
    int desplazamiento_sombra = 2;

    /* Sombra del icono */
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(x + desplazamiento_sombra, y + desplazamiento_sombra, x_fin + desplazamiento_sombra, y_fin_icono + desplazamiento_sombra);

    /* Fondo del icono */
	setfillstyle(SOLID_FILL, minijuego1->color);
	bar(x, y, x_fin, y_fin_icono);
    setcolor(BLACK);
	rectangle(x, y, x_fin, y_fin_icono);

    /* Figura: control de consola */
            
    /* Cuerpo del control (Gris oscuro) */
    setfillstyle(SOLID_FILL, DARKGRAY);
    /* Cuerpo central */
    bar(x + 7, y + 12, x_fin - 7, y_fin_icono - 12);
    /* Agarre izquierdo */
    bar(x + 4, y + 16, x + 7, y_fin_icono - 16);
    /* Agarre derecho */
    bar(x_fin - 7, y + 16, x_fin - 4, y_fin_icono - 16);

    /* Cruz del control */
    setfillstyle(SOLID_FILL, BLACK);
    bar(x + 10, y + 17, x + 18, y + 23); /* Horizontal */
    bar(x + 13, y + 14, x + 15, y + 26); /* Vertical */

    /* Botones (Rojo y Azul) */
    setfillstyle(SOLID_FILL, RED);
    fillellipse(x_fin - 12, y + 18, 3, 3); /* Boton A */
    
    setfillstyle(SOLID_FILL, BLUE);
    fillellipse(x_fin - 17, y + 23, 3, 3); /* Boton B */

    /* Titulo de la app */
    settextstyle(SMALL_FONT, HORIZ_DIR, 4);
    setcolor(WHITE); /* Color de texto en la barra de tareas */
	outtextxy(minijuego1->x1 + 2, y_fin_icono + 3, "CURSOR");
}
/* FIN DE LAS FUNCIONES DE ICONOS */

/* Funcion para dibujar la barra superior de las apps, recibe el titulo */
void barra_Superior_Apps(const char *titulo){
	/* Barra */
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(APP_BARRA_X1, APP_BARRA_Y1, APP_BARRA_X2, APP_BARRA_Y2);
    
    /* Titulo */
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    setcolor(BLACK);
	outtextxy(APP_BARRA_X1 + 5, APP_BARRA_Y1 + 4, (char *)titulo); /* Usa el titulo recibidod */

    /* Boton de salir */
    setfillstyle(SOLID_FILL, RED);
    bar(APP_CERRAR_X1, APP_CERRAR_Y1, APP_CERRAR_X2, APP_CERRAR_Y2);
    
    /* Equis */
    line(APP_CERRAR_X1 + 2, APP_CERRAR_Y1 + 2, APP_CERRAR_X2 - 2, APP_CERRAR_Y2 - 2);
    line(APP_CERRAR_X1 + 2, APP_CERRAR_Y2 - 2, APP_CERRAR_X2 - 2, APP_CERRAR_Y1 + 2);

    /* Borde del cuerpo principal de la ventana */
    setcolor(BLACK);
    rectangle(APP_X1, APP_Y1, APP_X2, APP_Y2);
}

/* Funcion para entrar a la base de datos */
void sistemas(void){
	closegraph();
	
	inicializarRecetas();
    menu();
    
    inicializar_graficos();
    mocultar();
    dibujar_Computadora();
    reproducir_efecto(0);
}

void minijuego1(Elemento *B_encendido){
	/* Las constantes que tienen la palabra APP son las coordenadas de la ventana estatica que usamos todo el tiempo para cada app */
	int X1 = APP_X1 + 10, Y1 = APP_Y1 + 20, X2 = APP_X2 - 10, Y2 = APP_Y2 - 50;
    int estado_del_juego = 0;
    int puntos = 0;
	int meta = 20;
    int tiempo_restante = 15;
    int tiempo_anterior = 0;

	/* Variables para el contador */
	time_t tiempo_de_inicio = 0, tiempo_actual = 0;

    /* Coordenadas del circulo */
    int x_circulo, y_circulo, radio = 15;

	/* Buffers donde almacenamos los valores del juego convertidos a texto para poderlos mostrar */
	char buffer_de_meta[20];
    char buffer_de_puntos[20];
    char buffer_de_tiempo[20];

	APP_activa = MINIJUEGO1;

	srand((unsigned) time(NULL)); /* Este es el patron que usa rand() para generar los numeros aleatorios */

    /* Dibujo de la ventana */
    setfillstyle(SOLID_FILL, WHITE);
    bar(APP_X1, APP_Y1, APP_X2, APP_Y2);

    /* Contorno de la ventana */
    rectangle(APP_X1, APP_Y1, APP_X2, APP_Y2);

    /* Barra superior */
	barra_Superior_Apps("Cursor");

	/* Dibujo del area del juego */
	setfillstyle(SOLID_FILL, LIGHTCYAN);
	bar(X1, Y1, X2, Y2);
	setcolor(BLACK);
	rectangle(X1, Y1, X2, Y2);

	/* Boton de inicio */
    setfillstyle(SOLID_FILL, GREEN);
    bar(APP_X1 + 20, APP_Y2 - 40, APP_X1 + 120, APP_Y2 - 10);
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	outtextxy(APP_X1 + 43, APP_Y2 - 27, "INICIAR");
	setcolor(BLACK);
	rectangle(APP_X1 + 20, APP_Y2 - 40, APP_X1 + 120, APP_Y2 - 10);

    /* Puntos y tiempo */
    setcolor(BLACK);
	outtextxy(APP_X1 + 140, APP_Y2 - 30, "Puntos: 0");

	sprintf(buffer_de_meta, "%d", meta);
	outtextxy(APP_X1 + 140, APP_Y2 - 17, "Meta: ");
	outtextxy(APP_X1 + 185, APP_Y2 - 17, buffer_de_meta);

	outtextxy(APP_X1 + 240, APP_Y2 - 30, "Tiempo: 15");

    restaurar_mouse();

    while(1){
		dibujar_Fecha_Hora(); /* Actualiza durante la app */
    	verificar_Actividad();

        if(estado_del_juego == 1){
            tiempo_actual = time(NULL); /* Guarda el tiempo */

            /* Tiempo que queda */
			tiempo_restante = 15 - difftime(tiempo_actual, tiempo_de_inicio);

            /* Cambia el tiempo solo cuando cambia el segundo */
            if(tiempo_restante != tiempo_anterior){

                /* Limpia el tiempo */
                guardar_y_ocultar_mouse();
                setfillstyle(SOLID_FILL, WHITE);
				bar(APP_X1 + 300, APP_Y2 - 35, APP_X1 + 340, APP_Y2 - 15);

                /* Dibuja el tiempo*/
				sprintf(buffer_de_tiempo, "%d", tiempo_restante);
                setcolor(BLACK);
    			settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
				outtextxy(APP_X1 + 304, APP_Y2 - 30, buffer_de_tiempo);
                restaurar_mouse();

                tiempo_anterior = tiempo_restante;
            }

            /* Verifica si se perdio */
            if(tiempo_restante == 0){
				estado_del_juego = 2;
                guardar_y_ocultar_mouse();

				/* Marco del texto de derrota */
				setfillstyle(SOLID_FILL, WHITE);
				bar(APP_X1 + 105, APP_Y1 + 105, APP_X1 + 260, APP_Y1 + 137);
				setcolor(BLACK);
				rectangle(APP_X1 + 105, APP_Y1 + 105, APP_X1 + 260, APP_Y1 + 137);

				/* Texto de derrota */
                setcolor(RED);
                settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
				outtextxy(APP_X1 + 115, APP_Y1 + 115, "PERDISTE!");
                restaurar_mouse();
            }
        }

        if(mclick() == 1){
            guardar_y_ocultar_mouse();

            /* Boton de inicio */
            if(minlimit(1, APP_X1 + 20, APP_Y2 - 40, APP_X1 + 120, APP_Y2 - 10) == 1){
				/* Reinicia todo */
                puntos = 0;
                estado_del_juego = 1;
				tiempo_restante = 15;
				tiempo_de_inicio = time(NULL);

				/* Limpia la ventana */
				setfillstyle(SOLID_FILL, LIGHTCYAN);
				bar(APP_X1 + 10, APP_Y1 + 20, APP_X2 - 10, APP_Y2 - 50);
				setcolor(BLACK);
				rectangle(APP_X1 + 10, APP_Y1 + 20, APP_X2 - 10, APP_Y2 - 50);

                /* Primer circulo */
				x_circulo = (X1 + radio) + (rand() % (X2 - X1 - radio*2));
				y_circulo = (Y1 + radio + 5) + (rand() % (Y2 - Y1 - radio*2 - 5));
				setfillstyle(SOLID_FILL, YELLOW);
				fillellipse(x_circulo, y_circulo, radio, radio);
				setcolor(BLACK);
                circle(x_circulo, y_circulo, radio);

				/* Resetea los puntos */
                setfillstyle(SOLID_FILL, WHITE);
                bar(APP_X1 + 200, APP_Y2 - 35, APP_X1 + 230, APP_Y2 - 15);
                setcolor(BLACK);
                settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
				outtextxy(APP_X1 + 204, APP_Y2 - 30, "0");
            }

			/* Verifica si se presiono el circulo mientras el juego esta activo */
            else if(estado_del_juego == 1){
                if(minlimit(1, x_circulo - radio, y_circulo - radio, x_circulo + radio, y_circulo + radio) == 1){
                	reproducir_efecto(5); /* Sonido de exito */
                    puntos++;

                    /* Se actualizan los puntos */
                    setfillstyle(SOLID_FILL, WHITE);
					bar(APP_X1 + 200, APP_Y2 - 35, APP_X1 + 230, APP_Y2 - 15);
					sprintf(buffer_de_puntos, "%d", puntos);
                    setcolor(BLACK);
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
					outtextxy(APP_X1 + 206, APP_Y2 - 30, buffer_de_puntos);

                    /* Se limpia el circulo */
					setfillstyle(SOLID_FILL, LIGHTCYAN);
					setcolor(LIGHTCYAN);
                    fillellipse(x_circulo, y_circulo, radio, radio);

                    /* Se verifica si ya se gano */
                    if(puntos == meta){
						estado_del_juego = 2;

						/* Marco del texto de victoria */
						setfillstyle(SOLID_FILL, WHITE);
						bar(APP_X1 + 105, APP_Y1 + 105, APP_X1 + 245, APP_Y1 + 137);
						setcolor(BLACK);
						rectangle(APP_X1 + 105, APP_Y1 + 105, APP_X1 + 245, APP_Y1 + 137);

						/* Texto de victoria */
						setcolor(GREEN);
                        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
						outtextxy(APP_X1 + 115, APP_Y1 + 115, "GANASTE!");
					} else {
						/* Si no se gano se dibuja otro circulo */
						x_circulo = (X1 + radio) + (rand() % (X2 - X1 - radio*2));
						y_circulo = (Y1 + radio + 5) + (rand() % (Y2 - Y1 - radio*2 - 5));
						setfillstyle(SOLID_FILL, YELLOW);
						fillellipse(x_circulo, y_circulo, radio, radio);
                        setcolor(BLACK);
						circle(x_circulo, y_circulo, radio);
                    }
                }
            }

			/* Boton de apagado */
            if(minlimit(1, B_encendido->x1, B_encendido->y1, B_encendido->x2, B_encendido->y2) == 1){
                apagar_PC(B_encendido);
                break;
            }
            /* Boton de cerrar ventana */
            else if(minlimit(1, APP_CERRAR_X1, APP_CERRAR_Y1, APP_CERRAR_X2, APP_CERRAR_Y2) == 1){
                break;
            }

            restaurar_mouse();

            while(mclick() == 1){
            	delay(50);
			}
        }

		/* Escape para salir (con efecto de sonido) */
        if(kbhit()){
            int c = getch();
            guardar_y_ocultar_mouse();
            if(c==27){
            	reproducir_efecto(0);
            	break;
			}
            restaurar_mouse();
        }
    }
}
void dibujar_Configuracion(void){
	int y_fondos = APP_BARRA_Y2 + 25, x_iconos = APP_X1 + 7;
	int y_protectores = APP_BARRA_Y2 + 125;
	int ancho_iconos = 80;
	int alto_iconos = 60;
	char *nombres_protectores[] = {"Lineas", "Cuadrados", "Circulos"};
	/* El arreglo con los colores predefinidos lo puse en la libreria de globales.h */
	int i;

	/* Dibujo de la ventana */
	setfillstyle(SOLID_FILL, WHITE);
    bar(APP_X1, APP_Y1, APP_X2, APP_Y2);

    /* Contorno de la ventana */
    rectangle(APP_X1, APP_Y1, APP_X2, APP_Y2);

	/* Barra superior */
	barra_Superior_Apps("Paint");

	/* Textos de secciones */
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	setcolor(BLACK);
	outtextxy(APP_X1 + 5, APP_BARRA_Y2 + 10, "Color de fondo ___________________________");
	outtextxy(APP_X1 + 5, APP_BARRA_Y2 + 110, "Protector de pantalla ____________________");

	/* Iconos de los fondos */
	for(i=0; i<4; i++){
		x_iconos += i * ancho_iconos + (i*5);

		setfillstyle(SOLID_FILL, colores_predefinidos[i]);
		bar(x_iconos, y_fondos, x_iconos + ancho_iconos, y_fondos + alto_iconos);

		x_iconos = APP_X1 + 7;
	}

	/* Iconos de los protectores de pantalla */
    for(i=0; i<3; i++){
		x_iconos += 40 + i * ancho_iconos + (i*10);

		setfillstyle(SOLID_FILL, 8);
		bar(x_iconos, y_protectores, x_iconos + ancho_iconos, y_protectores + alto_iconos);

        setcolor(WHITE);
		outtextxy(x_iconos + 6, y_protectores + 20, nombres_protectores[i]);

		x_iconos = APP_X1 + 7;
	}

	/* Boton de aplicar */
	setfillstyle(SOLID_FILL, GREEN);
	bar(APP_X1 + 90, APP_Y2 - 45, APP_X1 + 280, APP_Y2 - 10);

	/* Texto del boton */
	setcolor(WHITE);
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
	outtextxy(APP_X1 + 135, APP_Y2 - 35, "APLICAR");
}

void configuracion(Elemento *B_encendido){
	int y_fondos = APP_BARRA_Y2 + 25, x_iconos = APP_X1 + 7;
	int y_protectores = APP_BARRA_Y2 + 125;
	int ancho_iconos = 80;
	int alto_iconos = 60;
	/* El arreglo con los colores predefinidos, nuevamente, esta en la libreria de globales.h */
	int i;

	APP_activa = CONFIGURACION;

	dibujar_Configuracion();

	restaurar_mouse();

	while(1){
		dibujar_Fecha_Hora();
		verificar_Actividad();
		
		if(mclick()==1){
			guardar_y_ocultar_mouse();

			/* Colores de fondo */
            for(i=0; i<4; i++){
				x_iconos += i * ancho_iconos + (i*5);

				/* El bucle cambiamos la coordenada 'X' y detectamos el clic sobre cada rectangulo */
				if(minlimit(1, x_iconos, y_fondos, x_iconos + ancho_iconos, y_fondos + alto_iconos) == 1){
					fondo_elegido = i;
				}

				x_iconos = APP_X1 + 7;
			}

			/* Protectores de pantalla */
            for(i=0; i<3; i++){
				x_iconos += 40 + i * ancho_iconos + (i*10);

				if(minlimit(1, x_iconos, y_protectores, x_iconos + ancho_iconos, y_protectores + alto_iconos) == 1){
					protector_elegido = i;
				}

				x_iconos = APP_X1 + 7;
			}

			if(minlimit(1, APP_X1 + 90, APP_Y2 - 45, APP_X1 + 280, APP_Y2 - 10) == 1){
				escritorio();
				dibujar_Configuracion();
			}

			/* Boton de apagar */
			if(minlimit(1, B_encendido->x1, B_encendido->y1, B_encendido->x2, B_encendido->y2) == 1){
				apagar_PC(B_encendido);
				break;
			}
			/* Boton de cerrar ventana */
			else if(minlimit(1, APP_CERRAR_X1, APP_CERRAR_Y1, APP_CERRAR_X2, APP_CERRAR_Y2) == 1){ /* Boton salir */
				break;
			}

			restaurar_mouse();

			while(mclick()==1){
				delay(50);
			}
		}
		if(kbhit()){
			int c = getch();
			guardar_y_ocultar_mouse();

			if(c==27) {
				reproducir_efecto(0);
				break;
			}

			restaurar_mouse();
		}
	}
}

void paint(Elemento *B_encendido){
	/* Posicion X e Y de los iconos */
	int y_ini = APP_BARRA_Y2;
	int x_fig = APP_X1 + 29, x_colo = APP_X1 + 14;

	/* Coreccion: era ancho y alto de los iconos. Ya las usamos monton de veces jaja, es tedioso cambiarlo asi que lo aclaramos aqui*/
	int alto_figuras = 12, ancho_figuras = 12;

	int color_seleccionado = 0, figura_seleccionada = 0;
	int hay_figura_seleccionada = 0, hay_color_seleccionado = 0;

	/* Lista de cadenas para los nombres de figuras */
	char *nombre_figura[] = {"C", "T", "C", "P", "D"};

	/* Colores disponibles, estos se pueden cambiar */
	int colores[9] = {9, 10, 11, 12, 13, 14, 1, 0, 15};

    /* Variables para la posicion del cursor al dibujar */
	int x_mouse, y_mouse;

	/* Variables de las figuras dibujadas con vertices */
	int triangulo[6], pentagono[10], diamante[8];

    int i; /* Variable de bucle */


    /* Variables para las sugerencias */
    int icono_flotando = -1; /* Osea ninguno */
    int icono_detectado_ahora = -1;
	int mx_actual = -1, my_actual = -1;
    int y_calculada;

    /* Nombres de las figuras para las sugerencias */
    char *sugerencias[] = {"Circulo", "Triangulo", "Cuadrado", "Pentagono", "Diamante"};

	APP_activa = PAINT;

	/* Dibujo de la ventana */
	setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(APP_X1, APP_Y1, APP_X2, APP_Y2);

	/* Area de dibujo */
	setfillstyle(SOLID_FILL, WHITE);
	bar(APP_X1+50, APP_BARRA_Y2, APP_X2, APP_Y2);

	/* Barra superior */
	barra_Superior_Apps("Paint");

	/* Area de herramientas */
	setfillstyle(SOLID_FILL, DARKGRAY);
	bar(APP_X1, APP_BARRA_Y2, APP_X1 + 50, APP_Y2);

	/* Contorno de la ventana */
    rectangle(APP_X1, APP_Y1, APP_X2, APP_Y2);

	/* Iconos de las figuras */
	for(i=0; i<5; i++){
		y_ini += 15 + (i * alto_figuras + (i*15)); /* Incremento segun el icono */

		/* Rectangulo */
		setcolor(BLACK);
		rectangle(x_fig, y_ini, x_fig + ancho_figuras, y_ini + alto_figuras);

		/* Letra */
		settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
		setcolor(WHITE);
		outtextxy(x_fig+3, y_ini+3, nombre_figura[i]);

		y_ini = APP_BARRA_Y2; /* Reiniciamos la variable para que el
								incremento no se vaya acumulando */
	}

	/* Iconos de los colores */
	for(i=0; i<9; i++){
		/* Incremento de la coordenada Y segun el icono */
		y_ini += 21 + (i * alto_figuras + (i*15));

		setcolor(BLACK);
		setfillstyle(SOLID_FILL, colores[i]);
		fillellipse(x_colo, y_ini, 6, 6);
		y_ini = APP_BARRA_Y2;
	}

	/* Icono boton de borrar */
	bar(x_fig, y_ini + 230, x_fig + ancho_figuras, y_ini + 242);
	setcolor(LIGHTRED);
	rectangle(x_fig, y_ini + 230, x_fig + ancho_figuras, y_ini + 242);
	setcolor(RED);
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	outtextxy(x_fig+3, y_ini + 233, "B");

	restaurar_mouse();

	while(1){
		dibujar_Fecha_Hora(); /* Actualiza */
		verificar_Actividad();


		/* Logica para mostrar las sugerencias de las figuars */
		/* Actualizamos las variables que almacenan la posicion del mouse */
		mx_actual = mxpos(1);
      	my_actual = mypos(1);
      	icono_detectado_ahora = -1; /* Reiniciamos para detectar otros iconos */
		
		/* Bucle para detectar el icono sobre el que esta el mouse */
		for(i=0; i<5; i++){
        y_calculada = APP_BARRA_Y2 + 15 + (i * alto_figuras + (i*15));

        if(mx_actual >= x_fig && mx_actual <= x_fig + ancho_figuras &&
        my_actual >= y_calculada && my_actual <= y_calculada + alto_figuras){
        	icono_detectado_ahora = i;
        	break;  
       }   
     }
		/* Se comprueba si el mouse salio o entro */
        if(icono_detectado_ahora != icono_flotando){
            int y_texto; /* Variable auxiliar para calcular altura */

			guardar_y_ocultar_mouse();
			
            /* Se limpia el mensaje anterior si habia */
            if(icono_flotando != -1){
                /* Calculamos donde estaba el texto del icono anterior para borrarlo */
                y_texto = APP_BARRA_Y2 + 15 + (icono_flotando * alto_figuras + (icono_flotando*15)) + alto_figuras;
                
                setfillstyle(SOLID_FILL, DARKGRAY);
                /* Borramos un rectangulo pequeno justo debajo del icono anterior */
                bar(APP_X1 + 1, y_texto + 1, APP_X1 + 35, y_texto + 14);
            }
            
            /* Dibujamos nuevo mensaje (si hay icono_actual) */
            if(icono_detectado_ahora != -1){
                /* Calculamos la posicion Y del icono actual */
                y_texto = APP_BARRA_Y2 + 15 + (icono_detectado_ahora * alto_figuras + (icono_detectado_ahora*15)) + alto_figuras;
                
                setcolor(YELLOW);
                settextstyle(SMALL_FONT, HORIZ_DIR, 4);
                
                /* Dibujamos el texto justo debajo del icono (y_texto + 2) */
                outtextxy(APP_X1 + 3, y_texto + 2, sugerencias[icono_detectado_ahora]);
            }
            
            /* Limpiamos el mensaje anterior (si habia) */
            if(icono_flotando != -1){
                /* Calculamos la altura Y */
                y_texto = APP_BARRA_Y2 + 15 + (icono_flotando * alto_figuras + (icono_flotando*15)) + alto_figuras;
                
                /* Borramos la parte que cae dentro del area gris de herramientas */
                setfillstyle(SOLID_FILL, DARKGRAY);
                bar(APP_X1 + 1, y_texto + 1, APP_X1 + 50, y_texto + 14);

                /* Borramos la parte que se desborda a la derecha (area de dibujo) */
                setfillstyle(SOLID_FILL, WHITE);
                bar(APP_X1 + 51, y_texto + 1, APP_X1 + 55, y_texto + 12);
            }

            restaurar_mouse();
            
            /* Actualizamos estado */
            icono_flotando = icono_detectado_ahora;
        }
        /* Fin de la logica de las sugerencias */
		
		/* Deteccion del clic */
		if(mclick()){
			guardar_y_ocultar_mouse();

			/* Bucle para detectar clic sobre las figuras. Estan alineadas horizontalmente, asi que solo hay que cambiar la coord. X */
			for(i=0; i<5; i++){
            	y_ini += 15 + (i * alto_figuras + (i*15)); /* Posicion Y segun el icono */

				/* Aqui es donde se pregunta si se presiono algun boton de figura */
				if (minlimit(1, x_fig, y_ini, x_fig + ancho_figuras, y_ini + alto_figuras) == 1){

					/* Si la figura seleccionada no es la actual se resetea su borde */
					if(hay_figura_seleccionada && figura_seleccionada != i){
						int y_viejo = APP_BARRA_Y2 + 15 + (figura_seleccionada * alto_figuras + (figura_seleccionada*15));
						setcolor(BLACK);
						rectangle(x_fig, y_viejo, x_fig + ancho_figuras, y_viejo + alto_figuras);
					}

					/* Se selecciona la figura actual dibujandole un borde amarillo */
					setcolor(YELLOW);
					rectangle(x_fig, y_ini, x_fig + ancho_figuras, y_ini + alto_figuras);
					figura_seleccionada = i;
					hay_figura_seleccionada = 1;
				}

				y_ini = APP_BARRA_Y2;
			}

			/* Evalua si algun color fue presionado */
			for(i=0; i<9; i++){
            	y_ini += 21 + (i * alto_figuras + (i*15));

				/* Se pregunta si se presiono algun boton de color */
				if (minlimit(1, x_colo-6, y_ini-6, x_colo + 6, y_ini + 6) == 1){

					/* Si el color seleccionado no es el actual se resetea su borde */
					if(hay_color_seleccionado && color_seleccionado != i){
						int y_viejo = APP_BARRA_Y2 + 21 + (color_seleccionado * alto_figuras + (color_seleccionado*15));
						setcolor(BLACK);
						circle(x_colo, y_viejo, 6);
					}

					/* Se selecciona la figura actual dibujandole un borde amarillo */
					setcolor(YELLOW);
					circle(x_colo, y_ini, 6);
					color_seleccionado = i;
					hay_color_seleccionado = 1;
				}

				y_ini = APP_BARRA_Y2;
			}


			/* Clic en el area de dibujo (la limitamos un poco para que no saliera del borde) */
			if(minlimit(1, APP_X1+61, APP_BARRA_Y2+11, APP_X2-11, APP_Y2-11) == 1 && hay_color_seleccionado
			&& hay_figura_seleccionada){
				/* Inicializacion de variables para las formas */
				x_mouse = mxpos(1);
				y_mouse = mypos(1);

				triangulo[0] = x_mouse-10;
				triangulo[1] = y_mouse+10;
				triangulo[2] = x_mouse;
				triangulo[3] = y_mouse-10;
				triangulo[4] = x_mouse+10;
				triangulo[5] = y_mouse+10;

				pentagono[0] = x_mouse-5;
				pentagono[1] = y_mouse+10;
				pentagono[2] = x_mouse-10;
				pentagono[3] = y_mouse;
				pentagono[4] = x_mouse;
				pentagono[5] = y_mouse-10;
				pentagono[6] = x_mouse+10;
				pentagono[7] = y_mouse;
				pentagono[8] = x_mouse+5;
				pentagono[9] = y_mouse+10;

				diamante[0] = x_mouse;
				diamante[1] = y_mouse+15;
				diamante[2] = x_mouse-10;
				diamante[3] = y_mouse;
				diamante[4] = x_mouse;
				diamante[5] = y_mouse-15;
				diamante[6] = x_mouse+10;
				diamante[7] = y_mouse;

				/* Se establece como color de dibujo el color seleccionado */
				setcolor(colores[color_seleccionado]);
				setfillstyle(SOLID_FILL, colores[color_seleccionado]);

				/* Se dibuja la figura seleccionada en la coordenada donde esta el cursor */
				if(figura_seleccionada == 0){
					fillellipse(mxpos(1), mypos(1), 10, 10);
				}
				else if(figura_seleccionada == 1){
					fillpoly(3, triangulo);
				}
				else if(figura_seleccionada == 2){
					bar(x_mouse-10, y_mouse-10, x_mouse+10, y_mouse+10);
				}
				else if(figura_seleccionada == 3){
					fillpoly(5, pentagono);
				}
				else if(figura_seleccionada == 4){
					fillpoly(4, diamante);
				}
			}
			/* Boton de borrar */
			else if(minlimit(1, x_fig, y_ini + 230, x_fig + ancho_figuras, y_ini + 242) == 1){
				setfillstyle(SOLID_FILL, WHITE);
				bar(APP_X1+51, APP_BARRA_Y2+1, APP_X2-1, APP_Y2-1);
			}

			/* Boton de apagar */
			if(minlimit(1, B_encendido->x1, B_encendido->y1, B_encendido->x2, B_encendido->y2) == 1){
				apagar_PC(B_encendido);
				break;
			}
			/* Boton de cerrar ventana */
			else if(minlimit(1, APP_CERRAR_X1, APP_CERRAR_Y1, APP_CERRAR_X2, APP_CERRAR_Y2) == 1){ /* Boton salir */
				break;
			}

			restaurar_mouse();
			if(mclick() == 1){
				delay(15);
			}
		}
		if(kbhit()){
			int c = getch();
			guardar_y_ocultar_mouse();
			if(c == 27){
				reproducir_efecto(0);
				break;
			}
			restaurar_mouse();
		}
	}
}

void resetear_pantalla_calculadora(char *texto){
    setfillstyle(SOLID_FILL, WHITE);
    bar(APP_X1 + 10, APP_Y1 + 40, APP_X2 - 10, APP_Y1 + 70);
    setcolor(BLACK);
    rectangle(APP_X1 + 10, APP_Y1 + 40, APP_X2 - 10, APP_Y1 + 70);
    settextstyle(SMALL_FONT, HORIZ_DIR, 5);
    outtextxy(APP_X1 + 20, APP_Y1 + 45, texto);
}

/* _________________________________________________________________________ */
/* CALCULADORA                                                               */
/* _________________________________________________________________________ */
void calculadora(Elemento * B_encendido){
    /* Declaracion de variables */
    char tecla;
    char pantalla[20] = "0";
    double num1 = 0, num2 = 0, resultado = 0;
    char operacion = 0;
    int i, j;
    int x_ini; /* = APP_X1 + 30; */
    int y_ini; /* = APP_Y1 + 38; */
    int ancho = 50, alto = 37;
    int alto_operaciones = 28;
    int x, y;
    char num;
    char num_detectado;
    char texto[2];
    char ops[] = "+-*/=C";
    char t[2];

    int numero_cliqueado;
    int op_cliqueada;
    int len;

    /* Dibujo de la ventana */
	setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(APP_X1, APP_Y1, APP_X2, APP_Y2);
    setcolor(BLACK);
    rectangle(APP_X1, APP_Y1, APP_X2, APP_Y2);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
	outtextxy(APP_X1 + 95, APP_Y1 + 20, "CALCULADORA");

	barra_Superior_Apps(" ");

    /* Pantalla de visualizacion */
    resetear_pantalla_calculadora(pantalla); /* Llama a la funcion para dibujar la pantalla */

    /* --- RECALCULO DE BOTONES (usando constantes) --- */
    x_ini = APP_X1 + 20;
    y_ini = APP_Y1 + 75;

    /* Botones numericos (1-9) */
    setcolor(BLACK);
    settextstyle(SMALL_FONT, HORIZ_DIR, 5);
    for(i=0; i<3; i++){ /* Filas */
        for(j=0; j<3; j++){ /* Columnas */
            x = x_ini + j * (ancho + 5);
            y = y_ini + i * (alto + 5);

            setfillstyle(SOLID_FILL, CYAN);
            bar(x, y, x + ancho, y + alto);
            rectangle(x, y, x + ancho, y + alto);

            num='1' + i*3 + j;
            texto[0] = num;
            texto[1] = '\0';
            outtextxy(x + 20, y + 8, texto);
        }
    }

    /* Boton 0 */
    x = x_ini;
    y = y_ini + 3 * (alto + 5);
    setfillstyle(SOLID_FILL, CYAN);
    bar(x, y, x + ancho, y + alto);
    rectangle(x, y, x + ancho, y + alto);
    outtextxy(x + 20, y + 8, (char *)"0");

    /* Botones de operaciones */
    for(i=0; i<6; i++) {
        x = APP_X2 - 70; /* Alinear a la derecha de la app */
        y = y_ini + i * (alto_operaciones + 4);
        setfillstyle(SOLID_FILL, MAGENTA);
        bar(x, y, x + ancho, y + alto_operaciones);
        rectangle(x, y, x + ancho, y + alto_operaciones);
        t[0] = ops[i];
        t[1] = '\0';
        outtextxy(x + 20, y + 4, t);
    }

    /* Resetear las coordenadas para detectar clics */
    x_ini = APP_X1 + 20;
    y_ini = APP_Y1 + 80;

    restaurar_mouse();

    while (1) {
    	dibujar_Fecha_Hora(); /* Actualiza */
    	verificar_Actividad();
    	
        if(mclick()==1){
            numero_cliqueado = 0;

            guardar_y_ocultar_mouse();

            for(i=0; i<3; i++){ /* Filas */
                for(j=0; j<3; j++){ /* Columnas */
                    x = x_ini + j * (ancho + 5);
                    y = y_ini + i * (alto + 5);

                    if(minlimit(1, x, y, x + ancho, y + alto) == 1){
                        num_detectado = '1' + i*3 + j;
                        numero_cliqueado = 1;
                        break; /* Sale del bucle */
                    }
                }
                if(numero_cliqueado == 1) break;
            }

            /* Deteccion del 0 */
            if(numero_cliqueado == 0) {
                x = x_ini;
                y = y_ini + 3 * (alto + 5);
                if(minlimit(1, x, y, x + ancho, y + alto) == 1){
                    num_detectado = '0';
                    numero_cliqueado = 1;
                }
            }

          if(numero_cliqueado == 1){
                /* Se hace lo mismo que con las teclas */
                reproducir_efecto(0); /* Sonido de tecla clic */
               
                len = strlen(pantalla); 

                if (operacion == 0 && num1 != 0.0) {
                    len = 0;
                    pantalla[0] = '\0';
                    num1 = 0.0;
                }
                else if(strcmp(pantalla, "0") == 0){
                    len = 0;
                    pantalla[0] = '\0';
                }

                if (len < 15) {
                    pantalla[len] = num_detectado;
                    pantalla[len+1] = '\0';
                    resetear_pantalla_calculadora(pantalla);
                }
            }
            /* Si no se presiono un boton del teclado numerico se sigue preguntando con los demas botones */
            else {
                /* Boton de apagar */
                if(minlimit(1, B_encendido->x1, B_encendido->y1, B_encendido->x2, B_encendido->y2) == 1){
                    apagar_PC(B_encendido);
                    break;
                }
                else if(minlimit(1, APP_CERRAR_X1, APP_CERRAR_Y1, APP_CERRAR_X2, APP_CERRAR_Y2) == 1){ /* Boton salir */
                    break;
                }

                /* Deteccion de botones de operacion */
                op_cliqueada = -1; /* Ninguna */

                for(i=0; i<6; i++) {
                    x = APP_X2 - 70;
                    y = y_ini + i * (alto_operaciones + 4);
                    if(minlimit(1, x, y, x + ancho, y + alto_operaciones) == 1) {
                        op_cliqueada = i;
                        break;
                    }
                }


					if (op_cliqueada != -1) {
						char op_char = ops[op_cliqueada];
						if (op_char == '+') {
                        num1 = atof(pantalla);
                        operacion = '+';
                        strcpy(pantalla, "0");
                    } else if (op_char == '-') {
                        num1 = atof(pantalla);
                        operacion = '-';
                        strcpy(pantalla, "0");
                    } else if (op_char == '*') {
                        num1 = atof(pantalla);
                        operacion = '*';
                        strcpy(pantalla, "0");
                    } else if (op_char == '/') {
                        num1 = atof(pantalla);
                        operacion = '/';
                        strcpy(pantalla, "0");
                    } else if (op_char == '=') {
                        num2 = atof(pantalla);
                        if(operacion == '/' && num2 == 0){
                            strcpy(pantalla, "Error: div / 0");
                            num1 = num2 = resultado = 0.0;
                            operacion = 0;
                        } else {
                            switch(operacion){
                                case '+': resultado = num1 + num2; break;
                                case '-': resultado = num1 - num2; break;
                                case '*': resultado = num1 * num2; break;
                                case '/': resultado = (num2 != 0.0) ? num1 / num2 : 0.0; break;
                                default: resultado = num2; /* Si no hay operacion, solo muestra el num2 */
                            }
                            sprintf(pantalla, "%.2f", resultado);
                            num1 = resultado;
                            operacion = 0;
                        }
                    } else if (op_char == 'C') {
                        strcpy(pantalla, "0");
                        num1 = num2 = resultado = 0.0;
                        operacion = 0;
                    }
                    resetear_pantalla_calculadora(pantalla);
                }
            }

            restaurar_mouse();

            while(mclick() == 1){
                delay(50);
            }
        }

        if (kbhit()) {
            tecla = getch();
            reproducir_efecto(0); /* Sonido al teclear */
            guardar_y_ocultar_mouse();

            if (tecla>='0' && tecla<='9'){
               int len = strlen(pantalla);

                /* Limpia el resultado anterior */
                if (operacion == 0 && num1 != 0.0) {
                    len = 0;
                    pantalla[0] = '\0';
                    num1 = 0.0;
                }
                else if(strcmp(pantalla, "0") == 0){
                    len = 0;
                    pantalla[0] = '\0';
                }

                if (len < 15) {
                    pantalla[len] = tecla;
                    pantalla[len+1] = '\0';

                    resetear_pantalla_calculadora(pantalla);
                }
            } else if (tecla == '+' || tecla == '-' || tecla == '*' || tecla == '/') {
                num1 = atof(pantalla);
                operacion = tecla;
                strcpy(pantalla, "0"); /* Limpia para el siguiente numero */
                resetear_pantalla_calculadora(pantalla);
            } else if(tecla == '=' || tecla == 13) { /* Enter tambien calcula */
                num2 = atof(pantalla);
                switch (operacion) {
                    case '+': resultado = num1 + num2; break;
                    case '-': resultado = num1 - num2; break;
                    case '*': resultado = num1 * num2; break;
                    case '/': 
                        if (num2 != 0.0) {
                            resultado = num1 / num2;
                        } else {
                            strcpy(pantalla, (char *)"Error: division / 0");
                            num1 = num2 = resultado = 0.0;
                            operacion = 0;
                            resetear_pantalla_calculadora(pantalla);
                            restaurar_mouse();

                        }
                        break;
                    default: resultado = num2; /* Si no hay operacion, solo muestra el num2 */
                }

                sprintf(pantalla, "%.2f", resultado);
                num1 = resultado; /* Para calcular con el resultado */
                operacion = 0; /* Reinicia operacion */

                resetear_pantalla_calculadora(pantalla);
            } else if(tecla == 'c' || tecla == 'C') {
                strcpy(pantalla, "0");
                num1 = num2 = resultado = 0.0;
                operacion = 0;

                resetear_pantalla_calculadora(pantalla);
            } else if(tecla == 27) { /* ESC para salir */
                break;
            }
            restaurar_mouse();
        }
    }
}
