#define _CRT_SECURE_NO_WARNINGS
#define LSIZE 256
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

typedef struct Factura {
	unsigned int no;
	char* date;
	char* beneficiary;
	float amount;
	unsigned int noDays;
}Factura;

typedef struct Node {
	Factura* info;
	struct Node* next;
	struct Node* prev;
}Node;

Factura* createFactura(unsigned int no, char* date, char* ben, float amount, unsigned int noDays) {
	Factura* f = NULL;
	f = (Factura*)malloc(sizeof(Factura));
	f->no = no;
	f->amount = amount;
	f->noDays = noDays;
	f->date = (char*)malloc(strlen(date) + 1);
	strcpy(f->date, date);
	f->beneficiary = (char*)malloc(strlen(ben) + 1);
	strcpy(f->beneficiary, ben);
	return f;
}

void printFactura(Factura* f) {
	printf("\n%i, %s, %s, %.2f, %i", f->no, f->date, f->beneficiary, f->amount, f->noDays);
}

Node* createNode(Factura* f) {
	Node* n = (Node*)malloc(sizeof(Node));
	n->info = f;
	n->next = NULL;
	n->prev = NULL;
	return n;
}

void addToList(Node** list, Node* node) {
	if (*list == NULL) {
		*list = node;
	}
	else {
		node->next = *list;
		(*list)->prev = node;
		*list = node;
	}
}

void printListBothWays(Node* list) {
	while (list->next != NULL) {
		printFactura(list->info);
		list = list->next;
	}
	printFactura(list->info);

	printf("\n--------List backwards");
	while (list != NULL) {
		printFactura(list->info);
		list = list->prev;
	}
}

float totalFacturi(Node* list, const char* beneficiary) {
	float total = 0;
	while (list->next != NULL) {
		if (strcmp(list->info->beneficiary, beneficiary) == 0) {
			total += list->info->amount;
		}
		list = list->next;
	}
	if (strcmp(list->info->beneficiary, beneficiary) == 0) {
		total += list->info->amount;
	}
	return total;
}

void modifySum(Node* list, unsigned int no, float newSum) {
	while (list) {
		if (list->info->no == no) {
			list->info->amount = newSum;
			break;
		}
		else {
			list = list->next;
		}
	}
}

void deleteFactura(Node* list, unsigned int days) {
	while (list) {
		if (list->info->noDays < days) {
			if (list->prev == NULL) {
				Node* tmp = list->next;
				free(list->info);
				list = tmp;
			}
			else {
				Node* tmp = list;
				list->prev->next = list->next;
				list->next->prev = list->prev;
				free(list->info);
			}
		}
		list = list->next;
	}
}


int main() {
	FILE* fp = fopen("Data.txt", "r");
	if (fp == NULL) return -1;
	char buffer[LSIZE];
	char* tok;
	char del[] = ",\n";

	unsigned int no;
	char date[LSIZE];
	char ben[LSIZE];
	float amount;
	unsigned int noDays;

	Node* list = NULL;

	while (fgets(buffer, LSIZE, fp) != NULL) {
		tok = strtok(buffer, del);
		no = atoi(tok);

		tok = strtok(NULL, del);
		strcpy(date, tok);
		memmove(date, date + 1, strlen(date) + 1);

		tok = strtok(NULL, del);
		strcpy(ben, tok);
		memmove(ben, ben + 1, strlen(ben) + 1);

		tok = strtok(NULL, del);
		amount = atof(tok);

		tok = strtok(NULL, del);
		noDays = atoi(tok);

		Factura* factura = createFactura(no, date, ben, amount, noDays);
		Node* node = createNode(factura);
		addToList(&list, node);

	}

	//printListBothWays(list);
	char name[] = "Toma Andreea";
	float totalA = totalFacturi(list, name);
	//printf("--------%.2f", totalA);

	//modifySum(list, 6657, 45.54);
	printListBothWays(list);
	printf(" \n");
	deleteFactura(list, 11);
	printListBothWays(list);
	return 0;
}

