#define _CRT_SECURE_NO_WARNINGS
#define LINE_SIZE 256
#define HASHT_SIZE 26
#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>

typedef struct Student
{
	char* name;
	float income;
	int reference;
}Student;

typedef struct HashNode
{
	Student* info;
	struct HashNode* next;
}HashNode, *PHashNode;

typedef HashNode** HashTable; // this allows for an array of linked lists and not only one
HashNode* createHashNode(Student* info);

int hashFunction(const char* key);
HashNode* createHashNode(Student* info);
void deleteFromHashTable(HashNode** hashTable, const char* key);
Student* getFromHashTable(HashNode** hashTable, const char* key);
void putHT(HashNode*** hashTable, Student* stud);
Student* createStudent(const char* name, float income, int ref);
void deleteStudent(Student* stud);
void printStudent(Student* stud);


int main() {
	FILE* fp = fopen("Data.txt", "r");

	char* nume;
	float income;
	int reference;

	char del[] = ",\n";
	char buffer[LINE_SIZE];
	char* tok = NULL;

	HashTable hashTable = NULL;

	if (fp) {
		while (fgets(buffer, LINE_SIZE, fp)) {
			tok = strtok(buffer, del);
			nume = (char*)malloc(sizeof(tok) + 1);
			strcpy(nume, tok);

			tok = strtok(NULL, del);
			income = atof(tok);

			tok = strtok(NULL, del);
			reference = atoi(tok);

			Student* stud = createStudent(nume, income, reference);
			createHashNode(stud);
			putHT(&hashTable, stud);
		}
		char* key = (char*)malloc(sizeof("Popa Maria") + 1);
		strcpy(key, "Popa Maria");
		Student* student = getFromHashTable(hashTable, key);
		printStudent(student);
	}

	return 0;
}

void printStudent(Student* stud) {
	printf("#%5d - Nume: %s, Income : %.2f\n", stud->reference, stud->name, stud->income);
}

int hashFunction(const char* key) {
	return key[0] % HASHT_SIZE; //ia primul caracter al numelui % size
}

HashNode* createHashNode(Student* info) {
	HashNode* node = (HashNode*)malloc(sizeof(HashNode));
	node->info = info;
	node->next = NULL; 
}

void deleteFromHashTable(HashNode** hashTable, const char* key) {
	//calculate hash value
	int hashValue = hashFunction(key);
	//get the collision list
	HashNode* collisionList = hashTable[hashValue]; //Gets the linked list at the computed hash index.
	//search and delete the key 
	if (collisionList) //checks if the index is not null
	{
		if (strcmp(collisionList->info->name, key) == 0) { //checks if the first node has the desired key
			hashTable[hashValue] = collisionList->next; //updates the head of the list to the next one
			deleteStudent(collisionList->info); //frees the memory of the student
			free(collisionList); //free the auxiliary 
		}
		else {
			while (collisionList && collisionList->next) {
				if (strcmp(collisionList->next->info->name, key) == 0) { //check if the next codes key matches
					HashNode* tmp = collisionList->next; //temporarely stores the node to be deleted
					collisionList->next = tmp->next; //updates the current nodes next pointer to skip the node
					deleteStudent(tmp->info);
					free(tmp);
				}
				collisionList = collisionList->next;
			}
		}
	}
}

Student* getFromHashTable(HashNode** hashTable, const char* key) {
	int hashValue = hashFunction(key); //calculate hash value
	HashNode* collisionList = hashTable[hashValue];
	while (collisionList)
	{
		if (strcmp(collisionList->info->name, key) == 0)
		{
			return collisionList->info;
		}
		collisionList = collisionList->next;
	}
	return NULL;
}
void putHT(HashNode*** hashTable, Student* stud) {
	if (*hashTable == NULL) {
		(*hashTable) = (HashNode**)malloc(sizeof(HashNode*) * HASHT_SIZE);
		for (int i = 0; i < HASHT_SIZE; i++)
			(*hashTable)[i] = NULL;
	}
	HashNode* node = createHashNode(stud);
	int hashValue = hashFunction(stud->name);
	node->next = (*hashTable)[hashValue];
	(*hashTable)[hashValue] = node;
}
void deleteStudent(Student* stud)
{
	if (stud != NULL)
	{
		free(stud->name);
		free(stud);
	}
}
Student* createStudent(const char* name, float income, int ref)
{
	//1.declare variable
	Student* stud = NULL;
	//2.allocate memory
	stud = (Student*)malloc(sizeof(Student));
	//3.init attributes
	stud->name = (char*)malloc(strlen(name) + 1);
	strcpy(stud->name, name);
	stud->income = income;
	stud->reference = ref;
	//4.return student
	return stud;
}

