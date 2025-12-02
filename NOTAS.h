#ifndef NOTAS_H
#define NOTAS_H

#include <ctype.h>
#include <globales.h>
#include <dos.h>

/* FUNCIONES DE MOUSE */
/* Mouse.h nos daba problema asi que recreamos sus funciones aqui mismo leyendo su codigo */

/* Inicializa el mouse. Retorna 0 si falla, num botones si funciona */
int iniciar_raton() {
    union REGS i, o;
    i.x.ax = 0;
    int86(0x33, &i, &o);
    return (o.x.ax == 0) ? 0 : o.x.bx;
}

/* Muestra el puntero en pantalla */
void mostrar_raton() {
    union REGS i, o;
    i.x.ax = 1;
    int86(0x33, &i, &o);
}

/* Oculta el puntero */
void ocultar_raton() {
    union REGS i, o;
    i.x.ax = 2;
    int86(0x33, &i, &o);
}

/* Lee estado: x, y son coordenadas. btn: 1=Izq, 2=Der */
void leer_raton(int *x, int *y, int *btn) {
    union REGS i, o;
    i.x.ax = 3;
    int86(0x33, &i, &o);
    *x = o.x.cx;
    *y = o.x.dx;
    *btn = o.x.bx;
}

/* PROTOTIPOS */
void barra_Superior_Notas(char *titulo, int mostrar_menu_archivo);
void limpiar_Area_Texto(void);
void dibujar_Texto_App(int num_lineas, Cadenas lineas[], char *titulo, int cursor_i, int cursor_j);
void guardar_Archivo(Cadenas lineas[], const char *nombre_archivo);
int cargar_Archivo(Cadenas lineas[], const char *nombre_archivo);
void mostrar_Menu_Status_App(const char *mensaje);
void dibujar_Menu_Archivo(void);
void actualizar_Indice(IndiceArchivos *indice, const char *nombre);
int pedir_Nombre_Archivo(char *buffer_nombre, int max_len, int accion, Cadenas lineas[], char *titulo_dinamico, IndiceArchivos *indice, int cursor_i, int cursor_j);
int explorador_Archivos(IndiceArchivos *indice, Cadenas lineas[], char *filename_actual, char *titulo_dinamico, int cursor_i, int cursor_j);
void bloc_de_notas(void);

/* PROTOTIPOS */
int iniciar_raton(void);     
void mostrar_raton(void);    
void ocultar_raton(void);     
void leer_raton(int *x, int *y, int *btn); 

/* IMPLEMENTACIONES */
void barra_Superior_Notas (char *titulo, int mostrar_menu_archivo) {
    int centro_x = APP_X1 + ((APP_X2 - APP_X1) / 2);

	/* Ventana */
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(APP_X1, APP_Y1, APP_X2, APP_BARRA_Y2);

    setcolor(BLACK);
    settextjustify(CENTER_TEXT, TOP_TEXT); /* Alinear al centro */
    outtextxy(centro_x, APP_Y1 + 5, titulo); 
    settextjustify(LEFT_TEXT, TOP_TEXT); /* Restaurar alineacion */

    if (mostrar_menu_archivo) {
    dibujar_Menu_Archivo();
    }

	/* Boton de cerrar ventana */
    setfillstyle(SOLID_FILL, RED);
    bar(APP_CERRAR_X1, APP_CERRAR_Y1, APP_CERRAR_X2, APP_CERRAR_Y2);
    
    /* Dibujo de la equis */
    setcolor(BLACK);
    line(APP_CERRAR_X1 + 2, APP_CERRAR_Y1 + 2, APP_CERRAR_X2 - 2, APP_CERRAR_Y2 - 2);
    line(APP_CERRAR_X1 + 2, APP_CERRAR_Y2 - 2, APP_CERRAR_X2 - 2, APP_CERRAR_Y1 + 2);

    /* Borde del cuerpo principal de la ventana */
    setcolor(BLACK);
    rectangle(APP_X1, APP_Y1, APP_X2, APP_Y2);
}

void limpiar_Area_Texto(void) {
    setfillstyle(SOLID_FILL, WHITE);
    bar(APP_X1 + 1, APP_BARRA_Y2 + 1, APP_X2 - 1, APP_Y2 - 16);
    setcolor(BLACK);
}

