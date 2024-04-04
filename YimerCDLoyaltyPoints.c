#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// Programing Assignment 4
// Created by Yimer C. Duggan on 4/3/2024.
// COP3502 Professor Torosdogli
// Objective:
//      Create a loyalty point system that stores the customers in a alphabetical binary tree.
//      You can add, remove, count, and delete customers by entering commands.
//      After your done the program prints the list of commands.

typedef struct Customer{
 char name[20];
 int points;
}Customer;

typedef struct TNode {
    Customer* customerPtr;
    int size;
    struct TNode* Left, *Right;
}TNode;


//Collects the input for later processing.
void commandAnalyzer(char* str, char* command,int* importantNum);

//Adds a node to the alphabetical binary tree or adds points to an existing node.
TNode* add(TNode* head, char* str, int points);

//Removes a node from the alphabetical binary tree.
void del(TNode** head, char* str);

//Subtracts points form a customer.
void sub(TNode* head, char* str, int points);

//search looks for a customer in the tree. It reports back how far down it was in the tree as well as their point value, if the customer is even there.
TNode* search(TNode* head, char* str, TNode** Prevholder);

//counts the number of things that came before in the binary tree.
int count_smaller(TNode* head, char* str);

void arrayify(TNode* head, Customer** array, int* index);

void mergeSort(Customer *pData[], int left, int right);

void printArray(Customer** array, int size);

int main() {
    int NumOfCommands, NumForCommand, i, holder;
    char commandStr[14], NameStr[20];
    TNode* THead = (TNode*) malloc(sizeof(TNode));
    TNode *otherTemp;
    Customer** array;
    THead->customerPtr = NULL;
    scanf("%d", &NumOfCommands);
    for (i = 0; i < NumOfCommands;i++) {
        commandAnalyzer(NameStr, commandStr, &NumForCommand);
        if (!strcmp("add", commandStr)) {
           THead = add(THead, NameStr, NumForCommand);
        } if (!strcmp("del", commandStr)) {
            del(&THead, NameStr);
        } if (!strcmp("sub", commandStr)) {
            sub(THead, NameStr, NumForCommand);
        } if (!strcmp("search", commandStr)) {
            otherTemp = search(THead, NameStr, NULL);
            if (otherTemp == NULL) {
                printf("%s not found\n", NameStr);
            }
        } if (!strcmp("count_smaller", commandStr)) {
            holder = count_smaller(THead, NameStr);
            printf("%d\n", holder);
        }
    }
    array = (Customer**) malloc(THead->size*sizeof(Customer*));
    holder = THead->size;
    i = 0;
    arrayify(THead, array, &i);
    mergeSort(array, 0, holder-1);
    printArray(array, holder);
    return 0;
}

//Collects the input for later processing.
void commandAnalyzer(char* str, char* command,int* importantNum){
    scanf("%s %s %d", command, str, importantNum);
}

//Adds a node to the alphabetical binary tree or adds points to an existing node.
TNode* add(TNode* head, char* str, int points){
    int ans;
    if (head->customerPtr == NULL) {
        head->customerPtr = (Customer*) malloc(sizeof(Customer));
        head->Right = NULL;
        head->Left = NULL;
        head->size = 1;
        head->customerPtr->points = points;
        strcpy(head->customerPtr->name, str);
        printf("%s %d\n", str, points);
        return head;
    }
    TNode* prev;
    TNode* temp = search(head, str, &prev);
    if (temp != NULL) {
        temp->customerPtr->points += points;
        printf("%s %d\n", str, temp->customerPtr->points);
        return head;
    }
    prev = head;
    temp = head;
    while (temp != NULL) {
        ans = strcmp(str, temp->customerPtr->name);
        if (ans > 0){
            temp->size += 1;
            prev = temp;
            temp = temp->Right;
        } else {
            temp->size += 1;
            prev = temp;
            temp = temp->Left;
        }
    }
    if (strcmp(str, prev->customerPtr->name) > 0){
        prev->Right = (TNode*) malloc(sizeof(TNode));
        prev->Right->customerPtr = (Customer*) malloc(sizeof(Customer));
        prev->Right->size = 1;
        prev->Right->customerPtr->points = points;
        strcpy(prev->Right->customerPtr->name, str);
        prev->Right->Right = NULL;
        prev->Right->Left = NULL;
    } else {
        prev->Left = (TNode*) malloc(sizeof(TNode));
        prev->Left->customerPtr = (Customer*) malloc(sizeof(Customer));
        prev->Left->size = 1;
        prev->Left->customerPtr->points = points;
        strcpy(prev->Left->customerPtr->name, str);
        prev->Left->Right = NULL;
        prev->Left->Left = NULL;
    }
    printf("%s %d\n", str, points);
    return head;
}

