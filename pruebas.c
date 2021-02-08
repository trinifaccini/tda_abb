#include <stdio.h>
#include "abb.h"
#include "pa2m.h"

#define VACIA 0
#define ERROR -1
#define EXITO 0
#define MAX_1 6
#define MAX_2 4
#define FALSO 0
#define VERDADERO 1
#define MAX_FRUTERA 30

typedef struct pomelos{
    int peso;
}pomelo_t;


/* Funcion que crea los elementos que iran en el árbol */
pomelo_t* crear_pomelo(int peso){

    pomelo_t* pomelo = (pomelo_t*)malloc(sizeof(pomelo_t));
    if(pomelo)pomelo->peso = peso;

    return pomelo;
}

/* Destructor */
void exprimir_pomelo(pomelo_t* pomelo_actual){

    free(pomelo_actual);
}

/* Funcion que invoca al destructor */
void exprimidor_de_pomelos(void* elemento){
    if(!elemento)
        return;
    exprimir_pomelo((pomelo_t*)elemento);
}

/* Funcion que compara los elementos segun su clave, la cual en este caso es
   el peso de los pomelos */
int comparar_pomelos_por_peso(void* pomelo1, void* pomelo2){

    if(!pomelo1 || !pomelo2) return 0;

    if(((pomelo_t*)pomelo1)->peso > ((pomelo_t*)pomelo2)->peso) return 1;

    if(((pomelo_t*)pomelo1)->peso < ((pomelo_t*)pomelo2)->peso) return -1;

    return 0;
}

/* Funcion que sera usada con el iterador interno */
bool mostrar_peso_pomelos(void* elemento, void* extra){
    extra=extra; 
    if(elemento != extra)
        printf("%i ", ((pomelo_t*)elemento)->peso);
    return false;
}

/* Funcion que sera usada con el iterador interno */
bool recorrer_hasta_raiz(void* elemento, void* extra){

	extra = elemento;

    if(elemento){
        if(((pomelo_t*)elemento)->peso == 12){
            return true;
        }
    }
    return false;
}


/******************* PRUEBAS *******************/

void probar_arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){

	abb_t * arbol = arbol_crear(comparador, destructor);
	pa2m_afirmar(arbol != NULL, "Puedo crear una arbol");
	pa2m_afirmar(!(arbol->nodo_raiz) && arbol->comparador == comparador && arbol->destructor == destructor, "Todas las propiedades del arbol fueron asignadas correctamente");

	arbol_destruir(arbol);
}


void probar_arbol_vacio(abb_comparador comparador, abb_liberar_elemento destructor){

	abb_t * arbol = arbol_crear(comparador, destructor);

	pomelo_t * un_pomelo = crear_pomelo(15);
	pomelo_t * frutera[MAX_FRUTERA];

	pa2m_afirmar(arbol_vacio(arbol), "El arbol se crea vacio");
	pa2m_afirmar(arbol_insertar(arbol, un_pomelo) == EXITO, "Es posible insertar un pomelo en un arbol que estaba vacio");
	pa2m_afirmar(arbol_raiz(arbol) == un_pomelo, "El pomelo en la raiz coincide con el insertado anteriormente");
	pa2m_afirmar(!arbol_vacio(arbol), "El arbol ya no esta vacio");
	pa2m_afirmar(arbol_borrar(arbol, un_pomelo) == EXITO, "Es posible borrar el pomelo que acabamos de insertar");

	pa2m_afirmar(arbol_vacio(arbol), "El arbol se encuentra vacio nuevamente");
	pa2m_afirmar(arbol_borrar(arbol, un_pomelo) == ERROR, "No es posible borrar en un arbol vacio"); //Arreglar
	pa2m_afirmar(!arbol_raiz(arbol), "No existe pomelo en raiz / no existe raiz");
	pa2m_afirmar(!arbol_buscar(arbol, un_pomelo), "No es posible buscar en un arbol vacio");
	pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**)frutera, MAX_FRUTERA) == 0 && arbol_recorrido_preorden(arbol, (void**)frutera, MAX_FRUTERA) == 0 &&arbol_recorrido_postorden(arbol, (void**)frutera, MAX_FRUTERA) == 0, "No es posible recorrer un arbol vacio en ningun tipo de orden");

	arbol_destruir(arbol);
}


