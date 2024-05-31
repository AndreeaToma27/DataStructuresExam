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

Student* createStudent(char*, float, int);
void printStudent(Student*);

int main() {
	FILE* fp = fopen("Data.txt", "r");
	if (fp == NULL) return -1;

	char* nume;
	float income;
	int reference;

	char buffer[LINE_SIZE];
	char del[] = ",\n"; //verifica daca delimitatorul e ori , ori \n
	char* tok = NULL;

	while (fgets(buffer, sizeof(buffer), fp)) {
		tok = strtok(buffer, del); //linia asta merge in buffer, gaseste primul delimitator si pune un /0
		//tok e un pointer care are adresa primului char* din buffer
		nume = (char*)malloc(strlen(tok) + 1);
		strcpy(nume, tok);
		
		tok = strtok(NULL, del); //daca ii dau null ca parametru, continua exact de unde a ramas
		income = atof(tok); //atof -> ASCII to float 

		tok = strtok(NULL, del);
		reference = atoi(tok); //atoi -> ascii to integer

		Student* stud = createStudent(nume, income, reference);
		printStudent(stud);
	}

	return 0;
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