void mostrar_Menu_Status_App(const char *mensaje) {
    int x_ini = APP_X1, x_fin = APP_X2, y_ini = APP_Y2 - 15, y_fin = APP_Y2;

	/* Barra inferior */
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(x_ini, y_ini, x_fin, y_fin);
    
    /* Redibujo del borde */
    setcolor(BLACK);
    rectangle(APP_X1, APP_Y1, APP_X2, APP_Y2);
    
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);

    if (mensaje != NULL && mensaje[0] != '\0') {
        /* Si el explorador manda mensaje se muestra solo el mensaje */
        settextjustify(RIGHT_TEXT, TOP_TEXT);
        outtextxy(x_fin - 5, y_ini + 5, (char*)mensaje);
    } 
    else {
        /* Si no hay mensaje se hace el uso normal)*/
        settextjustify(LEFT_TEXT, TOP_TEXT);
        outtextxy(x_ini + 5, y_ini + 5, (char*)"F1: Guardar | F2: Abrir | ESC: Salir"); 
    }
}

void dibujar_Texto_App(int num_lineas, Cadenas lineas[], char *titulo, int cursor_i, int cursor_j) {
    int k;
    char temp_str[MAX_CHARS_POR_LINEA + 5];
    int cursor_x_pos;
    int cursor_y_pos;

    limpiar_Area_Texto();
    setcolor(BLACK);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	settextjustify(LEFT_TEXT, TOP_TEXT);
	
    for (k = 0; k < num_lineas; k++) {
        int y_pos = APP_TEXT_Y + (k * ESPACIADO_Y);
        outtextxy(APP_TEXT_X, y_pos, lineas[k].texto);
    }

    if (cursor_i < 0) cursor_i = 0;
    if (cursor_i >= num_lineas) cursor_i = num_lineas - 1;

    cursor_y_pos = APP_TEXT_Y + (cursor_i * ESPACIADO_Y);
    memset(temp_str, 0, sizeof(temp_str));
    
    if (cursor_j > 0) {
        int len = strlen(lineas[cursor_i].texto);
        if (cursor_j > len) cursor_j = len;
        strncpy(temp_str, lineas[cursor_i].texto, cursor_j);
        temp_str[cursor_j] = '\0';
    } else {
        temp_str[0] = '\0';
    }

    cursor_x_pos = APP_TEXT_X + textwidth(temp_str);
    setcolor(BLACK);
    outtextxy(cursor_x_pos, cursor_y_pos, (char*)"_");

    barra_Superior_Notas(titulo, 1);
    mostrar_Menu_Status_App(NULL);
}

void guardar_Archivo(Cadenas lineas[], const char *nombre_archivo) {
    FILE *fp;
    int k;
    fp = fopen(nombre_archivo, "w");
    if (fp == NULL) {
        mostrar_Menu_Status_App("ERROR: No se pudo guardar.");
        return;
    }
    for (k = 0; k < MAX_LINEAS; k++) {
        if (strlen(lineas[k].texto) > 0) fprintf(fp, "%s\n", lineas[k].texto);
        else break; 
    }
    fclose(fp);
    mostrar_Menu_Status_App("Archivo GUARDADO!");
}

int cargar_Archivo(Cadenas lineas[], const char *nombre_archivo) {
    FILE *fp;
    int k = 0;
    fp = fopen(nombre_archivo, "r");
    if (fp == NULL) {
        return 0;
    }
    for (k = 0; k < MAX_LINEAS; k++) lineas[k].texto[0] = '\0';
    k = 0;
    while (k < MAX_LINEAS && fgets(lineas[k].texto, MAX_CHARS_POR_LINEA + 1, fp) != NULL) {
        size_t len = strlen(lineas[k].texto);
        if (len > 0 && lineas[k].texto[len - 1] == '\n') lineas[k].texto[len - 1] = '\0';
        k++;
    }
    fclose(fp);
    mostrar_Menu_Status_App("Archivo CARGADO!");
    return 1;
}

void dibujar_Menu_Archivo(void) {
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(MENU_ARCHIVO_X1, MENU_ARCHIVO_Y1, MENU_ARCHIVO_X2, APP_BARRA_Y2);
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(MENU_ARCHIVO_X1 + 5, APP_Y1 + 5, (char*)"Archivo (F2)"); 
}