void probar_arbol_nulo(abb_comparador comparador, abb_liberar_elemento destructor){

	abb_t * arbol = NULL;
	pomelo_t * un_pomelo = crear_pomelo(15);
	pomelo_t * frutera[MAX_FRUTERA];

	pa2m_afirmar(arbol_vacio(arbol), "El arbol no existe");
	pa2m_afirmar(arbol_insertar(arbol, un_pomelo) == ERROR, "No es posible insertar en un arbol inexistente");
	pa2m_afirmar(arbol_borrar(arbol, un_pomelo) == ERROR, "No es posible borrar en un arbol inexistente");
	pa2m_afirmar(!arbol_raiz(arbol), "No existe raiz");
	pa2m_afirmar(!arbol_buscar(arbol, un_pomelo), "No es posible buscar en un arbol inexistente");
	pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**)frutera, MAX_FRUTERA) == 0 && arbol_recorrido_preorden(arbol, (void**)frutera, MAX_FRUTERA) == 0 && arbol_recorrido_postorden(arbol, (void**)frutera, MAX_FRUTERA) == 0, "No es posible recorrer un arbol inexistente en ningun tipo de orden");

	destructor(un_pomelo);
}


void probar_comparador_nulo(abb_comparador comparador, abb_liberar_elemento destructor){

	pa2m_afirmar(comparador == NULL, "El comparador es nulo");
	abb_t * arbol = arbol_crear(comparador, destructor);
	pa2m_afirmar(!arbol && arbol_vacio(arbol), "El arbol no se crea si el comparador es nulo");
}


void probar_recorridos(abb_comparador comparador, abb_liberar_elemento destructor){

	bool todos_insertados = true;
	bool orden_correcto   = true;

	abb_t    * arbol   = arbol_crear(comparador, destructor);
	pomelo_t * pomelo1 = crear_pomelo(10);
	pomelo_t * pomelo2 = crear_pomelo(15);
	pomelo_t * pomelo3 = crear_pomelo(5);
	pomelo_t * pomelo4 = crear_pomelo(6);
	pomelo_t * pomelo5 = crear_pomelo(3);
	pomelo_t * pomelo6 = crear_pomelo(11);
	pomelo_t * pomelo7 = crear_pomelo(17);
	pomelo_t * frutera_resultado[MAX_FRUTERA];

	pomelo_t * frutera_inorden[] = {pomelo5, pomelo3, pomelo4, pomelo1, pomelo6, pomelo2, pomelo7};
	pomelo_t * frutera_preorden[] = {pomelo1, pomelo3, pomelo5, pomelo4, pomelo2, pomelo6, pomelo7};
	pomelo_t * frutera_postorden[] = {pomelo5, pomelo4, pomelo3, pomelo6, pomelo7, pomelo2, pomelo1};

	pomelo_t * frutera_insertar[] = {pomelo1, pomelo2, pomelo3, pomelo4, pomelo5, pomelo6, pomelo7};

	for(int i = 0; i < 7; i++){
		if(arbol_insertar(arbol, frutera_insertar[i]) == ERROR) todos_insertados = false;	
	}

	pa2m_afirmar(todos_insertados, "Se pudo insertar 7 pomelos en el arbol");
	
	pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**)frutera_resultado, MAX_FRUTERA) == 7, "La cantidad de pomelos recorridos inorden es correcta");

	for(int i = 0; i < 7; i++){
		if(frutera_resultado[i] != frutera_inorden[i]) orden_correcto = false;
	}

	pa2m_afirmar(orden_correcto, "El recorrido inorden es realizado en el orden correcto");

	pa2m_afirmar(arbol_recorrido_preorden(arbol, (void**)frutera_resultado, MAX_FRUTERA) == 7, "La cantidad de pomelos recorridos preorden es correcta");

	for(int i = 0; i < 7; i++){
		if(frutera_resultado[i] != frutera_preorden[i]) orden_correcto = false;
	}

	pa2m_afirmar(orden_correcto, "El recorrido preorden es realizado en el orden correcto");

	pa2m_afirmar(arbol_recorrido_postorden(arbol, (void**)frutera_resultado, MAX_FRUTERA) == 7, "La cantidad de pomelos recorridos postorden es correcta");

	for(int i = 0; i < 7; i++){
		if(frutera_resultado[i] != frutera_postorden[i]) orden_correcto = false;
	}

	pa2m_afirmar(orden_correcto, "El recorrido inorden es realizado en el orden correcto");

	arbol_destruir(arbol);
}


