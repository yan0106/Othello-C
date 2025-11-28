#include <stdio.h> // para entreda y salida
#include <stdlib.h> // para func. system(), rand(), srand()
#include <time.h> // para usar time()
#define MAX_NOMBRE 20
#define FILAS 8
#define COLUMNAS 8

//INICIO:
void bienvenidaJuego ();
void borrarPantalla (); // función auxiliar
void limpiarBufer (); // función auxiliar
void cargarParticipantes (char participante_1[MAX_NOMBRE], char participante_2[MAX_NOMBRE]);
void elegirFichas (char participante_1[MAX_NOMBRE], char participante_2[MAX_NOMBRE], char *fichas1, char *fichas2);
void construirTablero (char tablero[FILAS][COLUMNAS]);
void imprimirTablero (char tablero[FILAS][COLUMNAS]);
void imprimirReglas ();
int sortearParticipante ();
void imprimirTurno (int turno, char participante_1[MAX_NOMBRE], char participante_2[MAX_NOMBRE]);
//DESARROLLO DEL JUEGO:
char fichadelParticipante (int turno, char fichas1, char fichas2);
int solicitarMovimiento (int *fila, int *columna);
int validarMovimiento (char tablero[FILAS][COLUMNAS], int fila, int columna, char fichaParticipante, int imprimirMensajes);
int contarFichasObtenidas(char tablero[FILAS][COLUMNAS], int fila, int columna, char fichaParticipante);
void actualizarTablero (char tablero[FILAS][COLUMNAS], int fila, int columna, char fichaParticipante);
int hayMovimientoPosible (char tablero[FILAS][COLUMNAS], char fichaParticipante, int imprimirMensajes);
void marcarMovimientosPosibles(char tablero[FILAS][COLUMNAS], char fichaParticipante);
void limpiarMovimientosPosibles(char tablero[FILAS][COLUMNAS]);
void contarPuntos (char tablero[FILAS][COLUMNAS], int *blancas, int *negras); // función auxiliar
void imprimirPuntos (char tablero[FILAS][COLUMNAS], char participante_1[MAX_NOMBRE], char participante_2[MAX_NOMBRE], char fichas1, char fichas2);
//FINALIZACION:
void imprimirGanador (char tablero[FILAS][COLUMNAS], char participante_1[MAX_NOMBRE], char participante_2[MAX_NOMBRE], char fichas1, char fichas2);
int solicitarReinicio ();

