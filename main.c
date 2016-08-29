#include <stdio.h>
#include <stdlib.h>

// queue structure
typedef struct _node
{
	char id;
	struct _node* back;
} Queue;

//
// General fuction for the queue
// Enqueue 
// Dequeue
//
int enqueue(Queue** front, Queue** end, char data){
	Queue* new_node = (Queue *) malloc(sizeof(Queue));

	// if somethig is wrong return false
	if (new_node == NULL)
	{
		printf("Error: malloc in enqueue\n");
		return 0;
	}

	new_node->id  = data;

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
	{
		printf(">>>>Falso\n");
		return 0;
	}


	Queue* aux = *front;

	if (aux->back == NULL)
	{
	
		*front = NULL;
		*end = NULL;
	
	}else{
		*front = aux->back;
	}

	
	printf("%c, ", aux->id);

	free(aux);

	return 1;	
}



int main(int argc, char const *argv[])
{
	int nodes = 5;
	int matrix[nodes][nodes];

	Queue* front = NULL;
	Queue* end = NULL;

	printf("Inicio\n");

	enqueue(&front, &end, 'A');
	enqueue(&front, &end, 'B');
	enqueue(&front, &end, 'C');

	printf("------\n");

	// Queue *perro = end;
	// printf("%c\n", perro->id);


	dequeue(&front, &end);
	dequeue(&front, &end);
	dequeue(&front, &end);
	// dequeue(&front, &end);


	return 0;
}