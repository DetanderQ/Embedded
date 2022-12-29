#include <stdio.h>
#include <stdlib.h>

#define SPACE 0
#define BLOCK 1
#define ENTRY 2
#define EXIT 3
#define PATH 4

int findPath(int** map,int x, int y) {
	int isMove = 0;
	
	if (map[y][x] == EXIT) {
		map[y][x] = PATH;
		return 1;
	}
	
	map[y][x] = PATH;
	
	if (y != 0) {
		if ((map[y - 1][x] == SPACE) || (map[y - 1][x] == EXIT)) {
			//printf("up\n");
			if (findPath(map, x, y - 1)) {
				isMove = 1;
			}
		}
	}
	if ((map[y + 1][x] == SPACE) || (map[y + 1][x] == EXIT)) {
			//printf("down\n");
			if (findPath(map, x, y + 1)) {
				isMove = 1;
			}
	}
	if ((map[y][x - 1] == SPACE) || (map[y][x - 1] == EXIT)) {
			//printf("left\n");
			if (findPath(map, x - 1, y)) {
				isMove = 1;
			}
	}
	if ((map[y][x + 1] == SPACE) || (map[y][x + 1] == EXIT)) {
			//printf("right\n");
			if (findPath(map, x + 1, y)) {
				isMove = 1;
			}
	}
	
	if(!isMove) {
			map[y][x] = SPACE;
	}
	
	return isMove;
}

void printMap(int** array, int w, int h) {
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			char tmp = array[i][j];
			
			switch (tmp) {
				case SPACE:
					tmp = ' ';
					break;
				case BLOCK:
					tmp = '1';
					break;
				case ENTRY:
					tmp = ' ';
					break;
				case EXIT:
					tmp = ' ';
					break;
				case PATH:
					tmp = '.';
					break;
					}
	
			printf("%c", tmp);
		}
		printf("\n");
	}
}

int main(void) {
	int h;  
	int w;
	
	FILE* lab;
	
	lab = fopen("1.lab", "r");
	fscanf(lab, "%d %d", &w, &h);
	
	printf("size: %d %d\n", w, h);
	
	int** map;
	map = malloc(sizeof(int*) * h); 
	for (int i = 0; i < h; i++) {
		map[i] = malloc(sizeof(int) * w); 
	}
	
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			int c = getc(lab);
			if (c == '\n') {
				c = getc(lab);
			}
			map[i][j] = c - '0';
		}
	}
	
	printf("Input map:\n");
	printMap(map, w, h);

	findPath(map, 5, 0);
	printf("Result:\n");
	printMap(map, w, h);

	return 0;
}