//Removes a node from the alphabetical binary tree.
void del(TNode** head, char* str) {
    TNode *temp, *prev, *tempTwo, *prevTwo;
    temp = search(*head, str, &prev);
    if (temp == NULL) {
        printf("%s not found\n", str);
        return;
    }
    temp = *head;
    prev = *head;
    while (strcmp(str, temp->customerPtr->name)) {
        temp->size -= 1;
        if (temp->Right == NULL && temp->Left == NULL) {
            break;
        }
        prev = temp;
        if(strcmp(str,temp->customerPtr->name) > 0) {
            temp = temp->Right;
        } else {
            temp = temp->Left;
        }
        if (temp == NULL) {
            break;
        }
    }
    if (temp->Left == NULL && temp->Right == NULL) {
        if (strcmp(str, prev->customerPtr->name) > 0) {
            prev->Right = NULL;
        } else {
            prev->Left = NULL;
        }
    } else if (temp->Left == NULL) {
        if (prev == temp) {
            *head = temp->Right;
            (*head)->size -= 1;
        } else if (strcmp(str, prev->customerPtr->name) > 0) {
            prev->Right = temp->Right;

        } else {
            prev->Left = temp->Right;

        }

    } else {
        tempTwo = temp->Left;
        prevTwo = temp;
        while (tempTwo->Right != NULL) {
            tempTwo->size -= 1;
            prevTwo = tempTwo;
            tempTwo = tempTwo->Right;
        }
        if (strcmp(str, prev->customerPtr->name) > 0) {
            prev->Right = tempTwo;
            tempTwo->Right = temp->Right;
            if (tempTwo->Left != NULL) {
                prevTwo->Right = temp->Left;
            } else if (prevTwo != temp) {
                prevTwo->Right = NULL;
            }
            if (tempTwo != temp->Left) {
                tempTwo->Left = temp->Left;
            }
            tempTwo->size = temp->size-1;
        } else if (prev == temp) {
            *head = tempTwo;
            tempTwo->Right = temp->Right;
            if (tempTwo != temp->Left) {
                tempTwo->Left = temp->Left;
            }
            prevTwo->Right = NULL;
            (*head)->size = temp->size-1;
        } else {
            prev->Left = tempTwo;
            tempTwo->Right = temp->Right;
            if (tempTwo->Left != NULL) {
                prevTwo->Right = temp->Left;
            } else if (prevTwo != temp) {
                prevTwo->Right = NULL;
            }
            if (tempTwo != temp->Left) {
                tempTwo->Left = temp->Left;
            }
            tempTwo->size = temp->size-1;
        }
    }
    printf("%s deleted\n", str);
    temp->Right = NULL;
    temp->Left = NULL;
    free(temp->customerPtr);
    free(temp);
}

//Subtracts points form a customer.
void sub(TNode* head, char* str, int points){
    TNode *target, *prev;
    target = search(head, str, &prev);
    if (target == NULL) {
        printf("%s not found\n", str);
        return;
    }
    if (target->customerPtr->points - points >= 0) {
        target->customerPtr->points -= points;
    } else {
        target->customerPtr->points = 0;
    }
    printf("%s %d\n", str, target->customerPtr->points);
}

