#ifndef BTREE_H
#define BTREE_H

typedef struct Node {
    int key;          // Clé primaire
    long offset;      // Position dans le fichier .dat
    struct Node *left;
    struct Node *right;
} Node;

typedef struct StackNode {
    Node* treeNode;
    struct StackNode* next;
} StackNode;

Node* insertRecordNode(Node* root, int key, long offset);
Node* searchRecordNode(Node* root, int key);
Node* deleteRecordNode(Node* root, int key);
Node* findMinRecordNode(Node* root);
void viewKeyRecordNode(Node* root);
void freeRecordTree(Node* root);

#endif //BTREE_H
