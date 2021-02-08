TDA ABB - README: Faccini Trinidad

~~PRIMERA SECCION~~

*COMPILACION*

	Para compilar el trabajo se utiliza el siguiente comando: gcc *.c -o abb -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0

	•*.c: el compilador tomará todos los archivos .c de la carpeta en la que se encuentra y los compilará.
	•-o: para asignarle el nombre al fichero de salida.
	•abb: el nombre del archivo ejecutable.
	•-g: da la opción de guardar información de depuración del archivo. Permite que luego se pueda seguir el código a través de un depurador para 
	    analizar los errores.
	•-std=c99: especifica el estándar del lenguaje (en este caso c99)
	•-Wall: habilitará todos los avisos.
	•-Wconversion: avisará si hubo algún tipo de conversión que puede alterar un valor/resultado.
	•-Wtype-limits: generará un aviso en el caso de que exista una comparación que de siempre verdadera o falsa, debido al rango limitado del tipo 
	    de datos. 
	•-pedantic: va en conjunto con –std=c99, le dice al compilador que rechace cualquier código que no sea compatible con el estándar del lenguaje.
	•-Werror: esta opción convertirá a los avisos anteriores en errores.
	•-O0: hace que el compilador compile el código fuente de la forma más sencilla posible.


*EJECUCION*

	Para ejecutar el trabajo se utiliza la siguiente línea: valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./abb

	•--leak-check=full: sirve para detectar fugas de memoria de manera individual, viendo cada una especificada.
	•--track-origins=yes: permite ver los errores que pueden depender de variables no inicializadas.
	•--show-reachable=yes: muestra, además de los bloques de memoria que fueron definitiva o posiblemente perdidos, aquellos a los que si podemos 
	     acceder y aquellos que pueden haber sido perdidos indirectamente.


*ESPECIFICACIONES Y ACLARACIONES SOBRE LA IMPLEMENTACION* 

	--ABB.C--

		1) Las funciones de este TDA fueron implementadas de manera recursiva ya que al hacerlas de esta forma, se podía pensar de manera mucho mas sencilla 
		el flujo del programa en si y tambien los casos especiales que existian en el mismo. 

		2) Se tomó la convención de no crear el arbol en caso de no existir el comparador (recibir comparador nulo en arbol_crear()) y por ello 
		no se comprueba la existencia del mismo en cada funcion en la que luego se usa (como insertar, buscar o borrar) porque si se diera este caso, el 
		arbol simplemente no existiría. 


	--PRUEBAS.C--

		En primer lugar se comprueba si la función inicial/principal (arbol_crear()) funcionaba correctamene. A partir de allí todas las funciones --excepto 
		abb_con_cada_elemento()-- fueron probadas sobre una árbol vacío y sobre un árbol nulo. De esta manera se pudo ver que la comprobación que se hace en la 
		primera línea de cada función para que devuelvan un determinado valor cuando recibían arboles vacíos/nulos, daba el resultado esperado. 

		!!Aclaración: los recorridos fueron probados anterior a probar arbol_insertar() pero gracias al valor que devuelven estos (cantidad de elementos sobre 
		los que iteraron) se pudo comprobar su correcto funcionamiento. 

		Luego, se hicieron pruebas sobre el resto de las funciones principales del TDA ABB (insertar, borrar y buscar). Dentro de estas, funciones como 
		arbol_vacio(), arbol_raiz y los recorridos fueron usadas para comprobar:
			
			• Que estas ultimas tres funciones nombradas funcionaban bien en casos diferentes, ya que no era útil probarlas individualmente.
			• Que cada vez que se insertaba/borraba un elemento, el orden de estos era correcto dentro del árbol.

		!!Aclaración: existe una prueba especial para analizar el comportamiento del árbol y funciones cuando se encontraban duplicados dentro del mismo.

		Finalmente, se realizaron pruebas sobre el iterador interno abb_con_cada_elemento():
		Para hacerlo, se creó una función que devuelve true en el caso de que el elemento pasado por referencia coincida con el elemento que se encuentra en la raíz 
		del ABB. Entonces, la forma de probar que funcionaba de la manera esperada para los 3 tipos de recorridos, fue igualando la cantidad de elementos que 
		había visitado en cada caso a un valor manualmente calculado. Si coincidía con este, significaba que funcionaba bien. 


~~SEGUNDA SECCION~~

• ¿Qué es un ABB? ¿Cómo se diferencia de un Árbol Binario?

	Un ABB es un Árbol Binario de Búsqueda.
	Un árbol binario de búsqueda es un árbol binario que bien puede ser vacío o bien en cada nodo del mismo contener un campo "clave"
	y que satisface las siguientes condiciones:

	A) Que para cada nodo, su hijo izquierdo (si existe) sea menor en su campo clave.
	B) Que para cada nodo, su hijo derecho (si existe) sea mayor en su campo clave.
	C) Que al cumplirse las dos anteriores, se pueda asegurar que al moverse a un sub-árbol derecho va a encontrar nodos con TODAS claves más grandes y que al 
	moverse a un sub-árbol izquierdo va a encontrar nodos con TODAS claves más chicas. 

	La gran diferencia entre un ABB y un Árbol Binario es que, más allá de que en ambos, cada nodo puede poseer como máximo dos hijos (hijo izquierdo e hijo 
	derecho), en el segundo no se deben cumplir las condiciones anteriormente nombradas. Es decir, no existe una pauta específica al momento de insertar los nodos 
	en este. 


• ¿Cuál es el objetivo de tener una función de destrucción en el TDA? ¿Qué implicaría para el usuario no tenerla?
	
	El objetivo de tener esta funcion es que el usuario pueda indicarle al implementador que quiere hacer algo en especifico con los elementos 
	que fueron insertados en el árbol. 
	Un ejemplo muy común es que el destructor se encargue de liberar la memoria que fue reservada para cada elemento en la funcion que los crea
	-- la cual tambien fue hecha por el usuario --.

	No tener esta funcion puede implicar cosas diferentes. Por un lado, que no se quiera realizar nada con los elementos -- por ejemplo, que estos
	hayan sido creados en el stack y no necesite una funcion que los libere-- y por el otro, un error en el programa -- por ejemplo, que los elementos
	esten almacenados en el heap y no exista una funcion que se encargue de liberar la memoria que ocupan-- ya que el **** no sabria cual es la 
	intencion del usuario. 


• ¿Cuál es la complejidad de las diferentes operaciones del ABB? Justifique.

	~Crear    (arbol_crear): O(1) --> Es una sola operacion, la cual se encarga de reservar la memoria para el arbol y asignarle sus campos si los tuviera.

	~Destruir (arbol_destruir): O(n) --> Porque debe liberar la memoria de todos y cada uno de los nodos del árbol.

	~Vacio    (arbol_vacio): O(1) --> Es una sola operacion que chequea si el arbol es igual a NULL o si su raiz es igual a NULL.

	~Insertar (arbol_insertar): \
                                 \  O(log(n)) en el caso de que el ABB se encuentre balanceado, porque se irá diviendo el problema en 2 POR CADA NODO que se visite.
	~Eliminar (arbol_borrar):     > Eligiendo si moverse hacia la derecha en caso de que el nodo a insertar/borrar/buscar sea mayor al que esta siendo visitado o  	 							
	                             /	hacia la izquierda si es menor. Si se da que el árbol se degeneró en lista, sera O(n) ya que tendrían que recorrer todos los nodos del 
	~Buscar   (arbol_buscar):   /   nodos del arbol. 

	~Recorrer (arbol_recorrido_): O(n) --> Ya que tiene que iterar sobre cada nodo del arbol. 

