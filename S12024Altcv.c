#define _CRT_SECURE_NO_WARNINGS
#define LSIZE 256
#define HSIZE 26
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>	

struct Reteta {
	unsigned int noReteta;
	char* namePacient;
	char* nameDoctor;
	char* status;
	unsigned char noMedicamente;
	float compensare;
}; typedef struct Reteta Reteta;

struct DLL {
	Reteta* data;
	struct DLL* next;
	struct DLL* prev;
}; typedef struct DLL Node;

struct HashTable {
	Node** buckets;
	int size;
}; typedef struct HashTable HT;

Reteta* createReteta(unsigned int noReteta,char* namePacient,char* nameDoctor,char* status,unsigned char noMedicamente, float compensare)
{
	Reteta* reteta = malloc(sizeof(Reteta));
	if (reteta)
	{
		reteta->noReteta = noReteta;
		reteta->namePacient = malloc(strlen(namePacient) + 1);
		if (reteta->namePacient) strcpy(reteta->namePacient, namePacient);

		reteta->nameDoctor = malloc(strlen(nameDoctor) + 1);
		if (reteta->nameDoctor) strcpy(reteta->nameDoctor, nameDoctor);

		reteta->status = malloc(strlen(status) + 1);
		if (reteta->status) strcpy(reteta->status, status);

		reteta->noMedicamente = noMedicamente;
		reteta->compensare = compensare;
	}
	return reteta;
}

Node* createNode(Reteta* r)
{
	Node* node = malloc(sizeof(Node));
	if (node)
	{
		node->data = r;
		node->next = node->prev = NULL;
	}
	return node;
}

void addNode(Node** list, Node* node, int position)
{
	if (*list == NULL || position == 0) {
		node->next = *list;
		if (*list) (*list)->prev = node;
		*list = node;
		return;
	}

	Node* current = *list;
	int currentPosition = 0;

	while (current->next != NULL && currentPosition < position - 1) {
		current = current->next;
		currentPosition++;
	}

	node->next = current->next;
	if (current->next) current->next->prev = node;
	node->prev = current;
	current->next = node;
}

void printReteta(Reteta* r) {
	printf("\nNo.Reteta: %d, Pacient: %s, Medic: %s, Status: %s, No.Medicamente: %d, Procent compensare: %.2f%",
		r->noReteta, r->namePacient, r->nameDoctor, r->status, r->noMedicamente, r->compensare);
}

void printList(Node* list)
{
	printf("\nASCENDING");
	while (list->next)
	{
		printReteta(list->data);
		list = list->next;
	}
	printReteta(list->data);
	printf("\nDESCENDING");
	while (list)
	{
		printReteta(list->data);
		list = list->prev;
	}
}

void freeReteta(Reteta* r)
{
	free(r->namePacient);
	free(r->nameDoctor);
	free(r->status);
}

void freeList(Node** list)
{
	while (*list)
	{
		Node* freeMe = *list;
		*list = (*list)->next;
		freeReteta(freeMe->data);
		free(freeMe);
	}
}

//ex2
int getNoReteteByMedicamente(Node* list, int threshold)
{
	int cnt = 0;
	while (list)
	{
		if (list->data->noMedicamente > threshold) cnt++;
		list = list->next;
	}
	return cnt;
}

//ex3
void updateCompensare(Node* list, const char* pacient, float newProcentCompensare)
{
	while (list)
	{
		if (strcmp(list->data->namePacient, pacient)==0)
		{
			list->data->compensare = newProcentCompensare;
		}
		list = list->next;
	}
}

//ex4
void deleteNodeByKey(Node** list, unsigned char threshold)
{
	Node* current = *list;
	while (current)
	{
		if (current->data->noMedicamente < threshold)
		{
			Node* tmp = current;

			if (current->prev)
				current->prev->next = current->next;
			else
				*list = current->next;  

			if (current->next)
				current->next->prev = current->prev;

			current = current->next;
			free(tmp->data);
			free(tmp);
		}
		else
			current = current->next;
	}
}

//ex5
int fHash(unsigned int key, int size)
{
	return key % size;
}

void addHT(HT* ht, Node* node)
{
	if (ht->buckets == NULL)
	{
		ht->size = HSIZE;
		ht->buckets = malloc(sizeof(Node*) * ht->size);
		if (ht->buckets) memset(ht->buckets, 0, sizeof(Node*) * ht->size);
	}
	int index = fHash(node->data->noReteta, ht->size);
	if (ht->buckets) addNode(&ht->buckets[index], node,0);
}

void printList2(Node* list)
{
	while (list)
	{
		printReteta(list->data);
		list = list->next;
	}
}

void printHT(HT* ht)
{
	for (int i = 0; i < ht->size; i++)
	{
		printf("\nBucket: %d", i);
		printList2(ht->buckets[i]);
	}
}

HT createHT(Node* list, const char* medic)
{
	HT ht = { NULL,0 };
	while (list)
	{
		if (strcmp(list->data->nameDoctor, medic) == 0)
		{
			Reteta* reteta = createReteta(list->data->noReteta, list->data->namePacient,
				list->data->nameDoctor,list->data->status, list->data->noMedicamente, list->data->compensare);
			Node* node = createNode(reteta);
			addHT(&ht, node);
		}
		list = list->next;
	}
	return ht;
}

void freeHT(HT* ht)
{
	for (int i = 0; i < ht->size; i++)
	{
		free(ht->buckets[i]);
	}
	ht->buckets = NULL;
	ht->size = 0;
}

int main()
{
	FILE* f = fopen("Text.txt", "r");

	char* tok = NULL;
	char del[] = ",\n";
	char buffer[LSIZE];
	char namePacient[LSIZE];
	char nameDoctor[LSIZE];
	char status[LSIZE];
	unsigned int noReteta;
	unsigned char noMedicamente;
	float compensare;

	Node* list = NULL;
	int position = 0;

	if (f)
	{
		while (fgets(buffer, LSIZE, f))
		{
			tok = strtok(buffer, del);
			noReteta = (unsigned int)atoi(tok);

			tok = strtok(NULL, del);
			strcpy(namePacient, tok);
			memmove(namePacient, namePacient + 1, strlen(namePacient) + 1);

			tok = strtok(NULL, del);
			strcpy(nameDoctor, tok);
			memmove(nameDoctor, nameDoctor + 1, strlen(nameDoctor) + 1);

			tok = strtok(NULL, del);
			strcpy(status, tok);
			memmove(status, status + 1, strlen(status) + 1);

			tok = strtok(NULL, del);
			noMedicamente = (unsigned char)atoi(tok);

			tok = strtok(NULL, del);
			compensare = (float)atof(tok);

			Reteta* ret = createReteta(noReteta, namePacient, nameDoctor, status, noMedicamente, compensare);
			Node* node = createNode(ret);
			addNode(&list, node,position);
		}
		printList(list);
		
		//ex2
		printf("\n\nEX.2");
		int countMedicamente = 4;
		int countRetete = getNoReteteByMedicamente(list, countMedicamente);
		printf("\nExista %d retete care au peste %d medicamente",countRetete,countMedicamente);

		//ex3
		printf("\n\nEX.3");
		updateCompensare(list, "Marian",10.2);
		printList(list);

		//ex4
		printf("\n\nEX.4");
		deleteNodeByKey(&list, 4);
		printList(list);

		//ex5
		printf("\n\nEX.5");
		HT ht = createHT(list, "Dr. George");
		printHT(&ht);


		freeHT(&ht);
		freeList(&list);
		fclose(f);
	}

}
