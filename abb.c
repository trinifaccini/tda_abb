#include "abb.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define ERROR -1
#define EXITO 0
#define NINGUNO 0 


abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){

	if(!comparador) return NULL;

	abb_t* mi_arbol = calloc(1,sizeof(abb_t));
	if(!mi_arbol) return NULL;

	mi_arbol->comparador = comparador;
	mi_arbol->destructor = destructor;
	return mi_arbol;
}


/* PRE:  recibe la funcion con la que va a comparar los elementos que contienen los nodos, un puntero al nodo que 
 *       va a ser el padre del nodo a insertar y un puntero al nodo a insertar (es decir, al nodo que contiene
 *       el elemento que se desea insertar).
 *
 * POST: devuelve 0 si pudo insertarlo o -1 si no pudo.
 */
int nodo_insertar(abb_comparador comparador, nodo_abb_t * padre, nodo_abb_t * nuevo){

	if(comparador(nuevo->elemento, padre->elemento) > 0){ 
		if(padre->derecha == NULL) padre->derecha = nuevo; 
		else nodo_insertar(comparador, padre->derecha, nuevo); 
		return EXITO;
	}

	if(comparador(nuevo->elemento, padre->elemento) <= 0){ 
		if(padre->izquierda == NULL) padre->izquierda = nuevo; 
		else nodo_insertar(comparador, padre->izquierda, nuevo); 
		return EXITO;
	}

	return ERROR;
}


int arbol_insertar(abb_t* arbol, void* elemento){

	if(!arbol) return ERROR; 

	if(!arbol->nodo_raiz){ 
		arbol->nodo_raiz = calloc(1, sizeof(nodo_abb_t));
		arbol->nodo_raiz->elemento = elemento;
		return EXITO;
	}

	else{ 
		nodo_abb_t * hoja = calloc(1, sizeof(nodo_abb_t)); 
		hoja->elemento = elemento;
		return nodo_insertar(arbol->comparador, arbol->nodo_raiz, hoja);
	}
}


bool arbol_vacio(abb_t* arbol){
	return (!arbol || !arbol->nodo_raiz);
}


/* PRE:  recibe la función con la que va a comparar los elementos que contienen los nodos, un puntero al nodo actual 
 *       y el elemento que queremos buscar dentro del árbol. 
 * 
 * POST: si pudo encontrar el elemento devuelve un puntero a este o NULL si este elemento no existe dentro del árbol.
 */
void* elemento_buscar(abb_comparador comparador, nodo_abb_t * actual, void * buscado){

	if(!actual) return NULL;

	int comparacion = comparador(buscado, actual->elemento);

	if(comparacion == 0) return actual->elemento;
	if(comparacion > 0) return elemento_buscar(comparador, actual->derecha, buscado);
	else return elemento_buscar(comparador, actual->izquierda, buscado); 
}


void* arbol_buscar(abb_t* arbol, void* elemento){

	if(arbol_vacio(arbol)) return NULL;
	return elemento_buscar(arbol->comparador, arbol->nodo_raiz, elemento);
}


void* arbol_raiz(abb_t* arbol){

	if(arbol_vacio(arbol)) return NULL;
	return arbol->nodo_raiz->elemento;
}


/* PRE:  recibe un puntero al nodo raíz de un sub-árbol
 *
 * POST: devuelve un puntero al nodo hoja que se encuentra mas a la derecha de este sub-árbol
 *
 * !!En caso de que raiz_actual coincida con la raiz del arbol, al decir sub-árbol este coincidiría con el árbol.
 */
nodo_abb_t * buscar_predecesor_inorden(nodo_abb_t * raiz_actual){

	nodo_abb_t * actual = raiz_actual;

	while(actual && actual->derecha){
		actual = actual->derecha;
	}

	return actual;
}

/* Se encargara de buscar el nodo a borrar pudiendo proceder de 3 formas diferentes:
 --> Borrando el nodo que contiene al elemento que se quiere borrar en caso de que este sea una hoja
 --> En el caso de que este tenga un solo hijo (a derecha o a izquierda) se borrara el nodo reemplazandolo con su unico hijo
 --> En el caso de que tenga dos hijos, se eliminara el nodo y su reemplazo sera su predecesor inorden
 */