void probar_insertar_7_pomelos(abb_comparador comparador, abb_liberar_elemento destructor){

	bool todos_insertados = true;
	bool orden_correcto   = true;

	abb_t    * arbol   = arbol_crear(comparador, destructor);
	pomelo_t * pomelo1 = crear_pomelo(10);
	pomelo_t * pomelo2 = crear_pomelo(15);
	pomelo_t * pomelo3 = crear_pomelo(5);
	pomelo_t * pomelo4 = crear_pomelo(6);
	pomelo_t * pomelo5 = crear_pomelo(3);
	pomelo_t * pomelo6 = crear_pomelo(11);
	pomelo_t * pomelo7 = crear_pomelo(17);

	pomelo_t * frutera_insertar[] = {pomelo2, pomelo3, pomelo4, pomelo5, pomelo6, pomelo7};
	pomelo_t * frutera_resultado[MAX_FRUTERA];

	pa2m_afirmar(arbol_vacio(arbol), "El arbol se crea vacio");
	pa2m_afirmar(arbol_insertar(arbol, pomelo1) == EXITO, "Es posible insertar un pomelo en un arbol que estaba vacio");
	pa2m_afirmar(!arbol_vacio(arbol), "El arbol ya no esta vacio");
	pa2m_afirmar(arbol_raiz(arbol) == pomelo1, "El pomelo en la raiz es el correcto y coincide con el unico insertado");

	for(int i = 0; i < 6; i++){
		if(arbol_insertar(arbol, frutera_insertar[i]) == ERROR) todos_insertados = false;	
	}

	pa2m_afirmar(todos_insertados, "Se pudo insertar otros 6 pomelos en el arbol");
	pa2m_afirmar(arbol_raiz(arbol) == pomelo1, "El pomelo en la raiz es el correcto y coincide con el primer insertado");

	pa2m_afirmar(arbol_recorrido_preorden(arbol, (void**)frutera_resultado, MAX_FRUTERA) == 7, "La cantidad de pomelos recorridos preorden es correcta");
	pa2m_afirmar(arbol_recorrido_postorden(arbol, (void**)frutera_resultado, MAX_FRUTERA) == 7, "La cantidad de pomelos recorridos postorden es correcta");
	pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**)frutera_resultado, MAX_FRUTERA) == 7, "La cantidad de pomelos recorridos inorden es correcta");

	pomelo_t * frutera_esperada[] = {pomelo5, pomelo3, pomelo4, pomelo1, pomelo6, pomelo2, pomelo7};

	for(int i = 0; i < 6; i++){
		if(frutera_resultado[i] != frutera_esperada[i]) orden_correcto = false;
	}

	pa2m_afirmar(orden_correcto, "Los pomelos fueron insertados en el arbol y en el orden correcto");
	arbol_destruir(arbol);
}