int main () {
    //Inicio:
    bienvenidaJuego();
    char participante_1[MAX_NOMBRE];
    char participante_2[MAX_NOMBRE];
    char fichas1;
    char fichas2;
    cargarParticipantes (participante_1, participante_2);
    elegirFichas(participante_1, participante_2, &fichas1, &fichas2); // & para pasar la dirección en memoria de las variables

    int reinicio = 1; // controla el while para reiniciar el juego

    while (reinicio) { // bucle while para que el juego pueda reiniciarse si los participantes lo deciden al final del juego
        // Tablero del Juego:
        char tablero[FILAS][COLUMNAS]; // se crea el tablero local a main, pero lo paso por parámetro en las funciones
        construirTablero(tablero);
        imprimirTablero (tablero);
        imprimirReglas();

        //Sorteo de los Participantes:
        srand(time(NULL)); // le paso el tiempo como parámetro para que los nros. cambien cada vez que se corre el programa.
        int turno = sortearParticipante ();
        imprimirTurno (turno, participante_1, participante_2);

        int finalizaJuego = 0; // 0 = falso, 1 = verdadero // para controlar el bucle del juego

         //Desarrollo del Juego:
        while (!finalizaJuego) { // mientras no finalice el juego
            char fichaParticipante = fichadelParticipante (turno, fichas1, fichas2); // obtengo la ficha del participante actual
            int fila, columna;

            // Se verifica si hay movimientos posibles antes de pedir la jugada
            if (!hayMovimientoPosible (tablero, fichaParticipante, 0)) { // 0 no imprimir mensajes si no hay movimiento posible
                printf("\n%s no tiene movimientos posibles. Se pasa el turno.\n", (turno == 0) ? participante_1 : participante_2);
                printf("\nPresiona 'Enter' para continuar...");
                limpiarBufer();
                // cambio de turno
                turno = (turno == 0) ? 1 : 0; // si turno es igual a 0, cambio a turno 1, sino cambio a turno 0
                borrarPantalla();
                continue; // va directo a la siguiente vuelta del while, con el turno cambiado
            }

            limpiarMovimientosPosibles(tablero); // limpia cualquier '*' previo antes de marcar
            marcarMovimientosPosibles(tablero, fichaParticipante);

            imprimirTablero(tablero);
            imprimirPuntos (tablero, participante_1, participante_2, fichas1, fichas2);

            // Muestra el turno del participante
            printf("\nTURNO:");
            if (turno == 0) {
                printf("\n\nEs el turno de %s (fichas %c)\n", participante_1, fichaParticipante);
            } else {
                printf("\n\nEs el turno de %s (fichas %c)\n", participante_2, fichaParticipante);
            }

            limpiarMovimientosPosibles(tablero); // limpia los movimientos posibles antes de solicitar el movimiento para que no interfiera en la validación

            int mov_valido = 0; // falso

            // Pide un movimiento y lo valida
            while (mov_valido == 0) {
                solicitarMovimiento (&fila, &columna);
                mov_valido = validarMovimiento(tablero, fila, columna, fichaParticipante, 1); // 1 para mostrar mensajes // si la func. retorna 1 sale del bucle
            }

            // Muestra la cantidad de fichas obtenidas
            int fichas_obtenidas = contarFichasObtenidas(tablero, fila, columna, fichaParticipante); // guarda el valor que retorna la función
            printf("\nObtenes %d ficha(s) de tu adversario.\n\n", fichas_obtenidas);
            printf("Presiona 'Enter'...");
            getchar();
            limpiarBufer();

            // Actualiza el tablero
            actualizarTablero(tablero, fila, columna, fichaParticipante);
            borrarPantalla();

            // Verifica si no hay más movimientos posibles para finalizar el juego:
            if (!hayMovimientoPosible(tablero, fichas1, 0) && !hayMovimientoPosible(tablero, fichas2, 0)) { // si no hay movimientos posibles de blancas ni de negras, termina el juego
                finalizaJuego = 1; // sale del bucle
                break;
            }
            // Cambio de turno
            turno = (turno == 0) ? 1 : 0;
        }
        // Muestra el ganador:
        imprimirGanador(tablero, participante_1, participante_2, fichas1, fichas2);

        // Solicita el reinicio del juego
        reinicio = solicitarReinicio();
    }

    return 0;
}

void bienvenidaJuego () {
    printf("================================\n");
    printf("  Bienvenidos al Juego Othello\n");
    printf("================================\n");
    printf("\nPresiona 'Enter' para continuar...");
    limpiarBufer ();
    borrarPantalla();
}

void borrarPantalla () { // Limpia la pantalla según el sistema operativo. # instrucción especial para el preprocesador
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
}