nodo_abb_t * nodo_borrar(abb_t * arbol, nodo_abb_t * actual, void * buscado, bool * encontro){

	if(!actual) return NULL;

	nodo_abb_t * reemplazo = NULL;
	int comparacion = arbol->comparador(buscado, actual->elemento);

	if(comparacion == 0){

		*encontro = true;

		if(actual->derecha && actual->izquierda){

			void * elem_aux = NULL;

			reemplazo = buscar_predecesor_inorden(actual->izquierda); 
			elem_aux  = actual->elemento;
			actual->elemento = reemplazo->elemento;
			reemplazo->elemento = elem_aux;                  
			actual->izquierda = nodo_borrar(arbol, actual->izquierda, elem_aux, encontro);
		}

		else if (!actual->derecha && !actual->izquierda){
			if(arbol->destructor) arbol->destructor(actual->elemento);
			free(actual);
			return NULL;
		}
		
		else{
			reemplazo = actual->derecha?actual->derecha:actual->izquierda;
			if(arbol->destructor) arbol->destructor(actual->elemento);
			free(actual);
			return reemplazo;
		}
	}

	else if(comparacion > 0) actual->derecha = nodo_borrar(arbol, actual->derecha, buscado, encontro);
	
	else if(comparacion < 0) actual->izquierda = nodo_borrar(arbol, actual->izquierda, buscado, encontro); 

	return actual;
}
	


int arbol_borrar(abb_t* arbol, void* elemento){

	if(arbol_vacio(arbol)) return ERROR;

	bool encontro = false;
	arbol->nodo_raiz = nodo_borrar(arbol, arbol->nodo_raiz, elemento, &encontro);

	if(encontro) return EXITO;
	else return ERROR;
}


/* Recibe un puntero al nodo actual, un vector de punteros, un contador y un entero referenciando al 
 * tamaño del vector.
 * Recorrerá el arbol de manera inorden e irá guardando la referencia a cada elemento visitado 
 * del arbol dentro del vector. A su vez, aumentará contador en uno por cada nodo visitado.
 */
void inorden(nodo_abb_t * actual, void** array, int * contador, size_t tamanio_array){

	if(actual && (*contador) < tamanio_array){
		inorden(actual->izquierda, array, contador, tamanio_array);
		array[*contador] = actual->elemento;
		(*contador)++;
		inorden(actual->derecha, array, contador, tamanio_array);
	}
}


size_t arbol_recorrido_inorden(abb_t* arbol, void** array, size_t tamanio_array){

	if(arbol_vacio(arbol)) return NINGUNO; 
	int contador = NINGUNO;
	inorden(arbol->nodo_raiz, array, &contador, tamanio_array);
	return (size_t)contador;
}


/* Recibe un puntero al nodo actual, un vector de punteros, un contador y un entero referenciando al 
 * tamaño del vector.
 * Recorrerá el arbol de manera preorden e irá guardando la referencia a cada elemento visitado 
 * del arbol dentro del vector. A su vez, aumentará contador en uno por cada nodo visitado.
 */
void preorden(nodo_abb_t * actual, void** array, int * contador, size_t tamanio_array){

	if(actual && (*contador) < tamanio_array){
		array[*contador] = actual->elemento;
		(*contador)++;
		preorden(actual->izquierda, array, contador, tamanio_array);
		preorden(actual->derecha, array, contador, tamanio_array);
	}
}


size_t arbol_recorrido_preorden(abb_t* arbol, void** array, size_t tamanio_array){

	if(arbol_vacio(arbol)) return NINGUNO; 
	int contador = NINGUNO;
	preorden(arbol->nodo_raiz, array, &contador, tamanio_array);
	return (size_t)contador;
}


/* Recibe un puntero al nodo actual, un vector de punteros, un contador y un entero referenciando al 
 * tamaño del vector.
 * Recorrerá el arbol de manera postorden e irá guardando la referencia a cada elemento visitado 
 * del arbol dentro del vector. A su vez, aumentará contador en uno por cada nodo visitado.
 */
void postorden(nodo_abb_t * actual, void** array, int * contador, size_t tamanio_array){

	if(actual){
		postorden(actual->izquierda, array, contador, tamanio_array);
		postorden(actual->derecha, array, contador, tamanio_array);
		if(*contador < tamanio_array){
			array[*contador] = actual->elemento;
			(*contador)++;
		}	
	}
}


