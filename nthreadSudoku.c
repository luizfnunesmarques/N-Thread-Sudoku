#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#define SIZE 9
#define MAXOPERATIONS 27
#define LAST_LINE 9
#define LAST_COLUMN 18
#define LAST_REGION 26
#define COLUMN 1
#define LINE 2
#define REGION 3

int block = 0;
int errorCount = 0;
int grid[SIZE][SIZE];
int quantidadeThreads;

pthread_mutex_t mutex;

int CheckRegion(int regiao) {
	int isValid;	
	switch (regiao) {
	case 18: {				
		isValid = IsSudokuValid(0, 2, 0, 2);				
		break;
	}
	case 19: {				
		isValid = IsSudokuValid(0, 2, 3, 5);		
		break;
	}
	case 20: {		
		isValid = IsSudokuValid(0, 2, 6, 8);		
		break;
	}
	case 21: {		
		isValid = IsSudokuValid(3, 5, 0, 2);			
		break;
	}
	case 22: {		
		isValid = IsSudokuValid(3, 5, 3, 5);		
		break;
	}
	case 23: {		
		isValid = IsSudokuValid(3, 5, 6, 8);		
		break;
	}

	case 24: {		
		isValid = IsSudokuValid(6, 8, 0, 2);		
		break;
	}

	case 25: {				
		isValid = IsSudokuValid(6, 8, 3, 5);		
		break;
	}

	case 26: {		
		isValid = IsSudokuValid(6, 8, 6, 8);			
		break;
	}		
	}

	return isValid;
}

int IsSudokuValid(int startY, int endY, int startX, int endX) {
	int ocurrences[10] = {};	
	for (int i = startX; i <= endX; ++i) {
		for (int j = startY; j <= endY; ++j) {			
		if (ocurrences[grid[i][j]] != 0)
			return 1;
		ocurrences[grid[i][j]] = 1;
		}
	}	
	return 0;
}

void threadExecute(void *arg) {
	int id = (int) arg;
	int startCheck = (id -1) * block;
	int endBlock = startCheck + block;	

	if (id > 27)
		return;
	
	if ((id < 27) && (id == quantidadeThreads))
		endBlock = 27;	
	
	for (int operacao = startCheck; operacao < endBlock; ++operacao) {				
		if (operacao < LAST_LINE) {
			if (IsSudokuValid(operacao, operacao, 0, 8) == 1){				
				printf("Thread %d: Erro na linha: %d", id, operacao + 1);
				printf("\n");
				pthread_mutex_lock(&mutex);
				errorCount++;
				pthread_mutex_unlock(&mutex);
			}	
			continue;
		}

		if (operacao < LAST_COLUMN) {
			if (IsSudokuValid(0, 8, operacao - 9,  operacao - 9) == 1){		
				printf("Thread %d: Erro na Coluna: %d", id, operacao - 8);
				printf("\n");
				pthread_mutex_lock(&mutex);
				errorCount++;
				pthread_mutex_unlock(&mutex);		
			}
			continue;
		}
		if (operacao <= LAST_REGION) {
			if (CheckRegion(operacao) == 1){				
				printf("Thread %d: Erro na Região: %d", id, operacao - 17);
				printf("\n");
				pthread_mutex_lock(&mutex);
				errorCount++;
				pthread_mutex_unlock(&mutex);
			}
			continue;
		}
	pthread_exit(NULL);
	}
}
/* Funcao que le um grid do arquivo "filename" e o armazena em uma matriz */
int load_grid(int grid[][SIZE], char *filename) {
	FILE *input_file = fopen(filename, "rw+");
	if (input_file != NULL) {		
		for (int i = 0; i < SIZE; i++)
			for (int j = 0; j < SIZE; j++)
				fscanf(input_file, "%d", &grid[i][j]);
		fclose(input_file);
		return 1;
	}else{
		perror(input_file);
	}
	return 0;
}



int main(int argc, char *argv[]) {	
	if(argc != 3) {
		printf("Erro: informe o arquivo de entrada!\nUso: %s <arquivo de entrada>\n\n", argv[0]);
		return 1;
	}

	if (load_grid(grid, argv[1])) {
		printf("Quebra-cabecas fornecido:\n");
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++)
				printf("%d ", grid[i][j]);
			printf("\n");
		}
		printf("\n");

	}else{
		return 1;
	}

	quantidadeThreads = atoi(argv[2]);		
	block = (quantidadeThreads <=27) ? (27 / quantidadeThreads) : 1;	

	pthread_t threads[quantidadeThreads];
	pthread_mutex_init(&mutex, NULL);	

	for (int i = 1; i <= quantidadeThreads; ++i) {		
		pthread_create(&threads[i], NULL,threadExecute,(void*)i);
	}
	for (int i = 1; i <= quantidadeThreads; ++i) {
		pthread_join(threads[i], NULL);	
	}

	pthread_mutex_destroy(&mutex);
	printf("%s %d \n", "Erros encontrados: ", errorCount);

	if (errorCount == 0 )
		printf("A Solução está correta!");	

	pthread_exit(NULL);	
	return 0;

}