void limpiarBufer () {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void cargarParticipantes (char participante_1[MAX_NOMBRE], char participante_2[MAX_NOMBRE]) {
    // Solicita los nombres de los participantes y los guarda en los arrays

    printf("REGISTRO DE LOS PARTICIPANTES:\n\n");
    printf("Por favor, ingresa el nombre del Participante 1: ");
    scanf("%s", participante_1); // ver para leer nombres compuestos
    printf("\nPor favor, ingresa el nombre del Participante 2: ");
    scanf("%s", participante_2);
    borrarPantalla();
}

void elegirFichas(char participante_1[MAX_NOMBRE], char participante_2[MAX_NOMBRE], char *fichas1, char *fichas2) {
    // Consulta con qué ficha quiere jugar el participante_1 y le asigna automáticamente la otra ficha al participante_2

    char opcion_ficha = ' '; // se guarda la elección de la ficha del participante_1 // se inicia con vacío para que no quede basura

    printf("ELECCION DE FICHAS:\n");
    while (opcion_ficha != 'B' && opcion_ficha != 'b' // mientras no se ingrese 'B', 'b', 'N' o 'n', vuelve a pedir la elección
        && opcion_ficha != 'N' && opcion_ficha != 'n'){
        printf("\nCon que fichas prefiere jugar %s? ('B' para elegir las Blancas, 'N' para elegir las Negras): ", participante_1);
        scanf(" %c", &opcion_ficha); // lee el carácter y lo guarda en la variable 'opcion_ficha'
        limpiarBufer (); // borra el búfer
    }
    // Asignación de fichas:
    if (opcion_ficha == 'B' || opcion_ficha == 'b') { // si el participante_1 elige fichas blancas
        *fichas1 = 'O'; // Participante_1 = blancas
        *fichas2 = 'X'; // Participante_2 = negras
    } else { // sino, si el participante elige las fichas negras
        *fichas1 = 'X'; // Participante_1 = negras
        *fichas2 = 'O'; // Participante_2 = blancas
    }
    // Confirma la elección
    printf("\n%s jugara con las fichas %c", participante_1, *fichas1);
    printf("\n\n%s jugara con las fichas %c\n", participante_2, *fichas2);
    printf("\nPresiona 'Enter' para continuar...");
    limpiarBufer();
    borrarPantalla();
}

void construirTablero (char tablero[FILAS][COLUMNAS]) {
    // Recorre el tablero con dos for anidados para cargarlos con "." (representa vacío) y coloca las fichas iniciales
    for (int i = 0; i < FILAS; i++) { // filas
        for (int j = 0; j < COLUMNAS; j++) { // columnas
            tablero[i][j] = '.';
        }
    }
    // Se colocan las 4 fichas del incio en tablero:
    tablero[3][3] = 'O'; // en fila 3, columna 3: Blanca
    tablero[3][4] = 'X'; // en fila 3, columna 4: Negra
    tablero[4][3] = 'X'; // en fila 4, columna 3: Negra
    tablero[4][4] = 'O'; // en fila 4, columna 4: Blanca
}

void imprimirTablero (char tablero[FILAS][COLUMNAS]) {
    // Muestra el tablero del juego con las columnas en letras y las filas en números
    printf("TABLERO DEL JUEGO: \n\n\n");
    printf("  "); // el espacio es para alinear con los números de la izquierda
    for (int j = 0; j < COLUMNAS; j++) {
        printf("%c ", 'A' + j);  // columnas: A B C D E F G H. En cada iteración va sumando a 'A' el valor de j = 0, j = 1, j = 2,...
    }
    printf("\n");
    for (int i = 0; i < FILAS; i++) {
        printf("%d ", i + 1);  // filas: 1 2 3 4 5 6 7 8 . (i + 1) para que imprima desde el 1 en adelante
        for (int j = 0; j < COLUMNAS; j++) {
            printf("%c ", tablero [i][j]);
        }
        printf("\n"); // salto de línea para cada fila
    }
}

void imprimirReglas() {
    printf("\n\nREGLAS BASICAS: \n\n'O' son las fichas Blancas. \n\n'X' son las fichas Negras. \n");
    printf("\nCada jugador debe ingresar una ficha de su color en el tablero, de modo que encierre una o mas fichas del color contrario, entre la nueva ficha y otra ya existente.\n");
    printf("\nEjemplo de una jugada: XOX\n");
    printf("\n(La ficha 'O' del medio, es capturada por la nueva ficha 'X')\n");
    printf("\nPresiona 'Enter' para continuar...");
    limpiarBufer();
    borrarPantalla();
}

int sortearParticipante() {
    // Genera un número 0 o 1 para decidir qué participante empieza y lo devuelve
    return rand()%2;
}

void imprimirTurno (int turno, char participante_1[MAX_NOMBRE], char participante_2[MAX_NOMBRE]) {
    // Recibe el valor del turno (resultado de la func. sortearParticipante()) y muestra qué participante comienza
    printf ("SORTEO DEL PRIMER PARTICIPANTE:");
    if (turno == 0) {
        // si en el sorteo salió 0, comienza el participante_1
        printf("\n\nComienza %s\n", participante_1);
    } else {
        // si en el sorteo salió 1, comienza el participante_2
        printf("\n\nComienza %s\n", participante_2);
    }
    printf("\nPresiona 'Enter' para continuar...");
    limpiarBufer();
    borrarPantalla();
}

// FUNCIONES DEL CILO DEL JUEGO:
char fichadelParticipante (int turno, char fichas1, char fichas2){
    // Devuelve la ficha del participante actual ('X' o 'O')
    char fichaParticipante;

    if (turno == 0) {
        return fichaParticipante = fichas1;
    } else {
        return fichaParticipante = fichas2;
    }
}

int solicitarMovimiento (int *fila, int *columna) {
    // Solicita un movimiento, convierte la entrada en índices y evalúa que esté dentro del tablero
    char letraColumna;
    int numeroFila;

    int mov_valido = 0;

    while (mov_valido == 0) {
        printf("\nPor favor, ingresa un movimiento (por ejemplo: D3): ");

        if (scanf(" %c" "%d", &letraColumna, &numeroFila) != 2) { // si lee algo distinto de 2 (1 caracter y 1 número)
            printf("\nMovimiento invalido. Intentalo de nuevo!\n");
            limpiarBufer();
            continue; // vuelve a pedir el movimiento
        }

        // Valida la columna y la convierte a índice
        if (letraColumna >= 'A' && letraColumna <= 'H') {
            *columna = letraColumna - 'A'; // si es mayúscula
        } else if (letraColumna >= 'a' && letraColumna <= 'h') {
            *columna = letraColumna - 'a'; // si es minúscula
        } else {
            printf("\nMovimiento invalido. Intentalo de nuevo!\n");
            limpiarBufer();
            continue;
        }

        // Valida la fila
        if (numeroFila < 1 || numeroFila > FILAS) {
            printf("\nMovimiento invalido. Intentalo de nuevo!\n");
            limpiarBufer();
            continue;
        }

        // Convierte la fila a índice 0, 1, 2, 3, 4, 5, 6, 7, 8. En el juego arranca en 1 pero en el programa en 0
        *fila = numeroFila - 1;

        // El movimiento es válido, sale del ciclo
        mov_valido = 1;
    }
    return 1;
}

/* 8 DIRECCIONES POSIBLES DESDE UNA POSICION (se tienen en cuenta para validarMovimiento)
arriba a la izquierda  (-1, -1)
arriba  (-1,  0)
arriba a la derecha  (-1, +1)
izquierda  ( 0, -1)
derecha  ( 0, +1)
abajo a la izquierda  (+1, -1)
abajo  (+1,  0)
abajo a la derecha  (+1, +1)
*/

int validarMovimiento (char tablero[FILAS][COLUMNAS], int fila, int columna, char fichaParticipante, int imprimirMensajes){
    // Verifica si un moviemiento es válido: evalua si no está vacía la casilla y si obtiene fichas del adversario en alguna dirección
    char fichaAdversario = (fichaParticipante == 'X') ? 'O' : 'X'; // si la ficha del jugador es negra ('X'), la del oponente es blanca ('O'); sino, es negra

    int mov_valido = 0; // se inicia en falso

    if (tablero [fila][columna] != '.') { // chequea si la posición no está vacía
        printf ("\nOps! Hay otra ficha en esa posicion.\n");
        return 0; // sale de la función inmediatamente
    // recorre las 8 direcciones posibles usando los dos for
    } else { for (int direFila = -1; direFila <= 1; direFila++) { //direFila = direcciones de las filas (-1, 0, 1)
                    for (int direCol = -1; direCol <= 1; direCol++) { //direCol = direcciones de las columnas (-1, 0, 1)
                        if (direFila == 0 && direCol == 0) {
                            continue; // saltea la posición actual
                        }
                        int nuevaFila = fila + direFila;
                        int nuevaColumna = columna + direCol;

                        int hayAdversario = 0; // contador // se incrementa dentro del while si hay fichas del adversario

                        while (nuevaFila >= 0 && nuevaFila < FILAS && // controla que no se salga del tablero en las filas
                            nuevaColumna >= 0 && nuevaColumna < COLUMNAS && // controla que no se salga del tablero en las columnas
                            tablero[nuevaFila][nuevaColumna] == fichaAdversario) { // mientras en la posición encuentre fichas del adversario
                            hayAdversario = hayAdversario + 1; // suma

                           // avanza a la siguiente casilla en la misma dirección
                            nuevaFila = nuevaFila + direFila;
                            nuevaColumna = nuevaColumna + direCol;
                        }
                        // Si al menos hay 1 ficha del adversario y termina en una ficha propia, es válido el movimiento
                        if (hayAdversario &&
                            nuevaFila >= 0 && nuevaFila < FILAS &&
                            nuevaColumna >= 0 && nuevaColumna < COLUMNAS &&
                            tablero[nuevaFila][nuevaColumna] == fichaParticipante) { //si termina encontrándose con la propia ficha del participante
                            mov_valido = 1; // se hace verdadero
                        }
            }
        }
    }
    if (mov_valido == 0 && imprimirMensajes) {
        printf("\nEl movimiento no obtiene ninguna ficha del adversario. Intentalo de nuevo!\n");
    }
    return mov_valido;
}

int contarFichasObtenidas(char tablero[FILAS][COLUMNAS], int fila, int columna, char fichaParticipante) {
    // Cuenta cuántas fichas del adversario se obtienen, si el participante actual coloca una ficha en una determinada posición
    // El valor retornado se guarda en fichasObtenidas en main () .
    char fichaAdversario = (fichaParticipante == 'X') ? 'O' : 'X'; // asigna la ficha del adversario: si el participante usa 'X' (negra), el adversario usa 'O' (blanca); si no, al revés
    int total_obtenidas = 0;

    // recorre las 8 direcciones posibles
    for (int direFila = -1; direFila <= 1; direFila++) { // filas
        for (int direCol = -1; direCol <= 1; direCol++) { // columnas
            if (direFila == 0 && direCol == 0) {
            continue; // saltea la posición actual
            }

            int nuevaFila = fila + direFila;
            int nuevaColumna = columna + direCol;

            int fichasAObtener = 0;

            while (nuevaFila >= 0 && nuevaFila < FILAS &&
                nuevaColumna >= 0 && nuevaColumna < COLUMNAS &&
                tablero[nuevaFila][nuevaColumna] == fichaAdversario) {
                fichasAObtener++; // suma
                nuevaFila = nuevaFila + direFila;
                nuevaColumna = nuevaColumna + direCol;
            }

            if (fichasAObtener > 0 &&
                nuevaFila >= 0 && nuevaFila < FILAS &&
                nuevaColumna >= 0 && nuevaColumna < COLUMNAS &&
                tablero[nuevaFila][nuevaColumna] == fichaParticipante) {
                total_obtenidas = total_obtenidas + fichasAObtener;
            }
        }
    }
    return total_obtenidas;
}

void actualizarTablero(char tablero[FILAS][COLUMNAS], int fila, int columna, char fichaParticipante) {
    // Coloca la ficha del participante donde eligió mover. Cuenta cuántas fichas a obtener hay (cuidando que no salga del tablero), dentro de dos fichas propias
    // Recorre las fichas del adversario y las va dando vuelta
    char fichaAdversario = (fichaParticipante == 'X') ? 'O' : 'X'; // asigna la ficha del adversario, si el participante usa 'X', entonces el adversario usa 'O', sino 'X'

    tablero[fila][columna] = fichaParticipante; // se coloca la ficha del participante donde decidió mover

    // recorre las 8 direcciones posibles
    for (int direFila = -1; direFila <= 1; direFila++) {
        for (int direCol = -1; direCol <= 1; direCol++) {
            if (direFila == 0 && direCol == 0) {
                continue; // salteo la posición actual
            }

            int nuevaFila = fila + direFila;
            int nuevaColumna = columna + direCol;

            int fichasAObtener = 0;

            // Cuenta cuántas fichas del adversario hay en esa dirección
            while (nuevaFila >= 0 && nuevaFila < FILAS &&
                nuevaColumna >= 0 && nuevaColumna < COLUMNAS &&
                tablero[nuevaFila][nuevaColumna] == fichaAdversario) {
                // Avanza en la dirección
                nuevaFila = nuevaFila + direFila;
                nuevaColumna = nuevaColumna + direCol;
                fichasAObtener++; // suma
            }

            // Si al final hay una ficha propia del participante, se convierten las del medio
            if (fichasAObtener > 0 && // verifica que al menos haya una ficha a obtener
                nuevaFila >= 0 && nuevaFila < FILAS && // verifica que no se vaya de la fila
                nuevaColumna >= 0 && nuevaColumna < COLUMNAS && // verifica que no se vaya de la columna
                tablero[nuevaFila][nuevaColumna] == fichaParticipante) { // si en la posición siguiente hay una ficha propia del participante

                // Se crean dos variables para recorrer desde la primera ficha del adversario
                int f = fila + direFila;
                int c = columna + direCol;

                // Recorre el largo de las fichas del adversario que se detectaron en fichasAObtener y las da vuelta
                for (int i = 0; i < fichasAObtener; i++) {
                    tablero[f][c] = fichaParticipante; // da vuelta las fichas del adversario
                    f = f + direFila; // avanza para seguir dando vuelta las fichas
                    c = c + direCol; // avanza
                }
            }
        }
    }
}

int hayMovimientoPosible (char tablero[FILAS][COLUMNAS], char fichaParticipante, int imprimirMensajes) {
    // Recorre el tablero con los dos for para verificar si hay posiciones vacías ('.')
    // Para cada posición vacía, verifica si colocar la ficha del participante sería un movimiento válido
    // Devuelve 1 si encuentra al menos un movimiento posible, sino devuelve 0
    for (int fila = 0; fila < FILAS; fila++) {
        for (int columna = 0; columna < COLUMNAS; columna++) {
            if (tablero[fila][columna] == '.') {
                if (validarMovimiento(tablero, fila, columna, fichaParticipante, imprimirMensajes)) { // si validarMovimiento() retorna 1, el movimiento es válido
                    return 1; // hay al menos un movimiento posible
                    }
                }
            }
        }
    return 0; // después de revisar todo y no encontrar nada
}

void marcarMovimientosPosibles (char tablero[FILAS][COLUMNAS], char fichaParticipante) {
    // Marca los movimientos posibles del participante en el tablero con '*'
    for (int fila = 0; fila < FILAS; fila++) {
        for (int columna = 0; columna < COLUMNAS; columna++) {
            // si la casilla está vacía ('.') y el movimiento es válido para el participante
            if (tablero[fila][columna] == '.' &&
                validarMovimiento(tablero, fila, columna, fichaParticipante, 0)) {
                tablero[fila][columna] = '*';
            }
        }
    }
}

void limpiarMovimientosPosibles(char tablero[FILAS][COLUMNAS]) {
    // Vuelve a cambiar las casillas que mostraban los posibles movimientos por casillas vacías
    for (int fila = 0; fila < FILAS; fila++) {
        for (int columna = 0; columna < COLUMNAS; columna++) {
            if (tablero[fila][columna] == '*') {
                tablero[fila][columna] = '.';
            }
        }
    }
}

void contarPuntos (char tablero[FILAS][COLUMNAS], int *blancas, int *negras) {
    // Recorre el tablero, cuenta la cantidad de fichas blancas y negras
    *blancas = 0;
    *negras = 0;

    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++){
            if (tablero[i][j] == 'O') {
                (*blancas)++;
            } else if (tablero[i][j] == 'X'){
                (*negras)++;
            }
        }
    }
}

