#define _CRT_SECURE_NO_WARNINGS
#define LSIZE 256
#define HSIZE 26
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>

typedef struct Order {
	unsigned int id;
	char* datePlaced;
	char* dateDelivery;
	char* name;
	float price;

}Order;

typedef struct Node {
	Order* info;
	struct Node* next;
}Node;

typedef struct HashTable {
	Node** buckets;
	int dimension;
}HT;

Order* createOrder(unsigned int id, char* datePlaced, char* dateDelivery, char* name, float price) {
	Order* o = (Order*)malloc(sizeof(Order));
	if (o) {
		o->id = id;
		o->datePlaced = (char*)malloc(strlen(datePlaced) + 1);
		strcpy(o->datePlaced, datePlaced);
		o->dateDelivery = (char*)malloc(strlen(dateDelivery) + 1);
		strcpy(o->dateDelivery, dateDelivery);
		o->name = (char*)malloc(strlen(name) + 1);
		strcpy(o->name, name);
		o->price = price;
	}
	return o;

}

void printOrder(Order* o) {
	printf("\n#%x, Date placed: %s, DeliveryDate: %s, Name: %s, Price: %.2f", o->id, o->datePlaced, o->dateDelivery, o->name, o->price);
}

Node* createNode(Order* o) {
	Node* n = (Node*)malloc(sizeof(Node));
		n->info = o;
		n->next = NULL;
	return n;
}

void addToList(Node** list, Order* o) {
	Node* node = createNode(o);
	if (*list == NULL) {
		*list = node;
	}
	else {
		node->next = *list;
		(*list) = node;
	}
}

void printList(Node* list) {
	while (list) {
		printOrder(list->info);
		list = list->next;
	}
}

int fHash(Order* o) {
	return strlen(o->name) % HSIZE;
}

void addHT(HT** ht, Order* o) {
	if (*ht == NULL) {
		*ht = (HT*)malloc(sizeof(HT));
		if (*ht) {
			(*ht)->dimension = HSIZE;
			(*ht)->buckets = (Node**)malloc((*ht)->dimension * sizeof(Node*));
			if ((*ht)->buckets)
				memset((*ht)->buckets, 0, (*ht)->dimension * sizeof(Node*));
		}
	}
	if (*ht) {
		int index = fHash(o);
		if ((*ht)->buckets) {
			addToList(&(*ht)->buckets[index], o);
		}
	}
}

void printHT(HT* ht) {
	for (int i = 0; i < HSIZE; i++) {
		printf("\nBucket %d", i);
		printList(ht->buckets[i]);
	}
}

int noOrdersByASpecificCLient(HT* ht, char* name) {
	int counter = 0;
	for (int i = 0; i < HSIZE; i++) {
		Node* current = ht->buckets[i];
		while (current) {
			if (strcmp(current->info->name, name) == 0) {
				counter++;
			}
			current = current->next;
		}
	}
	return counter;
}

void modifyDate(HT* ht, unsigned int id, char* newDate) {
	for (int i = 0; i < HSIZE; i++) {
		Node* current = ht->buckets[i];
		while (current) {
			if (current->info->id == id) {
				free(current->info->dateDelivery);
				current->info->dateDelivery = (char*)malloc(strlen(newDate) + 1);
				strcpy(current->info->dateDelivery, newDate);
				return;
			}
			current = current->next;
		}
	}
}

int main() {
	HT* ht = NULL;
	char del[] = ",\n";
	char* tok;
	char buffer[LSIZE];

	unsigned int id;
	char datePlaced[11];
	char dateDelivery[11];
	char name[LSIZE];
	float price;

	FILE* fp = fopen("Data.txt", "r");
	if (fp == NULL) return -1;
	while (fgets(buffer, LSIZE, fp)) {
		tok = strtok(buffer, del);
		id = (unsigned int)atoi(tok);
		tok = strtok(NULL, del);
		strcpy(datePlaced, tok);
		tok = strtok(NULL, del);
		strcpy(dateDelivery, tok);
		tok = strtok(NULL, del);
		strcpy(name, tok);
		tok = strtok(NULL, del);
		price = atof(tok);

		Order* o = createOrder(id, datePlaced, dateDelivery, name, price);
		addHT(&ht, o);
	}
	//printHT(ht);
	char name2[] = "Charlie Green";
	//printf("No orders: %d", noOrdersByASpecificCLient(ht, name2));
	char newDate[] = "0000-00-00";

	modifyDate(ht, 1, newDate);
	printHT(ht);
	return 0;
	
}
