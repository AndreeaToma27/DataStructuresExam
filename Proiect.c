#define _CRT_SECURE_NO_WARNINGS
#define LSIZE 256
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

typedef struct Proiect {
	unsigned int id;
	char* titlu;
	char* beneficiar;
	unsigned char nrExec;
	float buget;
}Proiect;

typedef struct Node {
	Proiect* info;
	struct Node* next;
	struct Node* prev;
}Node;

Proiect* creareProiect(unsigned int id, char* titlu, char* beneficiar, unsigned char nrExec, float buget) {
	Proiect* p = (Proiect*)malloc(sizeof(Proiect));
	p->id = id;
	p->titlu = (char*)malloc(strlen(titlu) + 1);
	strcpy(p->titlu, titlu);
	p->beneficiar = (char*)malloc(strlen(beneficiar) + 1);
	strcpy(p->beneficiar, beneficiar);
	p->nrExec = nrExec;
	p->buget = buget;
	return p;
}

void printProiect(Proiect* p) {
	printf("\n%d, %s, %s, %u, %.2f", p->id, p->titlu, p->beneficiar, p->nrExec, p->buget);
}

Node* createNode(Proiect* p) {
	Node* n = (Node*)malloc(sizeof(Node));
	n->info = p;
	n->next = NULL;
	n->prev = NULL;
	
	return n;
}

void addNode(Node** list, Node* node) {
	if (*list == NULL) {
		*list = node;
	}
	else {
		node->next = *list;
		(*list)->prev = node;
		*list = node;
	}
}

void printList(Node* list) {
	while (list->next != NULL) {
		printProiect(list->info);
		list = list->next;
	}
	printProiect(list->info);
}

int nrExecOver(Node* list, int prag) {
	int counter = 0;
	while (list->next != NULL) {
		if (list->info->nrExec > prag) {
			counter++;
		}
		list = list->next;
	}
	if (list->info->nrExec > prag) {
		counter++;
	}
	return counter;
}

void updateBuget(Node* list, char* ben, float newB) {
	while (list->next != NULL) {
		if (strcmp(list->info->beneficiar, ben) == 0) {
			list->info->buget = newB;
			break;
		}
		list = list->next;
	}
	if (strcmp(list->info->beneficiar, ben) == 0) {
		list->info->buget = newB;
	}
}

int main() {

	Node* list = NULL;
	char* tok;
	char buffer[LSIZE];
	char del[] = ",\n";

	unsigned int id;
	char titlu[LSIZE];
	char beneficiar[LSIZE];
	unsigned char nrExec;
	float buget;



	FILE* fp = fopen("Data.txt", "r");
	if (fp == NULL) return -1;
	while (fgets(buffer, LSIZE, fp)) {
		tok = strtok(buffer, del);
		id = (unsigned int)atoi(tok);

		tok = strtok(NULL, del);
		strcpy(titlu, tok);

		tok = strtok(NULL, del);
		strcpy(beneficiar, tok);

		tok = strtok(NULL, del);
		nrExec = (unsigned char)atoi(tok);

		tok = strtok(NULL, del);
		buget = atof(tok);

		Proiect* p = creareProiect(id, titlu, beneficiar, nrExec, buget);
		Node* n = createNode(p);
		addNode(&list, n);
		
	}
	int no = nrExecOver(list, 5);
	printf("Nr: %d", no);

	printList(list);
	printf("-----------------------\n");
	char newben[] = "Pop Ion";
	updateBuget(list, newben, 100.00);
	printList(list);
	return 0;
}#define _CRT_SECURE_NO_WARNINGS
#define LSIZE 256
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

typedef struct Proiect {
	unsigned int id;
	char* titlu;
	char* beneficiar;
	unsigned char nrExec;
	float buget;
}Proiect;

typedef struct Node {
	Proiect* info;
	struct Node* next;
	struct Node* prev;
}Node;

Proiect* creareProiect(unsigned int id, char* titlu, char* beneficiar, unsigned char nrExec, float buget) {
	Proiect* p = (Proiect*)malloc(sizeof(Proiect));
	p->id = id;
	p->titlu = (char*)malloc(strlen(titlu) + 1);
	strcpy(p->titlu, titlu);
	p->beneficiar = (char*)malloc(strlen(beneficiar) + 1);
	strcpy(p->beneficiar, beneficiar);
	p->nrExec = nrExec;
	p->buget = buget;
	return p;
}

void printProiect(Proiect* p) {
	printf("\n%d, %s, %s, %u, %.2f", p->id, p->titlu, p->beneficiar, p->nrExec, p->buget);
}

Node* createNode(Proiect* p) {
	Node* n = (Node*)malloc(sizeof(Node));
	n->info = p;
	n->next = NULL;
	n->prev = NULL;
	
	return n;
}

void addNode(Node** list, Node* node) {
	if (*list == NULL) {
		*list = node;
	}
	else {
		node->next = *list;
		(*list)->prev = node;
		*list = node;
	}
}

void printList(Node* list) {
	while (list->next != NULL) {
		printProiect(list->info);
		list = list->next;
	}
	printProiect(list->info);
}

int nrExecOver(Node* list, int prag) {
	int counter = 0;
	while (list->next != NULL) {
		if (list->info->nrExec > prag) {
			counter++;
		}
		list = list->next;
	}
	if (list->info->nrExec > prag) {
		counter++;
	}
	return counter;
}

void updateBuget(Node* list, char* ben, float newB) {
	while (list->next != NULL) {
		if (strcmp(list->info->beneficiar, ben) == 0) {
			list->info->buget = newB;
			break;
		}
		list = list->next;
	}
	if (strcmp(list->info->beneficiar, ben) == 0) {
		list->info->buget = newB;
	}
}

int main() {

	Node* list = NULL;
	char* tok;
	char buffer[LSIZE];
	char del[] = ",\n";

	unsigned int id;
	char titlu[LSIZE];
	char beneficiar[LSIZE];
	unsigned char nrExec;
	float buget;



	FILE* fp = fopen("Data.txt", "r");
	if (fp == NULL) return -1;
	while (fgets(buffer, LSIZE, fp)) {
		tok = strtok(buffer, del);
		id = (unsigned int)atoi(tok);

		tok = strtok(NULL, del);
		strcpy(titlu, tok);

		tok = strtok(NULL, del);
		strcpy(beneficiar, tok);

		tok = strtok(NULL, del);
		nrExec = (unsigned char)atoi(tok);

		tok = strtok(NULL, del);
		buget = atof(tok);

		Proiect* p = creareProiect(id, titlu, beneficiar, nrExec, buget);
		Node* n = createNode(p);
		addNode(&list, n);
		
	}
	int no = nrExecOver(list, 5);
	printf("Nr: %d", no);

	printList(list);
	printf("-----------------------\n");
	char newben[] = "Pop Ion";
	updateBuget(list, newben, 100.00);
	printList(list);
	return 0;
}