void probar_insertar_20_pomelos(abb_comparador comparador, abb_liberar_elemento destructor){

	bool todos_insertados = true;
	bool orden_correcto   = true;

	abb_t    * arbol    = arbol_crear(comparador, destructor);
	pomelo_t * pomelo1  = crear_pomelo(12);
	pomelo_t * pomelo2  = crear_pomelo(6);
	pomelo_t * pomelo3  = crear_pomelo(18);
	pomelo_t * pomelo4  = crear_pomelo(4);
	pomelo_t * pomelo5  = crear_pomelo(10);
	pomelo_t * pomelo6  = crear_pomelo(14);
	pomelo_t * pomelo7  = crear_pomelo(23);
	pomelo_t * pomelo8  = crear_pomelo(2);
	pomelo_t * pomelo9  = crear_pomelo(5);
	pomelo_t * pomelo10 = crear_pomelo(9);
	pomelo_t * pomelo11 = crear_pomelo(11);
	pomelo_t * pomelo12 = crear_pomelo(8);
	pomelo_t * pomelo13 = crear_pomelo(13);
	pomelo_t * pomelo14 = crear_pomelo(15);
	pomelo_t * pomelo15 = crear_pomelo(20);
	pomelo_t * pomelo16 = crear_pomelo(24);
	pomelo_t * pomelo17 = crear_pomelo(19);
	pomelo_t * pomelo18 = crear_pomelo(21);
	pomelo_t * pomelo19 = crear_pomelo(1);
	pomelo_t * pomelo20 = crear_pomelo(3);

	pomelo_t * frutera_resultado[MAX_FRUTERA];

	pomelo_t * frutera_insertar[] = {pomelo1, pomelo2, pomelo3, pomelo4, pomelo5, pomelo6, pomelo7, pomelo8, pomelo9, pomelo10, pomelo11, pomelo12, pomelo13, pomelo14, pomelo15, pomelo16, pomelo17, pomelo18, pomelo19, pomelo20};

	for(int i = 0; i < 20; i++){
		if(arbol_insertar(arbol, frutera_insertar[i]) == ERROR) todos_insertados = false;	
	}

	pa2m_afirmar(todos_insertados, "Fue posible insertar 20 pomelos en el arbol");
	pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**)frutera_resultado, MAX_FRUTERA) == 20, "La cantidad de pomelos contados es 20 y es correcta");

	pomelo_t * frutera_esperada[] = {pomelo19, pomelo8, pomelo20, pomelo4, pomelo9, pomelo2, pomelo12, pomelo10, pomelo5, pomelo11, pomelo1, pomelo13, pomelo6, pomelo14, pomelo3, pomelo17, pomelo15, pomelo18, pomelo7, pomelo16};

	for(int i = 0; i < 20; i++){
		if(frutera_resultado[i] != frutera_esperada[i]) orden_correcto = false;
	}

	pa2m_afirmar(orden_correcto, "Los 20 pomelos fueron insertados en el arbol y en el orden correcto");

	arbol_destruir(arbol);
}


