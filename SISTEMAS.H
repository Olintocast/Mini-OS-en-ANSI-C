#ifndef SISTEMAS_H
#define SISTEMAS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>

#define MAX_RECETAS 20
#define MAX_LONGITUD 100

struct Receta {
    char nombre[MAX_LONGITUD];
    char ingredientes[MAX_LONGITUD];
    char instrucciones[MAX_LONGITUD];
};

struct Receta baseRecetas[MAX_RECETAS];
int cantidadRecetas = 15;
/* Variables globales requeridas */
int i, j;
int encontrado;
int index;

/* Prototipos de funciones */
void inicializarRecetas(void);
void listarRecetas(void);
void buscarReceta(void);
void eliminarReceta(void);
void agregarReceta(void);
void menu(void);

/* Inicializa las 15 recetas nicaraguenses */
void inicializarRecetas() {
    strcpy(baseRecetas[0].nombre, "Gallo Pinto");
    strcpy(baseRecetas[0].ingredientes, "Arroz, frijoles, cebolla, pimiento");
    strcpy(baseRecetas[0].instrucciones, "Mezclar arroz y frijoles refritos con cebolla y pimiento.");

    strcpy(baseRecetas[1].nombre, "Vigoron");
    strcpy(baseRecetas[1].ingredientes, "Yuca, chicharron, ensalada de repollo");
    strcpy(baseRecetas[1].instrucciones, "Servir yuca cocida con chicharron y ensalada de repollo.");

    strcpy(baseRecetas[2].nombre, "Nacatamal");
    strcpy(baseRecetas[2].ingredientes, "Masa de maiz, carne de cerdo, arroz, verduras, hoja de platano");
	strcpy(baseRecetas[2].instrucciones, "Envolver masa con ingredientes en hoja de platano y cocer al vapor.");

    strcpy(baseRecetas[3].nombre, "Indio Viejo");
    strcpy(baseRecetas[3].ingredientes, "Carne de res, maiz, verduras, especias");
    strcpy(baseRecetas[3].instrucciones, "Cocer carne y maiz, luego licuar y mezclar con verduras.");

    strcpy(baseRecetas[4].nombre, "Baho");
    strcpy(baseRecetas[4].ingredientes, "Yuca, platano verde, carne de res, especias");
    strcpy(baseRecetas[4].instrucciones, "Cocer todos los ingredientes envueltos en hojas de platano.");

    strcpy(baseRecetas[5].nombre, "Quesillo");
    strcpy(baseRecetas[5].ingredientes, "Queso, crema, tortilla de maiz");
    strcpy(baseRecetas[5].instrucciones, "Servir queso y crema en tortilla enrollada.");

    strcpy(baseRecetas[6].nombre, "Vaho de Pollo");
    strcpy(baseRecetas[6].ingredientes, "Pollo, platano verde, yuca, especias");
    strcpy(baseRecetas[6].instrucciones, "Cocer todos los ingredientes juntos al vapor.");

    strcpy(baseRecetas[7].nombre, "Sopa de Mondongo");
    strcpy(baseRecetas[7].ingredientes, "Mondongo, verduras, especias");
    strcpy(baseRecetas[7].instrucciones, "Cocer mondongo con verduras y condimentos.");

    strcpy(baseRecetas[8].nombre, "Rosquillas");
    strcpy(baseRecetas[8].ingredientes, "Masa de maiz, queso, azucar, manteca");
    strcpy(baseRecetas[8].instrucciones, "Formar rosquillas y hornear o freir.");

    strcpy(baseRecetas[9].nombre, "Tajadas con Miel");
    strcpy(baseRecetas[9].ingredientes, "Platano maduro, miel de panela");
    strcpy(baseRecetas[9].instrucciones, "Freir platano y banar con miel de panela.");

    strcpy(baseRecetas[10].nombre, "Arroz a la Jardinera");
    strcpy(baseRecetas[10].ingredientes, "Arroz, verduras, pollo o res");
    strcpy(baseRecetas[10].instrucciones, "Cocer arroz con verduras y carne al gusto.");

    strcpy(baseRecetas[11].nombre, "Ceviche de Camaron");
    strcpy(baseRecetas[11].ingredientes, "Camaron, limon, cebolla, tomate, cilantro");
    strcpy(baseRecetas[11].instrucciones, "Mezclar camaron con limon, verduras y cilantro.");

    strcpy(baseRecetas[12].nombre, "Sopa de Pata");
    strcpy(baseRecetas[12].ingredientes, "Pata de res, verduras, especias");
    strcpy(baseRecetas[12].instrucciones, "Cocer pata con verduras y especias hasta que este suave.");

    strcpy(baseRecetas[13].nombre, "Enchiladas Nicas");
    strcpy(baseRecetas[13].ingredientes, "Tortilla, carne molida, salsa, lechuga, huevo");
    strcpy(baseRecetas[13].instrucciones, "Preparar tortilla con carne, salsa y decorar con lechuga y huevo.");

    strcpy(baseRecetas[14].nombre, "Pinolillo");
    strcpy(baseRecetas[14].ingredientes, "Maiz tostado, cacao, agua, azucar");
    strcpy(baseRecetas[14].instrucciones, "Licuar maiz y cacao con agua y azucar.");


}