void actualizar_Indice(IndiceArchivos *indice, const char *nombre) {
    int i;
    
    for (i = 0; i < indice->count; i++) {
        if (strcmp(indice->nombres[i], nombre) == 0) { 
            return;
        }
    }

    if (strcmp(nombre, FILENAME_DEFAULT) != 0) {
        for (i = 0; i < indice->count; i++) {
            if (strcmp(indice->nombres[i], FILENAME_DEFAULT) == 0) {

                strncpy(indice->nombres[i], nombre, MAX_FILENAME_LEN - 1);
                indice->nombres[i][MAX_FILENAME_LEN - 1] = '\0';

                remove(FILENAME_DEFAULT);
                
                return;
            }
        }
    }
    
    if (indice->count < MAX_FILES) {
        strncpy(indice->nombres[indice->count], nombre, MAX_FILENAME_LEN - 1);
        indice->nombres[indice->count][MAX_FILENAME_LEN - 1] = '\0';
        indice->count++;
    }
}

int pedir_Nombre_Archivo(char *buffer_nombre, int max_len, int accion, Cadenas lineas[], char *titulo_dinamico, IndiceArchivos *indice, int cursor_i, int cursor_j) {
    int dialog_x1 = APP_X1 + 50;
    int dialog_y1 = APP_BARRA_Y2 + 50;
    int dialog_x2 = APP_X2 - 50;
    int dialog_y2 = dialog_y1 + 50;
    int text_x = dialog_x1 + 10;
    int text_y = dialog_y1 + 10;
    char c;
    int j = 0;
    const char *prompt_text = (accion == ACCION_GUARDAR) ? "GUARDAR como (.TXT):" : "ABRIR archivo (.TXT):";
    char temp_str[MAX_FILENAME_LEN]; 
    int cursor_x;
    int resultado = 0; /* 0: Cancelado/Fallo, 1: Exito */

    /* Dibujar el cuadro de dialogo (Fondo) */
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(dialog_x1, dialog_y1, dialog_x2, dialog_y2);
    setcolor(BLACK);
    rectangle(dialog_x1, dialog_y1, dialog_x2, dialog_y2);

    settextjustify(LEFT_TEXT, TOP_TEXT); 
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(text_x, text_y, (char*)prompt_text);

    /* Dibujar el campo de texto */
    setfillstyle(SOLID_FILL, WHITE);
    bar(text_x, text_y + 15, dialog_x2 - 10, dialog_y2 - 10);
    rectangle(text_x, text_y + 15, dialog_x2 - 10, dialog_y2 - 10);

    buffer_nombre[0] = '\0';

    while (1) {
        if (kbhit()) {
            c = getch();
            if (c == 13) { /* ENTER */
                buffer_nombre[j] = '\0';
                
                if (accion == ACCION_GUARDAR && strlen(buffer_nombre) > 0) {
                    guardar_Archivo(lineas, buffer_nombre);
                    actualizar_Indice(indice, buffer_nombre);
                    sprintf(titulo_dinamico, "Bloc De Notas - [%s]", buffer_nombre);
                    resultado = 1;
                }
                break;
            } else if (c == 27) { /* Escape */
                buffer_nombre[0] = '\0'; /* Cancelar */
                break;
            } else if (c == 8) { /* Tecla de borrado (basckspace) */
                if (j > 0) {
                    j--;
                    memmove(&buffer_nombre[j], &buffer_nombre[j+1], strlen(&buffer_nombre[j+1]) + 1);
                }
            } else if (c >= 32 && c <= 126) { /* Caracter imprimible */
                if (j < max_len - 1) {
                    memmove(&buffer_nombre[j + 1], &buffer_nombre[j], strlen(&buffer_nombre[j]) + 1);
                    buffer_nombre[j] = (char) toupper(c);
                    j++;
                }
            }

            /* Redibujar campo de texto */
            setfillstyle(SOLID_FILL, WHITE);
            bar(text_x + 1, text_y + 16, dialog_x2 - 11, dialog_y2 - 11);
            setcolor(BLACK);
            settextjustify(LEFT_TEXT, TOP_TEXT); 
            outtextxy(text_x + 2, text_y + 17, buffer_nombre);

            /* Dibujar cursor */
            memset(temp_str, 0, sizeof(temp_str));
            strncpy(temp_str, buffer_nombre, j);
            temp_str[j] = '\0';
            cursor_x = text_x + 2 + textwidth(temp_str);
            
            outtextxy(cursor_x, text_y + 17, (char*)"_");
        }
        delay(30);
    }

    /* Limpiar el cuadro de dialogo despues de la entrada */
    setfillstyle(SOLID_FILL, WHITE);
    bar(dialog_x1, dialog_y1, dialog_x2, dialog_y2);
    /* Redibujar la app principal detras con el nuevo estado o titulo si aplica */
    dibujar_Texto_App(MAX_LINEAS, lineas, titulo_dinamico, cursor_i, cursor_j); 

    return resultado;
}

