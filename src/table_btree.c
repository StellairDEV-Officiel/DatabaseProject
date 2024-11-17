#include "../include/table_btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


TableNode* insertTableNode(TableNode* root, const char* name, Table* table) {
    TableNode* parent = NULL;
    TableNode* current = root;

    while (current != NULL) {
        parent = current;
        const int cmp = strcmp(name, current->name);
        if (cmp < 0)
            current = current->left;
        else if (cmp > 0)
            current = current->right;
        else
            return root;
    }

    TableNode* newNode = (TableNode*)malloc(sizeof(TableNode));
    if (!newNode) {
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->name, name);
    newNode->table = table;
    newNode->left = newNode->right = NULL;

    if (parent == NULL) {
        return newNode;
    } else {
        const int cmp = strcmp(name, parent->name);
        if (cmp < 0)
            parent->left = newNode;
        else
            parent->right = newNode;
    }

    return root;
}

TableNode* searchTableNode(TableNode* root, const char* name) {
    TableNode* current = root;
    while (current != NULL) {
        const int cmp = strcmp(name, current->name);
        if (cmp == 0)
            return current;
        else if (cmp < 0)
            current = current->left;
        else
            current = current->right;
    }
    return NULL;
}

TableNode* findMinTableNode(TableNode* root) {
    if (root == NULL)
        return NULL;
    TableNode* current = root;
    while (current->left != NULL)
        current = current->left;
    return current;
}

TableNode* deleteTableNode(TableNode* root, const char* name) {
    TableNode* parent = NULL;
    TableNode* current = root;

    while (current != NULL && strcmp(current->name, name) != 0) {
        parent = current;
        const int cmp = strcmp(name, current->name);
        if (cmp < 0)
            current = current->left;
        else
            current = current->right;
    }

    if (current == NULL) {
        return root;
    }

    if (current->left == NULL && current->right == NULL) {
        if (parent == NULL) {
            free(current->table->schema.columns);
            free(current->table);
            free(current);
            return NULL;
        }
        if (parent->left == current)
            parent->left = NULL;
        else
            parent->right = NULL;
        free(current->table->schema.columns);
        free(current->table);
        free(current);
    }

    else if (current->left == NULL || current->right == NULL) {
        TableNode* child = (current->left != NULL) ? current->left : current->right;
        if (parent == NULL) {
            free(current->table->schema.columns);
            free(current->table);
            free(current);
            return child;
        }
        if (parent->left == current)
            parent->left = child;
        else
            parent->right = child;
        free(current->table->schema.columns);
        free(current->table);
        free(current);
    }

    else {
        TableNode* succParent = current;
        TableNode* succ = current->right;
        while (succ->left != NULL) {
            succParent = succ;
            succ = succ->left;
        }

        strcpy(current->name, succ->name);
        current->table = succ->table;


        if (succParent->left == succ)
            succParent->left = succ->right;
        else
            succParent->right = succ->right;

        free(succ->table->schema.columns);
        free(succ->table);
        free(succ);
    }

    return root;
}

void viewKeyTableNode(TableNode* root) {
    if (root == NULL)
        return;

    StackTableNode* stack = NULL;
    TableNode* current = root;

    while (current != NULL || stack != NULL) {
        while (current != NULL) {
            StackTableNode* newStackNode = (StackTableNode*)malloc(sizeof(StackTableNode));
            if (!newStackNode) {
                exit(EXIT_FAILURE);
            }
            newStackNode->tableNode = current;
            newStackNode->next = stack;
            stack = newStackNode;
            current = current->left;
        }

        if (stack != NULL) {
            StackTableNode* temp = stack;
            stack = stack->next;
            current = temp->tableNode;
            printf("%s\n", current->name);
            free(temp);

            current = current->right;
        }
    }
}

void freeTableTree(TableNode* root) {
    if (root == NULL)
        return;

    StackTableNode* stack = NULL;
    const TableNode* lastVisited = NULL;
    TableNode* current = root;

    while (current != NULL || stack != NULL) {
        if (current != NULL) {
            StackTableNode* newStackNode = (StackTableNode*)malloc(sizeof(StackTableNode));
            if (!newStackNode) {
                exit(EXIT_FAILURE);
            }
            newStackNode->tableNode = current;
            newStackNode->next = stack;
            stack = newStackNode;
            current = current->left;
        } else {
            StackTableNode* peekNode = stack;
            if (peekNode->tableNode->right != NULL && lastVisited != peekNode->tableNode->right) {
                current = peekNode->tableNode->right;
            } else {
                stack = stack->next;
                lastVisited = peekNode->tableNode;

                free(peekNode->tableNode->table->schema.columns);
                free(peekNode->tableNode->table);
                free(peekNode);
            }
        }
    }
}
