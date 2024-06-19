#define _CRT_SECURE_NO_WARNINGS
#define LINE_SIZE 256
#define HSIZE 26
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>


typedef struct Arrival {
	unsigned short trainNo;
	char* destination;
	char arrivalTime[6];
	unsigned short delay;
	unsigned short noPassengers;
}Arrival;

typedef struct Node {
	Arrival* info;
	struct Node* next;
}Node;

typedef struct HashTable {
	int length;
	Node** vector;
}HT;

Arrival* createArrival(unsigned short no, char* dest, char arrivalTime[6], unsigned short delay, unsigned short noPass) {
	Arrival* a = (Arrival*)malloc(sizeof(Arrival));
	if (a) {
		a->trainNo = no;
		a->destination = (char*)malloc(strlen(dest) + 1);
		strcpy(a->destination, dest);
		strcpy(a->arrivalTime, arrivalTime);
		a->delay = delay;
		a->noPassengers = noPass;
	}
	return a;

}

void printArrival(Arrival* a) {
	printf("\n#%hu: Destination: %s, ArrivalTime: %s, Delay: %hu, Passangers: %hu", a->trainNo, a->destination, a->arrivalTime, a->delay, a->noPassengers);
}

Node* createNode(Arrival* a) {
	Node* n = (Node*)malloc(sizeof(Node));
	n->info = a;
	n->next = NULL;
	return n;
}

void printList(Node* list) {
	while (list) {
		printArrival(list->info);
		list = list->next;
	}
}

int fhash(Arrival* a) {
	int hash = 0;
	char* dest = a->destination;
	while (*dest) {
		hash += *dest++;
	}
	return hash % HSIZE;
}

void addNode(Node** list, Arrival* a) {
	Node* n = createNode(a);
	if (*list == NULL)
		*list = n;
	else {
		n->next = *list;
		*list = n;
	}
}

void addHT(HT** ht, Arrival* a) {
	//initialize the hash table on the first run
	if (*ht == NULL) {
		*ht = (HT*)malloc(sizeof(HT));
		if (*ht) {
			(*ht)->length = HSIZE;
			(*ht)->vector = (Node**)malloc((*ht)->length * sizeof(Node*));
			if ((*ht)->vector)
				memset((*ht)->vector, 0, (*ht)->length * sizeof(Node*));
		}
	}

	//adding to the table
	if (*ht)
	{
		int index = fhash(a);
		if ((*ht)->vector)
			addNode(&(*ht)->vector[index], a);
	}
}

void printHT(HT* ht) {
	for (int i = 0; i < ht->length; i++) {
		printf("\nBucket %d: ", i);
		printList(ht->vector[i]);
	}
}

int nbDelays(HT* ht) {
	int noDelays = 0;
	for (int i = 0; i < HSIZE; i++) {
		Node* current = ht->vector[i];
			while (current) {
				if (current->info->delay != 0) {
					noDelays++;
				}
				current = current->next;
			}
	}
	return noDelays;
}

void changeDestination(HT* ht, unsigned short trainno, char* newDest) {
	for (int i = 0; i < HSIZE; i++) {
		Node* current = ht->vector[i];
		while (current) {
			if (current->info->trainNo == trainno) {
				free(current->info->destination);
				current->info->destination = (char*)malloc(strlen(newDest) + 1);
				strcpy(current->info->destination, newDest);
				return;
			}
			current = current->next;
		}
	}
}


int main() {

	HT* ht = NULL;

	char buffer[LINE_SIZE];
	char* tok;
	char del[] = " ,\n";

	unsigned short no;
	char dest[100];
	char arrivalTime[6];
	unsigned short delay;
	unsigned short noPass;

	FILE* fp = fopen("Data.txt", "r");
	if (fp == NULL) return -1;

	while (fgets(buffer, LINE_SIZE, fp)) {
		tok = strtok(buffer, del);
		no = (unsigned short)atoi(tok);
		tok = strtok(NULL, del);
		strcpy(dest, tok);
		tok = strtok(NULL, del);
		strcpy(arrivalTime, tok);
		tok = strtok(NULL, del);
		delay = (unsigned short)atoi(tok);
		tok = strtok(NULL, del);
		noPass = (unsigned short)atoi(tok);
		Arrival* a = createArrival(no, dest, arrivalTime, delay, noPass);
		addHT(&ht, a);
	}
	//printHT(ht);
	printf("No delays = %d", nbDelays(ht));
	printf("\n-----------------------------");
	char newDest[] = "Constanta";
	changeDestination(ht, 1234, newDest);
	printHT(ht);
}
