#define _CRT_SECURE_NO_WARNINGS
#define LINE_SIZE 256
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>



typedef struct MutualFund{
	char* fundCode;
	char* fundGroup;
	int riskLevel;
	double netValue;
	float ROI;
}MutualFund;

typedef struct Node {
	MutualFund* info;
	struct Node* next;
	struct Node* prev;
}Node;


MutualFund* createMutualFund(char* code, char* group, int risk, double value, float ROI);
void printFund(MutualFund* fund);
Node* createNode(MutualFund* fund);
void addToList(Node** list, Node* node);
void printList(Node* list);
int noMutualFundsGreater(Node* list, double value);
void capitalGain(Node* list);
MutualFund* assetValueGrater(Node* list, float tresh);

int main() {
	FILE* fp = fopen("Data.txt", "r");
	Node* list = NULL;
	if (fp) {
		char buffer[LINE_SIZE];
		char del[] = ",\n";
		char* tok = NULL;

		
		char fundCode[LINE_SIZE];
		char fundGroup[LINE_SIZE];
		int riskLevel;
		double netValue;
		float ROI;

		while (fgets(buffer, sizeof(buffer), fp)) {
			tok = strtok(buffer, del);
			strcpy(fundCode, tok);

			tok = strtok(NULL, del);
			strcpy(fundGroup, tok);

			tok = strtok(NULL, del);
			riskLevel = atoi(tok);


			tok = strtok(NULL, del);
			netValue = atof(tok);

			tok = strtok(NULL, del);
			ROI = atof(tok);

			MutualFund* fund = createMutualFund(fundCode, fundGroup, riskLevel, netValue, ROI);
			
			Node* node = createNode(fund);
			addToList(&list, node);
		}
		printList(list);
		printf("-----------\n");
		//printf("No of risks >7: %d", noMutualFundsGreater(list, 7));
		//capitalGain(list);
		double treshold = 3000;
		MutualFund* fund2 = assetValueGrater(list, treshold);
		printf("\n\nFirst mutual fund above threshold %.2f:", treshold);
		if (fund2)
			printFund(fund2);
		else
			printf("\nNo fund found.");

	}
	
	return 0;
}

MutualFund* assetValueGrater(Node* list, double tresh) {
	MutualFund* fund = NULL;
	while (list) {
		if (list->info->netValue > tresh) {
			fund = list->info;
			break;
		}
		else {
			list = list->next;
		}
	}
	return fund;
}

void capitalGain(Node* list) {
	float capitalGain = 0;
	while (list) {
		double capitalGain = (list->info->netValue * list->info->ROI) - list->info->netValue;
		printf("%s - capital gain/loss: %.2f\n", list->info->fundCode, capitalGain);
		list = list->next;
	}
}

int noMutualFundsGreater(Node* list, double value) {
	int counter = 0;
	while (list->next != NULL) {
		if (list->info->riskLevel > value) {
			counter++;
		}
		list = list->next;
	}
	if (list->info->riskLevel > value) {
		counter++;
	}
	return counter;
	
}

void printList(Node* list) {
	while (list->next) {
		printFund(list->info);
		list = list->next;
	}
	printFund(list->info);

	printf("-----------------------\n");
	while (list->next != NULL) {
		list = list->next;
	}
	while (list->prev != NULL) {
		printFund(list->info);
		list = list->prev;
	}
	printFund(list->info);
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

Node* createNode(MutualFund* fund) {
	Node* node = NULL;
	node = (Node*)malloc(sizeof(Node));
	node->info = fund;
	node->next = NULL;
	node->prev = NULL;
	return node;
}

MutualFund* createMutualFund(char* code, char* group, int risk, double value, float ROI) {
	MutualFund* fund = NULL;
	fund = (MutualFund*)malloc(sizeof(MutualFund));
	fund->fundCode = (char*)malloc(sizeof(code) + 1);
	strcpy(fund->fundCode, code);
	fund->fundGroup = (char*)malloc(sizeof(group) + 1);
	strcpy(fund->fundGroup, group);
	fund->riskLevel = risk;
	fund->netValue = value;
	fund->ROI = ROI;
	return fund;
}

void printFund(MutualFund* fund) {
	printf("Code: %s, Group: %s, Risk: %d, Value: %.0f, ROI: %.2f\n", 
		fund->fundCode, fund->fundGroup, fund->riskLevel, fund->netValue, fund->ROI);
}
