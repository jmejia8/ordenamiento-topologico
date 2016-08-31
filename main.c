#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Número de nodos
#define NODES	30

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

int count_presreq(int nodes, int matrix[nodes][nodes], int *pres){
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

void topo_order(int nodes, int matrix[nodes][nodes], int* pres, Queue** _front, Queue** _end){
	// revisar error de segmentación
	// revisar error de segmentación
	// revisar error de segmentación
	// revisar error de segmentación

	Queue* front = *_front;
	Queue* end = *_end;
	int i, j, flag, flag2 = 0;

	// Regresa 0 si no hay prerequisitos
	flag = count_presreq(nodes, matrix, pres);
	

	for (i = 0; i < nodes; ++i){
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
	if (flag2 >= nodes) return;


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


	topo_order(nodes, matrix, pres, &front, &end);
}


void greeting(){
	printf("=============================================\n");
	printf("\tOrdenamiento topológico\n");
	printf("=============================================\n");
}

void save_latex(int nodes, int matrix[nodes][nodes]){
	FILE *txt = fopen("main.tex", "aw");

	if (txt == NULL)
	{
		printf("Error al cargar archivo tex\n");
		return;
	}
	/*
	* A partir de la matriz de adyacencia se genera código latex 
	* para generar los grafos de manera visual
	*/
	// ponemos el preambulo al archivo de latex
	fprintf(txt, "\\documentclass[12pt,letterpaper]{article} \\usepackage[utf8]{inputenc} \\usepackage[spanish]{babel} \\usepackage{tikz} \\author{Jesús A . Mejía de Dios} \\begin{document}\n");
	fprintf(txt, "\\centering \\begin{tabular}{|c|} \\hline\n");
	fprintf(txt, "\\begin{tikzpicture}[scale=1.3, transform shape]\n");
	fprintf(txt, "\\tikzstyle{every node} = [circle, fill=green!30]\n");

	int i, j;

	// colocamos cada nodo en la circunferencia de radio sqrt(nodes)
	float rad = sqrt(nodes), t;
	for (i = 0; i < nodes; ++i) {
		t = 2*3.1416*i/nodes;
		fprintf(txt, "\\node (%c) at ( %.2f, %.2f)  {%c}; \n", names[i], rad*cos(t), rad*sin(t), names[i] );
	}

	// Generamos cada arco
	for (int i = 0; i < nodes; ++i)
	{
		for (int j = 0; j < nodes; ++j)
		{
			if (matrix[i][j] != 0){
				fprintf(txt, "\\draw[<-, color=black] (%c) -- (%c);\n", names[j], names[i]);
			}
		}
	}


	fprintf(txt, "\\end{tikzpicture}\\\\ \n ");
	fprintf(txt, "\\hline\n\\end{tabular} % \n \\\\");
	fprintf(txt, "\\LARGE Ordenamiento: ");

	if (out_names_counter >= nodes)
	{
		
		for (i = 0; i < nodes; ++i) fprintf(txt, "%c, ", out_names[i]);
		
		fprintf(txt , "\n");

	}else{
		fprintf(txt, "\\\\Error: Grafo cíclico\n");
	}

	fprintf(txt, "\n\\end{document}\n");


	fclose(txt);

	system("latexmk -C && latexmk -pdf && exo-open main.pdf");	
	system("rm main.tex");	

}


int main(int argc, char const *argv[])
{

	greeting();

	int nodes, i, j;
	scanf("%d", &nodes);

	// matriz de adyacencia
	int matrix[nodes][nodes], tmp[nodes][nodes] /*= {
		{0, 1, 1, 1, 0, 0, 1},
		{0, 0, 1, 0, 0, 0, 0},
		{0, 0, 0, 0, 1, 1, 0},
		{0, 0, 0, 0, 0, 1, 0},
		{0, 0, 0, 0, 0, 0, 1},
		{0, 0, 0, 0, 1, 0, 1},
		{0, 0, 0, 0, 0, 0, 0}
	}*/;

	// vector de pre-requisitos
	int pres[nodes];

	for (i = 0; i < nodes; ++i)
		for (j = 0; j < nodes; ++j){
			scanf("%d", &matrix[i][j]);
			tmp[i][j]= matrix[i][j];
		}
	

	// apuntadores de la cola
	Queue* front = NULL;
	Queue* end = NULL;


	topo_order(nodes, matrix, pres, &front, &end);

	save_latex(nodes, tmp);

	printf("=============================================\n");

	return 0;
}