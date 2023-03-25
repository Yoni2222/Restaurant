#include "Header.h"

void readInstructions() {

	char c = 'a', choice = ' ';
	int flag = 0;
	item* head = NULL;
	table *tables;
	orderedItem* itemVar = NULL;
	tables = (table *)malloc(sizeof(table) * N);
	if (!tables) {
		printf("\nAllocation of tables had failed!");
	}
	FILE* instructionsReader = fopen("C:\\data\\Instructions.txt", "r");

	if (instructionsReader == NULL) {
		printf("\nfile can't be opened!");
		return;
	}
	initTables(tables);

	while (fscanf(instructionsReader, " %c", &choice) == 1) {
		
		showMenu();
		while (flag == 0) {
			scanf(" %c", &c);
			if (c == '0') {
				readLine(instructionsReader, &head, tables, &itemVar, choice);
				flag = 1;
			}			
		}
		flag = 0;
	}


	fclose(instructionsReader);
	printf("\n\nGood Bye!");
	//here we should free all lists that we have
	closeAllTables(tables);
	closeKitchen(&head);
	clearListOfOrderedItemsToday(&itemVar);
}

void readLine(FILE* instructionsReader, item** head, table tables[], orderedItem** itemVar, char c) {

	
	char prodName[256];
	int quantity, tableNum;

	
	switch (c) {
	case '1':
		printf("\n1 was read");
		*head = CreateProducts();
		break;
	case '2':
		printf("\n2 was read");
		fscanf(instructionsReader, "%s %d", prodName, &quantity);
		AddItems(prodName, quantity, head);
		break;
	case '3':
		printf("\n3 was read");
		fscanf(instructionsReader, "%d %s %d", &tableNum, prodName, &quantity);
		OrderItem(tableNum, prodName, quantity, *head, tables, itemVar);
		break;
	case '4':
		printf("\n4 was read");
		fscanf(instructionsReader, "%d %s %d", &tableNum, prodName, &quantity);
		RemoveItem(tableNum, prodName, quantity, tables);
		break;
	case '5':
		printf("\n5 was read");
		fscanf(instructionsReader, "%d", &tableNum);
		RemoveTable(tableNum, tables);
		break;
	case '6':
		printf("\n6 was read");
		c = fgetc(instructionsReader);
		c = fgetc(instructionsReader);
		if (c == 'A') {
			fscanf(instructionsReader, "%s", prodName);
			Report('A', prodName, tableNum, tables, *head, *itemVar);
		}
		else if (c == 'B') {
			fscanf(instructionsReader, "%s", prodName);
			Report('B', prodName, tableNum, tables, *head, *itemVar);
		}
		else if (c == 'C') {
			fscanf(instructionsReader, "%d", &tableNum);
			Report('C', prodName, tableNum, tables, *head, *itemVar);
		}
		break;
	default:
		printf("\nWrong choice!");
	}

}

item* CreateProducts() {

	FILE* ptr = fopen("C:\\data\\Manot.txt", "r");
	item *head = NULL, *end = NULL, *temp;
	char prodName[256], premium;
	int price, quantity;
	if (ptr == NULL) {
		printf("\nFailed to open Manot.txt");
		return NULL;
	}
	while (fscanf(ptr, "%s %d %d %c", prodName, &quantity, &price, &premium) == 4) {
		temp = (struct item *)malloc(sizeof(struct item));
		if (temp == NULL) {
			printf("\nAllocation of node has failed!");
			return NULL;
		}
		temp->productName = (char *)malloc(sizeof(char) * (strlen(prodName) + 1));
		if (!temp->productName) {
			printf("\nMemory allocation of product name has failed!");
			return NULL;
		}
		strcpy(temp->productName, prodName);
		temp->quantity = quantity;
		temp->price = price;
		temp->premium = premium;
		if (head == NULL) {
			head = temp;
			end = temp;
			end->next = NULL;
		}
		else {
			end->next = temp;
			end = temp;
			end->next = NULL;
		}
		

	}
	//if (feof(ptr))
		//printf("\nEnd of file");
	fclose(ptr);
	return head;
}

void AddItems(char* productName, int quantity, item** head) {

	if (quantity <= 0) {
		printf("\nNot a legal number!");
		return;
	}
	item* temp = *head;
	while (temp) {
		if (!strcmp(temp->productName, productName)) {
			temp->quantity += quantity;
			return;
		}
		temp = temp->next;
	}
	printf("\nThere is no such a product name!");
}