size_t arbol_recorrido_postorden(abb_t* arbol, void** array, size_t tamanio_array){

	if(arbol_vacio(arbol)) return NINGUNO; 
	int contador = NINGUNO;
	postorden(arbol->nodo_raiz, array, &contador, tamanio_array);
	return (size_t)contador;
}


/* Se encarga de recorrer los nodos del arbol de manera inorden y para cada uno de ellos invoca a la funcion booleana recibida.
 * Lo repetira hasta que esta devuelva true. Al mismo tiempo aumenta el contador en uno por cada nodo visitado.
 */
void iterador_interno_inorden(nodo_abb_t* nodo_actual, size_t * contador, bool * condicion, bool (*funcion)(void*, void*), void* extra){

	if(nodo_actual){
		iterador_interno_inorden(nodo_actual->izquierda, contador, condicion, funcion, extra);
		if(!(*condicion)){
			*condicion = funcion(nodo_actual->elemento, extra);
			(*contador)++;
		}
		iterador_interno_inorden(nodo_actual->derecha, contador, condicion, funcion, extra);
	}
}


/* Se encarga de recorrer los nodos del arbol de manera preorden y para cada uno de ellos invoca a la funcion booleana recibida.
 * Lo repetira hasta que esta devuelva true. Al mismo tiempo aumenta el contador en uno por cada nodo visitado.
 */
void iterador_interno_preorden(nodo_abb_t* nodo_actual, size_t * contador, bool * condicion,  bool (*funcion)(void*, void*), void* extra){

	if(nodo_actual && !(*condicion)){
		*condicion = funcion(nodo_actual->elemento, extra);
		(*contador)++;
		iterador_interno_preorden(nodo_actual->izquierda, contador, condicion, funcion, extra);
		iterador_interno_preorden(nodo_actual->derecha, contador, condicion, funcion, extra);
	}
}


/* Se encarga de recorrer los nodos del arbol de manera postorden y para cada uno de ellos invoca a la funcion booleana recibida.
 * Lo repetira hasta que esta devuelva true. Al mismo tiempo aumenta el contador en uno por cada nodo visitado.
 */
void iterador_interno_postorden(nodo_abb_t* nodo_actual, size_t * contador,  bool * condicion, bool (*funcion)(void*, void*), void* extra){

	if(nodo_actual){
		iterador_interno_postorden(nodo_actual->izquierda, contador, condicion, funcion, extra);
		iterador_interno_postorden(nodo_actual->derecha, contador, condicion, funcion, extra);
		if(!(*condicion)){
			*condicion = funcion(nodo_actual->elemento, extra);
			(*contador)++;
		}
	}
}


size_t abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra){

	if(arbol_vacio(arbol) || !funcion) return NINGUNO; 

	size_t contador = NINGUNO;
	bool condicion = false;
	
	if(recorrido == ABB_RECORRER_INORDEN) iterador_interno_inorden(arbol->nodo_raiz, &contador, &condicion, funcion, extra);
	else if(recorrido == ABB_RECORRER_PREORDEN) iterador_interno_preorden(arbol->nodo_raiz, &contador, &condicion, funcion, extra);
	else if(recorrido == ABB_RECORRER_POSTORDEN) iterador_interno_postorden(arbol->nodo_raiz, &contador, &condicion, funcion, extra);

	return contador;
}


/* Recibe un puntero al nodo actual y una funcion destructora.
 * Se encarga de invocar al destructor por cada nodo que visite al mismo tiempo
 * de liberar la memoria que fue reservada para ese nodo. 
 */
void destruir_elementos(nodo_abb_t* nodo_actual, abb_liberar_elemento destructor){

	if(nodo_actual){
		destruir_elementos(nodo_actual->izquierda, destructor);
		destruir_elementos(nodo_actual->derecha, destructor);
		if(destructor) destructor(nodo_actual->elemento); 
		free(nodo_actual);
	}
	else return;
}


void arbol_destruir(abb_t* arbol){

	if(!arbol) return;

	destruir_elementos(arbol->nodo_raiz, arbol->destructor);
	free(arbol);
}

