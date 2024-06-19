#define _CRT_SECURE_NO_WARNINGS
#define LSIZE 256
#define HSIZE 26
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

typedef struct Task {
	unsigned int id;
	char* date;
	char* owner;
	float completion;
}Task;

typedef struct Node {
	Task* info;
	struct Node* next;
}Node;

typedef struct HashTable {
	int length;
	Node** buckets;
}HT;



Task* createTask(unsigned int id, char* date, char* owner, float completion) {
	Task* task = (Task*)malloc(sizeof(Task));
	task->id = id;
	task->date = (char*)malloc(strlen(date) + 1);
	strcpy(task->date, date);
	task->owner = (char*)malloc(strlen(owner) + 1);
	strcpy(task->owner, owner);
	task->completion = completion;
	return task;
}

void printTask(Task* t) {
	printf("\n#%d: Date: %s, Owner: %s, Completion: %.2f", t->id, t->date, t->owner, t->completion);
}

Node* createNode(Task* t) {
	Node* node = (Node*)malloc(sizeof(Node));
	node->info = t;
	node->next = NULL;
}

void addToList(Node** list, Task* t) {
	Node* n = createNode(t);
	if ((*list) == NULL) {
		*list = n;
	}
	else {
		n->next = *list;
		*list = n;
	}

}

void printList(Node* list) {
	while (list) {
		printTask(list->info);
		list = list->next;
	}
}

int hashF(Task* t) {
	return strlen(t->owner) % HSIZE;
}

void addHT(HT** ht, Task* t) {
	if (*ht == NULL) {
		*ht = (HT*)malloc(sizeof(HT));
		if (*ht) {
			(*ht)->length = HSIZE;
			(*ht)->buckets = (Node**)malloc(sizeof(Node*) * HSIZE);
			if ((*ht)->buckets) {
				memset((*ht)->buckets, 0, sizeof(Node*) * HSIZE);
			}
		}
	}
	if (*ht) {
		int index = hashF(t);
		addToList(&(*ht)->buckets[index], t);
	}
}

void printHT(HT* ht) {
	for (int i = 0; i < HSIZE; i++) {
		printList(ht->buckets[i]);
	}
}

int noTasks(HT* ht, char* name) {
	int counter = 0;
	for (int i = 0; i < HSIZE; i++) {
		Node* current = ht->buckets[i];
		while (current) {
			if (strcmp(current->info->owner, name) == 0) {
				counter++;
			}
			current = current->next;
		}
	}
	return counter;
}

int noTasksAbove(HT* ht, float perc) {
	int counter = 0;
	for (int i = 0; i < HSIZE; i++) {
		Node* current = ht->buckets[i];
		while (current) {
			if (current->info->completion > perc) {
				counter++;
			}
			current = current->next;
		}
	}
	return counter;
}

int main() {
	char* tok;
	char buffer[LSIZE];
	char del[] = ",\n";
	HT* ht = NULL;

	unsigned int id;
	char date[LSIZE];
	char owner[LSIZE];
	float completion;

	FILE* fp = fopen("Data.txt", "r");
	while (fgets(buffer, LSIZE, fp)) {
		tok = strtok(buffer, del);
		id = (unsigned int)atoi(tok);
		tok = strtok(NULL, del);
		strcpy(date, tok);
		tok = strtok(NULL, del);
		strcpy(owner, tok);
		tok = strtok(NULL, del);
		completion = atof(tok);

		Task* t = createTask(id, date, owner, completion);
		addHT(&ht, t);
	}
	char name[] = "Charlie Green";
	printf("No tasks: %d", noTasks(ht, name));
	printf("No tasks compl level: %d", noTasksAbove(ht, 0.40));
	//printHT(ht);
	return 0;
}
