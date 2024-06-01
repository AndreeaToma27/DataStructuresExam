#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#define LINE_SIZE 1000

typedef struct Student {
	char* name;
	float income;
	int reference;
}Student;

typedef struct Node {
	Student* info;
	struct Node* next;
	struct Node* prev;
}Node;



Student* createStudent(char*, float, int);
void printStudent(Student*);
Node* createNode(Student*);
void addToList(Node**, Node*);
void printStudents(Node*);
void deleteStudent(Student* stud);
void deleteNode(Node* node);
void displayListBothWays(Node* head);
Node* insertTail(Node* list, Student* stud);


int main() {
	FILE* fp = fopen("Data.txt", "r");
	if (fp == NULL) return -1;

		Node* lista = NULL;
		char del[] = ",\n";
		char* tok = NULL;
		char buffer[LINE_SIZE];

		char nume[LINE_SIZE];
		float income;
		int reference;


		while (fgets(buffer, sizeof(buffer), fp)) 
		{
			tok = strtok(buffer, del); 
			strcpy(nume, tok);
		
			tok = strtok(NULL, del); 
			income = atof(tok); 

			tok = strtok(NULL, del);
			reference = atoi(tok);

			Student* stud = createStudent(nume, income, reference);
			lista = insertTail(lista, stud);
		}
		displayListBothWays(lista);
		return 0;
}

void printStudents(Node* list) {
	while (list) {
		printStudent(list->info);
		list = list->next;
	}
}

void addToList(Node** list, Node* node) {
	if (*list == NULL) {
		*list = node;
	}
	else {
		node->next = *list;
		*list = node;
	}
}

Node* createNode(Student* stud) {
	Node* node = NULL;
	node = (Node*)malloc(sizeof(Node));
	node->info = stud;
	node->next = NULL;
	node->prev = NULL;
	return node;
}

void printStudent(Student* stud) {
	printf("#%5d - Nume: %s, Income : %.2f\n", stud->reference, stud->name, stud->income);
}

Student* createStudent(char* name, float income, int reference) {
	Student* stud = NULL;
	stud = (Student*)malloc(sizeof(Student));
	stud->income = income;
	stud->reference = reference;
	stud->name = (char*)malloc(strlen(name) + 1);
	strcpy(stud->name, name);
	return stud;
}

void deleteStudent(Student* stud)
{
	if (stud != NULL)
	{
		free(stud->name);
		free(stud);
	}
}

void deleteNode(Node* node) {
	if (node != NULL) {
		deleteStudent(node->info);
		free(node);
	}
}

Node* insertTail(Node* list, Student* stud) {
	Node* node = createNode(stud);
	if (list == NULL) {
		return node;
	}
	else {
		Node* tmp = list; //parcurg lista
		while (tmp->next) {
			tmp = tmp->next;
		}
		tmp->next = node;
		node->prev = tmp;
		return list;
	}
}

void displayListBothWays(Node* head)
{
	if (head)
	{
		while (head->next)
		{
			printStudent(head->info);
			head = head->next;
		}
		printStudent(head->info);
		printf("----------Reversed order----------\n");
		while (head)
		{
			printStudent(head->info);
			head = head->prev;
		}
	}
}
