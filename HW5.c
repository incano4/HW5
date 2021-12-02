#include <stdio.h>
#include <stdlib.h>

void write_graph(FILE* fname, int** arr, int size)
{
	fprintf(fname, "graph G{\n");

	for (int i = 0; i < size; i++)
		fprintf(fname, "%d;\n", i);

	for (int j = 0; j < size; j++) {
		for (int i = j; i < size; i++)
			for (int k = 0; k < arr[i][j]; k++)
				fprintf(fname, "%d -- %d;\n", j, i);
	}

	fprintf(fname, "}");

	fclose(fname);
}


void check(int** arr, int* sort, int size) {

	for (int j = 0; j < size; j++) {		//проверка на симметричность
		for (int i = 0; i < size; i++) {
			if (arr[i][j] != arr[j][i]) {
				printf("Матрица несимметричная\n");
			break;
			}
		}
	}

	for (int sum, j = 0; j < size; j++) {		// цикл на подсчет степеней каждой вершины
		for (int i = 0; i < size; i++) {
			if (arr[i][j] != 0) {
				sort[i] = sort[i] + arr[i][j];	//считаем количество степеней... записываем их как элементы массива sort
			}
		}
	}

	for (int i = 0; i < size; i++) {			//проверим на ребра, приходящие в ту же вершину. удваиваем их количество
		if (arr[i][i] > 0) {
			sort[i] = sort[i] + arr[i][i];		//считаем количество степеней...
		}
	}
	
	for (int i = 0; i < size; ++i) {			//вывод несортированного массива со степенями вершин
		printf("%d ", sort[i]);
	}
}

void sorting(int* sort, int* number, int size) {

	for (int i = 0; i < size; i++) {		//заполняем массив, который даст номер вершине
		number[i] = i;
	}

	for (int gap = size / 2; gap > 1; gap = gap / 2) {	//задаем шаг gap и после каждого прохождения уменьшаем его на 2
		for (int i = 0; i < size - gap; i++) {			//рассматриваем элементы массива с шагом gap
			if (sort[i] < sort[i + gap]) {				//если 1й больше чем 2й, то меняем местами
				int change = sort[i];				//переменная change позволит менять местами два элемента массива
				sort[i] = sort[i + gap];
				sort[i + gap] = change;

				int peak = number[i]; //параллельно меняем номера вершин
				number[i] = number[i + gap];
				number[i + gap] = peak;
			}
		}
	}

	for (int i = 0; i < size - 1; i++) {
		if (sort[i] < sort[i+1]) {
			int change = sort[i+1]; //переменная change позволит менять местами два элемента массива
			sort[i+1] = sort[i];
			sort[i] = change;

			int peak = number[i+1]; //параллельно меняем номера вершин
			number[i+1] = number[i];
			number[i] = peak;
		}
	} 
	
	for (int i = 0; i < size; i++)
		printf("\n%d - степень вершины (%d)", sort[i], number[i]);
}



void free_arr(char* arr) {
	free(arr);
	arr = NULL;
	arr = (char*)malloc(sizeof(char));
}

int main(void) {
	int width;
	int* sort, size, i = 0;
	int* number;
	printf("Размер квадратной матрицы: ");
	scanf("%d", &width);


	printf("\nЗаполните строки матрицы. Нажимая \"ENTER\" вы перейдете на следующую строку. Если вы не ввели число, то оставшиеся ячейки будут заполнены '0'.\n");

	getchar();											


	int** arr = NULL;

	arr = (int**)malloc(sizeof(*arr) * width);
	for (int i = 0; i < width; i++) {
		arr[i] = (int*)malloc(sizeof(**arr) * width);
	}

	sort = malloc(sizeof(int) * size);
	number = malloc(sizeof(int) * size);

	char* s, c;
	s = (char*)malloc(sizeof(char));


	for (int i = 0, m = 1, j = 0; j < width; j++) {

		while ((c = getchar()) != '\n') {

			if ((c < '0') && c != ' ') {
				printf("Введенное значение некорректное\n");
				return -1;
			}

			if (c != ' ') {
				s[m - 1] = c;

				s = (char*)realloc(s, sizeof(char) * (++m));
			}

			else {
				s[m - 1] = '\0';

				if (i >= width) {
					printf("Чисел в строке больше, чем задано\n");
					return -1;
				}

				arr[i][j] = atoi(s);      // преобразуем строку в число
				i++;
				m = 1;

				free_arr(s);

			}

		}

		s[m - 1] = '\0';

		if (i >= width) {
			printf("Чисел в строке больше, чем задано\n");
			return -1;
		}

		arr[i][j] = atoi(s);

		if (i < width) {
			++i;
			while (i < width)
				arr[i++][j] = 0;
		}

		i = 0;
		m = 1;

		free_arr(s);
	}

	free(s);
	s = NULL;

	FILE* fout;
	fout = fopen("graph.gv", "w");

	if (fout == NULL) {
		perror("fopen()");
		return EXIT_FAILURE;
	}

	write_graph(fout, arr, width);

	check(arr, sort, width);

	sorting(sort, number, width);


	for (int i = 0; i < width; i++)
		free(arr[i]);
	free(arr);
	arr = NULL;

	free(sort);
	free(number);

	system("dot -Tjpg graph.gv -o HW2.jpg");
	return 0;
}