void probar_borrar_7_pomelos(abb_comparador comparador, abb_liberar_elemento destructor){

	bool todos_insertados = true;
	bool orden_correcto   = true;
	
	abb_t    * arbol   = arbol_crear(comparador, destructor);
	pomelo_t * pomelo1 = crear_pomelo(10);
	pomelo_t * pomelo2 = crear_pomelo(15);
	pomelo_t * pomelo3 = crear_pomelo(5);
	pomelo_t * pomelo4 = crear_pomelo(6);
	pomelo_t * pomelo5 = crear_pomelo(3);
	pomelo_t * pomelo6 = crear_pomelo(11);
	pomelo_t * pomelo7 = crear_pomelo(17);
	pomelo_t * pomelo_aux = crear_pomelo(0);
	pomelo_t * frutera_resultado[MAX_FRUTERA];

	pomelo_t * frutera_insertar[] = {pomelo1, pomelo2, pomelo3, pomelo4, pomelo5, pomelo6, pomelo7};
	
	for(int i = 0; i < 7; i++){
		if(arbol_insertar(arbol, frutera_insertar[i]) == ERROR) todos_insertados = false;	
	}

	pa2m_afirmar(todos_insertados, "Se pudo insertar 7 pomelos en el arbol");
	pa2m_afirmar(arbol_borrar(arbol, pomelo1) == EXITO, "El pomelo en la raiz pudo eliminarse (dos hijos)");
	pomelo_aux->peso = 10;
	pa2m_afirmar(!arbol_buscar(arbol, pomelo_aux), "El pomelo que estaba en la raiz ya no se encuentra en el arbol");
	pa2m_afirmar(arbol_raiz(arbol) == pomelo4, "El pomelo en la raiz fue reasignado correctamente");
	pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**)frutera_resultado, MAX_FRUTERA) == 6, "La cantidad de pomelos contados es 6 y es correcta");

	pomelo_t * frutera_esperada1[] = {pomelo5, pomelo3, pomelo4, pomelo6, pomelo2, pomelo7};

	for(int i = 0; i < 6; i++){
		if(frutera_resultado[i] != frutera_esperada1[i]) orden_correcto = false;
	}

	pa2m_afirmar(orden_correcto, "Los pomelos se encuentran en el orden correcto");

	orden_correcto = true;

	pa2m_afirmar(arbol_borrar(arbol, pomelo3) == EXITO, "El pomelo pudo eliminarse (un hijo)");
	pomelo_aux->peso = 5;
	pa2m_afirmar(!arbol_buscar(arbol, pomelo_aux), "El pomelo eliminado ya no se encuentra en el arbol");
	pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**)frutera_resultado, MAX_FRUTERA) == 5, "La cantidad de pomelos contados es 5 y es correcta");

	pomelo_t * frutera_esperada2[] = {pomelo5, pomelo4, pomelo6, pomelo2, pomelo7};

	for(int i = 0; i < 5; i++){
		if(frutera_resultado[i] != frutera_esperada2[i]) orden_correcto = false;
	}

	pa2m_afirmar(orden_correcto, "Los pomelos se encuentran en el orden correcto y el nodo eliminado fue reemplazado por su hijo");

	orden_correcto = true;

	pa2m_afirmar(arbol_borrar(arbol, pomelo5) == EXITO, "El pomelo pudo eliminarse (hoja)");
	pomelo_aux->peso = 3;
	pa2m_afirmar(!arbol_buscar(arbol, pomelo_aux), "El pomelo eliminado ya no se encuentra en el arbol");
	pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**)frutera_resultado, MAX_FRUTERA) == 4, "La cantidad de pomelos contados es 4 y es correcta");

	pomelo_t * frutera_esperada3[] = {pomelo4, pomelo6, pomelo2, pomelo7};

	for(int i = 0; i < 4; i++){
		if(frutera_resultado[i] != frutera_esperada3[i]) orden_correcto = false;
	}

	pa2m_afirmar(orden_correcto, "Los pomelos se encuentran en el orden correcto");

	pomelo_aux->peso = 12;
	pa2m_afirmar(arbol_borrar(arbol, pomelo_aux) == ERROR, "No puedo borrar un pomelo que no existe en el arbol");

	pa2m_afirmar(arbol_borrar(arbol, pomelo4) == EXITO, "Pude borrar otro pomelo");
	pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**)frutera_resultado, MAX_FRUTERA) == 3, "La cantidad de pomelos contados es 3 y es correcta");

	pa2m_afirmar(arbol_borrar(arbol, pomelo7) == EXITO, "Pude borrar otro pomelo");
	pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**)frutera_resultado, MAX_FRUTERA) == 2, "La cantidad de pomelos contados es 2 y es correcta");

	pa2m_afirmar(arbol_borrar(arbol, pomelo2) == EXITO, "Pude borrar otro pomelo");
	pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**)frutera_resultado, MAX_FRUTERA) == 1, "La cantidad de pomelos contados es 1 y es correcta");

	pa2m_afirmar(arbol_borrar(arbol, pomelo6) == EXITO, "Pude borrar el utlimo pomelo en el arbol");
	pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**)frutera_resultado, MAX_FRUTERA) == 0, "La cantidad de pomelos contados es 0 y es correcta");
	pa2m_afirmar(arbol_vacio(arbol), "El arbol se encuentra vacio luego de borrar todos sus pomelos");

	free(pomelo_aux);
	arbol_destruir(arbol);
}

void probar_buscar_existentes_y_no_existentes(abb_comparador comparador, abb_liberar_elemento destructor){

	abb_t    * arbol   = arbol_crear(comparador, destructor);
	pomelo_t * pomelo1 = crear_pomelo(10);
	pomelo_t * pomelo2 = crear_pomelo(15);
	pomelo_t * pomelo3 = crear_pomelo(5);
	pomelo_t * pomelo4 = crear_pomelo(6);
	pomelo_t * pomelo5 = crear_pomelo(3);
	pomelo_t * pomelo_aux = crear_pomelo(3);

	pomelo_t * frutera_insertar[] = {pomelo1, pomelo2, pomelo3, pomelo4, pomelo5};
	
	for(int i = 0; i < 5; i++){
		arbol_insertar(arbol, frutera_insertar[i]);
	}

	pa2m_afirmar(arbol_buscar(arbol, pomelo1) == pomelo1, "El primer pomelo insertado se encuentra en el arbol");
	pa2m_afirmar(arbol_buscar(arbol, pomelo2) == pomelo2, "El segundo pomelo insertado se encuentra en el arbol");
	pa2m_afirmar(arbol_buscar(arbol, pomelo3) == pomelo3, "El tercer pomelo insertado se encuentra en el arbol");
	pa2m_afirmar(arbol_buscar(arbol, pomelo4) == pomelo4, "El cuarto pomelo insertado se encuentra en el arbol");
	pa2m_afirmar(arbol_buscar(arbol, pomelo5) == pomelo5, "El quinto pomelo insertado se encuentra en el arbol");
	pa2m_afirmar(arbol_borrar(arbol, pomelo5) == EXITO,"El quinto pomelo insertado fue borrado del arbol");
	pomelo_aux->peso = 3;
	pa2m_afirmar(!arbol_buscar(arbol, pomelo_aux), "El quinto pomelo ya no se encuentra en el arbol");
	pomelo_aux->peso = 19;
	pa2m_afirmar(!arbol_buscar(arbol, pomelo_aux), "Un pomelo no insertado en el arbol no se encuentra en el arbol");
	
	free(pomelo_aux);
	arbol_destruir(arbol);
}