void OrderItem(int tableNumber, char* productName, int quantity, item* head, table tables[], orderedItem** itemVar) {

	if (tableNumber >= N || tableNumber < 0) {
		printf("\nNot valid table number!");
		return;
	}
	item* temp = head;
	while (temp) {
		if (!strcmp(productName, temp->productName)) {
			if (quantity > temp->quantity) {
				printf("\nNot enough quantity in the kitchen!");
				return;
			}

			createOrder(tables, tableNumber, productName, quantity, temp->premium, temp->price);
			temp->quantity -= quantity;
			updateOrderedItems(itemVar, quantity, productName);
			return;

		}
		temp = temp->next;
	}
	printf("\nThere is no such a product name!");
}


void printList(item* head) {
	item* temp = head;
	while (temp) {
		printf("\nProduct name: %s, quantity: %d, price: %d, premium: %c", temp->productName, temp->quantity, temp->price, temp->premium);
		temp = temp->next;
	}

}

void printTable(table tables[], int tableNum) {
	tableItem* temp = tables[tableNum].head;
	printf("\nItems that were ordered are:");
	while (temp) {
		printf("\nProduct name: %s, quantity: %d, price: %d, premium: %c", temp->productName, temp->quantity, temp->price, temp->premium);
		temp = temp->next;
	}
}

void initTables(table tables[]) {
	int i;
	for (i = 0; i < N; i++) {
		tables[i].account = 0;
		tables[i].head = NULL;
	}
}

void createOrder(table tables[], int tableNum, char* productName, int quantity, char premium, unsigned int price) {
	// create linked list for the specified table number:
	//char prodName[256];
	tableItem* temp = NULL, *temp2 = NULL, *prev = NULL;
	char* st;
	int lenOfProductStr = strlen(productName);

	if (tables[tableNum].head != NULL) {
		temp2 = tables[tableNum].head;
		while (temp2) {
			// check if this is not the first time this table ordered this item

			if (!strcmp(temp2->productName, productName)) {
				temp2->quantity += quantity;
				tables[tableNum].account += (float)(price * quantity);
				return;
			}
			temp2 = temp2->next;
		}
	}

	temp = (tableItem *)malloc(sizeof(tableItem));
	if (temp == NULL) {
		printf("\nAllocation of 'tables.head' has failed!");
		return;
	}
	temp->productName = (char*)malloc(lenOfProductStr + 1);
	if (!temp->productName) {
		printf("\nAllocation of 'tables.head->productName' has failed!");
		return;
	}
	strcpy(temp->productName, productName);
	temp->quantity = quantity;
	temp->premium = premium;
	temp->price = price;
	if (tables[tableNum].account == 0) {
		tables[tableNum].head = temp;
		tables[tableNum].head->next = NULL;
		tables[tableNum].head->prev = NULL;
	}
	else {
		temp2 = tables[tableNum].head;
		while (temp2->next) {
			// moving cursor to the end of list
			temp2 = temp2->next;
		}
		if (temp2->next == NULL) {
			temp2->next = temp;
			temp->prev = temp2;
			temp->next = NULL;
		}
	}

	tables[tableNum].account += (float)(price * quantity);

}

void RemoveItem(int tableNumber, char* productName, int quantity, table tables[]) {

	if (tableNumber >= N || tableNumber < 0) {
		printf("\nThere is no such a table number!");
		return;
	}
	int price;
	tableItem* temp = tables[tableNumber].head;
	while (temp) {
		if (!strcmp(temp->productName, productName)) {
			if (quantity > temp->quantity) {
				printf("\nThe quantity requested is higher than the quantity this table ordered for this item!");
				return;
			}
			else if (quantity < temp->quantity) {
				temp->quantity -= quantity;
				tables[tableNumber].account -= (float)(temp->price * quantity);
				return;
			}
			price = temp->price;
			deleteOrderFromTable(tables, tableNumber, temp);
			tables[tableNumber].account -= (float)(price * quantity);
			return;
		}
		temp = temp->next;
	}

	printf("\nThis table has never ordered this item");


}
void deleteOrderFromTable(table tables[], int tableNum, tableItem* node) {

	tableItem* temp = node;
	if (temp->prev == NULL && temp->next) {									// check whether the node is the head of the list and update the new head
		tables[tableNum].head = tables[tableNum].head->next;
	}
	else {
		if (node->prev) {
			node->prev->next = node->next;
		}
		if (node->next) {
			node->next->prev = node->prev;
		}
	}

	free(temp->productName);
	free(temp);
}
void RemoveTable(int tableNumber, table tables[]) {

	//tableItem* temp = tables[tableNumber].head;
	if (tableNumber >= N || tableNumber < 0) {
		printf("\nWrong table numner!");
		return;
	}

	if (!tables[tableNumber].head) {
		printf("\nThis table hasn't ordered any items.");
		return;
	}

	char s[5] = "";
	int premium = premiumTable(tables, tableNumber);
	if (premium)
		tables[tableNumber].account += (tables[tableNumber].account * 0.1);
	else
		strcpy(s, "not ");
	printTable(tables, tableNumber);
	printf("\nThis is %sa premium table, the final account is %.2f.", s, tables[tableNumber].account);
	clearTable(tables, tableNumber);
}

