#define _CRT_SECURE_NO_WARNINGS
#define LINE_SIZE 256
#define HASHT_SIZE 26
#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>

typedef struct Task {
	unsigned int taskId;
	char* taskDate;
	char* ownerName;
	float taskCompletion;
}Task;

typedef struct HashNode {
	Task* info;
	struct HashNode* next;
}Node;

typedef struct HashTable {
	Node** buckets;
	int size;
}HT;


Task* createTask(unsigned int taskId, char* taskDate, char* owner, float completion);
void printTask(Task* task);

Node* createNode(Task* task);
void freeTask(Task** t);
void addHT(HT** ht, Task* t);
int fhash(const char key, int size);
void addNode(Node** list, Task* t);
printList(Node* list);
void freeList(Node** list);
void printHT(HT* ht);
void freeHT(HT** ht);
int noTasks(HT* ht, const char* key);
int countNoTasks(HT* ht, float level);

int main() {
	char del[] = ",\n";
	char buffer[LINE_SIZE];
	char* tok = NULL;

	unsigned int taskId;
	char* taskDate;
	char* ownerName;
	float taskCompletion;
	HT* ht = NULL;

	FILE* fp = fopen("Data.txt", "r");
	if (fp) {
		while (fgets(buffer, LINE_SIZE, fp)) {
			tok = strtok(buffer, del);
			taskId = atoi(tok);

			tok = strtok(NULL, del);
			taskDate = (char*)malloc(strlen(tok) + 1);
			strcpy(taskDate, tok);
			memmove(taskDate, taskDate + 1, strlen(taskDate) + 1);

			tok = strtok(NULL, del);
			ownerName = (char*)malloc(strlen(tok));
			strcpy(ownerName, tok);
			memmove(ownerName, ownerName + 1, strlen(ownerName) + 1);

			tok = strtok(NULL, del);
			taskCompletion = (float)atof(tok);

			Task* task = createTask(taskId, taskDate, ownerName, taskCompletion);
			addHT(&ht, task);
		}
		//printHT(ht);

		char name[] = "Toma Andreea";
		int noTasksTotal = noTasks(ht, name);
		//printf("\n\nTotal tasks found for %s is: %d", name, noTasksTotal);

		float level = 0.5;
		int tasksAbove = countNoTasks(ht, level);
		printf("Tasks above %.1f: %d", level, tasksAbove);

	}
	freeHT(&ht);
	fclose(fp);
	return 0;
}

int countNoTasks(HT* ht, float level) {
	int count = 0;
	for (int i = 0; i < ht->size; i++)
	{
		Node* bucket = ht->buckets[i];
		while (bucket)
		{
			if (bucket->info->taskCompletion > level)
				count++;
			bucket = bucket->next;
		}
	}
	return count;
}

int noTasks(HT* ht, const char* key) {
	int count = 0;
	int index = fhash(key[0], ht->size);
	Node* bucket = ht->buckets[index];
	while (bucket) {
		if (strcmp(bucket->info->ownerName, key) == 0)
			count++;
		bucket = bucket->next;
	}
	return count;
}

Node* createNode(Task* task) {
	Node* hNode = (Node*)malloc(sizeof(Node));
	if (hNode) {
		hNode->info = task;
		hNode->next = NULL;
	}
	return hNode;
}
void printTask(Task* task) {
	printf("#%i : %s, %s, %.1f\n", task->taskId, task->taskDate, task->ownerName, task->taskCompletion);
}

Task* createTask(unsigned int taskId, char* taskDate, char* owner, float completion) {
	Task* t = (Task*)malloc(sizeof(Task));
	if (t) {
		t->taskId = taskId;
		t->taskCompletion = completion;
		t->taskDate = (char*)malloc(strlen(taskDate) + 1);
		strcpy(t->taskDate, taskDate);
		t->ownerName = (char*)malloc(strlen(owner) + 1);
		strcpy(t->ownerName, owner);
	}
	return t;
}

void freeTask(Task** t) {
	free((*t)->taskDate);
	free((*t)->ownerName);
	*t = NULL;
}

void addHT(HT** ht, Task* t) {
	if (*ht == NULL) {
		*ht = (HT*)malloc(sizeof(HT));
		(*ht)->size = HASHT_SIZE;
		(*ht)->buckets = (Node**)malloc((*ht)->size * sizeof(Node));
		memset((*ht)->buckets, 0, (*ht)->size * sizeof(Node));
	}

	if (*ht) {
		int index = fhash(t->ownerName[0], (*ht)->size);
		if ((*ht)->buckets)
			addNode(&(*ht)->buckets[index], t);
	}
}

int fhash(const char key, int size) {
	return key % size;
}

void addNode(Node** list, Task* t) {
	Node* n = createNode(t);
	if (*list == NULL) {
		*list = n;
	}

	else
	{
		n->next = *list;
		*list = n;
	}
}

void freeList(Node** list) {
	while (*list)
	{
		Node* freeMe = *list;
		*list = (*list)->next;
		freeTask(&freeMe->info);
		free(freeMe);
	}
	*list = NULL;
}

printList(Node* list) {
	while (list) {
		printTask(list->info);
		list = list->next;
	}
}

//ex2
void printHT(HT* ht) {
	printf("\nStart");
	for (int i = 0; i < ht->size; i++) {
		printf("\nBucket %d: ", i);
		printList(ht->buckets[i]);
	}
}

void freeHT(HT** ht) {
	for (int i = 0; i < (*ht)->size; i++) {
		freeList(&(*ht)->buckets[i]);
	}
	free((*ht)->buckets);
	free(*ht);
	*ht = NULL;
}