void imprimirPuntos(char tablero[FILAS][COLUMNAS], char participante_1[MAX_NOMBRE], char participante_2[MAX_NOMBRE], char fichas1, char fichas2) {
    // Muestra en pantalla la cantidad de fichas que tiene cada participante en el tablero, junto con sus nombres y qué ficha usan
    int blancas = 0;
    int negras = 0;

    contarPuntos(tablero, &blancas, &negras); // utiliza la función auxiliar contarPuntos() para no repetir lógica

    int fichasParticipante_1;
    int fichasParticipante_2;

    if (fichas1 == 'O') {
        fichasParticipante_1 = blancas;
        fichasParticipante_2 = negras;
    } else {
        fichasParticipante_1 = negras;
        fichasParticipante_2 = blancas;
    }

    printf("\n\nPUNTOS:\n\n");
    printf("%s (fichas %c): %d fichas // %s (fichas %c): %d fichas\n",participante_1, fichas1, fichasParticipante_1,
           participante_2, fichas2, fichasParticipante_2);
}

void imprimirGanador(char tablero[FILAS][COLUMNAS], char participante_1[MAX_NOMBRE], char participante_2[MAX_NOMBRE], char fichas1, char fichas2) {
    // Muestra el resultado final del juego, indicando quién ganó o si hubo empate, junto con la cantidad de fichas que le quedó a cada participante
    int blancas, negras;

    // Cuenta las fichas blancas y las negras
    contarPuntos(tablero, &blancas, &negras);

    // Determina los puntos de cada participante según qué ficha usan
    int fichasParticipante_1;
    int fichasParticipante_2;

    if (fichas1 == 'O') {
        fichasParticipante_1 = blancas;
        fichasParticipante_2 = negras;
    } else {
        fichasParticipante_1 = negras;
        fichasParticipante_2 = blancas;
    }

    printf("No quedan mas movimientos posibles.\n\n");

    // Muestra los resultados
    printf("=======================");
    printf("\nRESULTADOS FINALES:\n");
    printf("=======================\n\n");
    printf("%s (fichas %c): %d fichas\n\n", participante_1, fichas1, fichasParticipante_1);
    printf("%s (fichas %c): %d fichas\n", participante_2, fichas2, fichasParticipante_2);

    // Muestra el ganador o si hubo empate
    if (fichasParticipante_1 > fichasParticipante_2) {
        printf("\n>>> Gano %s <<<\n", participante_1);
    } else if (fichasParticipante_2 > fichasParticipante_1) {
        printf("\n>>> Gano %s <<<\n", participante_2);
    } else {
        printf("\nHay un empate!!!\n");
    }
}

int solicitarReinicio () {
    // Consulta si se desea reiniciar el juego, guarda la elección en una variable
    // Controla que se ingrese una opción válida
    // Retorna 1 o 0 (esto se utiliza en mai() para el bucle de reinicio)
    char opcion_reinicio;
    printf("\n\nReinicio: volver a jugar con los mismos participantes? (Ingresa 'S' para si o 'N' para no): ");
    scanf(" %c", &opcion_reinicio);
    limpiarBufer();

    while (opcion_reinicio != 's' && opcion_reinicio != 'S' && opcion_reinicio != 'n' && opcion_reinicio != 'N') {
        printf("\nPor favor, ingresa una opcion valida (S/N): ");
        scanf(" %c", &opcion_reinicio);
        limpiarBufer();
    }

    borrarPantalla();

    if (opcion_reinicio == 's' || opcion_reinicio == 'S') {
        return 1;  // quiere reiniciar
    } else {
        return 0;  // no quiere reiniciar
    }
}