void probar_elemetnos_duplicados(abb_comparador comparador, abb_liberar_elemento destructor){

	bool todos_insertados = true;
	bool orden_correcto = true;

	abb_t    * arbol   = arbol_crear(comparador, destructor);
	pomelo_t * pomelo1 = crear_pomelo(10);
	pomelo_t * pomelo2 = crear_pomelo(15);
	pomelo_t * pomelo3 = crear_pomelo(5);
	pomelo_t * pomelo4 = crear_pomelo(6);
	pomelo_t * pomelo5 = crear_pomelo(3);
	pomelo_t * pomelo6 = crear_pomelo(12);
	pomelo_t * pomelo7 = crear_pomelo(10);
	pomelo_t * pomelo8 = crear_pomelo(15);
	pomelo_t * pomelo_aux = crear_pomelo(0);
	pomelo_t * frutera_resultado[MAX_FRUTERA];

	pomelo_t * frutera_insertar[] = {pomelo1, pomelo2, pomelo3, pomelo4, pomelo5, pomelo6, pomelo7, pomelo8};
	
	for(int i = 0; i < 8; i++){
		if(arbol_insertar(arbol, frutera_insertar[i]) == ERROR) todos_insertados = false;
	}

	pa2m_afirmar(todos_insertados, "8 pomelos fueron insertados en el arbol");
	pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**)frutera_resultado, MAX_FRUTERA) == 8, "La cantidad de pomelos contados es 8 y es correcta");

	pomelo_t * frutera_esperada1[] = {pomelo5, pomelo3, pomelo4, pomelo7, pomelo1, pomelo6, pomelo8, pomelo2};

	for(int i = 0; i < 7; i++){
		if(frutera_resultado[i] != frutera_esperada1[i]) orden_correcto = false;
	}

	pa2m_afirmar(orden_correcto, "Los pomelos se encuentran en el orden correcto");
	printf("  El pomelo que se encuentra en la raiz está duplicado\n  Quiero borrar ESTE duplicado\n");
	pa2m_afirmar(arbol_borrar(arbol, pomelo7) == EXITO, "Se borro un pomelo, aun no se si la raiz o su duplicado");
	pomelo_aux->peso = 10;
	pa2m_afirmar(arbol_buscar(arbol, pomelo_aux) == pomelo7, "El pomelo que quise borrar sigue en el arbol porque se elimino el de la raíz, el cual encontró primero");

	pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**)frutera_resultado, MAX_FRUTERA) == 7, "La cantidad de pomelos contados luego de borrar uno es 7 y es correcta");

	pa2m_afirmar(arbol_raiz(arbol) == pomelo7, "El pomelo en la raiz fue reemplazado por su duplicado");
	pomelo_t * frutera_esperada2[] = {pomelo5, pomelo3, pomelo4, pomelo7, pomelo6, pomelo8, pomelo2};

	orden_correcto = true;

	for(int i = 0; i < 6; i++){
		if(frutera_resultado[i] != frutera_esperada2[i]) orden_correcto = false;
	}

	pa2m_afirmar(orden_correcto, "Los pomelos se encuentran en el orden correcto luego de borrar");

	printf("  Existe otro pomelo duplicado, el cual tiene como peso 15\n  Quiero borrar SU duplicado\n");
	pa2m_afirmar(arbol_borrar(arbol, pomelo8) == EXITO, "Se borro un pomelo, aun no se si el original o su duplicado");
	pomelo_aux->peso = 15;
	pa2m_afirmar(arbol_buscar(arbol, pomelo_aux) == pomelo8, "El pomelo que quise borrar sigue en el arbol porque se eliminó el original, el cual encontró primero");
	pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**)frutera_resultado, MAX_FRUTERA) == 6, "La cantidad de pomelos contados luego de borrar es 6 y es correcta");

	pomelo_t * frutera_esperada3[] = {pomelo5, pomelo3, pomelo4, pomelo7, pomelo6, pomelo8};

	orden_correcto = true;

	for(int i = 0; i < 6; i++){
		if(frutera_resultado[i] != frutera_esperada3[i]) orden_correcto = false;
	}

	pa2m_afirmar(orden_correcto, "Los pomelos se encuentran en el orden correcto luego de borrar");

	free(pomelo_aux);
	arbol_destruir(arbol);
}


