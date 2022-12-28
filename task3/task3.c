#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book {
	char title[50];
	float price;
	int numberOfPages;
	float weight;
	int year;
};

struct Node {
	struct Book* value;
	struct Node* next;
};

struct List {
	struct Node* first;
	int size;
};

struct List* newList() {
 	return calloc(1, sizeof(struct List));
}

struct Node* newNode(struct Book* book, struct Node* next) {
	struct Node* node = malloc(sizeof(struct Node));
	node->value = book;
	node->next = next;
	return node;
}

struct Node* findNode(struct List* list, int n) {
	if ((n < 0) || (n > list->size -1)) {
		printf("Element doesn`t exist");
		return NULL;
	}
	struct Node* tmp = list->first;
	for(int i = 0; i != n; i++) {
		tmp = tmp->next;
	}
	return tmp;
}

void addToList(struct List* list, struct Book* book) {
	if (list->size == 0) {
		list->first = newNode(book, NULL);
		list->size++;
		return;
	}
	struct Node* tmp = findNode(list, list->size - 1);
	if (tmp != NULL) {
		tmp->next = newNode(book, NULL);
		list->size++;
	}
}

void freeNode(struct Node* node) {
	free(node->value);
	free(node);
} 

void freeList(struct List* list) {
	struct Node* current = list->first;
	for(int i = 0; i < list->size; i++) {
		struct Node* tmp = current;
		current = current->next;
		freeNode(tmp); 
	}
	free(list);
}

struct Book* newBook(char* title, float price, int numberOfPages, float weight, int year) {
	struct Book* book = malloc(sizeof(struct Book));
	strcpy(book->title, title);
	book->price = price;
	book->numberOfPages = numberOfPages;
	book->weight = weight;
	book->year = year;
	return book;
}

void printBook(struct Book* book) {
	printf("[%s | %f | %d | %f | %d]\n", book->title, book->price, book->numberOfPages, book->weight, book->year);
}

void printList(struct List* list) {
	struct Node* tmp = list->first;
	for(int i = 0; i < list->size; i++) {
		printBook(tmp->value);
		tmp = tmp->next;
	}
}

int main(void)
{
	struct List* books = newList();
	
	for(int i = 0; i < 7; i++) {
		char title[100];
		float price;
		int numberOfPages;
		float weight;
		int year;
		scanf("[%s | %f | %d | %f | %d]\n", title, &price, &numberOfPages, &weight, &year);
		addToList(books, newBook(title, price, numberOfPages, weight, year));
	}
	printList(books);
	freeList(books);
	return 0;
}
