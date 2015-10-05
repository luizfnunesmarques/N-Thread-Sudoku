#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>	

#define SIZE 9

#define MAXOPERATIONS 27
#define LAST_LINE 9
#define LAST_COLUMN 18
#define LAST_REGION 27


int block = 0;
int errorCount = 0;
int grid[SIZE][SIZE];
pthread_mutex_t mutex;

void CheckRegion(int regiao) {
	int startX, endX;
	int startY, endY;
	switch (regiao) {
	case 19: {
		startX = 0;
		startY = 0;
		endX = 2;
		endY = 2;
		break;
	}
	case 20: {
		startX = 3;
		startY = 0;
		endX = 5;
		endY = 2;
		break;
	}
	case 21: {
		startX = 6;
		startY = 0;
		endX = 8;
		endY = 2;
		break;
	}
	case 22: {
		startX = 0;
		startY = 2;
		endX = 2;
		endY = 2;
		break;
	}
	case 23: {
		startX = 0;
		startY = 0;
		endX = 2;
		endY = 2;
		break;
	}
	case 24: {
		startX = 0;
		startY = 0;
		endX = 2;
		endY = 2;
		break;
	}
	case 25: {
		startX = 0;
		startY = 0;
		endX = 2;
		endY = 2;
		break;
	}
	case 26: {
		startX = 0;
		startY = 0;
		endX = 2;
		endY = 2;
		break;
	}
	case 27: {
		startX = 0;
		startY = 0;
		endX = 2;
		endY = 2;
		break;
	}
	default:
		break;
	}

	return checkMatrix(startY, endY, startX, endX);
}

int checkMatrix(int startY, int endY, int startX, int endX) {
	int ocurrences[9] = {};
	for (int i = startX; i <= endX; ++i) {
		for (int j = startY; j <= endY; ++j) {
		
		if (ocurrences[grid[j][i]] != 0)
			return 1;

		ocurrences[grid[j][i]] = 1;
		}	
	}
	
	return 0;
}

void threadExecute(void *arg) {
	int id = (int) arg;	
	int startCheck = (id -1) * block;	
	int endBlock = startCheck + block;
	int startX = 0, startY = 0, endX = 0, endY = 0;

	for (int operacao = startCheck; operacao < endBlock; ++operacao) {
		if (operacao < LAST_LINE) {
			startY = operacao;
			endY = operacao;
			startX = 0;
			endX = 8;			
			if (checkMatrix(startY, endY, startX, endX) == 1){
				printf("Thread %ld: Erro na linha: %d", id, operacao);
				printf("\n");
				pthread_mutex_lock(&mutex);
				errorCount++;
				pthread_mutex_unlock(&mutex);
			}

			continue;
		}

		if (operacao < LAST_COLUMN) {
			startX = operacao - 9;
			endX = operacao - 9;
			startY = 0;
			endY = 8;

			if (checkMatrix(startY, endY, startX, endX) == 1){				
				printf("Thread %ld: Erro na Coluna: %d", id, operacao-9);
				printf("\n");
				pthread_mutex_lock(&mutex);
				errorCount++;
				pthread_mutex_unlock(&mutex);
			}
			continue;
		}

		if (operacao < LAST_REGION) {
			continue;
		}
	pthread_exit(NULL);	
	}
}

/* Funcao que le um grid do arquivo "filename" e o armazena em uma matriz */
int load_grid(int grid[][SIZE], char *filename) {
	FILE *input_file = fopen("/home/luiz/Documents/b", "rw+");
  
	if (input_file != NULL) {
		printf("achou");
		for (int i = 0; i < SIZE; i++)
			for (int j = 0; j < SIZE; j++)
				fscanf(input_file, "%d", &grid[i][j]);
		fclose(input_file);
		return 1;
	}

	return 0;
}

int main(int argc, char *argv[]) {
	if (load_grid(grid, argv[1])) {
		printf("Quebra-cabecas fornecido:\n");
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++)
				printf("%d ", grid[i][j]);
			printf("\n");
		}
		printf("\n");
	}

	int quantidadeThreads = atoi(argv[2]);

	if (quantidadeThreads > 27)
		quantidadeThreads = 27;

	block = (27 / quantidadeThreads);

	pthread_t threads[quantidadeThreads];
	pthread_mutex_init(&mutex, NULL);
	
	int arg;
	for (int i = 1; i <= quantidadeThreads; ++i) {
		arg = i;
		pthread_create(&threads[i], NULL,threadExecute,(void*)arg);	 		
	}

	for (int i = 1; i <= quantidadeThreads; ++i) {
		pthread_join(threads[i],	NULL);	
	}
	
	pthread_mutex_destroy(&mutex);
	printf("%s %d \n", "Erros encontrados: ", errorCount);
	
	if (errorCount == 0 )
		printf("A Solução está correta!");
	
	pthread_exit(NULL);	

	return 0;
}