void probar_iterador_interno_funcion_nula(abb_comparador comparador, abb_liberar_elemento destructor, bool (*funcion)(void*, void*)){

	bool todos_insertados = true;

	abb_t * arbol = arbol_crear(comparador, destructor);

	pomelo_t * pomelo1  = crear_pomelo(12);
	pomelo_t * pomelo2  = crear_pomelo(6);
	pomelo_t * pomelo3  = crear_pomelo(18);
	pomelo_t * frutera_resultado[MAX_FRUTERA];

	pomelo_t * frutera_insertar[] = {pomelo1, pomelo2, pomelo3};

	for(int i = 0; i < 3; i++){
		if(arbol_insertar(arbol, frutera_insertar[i]) == ERROR) todos_insertados = false;	
	}

	pa2m_afirmar(funcion == NULL, "La funcion es nula");
	pa2m_afirmar(todos_insertados, "Fue posible insertar 3 pomelos en el arbol");
	pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**)frutera_resultado, MAX_FRUTERA) == 3, "La cantidad de pomelos contados es 3 y es correcta");
	pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, funcion, NULL) == 0, "No es posible iterar con una funcion nula");

	arbol_destruir(arbol);
}

void probar_iterador_interno_arbol_nulo(abb_comparador comparador, abb_liberar_elemento destructor, bool (*funcion)(void*, void*)){

	abb_t * arbol = NULL;

	pa2m_afirmar(!arbol && arbol_vacio(arbol), "El arbol es nulo");
	pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, funcion, NULL) == 0, "No es posible iterar sobre un arbol nulo");

}

void probar_iterador_interno(abb_comparador comparador, abb_liberar_elemento destructor, bool (*funcion)(void*, void*)){

	bool todos_insertados = true; 
	size_t visitados = 0;

	abb_t * arbol = arbol_crear(comparador, destructor);

	pomelo_t * pomelo1  = crear_pomelo(12);
	pomelo_t * pomelo2  = crear_pomelo(6);
	pomelo_t * pomelo3  = crear_pomelo(18);
	pomelo_t * pomelo4  = crear_pomelo(4);
	pomelo_t * pomelo5  = crear_pomelo(10);
	pomelo_t * pomelo6  = crear_pomelo(14);
	pomelo_t * pomelo7  = crear_pomelo(23);
	pomelo_t * pomelo8  = crear_pomelo(2);
	pomelo_t * pomelo9  = crear_pomelo(5);
	pomelo_t * pomelo10 = crear_pomelo(9);
	pomelo_t * pomelo11 = crear_pomelo(11);
	pomelo_t * pomelo12 = crear_pomelo(8);
	pomelo_t * pomelo13 = crear_pomelo(13);
	pomelo_t * pomelo14 = crear_pomelo(15);
	pomelo_t * pomelo15 = crear_pomelo(20);
	pomelo_t * pomelo16 = crear_pomelo(24);
	pomelo_t * pomelo17 = crear_pomelo(19);
	pomelo_t * pomelo18 = crear_pomelo(21);
	pomelo_t * pomelo19 = crear_pomelo(1);
	pomelo_t * pomelo20 = crear_pomelo(3);

	pomelo_t * frutera_insertar[] = {pomelo1, pomelo2, pomelo3, pomelo4, pomelo5, pomelo6, pomelo7, pomelo8, pomelo9, pomelo10, pomelo11, pomelo12, pomelo13, pomelo14, pomelo15, pomelo16, pomelo17, pomelo18, pomelo19, pomelo20};

	for(int i = 0; i < 20; i++){
		if(arbol_insertar(arbol, frutera_insertar[i]) == ERROR) todos_insertados = false;	
	}

	pa2m_afirmar(todos_insertados, "Fue posible insertar 20 pomelos en el arbol");


	visitados = abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, funcion, NULL);
	pa2m_afirmar(visitados == 11, "El iterador interno recorrio de forma inorden la cantidad correcta hasta encontrar la raiz");
	
	visitados = abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, funcion, NULL);
	pa2m_afirmar(visitados == 1, "El iterador interno recorrio de forma preorden la cantidad correcta hasta encontrar la raiz");

	visitados = abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, funcion, NULL);
	pa2m_afirmar(visitados == 20, "El iterador interno recorrio de forma postorden la cantidad correcta hasta encontrar la raiz");

	arbol_destruir(arbol);

	/* 
	1 2 3 4 5 6 8 9 10 11 12 13 14 15 18 19 20 21 23 24 
	12 6 4 2 1 3 5 10 9 8 11 18 14 13 15 23 20 19 21 24 
	1 3 2 5 4 8 9 11 10 6 13 15 14 19 21 20 24 23 18 12 
	*/
}