void clearTable(table tables[], int tableNumber) {
	tableItem *temp;

	while (tables[tableNumber].head) {
		temp = tables[tableNumber].head;
		tables[tableNumber].head = tables[tableNumber].head->next;
		free(temp->productName);
		free(temp);
	}
	tables[tableNumber].account = 0;
	tables[tableNumber].head = NULL;

}


void Report(char choice, char* name, int tableNum, table tables[], item* head, orderedItem* itemVar) {
	if (choice == 'A') {
		kitchenReport(head);
	}
	else if (choice == 'B') {
		itemReport(itemVar, name);
	}
	else if (choice == 'C') {
		tableReport(tables, tableNum);
	}
	else {
		printf("\nWrong character, try again!");
	}
}

void showMenu() {
	printf("\n\nWelcome to the restaurant!");
	printf("\nCreateProducts creates a list containing all items in the kitchen, according to 'Manot' text.");
	printf("\nAddItems adds supply for a given item in the kitchen.");
	printf("\nOrderItem creates an order for a given table and adds the order to the orders list of this table.");
	printf("\nRemoveItem returns an ordered item.");
	printf("\nRemoveTable clears a given table and deletes its list of ordered items.");
	printf("\nReport provides details about all the items that currently in the kitchen, also it provides a report about hom many times a given item was ordered today ");
	printf("and it can provide a report about all the items that a given table has ordered, including its final account and the type of service that they gotten.");
	printf("\nPlease press 0 for the system in order to continue receiving instructions from 'Instructions' file.\n");

}

int premiumTable(table tables[], int tableNumber) {
	tableItem* temp = tables[tableNumber].head;
	while (temp) {
		if (temp->premium == 'Y')
			return 1;
		temp = temp->next;
	}
	return 0;
}


void kitchenReport(item* head) {
	printf("\nkitchen report:");
	printList(head);
	/*while (head) {
		printf("\nProduct name: %s, quantity: %d, price: %d, premium: %c", head->productName, head->quantity, head->price, head->premium);
		head = head->next;
	}*/
}

void itemReport(orderedItem* head, char* name) {
	orderedItem* temp = head;
	while (temp) {
		if (!strcmp(temp->productName, name)) {
			printf("\nQuantity ordered today of %s is %d", temp->productName, temp->quantity);
			return;
		}
		temp = temp->next;
	}
	printf("\nQuantity ordered today of %s is 0", name);
}

void tableReport(table tables[], int tableNumber) {

	if (!tables[tableNumber].head || tables[tableNumber].account == 0) {
		printf("\nThis table is free! No one sits here.");
		return;
	}
	printTable(tables, tableNumber);
}

void updateOrderedItems(orderedItem** item, int quantity, char* name) {
	orderedItem* temp, *head, *prev = NULL;
	head = *item;
	
	while (head) {
		if (!strcmp((head)->productName, name)) {
			(head)->quantity += quantity;
			return;
		}
		prev = head;
		head = (head)->next;
	}
	temp = (orderedItem *)malloc(sizeof(orderedItem));
	if (!temp) {
		printf("\nAllocation of orderedItem failed!");
		return;
	}
	temp->productName = (char *)malloc(strlen(name) + 1);
	if (!temp->productName) {
		printf("\nAllocation of product name failed!");
		return;
	}
	strcpy(temp->productName, name);
	temp->quantity = quantity;
	temp->next = NULL;
	printf("\nproduct name of temp is %s, quantity is %d", temp->productName, temp->quantity);
	if (prev == NULL) {
		*item = temp;
	}
	else {
		(prev)->next = temp;
	}

}

void closeAllTables(table tables[]) {
	int i;
	for (i = 0; i < N; i++)
		clearTable(tables, i);

}

void closeKitchen(item** head) {

	item *temp;

	while (*head) {
		temp = *head;
		*head = (*head)->next;
		free(temp->productName);
		free(temp);
	}
}

void clearListOfOrderedItemsToday(orderedItem** item) {
	orderedItem *temp;

	while (*item) {
		temp = *item;
		*item = (*item)->next;
		free(temp->productName);
		free(temp);
	}
}