int explorador_Archivos(IndiceArchivos *indice, Cadenas lineas[], char *filename_actual, char *titulo_dinamico, int cursor_i, int cursor_j) {
    int dialog_x1 = APP_X1 + 30;
    int dialog_y1 = APP_BARRA_Y2 + 10;
    int dialog_x2 = APP_X2 - 30;
    int dialog_y2 = APP_Y2 - 10;
    int k;
    int seleccionado = 0;
    int item_height = 15;
    int offset_y = 5;
    char c; 
    
    /* Variables para controlar el redibujado */
    int ultimo_seleccionado = -1; /* Empieza en ninguno */
    int forzar_redibujado = 1;

    if (indice->count == 0) {
        mostrar_Menu_Status_App((char*)"No hay archivos guardados.");
        delay(1000);
        dibujar_Texto_App(MAX_LINEAS, lineas, titulo_dinamico, cursor_i, cursor_j);
        return 0;
    }
    
    /* Marco estatico */
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(dialog_x1, dialog_y1, dialog_x2, dialog_y2);
    setcolor(BLACK);
    rectangle(dialog_x1, dialog_y1, dialog_x2, dialog_y2);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    setcolor(BLACK);
    outtextxy(dialog_x1 + 5, dialog_y1 + 5, (char*)"ABRIR Archivo");

    /* Fondo blanco */
    setfillstyle(SOLID_FILL, WHITE);
    bar(dialog_x1 + 5, dialog_y1 + 25, dialog_x2 - 5, dialog_y2 - 5);
    rectangle(dialog_x1 + 5, dialog_y1 + 25, dialog_x2 - 5, dialog_y2 - 5);

    if (seleccionado >= indice->count) seleccionado = indice->count - 1;
    if (seleccionado < 0) seleccionado = 0;

	settextjustify(LEFT_TEXT, TOP_TEXT);
    mostrar_Menu_Status_App((char*)"ABRIR Archivo: Use ARRIBA/ABAJO, ENTER.");

    while (1) {
        verificar_Actividad();
        /* Solo se dibuja si algo cambio */
        if (seleccionado != ultimo_seleccionado || forzar_redibujado) {
        	settextjustify(LEFT_TEXT, TOP_TEXT);
            for (k = 0; k < indice->count; k++) {
                int item_y = dialog_y1 + 30 + (k * item_height);
                
                /* Dibujamos el fondo de cada item */
                if (k == seleccionado) {
                    /* Item seleccionado: Fondo Azul */
                    setfillstyle(SOLID_FILL, LIGHTBLUE);
                    bar(dialog_x1 + 6, item_y - offset_y, dialog_x2 - 6, item_y + item_height - offset_y);
                    setcolor(WHITE);
                } else {
                    /* Item no seleccionado: Fondo Blanco */
                    setfillstyle(SOLID_FILL, WHITE);
                    bar(dialog_x1 + 6, item_y - offset_y, dialog_x2 - 6, item_y + item_height - offset_y);
                    setcolor(BLACK);
                }
                outtextxy(dialog_x1 + 20, item_y, indice->nombres[k]);
            }
            
            ultimo_seleccionado = seleccionado;
            forzar_redibujado = 0;
        }

        if (kbhit()) {
            c = getch();
            if (c == 0) { 
                c = getch();
                if (c == 72) { /* Arriba */
                    if (seleccionado > 0) seleccionado--;
                    else seleccionado = indice->count - 1;
                } else if (c == 80) { /* Abajo */
                    if (seleccionado < indice->count - 1) seleccionado++;
                    else seleccionado = 0;
                }
            } else if (c == 27) { /* ESC */
                mostrar_Menu_Status_App(NULL); 
                dibujar_Texto_App(MAX_LINEAS, lineas, titulo_dinamico, cursor_i, cursor_j); 
                return 0;
            } else if (c == 13) { /* ENTER */
                if (seleccionado != -1 && cargar_Archivo(lineas, indice->nombres[seleccionado])) {
                    strncpy(filename_actual, indice->nombres[seleccionado], MAX_FILENAME_LEN-1);
                    filename_actual[MAX_FILENAME_LEN-1] = '\0';
                    sprintf(titulo_dinamico, "Bloc De Notas - [%s]", filename_actual);
                    
                    dibujar_Texto_App(MAX_LINEAS, lineas, titulo_dinamico, 0, 0); 
                    return 1;
                } else {
                    mostrar_Menu_Status_App((char*)"Error al cargar.");
                    delay(1000);
                    mostrar_Menu_Status_App((char*)"ABRIR Archivo: Use ARRIBA/ABAJO, ENTER.");
                    forzar_redibujado = 1; /* Para que se vuelva a ver la lista tras el error */
                }
            } 
        }
        /* Delay corto para respuesta rapida */
        delay(15); 
    }
    return 0; 
}
   void bloc_de_notas(void) {
    static Cadenas lineas[MAX_LINEAS];
    int i, j;
    char c;
    int pc_apagado = 0;
    static IndiceArchivos indice;
    static char filename_actual[MAX_FILENAME_LEN] = FILENAME_DEFAULT;
    char titulo_dinamico[80];
    int cambiado;

    /* Variables para el mouse */
    int mx, my, mbtn;
    
    /* Variables auxiliares para logica del clic */
    int nueva_i;
    int ancho_acumulado;
    int len_linea;
    int k_char;
    int ancho_char;
    char char_temp[2];

    /* Inicializar raton */
    if (!iniciar_raton()) {
        mostrar_Menu_Status_App("Error: No se detecta Mouse.");
    } else {
        mostrar_raton();
    }

    /* Inicializar y cargar archivo por defecto */
    if (indice.count == 0) {
        actualizar_Indice(&indice, FILENAME_DEFAULT);
    }

    for (i = 0; i < MAX_LINEAS; i++) lineas[i].texto[0] = '\0';
    cargar_Archivo(lineas, FILENAME_DEFAULT);
    i = 0; j = 0;

    sprintf(titulo_dinamico, "Bloc De Notas - [%s]", filename_actual);
    
    ocultar_raton();
    barra_Superior_Notas(titulo_dinamico, 1);
    mostrar_Menu_Status_App(NULL);
    dibujar_Texto_App(MAX_LINEAS, lineas, titulo_dinamico, i, j);
    mostrar_raton();

    while (1) {
        verificar_Actividad();

        /* Lectura del mouse */
        leer_raton(&mx, &my, &mbtn);
        
        if (mbtn == 1) { /* Clic Izquierdo */
            
            /* Clic en boton cerrar */
            if (mx >= APP_CERRAR_X1 && mx <= APP_CERRAR_X2 && 
                my >= APP_CERRAR_Y1 && my <= APP_CERRAR_Y2) {
                break;
            }

            /* Clic en menu archivo */
            else if (mx >= MENU_ARCHIVO_X1 && mx <= MENU_ARCHIVO_X2 && 
                     my >= MENU_ARCHIVO_Y1 && my <= APP_BARRA_Y2) {
                ocultar_raton();
                cambiado = explorador_Archivos(&indice, lineas, filename_actual, titulo_dinamico, i, j);
                if (cambiado) { i = 0; j = 0; }
                mostrar_raton();
            }

            /* Clic dentro del area de texto */
            else if (mx >= APP_X1 && mx <= APP_X2 && 
                     my >= APP_TEXT_Y && my <= APP_Y2 - 20) {
                
                /* Calcular Fila (i) */
                nueva_i = (my - APP_TEXT_Y) / ESPACIADO_Y;
                
                if (nueva_i < 0) nueva_i = 0;
                if (nueva_i >= MAX_LINEAS) nueva_i = MAX_LINEAS - 1;
                
                /* Calcular Columna (j) aproximada */
                ancho_acumulado = APP_TEXT_X;
                len_linea = strlen(lineas[nueva_i].texto);
                char_temp[1] = '\0'; /* Terminador nulo fijo */
                
                j = 0; 
                
                for (k_char = 0; k_char < len_linea; k_char++) {
                    char_temp[0] = lineas[nueva_i].texto[k_char];
                    ancho_char = textwidth(char_temp);
                    
                    if (mx > ancho_acumulado + (ancho_char / 2)) {
                        ancho_acumulado += ancho_char;
                        j++;
                    } else {
                        break;
                    }
                }
                
                i = nueva_i;
                
                ocultar_raton();
                dibujar_Texto_App(MAX_LINEAS, lineas, titulo_dinamico, i, j);
                mostrar_raton();
            }

            /* Esperar a que suelte el clic */
            while(mbtn == 1) {
                leer_raton(&mx, &my, &mbtn);
            }
        }

        if (kbhit()) {
            /* Sonido suave de maquina de escribir */
            sound(200); delay(10); nosound();
            c = getch();
            
            ocultar_raton(); 

            if (c == 0) { 
                int redraw = 1;
                char nuevo_nombre[MAX_FILENAME_LEN];
                c = getch();
                
                /* Teclas de direccion para moverse en el bloc de notas */
                if (c == 72) { if (i > 0) i--; } /* Arriba */
                else if (c == 80) { if (i < MAX_LINEAS - 1) i++; } /* Abajo */
                else if (c == 75) { if (j > 0) j--; } /* Izquierda */
                else if (c == 77) { if (j < (int)strlen(lineas[i].texto) && j < MAX_CHARS_POR_LINEA) j++; } /* Derecha */
                
                else if (c == 59) { /* F1 - Guardar */
                    if (pedir_Nombre_Archivo(nuevo_nombre, MAX_FILENAME_LEN, ACCION_GUARDAR, lineas, titulo_dinamico, &indice, i, j)) {
                        strncpy(filename_actual, nuevo_nombre, MAX_FILENAME_LEN-1);
                        filename_actual[MAX_FILENAME_LEN-1] = '\0';
                        i = 0; j = 0; 
                    }
                }
                else if (c == 60) { /* F2 - Abrir */
                    cambiado = explorador_Archivos(&indice, lineas, filename_actual, titulo_dinamico, i, j);
                    if (cambiado) { i = 0; j = 0; }
                } 
                else if (c == 61) {
                    mostrar_Menu_Status_App((char*)"F3: Funcion reservada.");
                }

                if (j > (int)strlen(lineas[i].texto)) j = strlen(lineas[i].texto);
                if (redraw) dibujar_Texto_App(MAX_LINEAS, lineas, titulo_dinamico, i, j);

            } else if (c == 27) { /* Escape */
                break;
            } else if (c == 13) { /* Enter */
                if (i < MAX_LINEAS - 1) { i++; j = 0; }
                dibujar_Texto_App(MAX_LINEAS, lineas, titulo_dinamico, i, j);
            } else if (c == 8) { /* Tecla de borrado */
                if (j > 0) {
                    j--;
                    memmove(&lineas[i].texto[j], &lineas[i].texto[j+1], strlen(&lineas[i].texto[j+1]) + 1);
                } else if (i > 0) {
                    i--;
                    j = strlen(lineas[i].texto);
                }
                dibujar_Texto_App(MAX_LINEAS, lineas, titulo_dinamico, i, j);
            } else if (c >= 32 && c <= 126) { 
                if (j < MAX_CHARS_POR_LINEA) {
                    memmove(&lineas[i].texto[j + 1], &lineas[i].texto[j], strlen(&lineas[i].texto[j]) + 1);
                    lineas[i].texto[j] = c;
                    j++;
                } else if (i < MAX_LINEAS - 1) {
                    i++; j = 0;
                    lineas[i].texto[j++] = c;
                    lineas[i].texto[j] = '\0';
                }
                dibujar_Texto_App(MAX_LINEAS, lineas, titulo_dinamico, i, j);
            }
            
            mostrar_raton(); 
        }

        if(pc_apagado) break;
        delay(30); 
    }
    ocultar_raton();
}
#endif
