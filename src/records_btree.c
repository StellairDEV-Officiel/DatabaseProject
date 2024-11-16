#include "../include/records_btree.h"
#include <stdio.h>
#include <stdlib.h>

Node* insertRecordNode(Node* root, const int key, const long offset) {
    Node* parent = NULL;
    Node* current = root;

    while (current != NULL) {
        parent = current;
        if (key < current->key)
            current = current->left;
        else if (key > current->key)
            current = current->right;
        else
            return root;
    }

    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        exit(EXIT_FAILURE);
    }
    newNode->key = key;
    newNode->offset = offset;
    newNode->left = newNode->right = NULL;

    if (parent == NULL) {
        return newNode;
    } else if (key < parent->key) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

    return root;
}

Node* searchRecordNode(Node* root, int key) {
    Node* current = root;
    while (current != NULL) {
        if (key == current->key)
            return current;
        else if (key < current->key)
            current = current->left;
        else
            current = current->right;
    }
    return NULL;
}

Node* findMinRecordNode(Node* root) {
    if (root == NULL)
        return NULL;
    Node* current = root;
    while (current->left != NULL)
        current = current->left;
    return current;
}


Node* deleteRecordNode(Node* root, int key) {
    Node* parent = NULL;
    Node* current = root;

    //TODO essayer de refacto le code pour utiliser la fonction searchNode
    while (current != NULL && current->key != key) {
        parent = current;
        if (key < current->key)
            current = current->left;
        else
            current = current->right;
    }

    if (current == NULL) {
        printf("Clé '%d' non trouvée.\n", key);
        return root;
    }

    // aucun
    if (current->left == NULL && current->right == NULL) {
        if (parent == NULL) {
            free(current);
            return NULL;
        }
        if (parent->left == current)
            parent->left = NULL;
        else
            parent->right = NULL;
        free(current);
    }
    // 1 seul enfant
    else if (current->left == NULL || current->right == NULL) {
        Node* child = (current->left != NULL) ? current->left : current->right;
        if (parent == NULL) {
            free(current);
            return child;
        }
        if (parent->left == current)
            parent->left = child;
        else
            parent->right = child;
        free(current);
    }
    // deux enfants
    else {
        const Node* minNode = findMin(current->right);
        const int minKey = minNode->key;
        const long minOffset = minNode->offset;
        current->key = minKey;
        current->offset = minOffset;
        current->right = deleteNode(current->right, minKey);
    }

    return root;
}

void viewKeyRecordNode(Node* root) {
    if (root == NULL)
        return;

    StackNode* stack = NULL;
    Node* current = root;

    while (current != NULL || stack != NULL) {
        // Atteindre le nœud le plus à gauche
        while (current != NULL) {
            StackNode* newStackNode = (StackNode*)malloc(sizeof(StackNode));
            if (!newStackNode) {
                exit(EXIT_FAILURE);
            }
            newStackNode->treeNode = current;
            newStackNode->next = stack;
            stack = newStackNode;
            current = current->left;
        }

        if (stack != NULL) {
            StackNode* temp = stack;
            stack = stack->next;
            current = temp->treeNode;
            free(temp);

            printf("%d ", current->key);

            current = current->right;
        }
    }
}

void freeRecordTree(Node* root) {
    if (root == NULL)
        return;

    StackNode* stack = NULL;
    Node* lastVisited = NULL;
    Node* current = root;

    while (current != NULL || stack != NULL) {
        if (current != NULL) {
            StackNode* newStackNode = (StackNode*)malloc(sizeof(StackNode));
            if (!newStackNode) {
                exit(EXIT_FAILURE);
            }
            newStackNode->treeNode = current;
            newStackNode->next = stack;
            stack = newStackNode;
            current = current->left;
        } else {
            StackNode* peekNode = stack;
            while (peekNode->treeNode->right == NULL && peekNode->treeNode != lastVisited) {
                break;
            }

            if (peekNode->treeNode->right != NULL && peekNode->treeNode->right != lastVisited) {
                current = peekNode->treeNode->right;
            } else {
                stack = stack->next;
                lastVisited = peekNode->treeNode;
                free(peekNode);
                free(lastVisited);
            }
        }
    }
}
