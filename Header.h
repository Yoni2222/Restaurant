#pragma once
#ifndef RESTAURANT_HEADER
#define RESTAURANT_HEADER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 15							// number of tables in the restaurant

typedef struct item {					// a node in the list of items in the kitchen
	char* productName;
	unsigned int price;
	int quantity;
	char premium;
	struct item *next;
}item;

typedef struct tableItem {				// a node in the list of the orders of the table
	char* productName;
	unsigned int price;
	int quantity;
	char premium;
	struct tableItem *next, *prev;
}tableItem;

typedef struct orderedItem {			// a node in the list that contains the number of orders each item has 
	char* productName;
	int quantity;
	struct orderedItem *next;
}orderedItem;

typedef struct table {					// a struct that represents each table

	float account;
	tableItem* head;
}table;


item* CreateProducts();																										// 1		initialize kitchen
void AddItems(char* productName, int quantity, item** head);																// 2		add item to kitchen
void OrderItem(int tableNumber, char* productName, int quantity, item* head, table tables[], orderedItem** item);			// 3		order item for table	
void RemoveItem(int tableNumber, char* productName, int quantity, table tables[]);											// 4		return ordered item
void RemoveTable(int tableNumber, table tables[]);																			// 5        final account
void Report(char choice, char* name, int tableNum, table tables[], item* head, orderedItem* item);							// 6

void readInstructions();																									// reads file 'Instructions.txt'
void readLine(FILE* instructionsReader, item** head, table tables[], orderedItem** item, char c);							// reads a single line from 'Instructions.txt'
void showMenu();																											// shows the restaurant menu
void printList(item* head);																									// prints all the items in the kitchen
void printTable(table tables[], int tableNum);																				// prints the orders list of a specified table
void initTables(table tables[]);																							// initializes all the tables array
void createOrder(table tables[], int tableNum, char* productName, int quantity, char premium, unsigned int price);			// a sub-function inside 'OrderItem'
void deleteOrderFromTable(table tables[], int tableNum, tableItem* node);													// deletes a single node from the orders list of the table
int premiumTable(table tables[], int tableNumber);																			// checks if a table is premium or not
void clearTable(table tables[], int tableNumber);																			// deletes the orders list of the table
void kitchenReport(item* head);																								// provides a report about the items in the kitchen
void itemReport(orderedItem* head, char* name);																				// provides a report about the orders number of the specified item
void tableReport(table tables[], int tableNumber);																			// provides a report about all the orders of the table
void updateOrderedItems(orderedItem** item, int quantity, char* name);														// updates the list that contains the number of orders each item has
void closeAllTables(table tables[]);																						// deletes the orders list of each table in the restaurant
void closeKitchen(item** head);																								// deletes the list of all the items in the kitchen
void clearListOfOrderedItemsToday(orderedItem** item);																		// deletes the list that contains the number of orders each item has
#endif // !RESTAURANT_HEADER