int main(){

	pa2m_nuevo_grupo("PRUEBAS CREACION ARBOL");
	probar_arbol_crear(comparar_pomelos_por_peso, exprimidor_de_pomelos);


	pa2m_nuevo_grupo("PRUEBAS EN ARBOL VACIO");
	probar_arbol_vacio(comparar_pomelos_por_peso, exprimidor_de_pomelos);


	pa2m_nuevo_grupo("PRUEBAS CON ARBOL NULO");
	probar_arbol_nulo(comparar_pomelos_por_peso, exprimidor_de_pomelos);


	pa2m_nuevo_grupo("PRUEBAS CON COMPARADOR NULO");
	probar_comparador_nulo(NULL, exprimidor_de_pomelos);


	pa2m_nuevo_grupo("PROBAR FUCNIONES DE RECORRIDOS");
	probar_recorridos(comparar_pomelos_por_peso, exprimidor_de_pomelos);


	pa2m_nuevo_grupo("PRUEBAS DE INSERCION:");

	printf("\nPROBAR INSERTAR VARIOS ELEMENTOS (vacio, raiz, recorridos)\n\n");
	probar_insertar_7_pomelos(comparar_pomelos_por_peso, exprimidor_de_pomelos);

	printf("\nPROBAR INSERTAR 20 ELEMENTOS (vacio, raiz, recorridos)\n\n");
	probar_insertar_20_pomelos(comparar_pomelos_por_peso, exprimidor_de_pomelos);

	//printf("\nPROBAR INSERTAR 1000 ELEMENTOS (vacio, raiz, recorridos)\n\n");
	//probar_insertar_1000_elementos();


	pa2m_nuevo_grupo("PRUEBAS DE BORRADO:");

	printf("\nPROBAR BORRAR VARIOS ELEMENTOS (dos hijos, un hijo, hoja)\n\n");
	probar_borrar_7_pomelos(comparar_pomelos_por_peso, exprimidor_de_pomelos);


	pa2m_nuevo_grupo("PROBAR BUSCAR");
	probar_buscar_existentes_y_no_existentes(comparar_pomelos_por_peso, exprimidor_de_pomelos);

	pa2m_nuevo_grupo("PROBAR COMPORTAMIENTO DE ARBOL CON DUPLICADOS");
	probar_elemetnos_duplicados(comparar_pomelos_por_peso, exprimidor_de_pomelos);


	pa2m_nuevo_grupo("PROBAR ITERADOR INTERNO");

	printf("\nPROBAR ITERADOR CON FUNCION NULA\n\n");
	probar_iterador_interno_funcion_nula(comparar_pomelos_por_peso, exprimidor_de_pomelos, NULL);

	printf("\nPROBAR ITERADOR CON ARBOL NULO\n\n");
	probar_iterador_interno_arbol_nulo(comparar_pomelos_por_peso, exprimidor_de_pomelos, recorrer_hasta_raiz);

	printf("\nPROBAR ITERAR 20 ELEMENTOS\n\n");
	probar_iterador_interno(comparar_pomelos_por_peso, exprimidor_de_pomelos, recorrer_hasta_raiz);

	pa2m_mostrar_reporte();

	return 0;

}
