#include <stdio.h>
#include <stdlib.h>

// Número de nodos
#define NODES	7

const char names[] = {'A', 'B', 'C', 'D', 'E',
					'F', 'G', 'H', 'I', 'J',
					'K', 'L', 'M', 'N', 'O',
					'P', 'Q', 'R', 'S', 'T',
					'U', 'V', 'W', 'X', 'Y', 'Z'};

// queue structure
typedef struct _node
{
	char id;
	int index;
	struct _node* back;
} Queue;

char out_names[NODES];
char out_names_counter = 0;

//
// General fuction for the queue
// Enqueue 
// Dequeue
//
int enqueue(Queue** front, Queue** end, char data, int index){
	Queue* new_node = (Queue *) malloc(sizeof(Queue));

	// if somethig is wrong return false
	if (new_node == NULL)
	{
		printf("Error: malloc in enqueue\n");
		return 0;
	}

	new_node->id  = data;
	new_node->index  = index;

	if (*end != NULL){
		Queue* aux = *end;
		aux->back = new_node; 
	}

	*end = new_node; 
	
	// if there's no elements in queue
	if (*front == NULL)
		*front = new_node;

	return 1;
}

int dequeue(Queue** front, Queue** end){


	if (*front == NULL)
		return -1;


	Queue* aux = *front;

	if (aux->back == NULL)
	{
	
		*front = NULL;
		*end = NULL;
	
	}else{
		*front = aux->back;
	}

	
	out_names[out_names_counter] = aux->id;
	out_names_counter++;

	int tmp = aux->index;

	free(aux);

	return tmp;	
}

int count_presreq(int matrix[NODES][NODES], int *pres, int nodes){
	// Cuenta los prerrequisitos
	// de cada nodo

	int i, j, flag = 0;
	for (i = 0; i < nodes; ++i){
		// Si ya revisé esté nodo, no lo cuento
		if (pres[i] == -1) continue;

		pres[i] = 0;

		for ( j = 0; j < nodes; ++j){
			pres[i] += matrix[j][i];
		}

		// Si hay almenos un nodo con prerrequisitos
		if (pres[i] > 0)
			flag = 1;
	}

	return flag;
}

void topo_order(int matrix[NODES][NODES], int* pres, int nodes, Queue** _front, Queue** _end){

	Queue* front = *_front;
	Queue* end = *_end;
	int i, j, flag, flag2 = 0;

	// Regresa 0 si no hay prerequisitos
	flag = count_presreq(matrix, pres, NODES);

	for (i = 0; i < NODES; ++i){
		if (pres[i] == 0){

			// Se eliminan las conecciones del padre
			for (j = 0; j < nodes; ++j) matrix[i][j] = 0; 

			// se agrega a la cola
			enqueue(&front, &end, names[i], i);
			continue;
		}

		flag2++;

	}

	// cuenta los nodos con prerrequisitos
	if (flag2 >= NODES) return;


	int index = dequeue(&front, &end);

	do {

		// Si no hay elementos en la cola
		// y ya no hay elementos a con prerrequisitos 
		// igual a cero, entonces se termina
		if (index == -1 && flag == 0) return;


		// El -1 describe que ese elemento ya se imprimió 
		// en pantalla
		pres[index] = -1;

		index = dequeue(&front, &end);

	} while (index > 0);


	topo_order(matrix, pres, NODES, &front, &end);
}

int main(int argc, char const *argv[])
{

	// matriz de adyacencia
	int matrix[NODES][NODES] = {
		{0, 1, 1, 1, 0, 0, 1},
		{0, 0, 1, 0, 0, 0, 0},
		{0, 0, 0, 0, 1, 1, 0},
		{0, 0, 0, 0, 0, 1, 0},
		{0, 0, 0, 0, 0, 0, 1},
		{0, 0, 0, 0, 1, 0, 1},
		{0, 0, 0, 0, 0, 0, 0}
	};

	// vector de pre-requisitos
	int pres[NODES];
	int i;

	// apuntadores de la cola
	Queue* front = NULL;
	Queue* end = NULL;

	topo_order(matrix, pres, NODES, &front, &end);

	if (out_names_counter >= NODES)
	{
		
		for (i = 0; i < NODES; ++i) printf("%c, ", out_names[i]);
		
		printf("\n");

	}else{
		printf("Grafo cíclico\n");
	}


	return 0;
}