/* Listar recetas */
void listarRecetas() {
    clrscr();
    textbackground(BLUE);
    clrscr();
	textcolor(WHITE);
	printf("        ----------------------------------------------------------\n");
	printf("                              Lista de Recetas                     \n");
	printf("        ----------------------------------------------------------\n\n");
    textcolor(WHITE);

    for (i = 0; i < cantidadRecetas; i++)
        printf("%2d. %s\n", i + 1, baseRecetas[i].nombre);

    printf("\nPresione una tecla para volver al menu...");
    getch();
}

/* Buscar receta */
void buscarReceta() {
    char nombre[MAX_LONGITUD];
    encontrado = 0;

    clrscr();
    textbackground(BLUE);
    clrscr();
    textcolor(LIGHTGREEN);
	printf("          ===                   Buscar Receta                ===\n\n");
    textcolor(WHITE);
    printf("Ingrese el nombre de la receta a buscar: ");
    gets(nombre);

    for (i = 0; i < cantidadRecetas; i++) {
        if (stricmp(nombre, baseRecetas[i].nombre) == 0) {
            printf("\nNombre: %s\nIngredientes: %s\nInstrucciones: %s\n",
                baseRecetas[i].nombre,
                baseRecetas[i].ingredientes,
                baseRecetas[i].instrucciones);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
        printf("\nReceta no encontrada.\n");

    printf("\nPresione una tecla para volver al menu...");
    getch();
}

/* Eliminar receta */
void eliminarReceta() {
    char nombre[MAX_LONGITUD];
    index = -1;

    clrscr();
    textbackground(BLUE);
    clrscr();
    textcolor(LIGHTRED);
	printf("                   ===         Eliminar Receta         ===\n\n");
    textcolor(WHITE);
    printf("Ingrese el nombre de la receta a eliminar: ");
    gets(nombre);

    for (i = 0; i < cantidadRecetas; i++) {
        if (stricmp(nombre, baseRecetas[i].nombre) == 0) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        for (j = index; j < cantidadRecetas - 1; j++)
            baseRecetas[j] = baseRecetas[j + 1];

        cantidadRecetas--;
        printf("\nReceta eliminada correctamente.\n");
    } else {
        printf("\nReceta no encontrada.\n");
    }

    printf("\nPresione una tecla para volver al menu...");
    getch();
}

/*/*Agregar receta */
void agregarReceta() {
    if (cantidadRecetas >= MAX_RECETAS) {
        textcolor(LIGHTRED);
        printf("\nNo se pueden agregar mas recetas.\n");
        getch();
        return;
    }

    clrscr();
    textbackground(BLUE);
    clrscr();
    textcolor(LIGHTCYAN);
	printf("                     ===         Agregar Receta        ===\n\n");
	textcolor(WHITE);



    printf("Ingrese nombre de la receta: ");
	gets(baseRecetas[cantidadRecetas].nombre);

    printf("Ingrese ingredientes: ");
	gets(baseRecetas[cantidadRecetas].ingredientes);


    printf("Ingrese instrucciones: ");
	gets(baseRecetas[cantidadRecetas].instrucciones);

    cantidadRecetas++;

    printf("\nReceta agregada correctamente.\n");
    printf("Presione una tecla para volver al menu...");
    getch();
}

/* Menu */
void menu() {
    int opcion;

    do {
        clrscr();
        textbackground(BLUE);
        clrscr();
        textcolor(LIGHTMAGENTA);
		printf(" -----------------------------------------------------------------------------\n");
		printf("                     Base De Datos = Recetario Nicaraguense                         \n");
		printf(" -----------------------------------------------------------------------------\n\n");
        textcolor(WHITE);
		printf("  1. Listar recetas\n");
		printf("  2. Buscar receta\n");
		printf("  3. Agregar receta\n");
		printf("  4. Eliminar receta\n");
		printf("  0. Salir\n\n");
		printf("  Seleccione una opcion: ");

		/*LOGO*/


		printf("                                                          \n");
		printf("                   ####     ####################      ####\n");
		printf("                   ####     ####            ####           \n");
		printf("                   ####     ####            ####      ####\n");
		printf("                   ####     ####    ####    ####      ####\n");
		printf("                   ####     ####    ####    ####      ####\n");
		printf("                   ####             ####    ####      ####\n");
		printf("                   ####             ####    ####      ####\n");
		printf("                   #####################    ##############\n");
		printf("                   #####################    ##############\n");
		textcolor(WHITE);

        scanf("%d", &opcion);
        fflush(stdin);

        switch (opcion) {
			case 1: listarRecetas(); break;
			case 2: buscarReceta(); break;
			case 3: agregarReceta(); break;
            case 4: eliminarReceta(); break;
            case 0:
                clrscr();
                textbackground(BLUE);
                clrscr();
				textcolor(LIGHTGREEN);
				gotoxy(35,10);
                printf("Hasta luego!\n");
                getch();
                break;
            default:
                textcolor(LIGHTRED);
                printf("Opcion invalida.\n");
                getch();
        }

    } while (opcion != 0);
}

#endif
