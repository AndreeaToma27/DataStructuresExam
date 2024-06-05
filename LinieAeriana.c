#define _CRT_SECURE_NO_WARNINGS
#define LINE_SIZE 256
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>

typedef struct Flight {
	unsigned int id;
	char* destination;
	char* date;
	unsigned short int delay;
	int passengers;
}Flight;

typedef struct Node {
	Flight* info;
	struct Node* next;
	struct Node* prev;
}Node;

Flight* createFlight(unsigned int id, char* destination, char* date, unsigned short int delay, int passengers);
void printFlight(Flight* f);
Node* createNode(Flight* f);
void addToList(Node** list, Node* node);
void printList(Node* list);
int totalIntarziere(Node* list, char* destinatie);

int main() {
	char del[] = ",\n";
	char buffer[LINE_SIZE];
	char* tok = NULL;

	unsigned int id;
	char* destination;
	char* date;
	unsigned short int delay;
	int passengers;

	Node* list = NULL;

	FILE* fp = fopen("Data.txt", "r");
	if (fp == NULL) return -1;
	while (fgets(buffer, LINE_SIZE, fp)) {
		tok = strtok(buffer, del);
		id = atoi(tok);

		tok = strtok(NULL, del);
		destination = (char*)malloc(strlen(tok) + 1);
		strcpy(destination, tok);

		tok = strtok(NULL, del);
		date = (char*)malloc(strlen(tok) + 1);
		strcpy(date, tok);

		tok = strtok(NULL, del);
		delay = atoi(tok);

		tok = strtok(NULL, del);
		passengers = atoi(tok);

		Flight* f = createFlight(id, destination, date, delay, passengers);
		Node* n = createNode(f);
		addToList(&list, n);
	}
	//2
	//printList(list);

	//3
	char dest[] = "Malaga";
	int total = totalIntarziere(list, dest);
	printf("%d", total);
	
	//4
	float media = medieCalatori(list, dest);
	printf("\nTotal: %.2f", media);
	return 0;
}

float medieCalatori(Node* list, char* destinatie) {
	int count = 0;
	int total = 0;
	float media = 0;
	while (list->next != NULL) {
		if (strcmp(list->info->destination, destinatie) == 0) {
			total += list->info->passengers;
			count++;
		}
		list = list->next;
	}
	if (strcmp(list->info->destination, destinatie) == 0) {
		total += list->info->passengers;
		count++;
	}

	
	return (total/count);
}

int totalIntarziere(Node* list, char* destinatie) {
	int total = 0;
	while (list->next != NULL) {
		if (strcmp(list->info->destination, destinatie) == 0) {
			total += list->info->delay;
		}
		list = list->next;
	}
	if (strcmp(list->info->destination, destinatie) == 0) {
		total += list->info->delay;
	}
	return total;
}

Flight* createFlight(unsigned int id, char* destination, char* date, unsigned short int delay, int passengers) {
	Flight* f = (Flight*)malloc(sizeof(Flight));

	f->id = id;
	f->destination = (char*)malloc(strlen(destination) + 1);
	strcpy(f->destination, destination);
	f->date = (char*)malloc(strlen(date) + 1);
	strcpy(f->date, date);
	f->delay = delay;
	f->passengers = passengers;

	return f;
}

void printFlight(Flight* f) {
	printf("\nFlight %i: Destinantion: %s, Date: %s, Delay: %i, No of passengers: %d",
		f->id, f->destination, f->date, f->delay, f->passengers);
}

Node* createNode(Flight* f) {
	Node* node = (Node*)malloc(sizeof(Node));
	if (node) {
		node->info = f;
		node->next = NULL;
		node->prev = NULL;
	}

}

void addToList(Node** list, Node* node) {
	if (*list == NULL) {
		*list = node;
	}
	else {
		Node* tmp = *list;
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = node;
		node->prev = tmp;
		return *list;
	}
}

void printList(Node* list) {
	while (list->next != NULL) {
		printFlight(list->info);
		list = list->next;
	}
	printFlight(list->info);
	
}