//search looks for a customer in the tree. It reports back how far down it was in the tree as well as their point value, if the customer is even there.
TNode* search(TNode* head, char* str, TNode** PrevHolder){
    TNode  *temp, *prev;
    int flag = 0, depth = 0;
    temp = head;
    prev = head;
    while (strcmp(str, temp->customerPtr->name)) {
        if (temp->Right == NULL && temp->Left == NULL) {
            flag = 1;
            break;
        }
        prev = temp;
        if(strcmp(str,temp->customerPtr->name) > 0) {
            temp = temp->Right;
        } else {
            temp = temp->Left;
        }
        if (temp == NULL) {
            flag = 1;
            break;
        }
        depth++;
    }
    if (flag) {
        return NULL;
    }
    if (PrevHolder != NULL) {
        *PrevHolder = prev;
    } else {
        printf("%s %d %d\n", temp->customerPtr->name, temp->customerPtr->points, depth);
    }
    return temp;
}

//counts the number of things that came before in the binary tree.
int count_smaller(TNode* head, char* str){
    int total = 0, ans;
    if (head == NULL) {
        return 0;
    }
    ans =strcmp(str, head->customerPtr->name);
    if (ans < 0) {
        total = count_smaller(head->Left, str);
        return total;
    }
    if (ans > 0) {
        total = count_smaller(head->Right, str);
        if (head->Left == NULL) {
            return total + 1;
        }
        return total + head->Left->size + 1;
    }
    if (head->Left == NULL) {
        return 0;
    }
    return head->Left->size;
}


void mergeSort(Customer *pData[], int left, int right)
{
    Customer **leftArray, **rightArray;
    Customer* temp, otherTemp;
    int mid, midR, counter, counterR = 0, counterL = 0;
    if (left >= right-1) {
        if (pData[left]->points < pData[right]->points) {
            temp = pData[left];
            pData[left] = pData[right];
            pData[right] = temp;
            return;
        }
        return;
    }
    mid = (right + left)/2;
    midR = mid + 1;
    leftArray = (Customer**) malloc(sizeof(Customer*)*(mid+1));
    for (counter = 0; counter <= mid; counter++) {
        leftArray[counter] = pData[counter];
    }
    rightArray = (Customer**) malloc(sizeof(Customer*)*(right - midR + 1));
    for (counter = counter; counter <= right; counter++) {
        rightArray[counter-midR] = pData[counter];
    }
    mergeSort(leftArray, 0, mid);
    mergeSort(rightArray, 0, right-midR);
    counter = 0;

    while ((counterL <= mid) || (counterR <= right-midR)) {
        if ((counterL <= mid) && (counterR <= right-midR)) {
            if (leftArray[counterL]->points <= rightArray[counterR]->points) {
                pData[counter] = rightArray[counterR];
                counterR++;
                counter++;
            } else {
                pData[counter] = leftArray[counterL];
                counterL++;
                counter++;
            }
        } else if (counterR <= right-midR) {
            pData[counter] = rightArray[counterR];
            counterR++;
            counter++;
        } else if (counterL <= mid) {
            pData[counter] = leftArray[counterL];
            counterL++;
            counter++;
        }
    }
    free(leftArray);
    free(rightArray);
}

void arrayify(TNode* head, Customer** array, int* index){
    if (head == NULL) {
        return;
    }
    arrayify(head->Left, array, index);
    arrayify(head->Right, array, index);
    array[*index] = head->customerPtr;
    (*index)++;
    free(head);
}

void printArray(Customer** array, int size) {
    int i = 0;
    for (i = 0; i < size; i++) {
        printf("%s %d\n", array[i]->name, array[i]->points);
        free(array[i]);
    }
}