#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
} Node;

Node* bst_create_node(int data);
Node* bst_insert(Node* root, int data);
Node* bst_delete(Node* root, int data);
    Node* bst_find_minimum(Node* root);
int bst_search(Node* root, int data);
void bst_traversal(Node* root);
void bst_destroy(Node* root);

/////////////////////////////////////////////////////////////////////////////////////////
/*********************************** user manual ***************************************\
**                                                                                     **
**    1st, create a Node* for the root, and call bst_create_node with a value for it.  **
**    then, insert stuff with bst_insert.                                              **
**                                                                                     **
**    delete stuff with bst_delete. bst_find_minimum is an accessory to bst_delete     **
**    and should not be used.                                                          **
**                                                                                     **
**    you can traverse the root, search on it, etc.                                    **
**                                                                                     **
**    finally, use bst_destroy to free the bst memory.                                 **
**                                                                                     **
\***************************************************************************************/
/////////////////////////////////////////////////////////////////////////////////////////

typedef struct BST {
    Node* root;
    Node* (*bst_create_node)(int);
    void (*bst_destroy)(Node*);
    int (*bst_search)(Node*, int);
    Node* (*bst_insert)(Node*, int);
    Node* (*bst_find_minimum)(Node*);
    Node* (*bst_delete)(Node*, int);
    void (*bst_traversal)(Node*);
} BST;

#define BST_DEFAULT {NULL, &bst_create_node, &bst_destroy, &bst_search, &bst_insert, &bst_find_minimum, &bst_delete, &bst_traversal};

/////////////////////////////////////////////////////////////////////////////////////////
/*********************************** user manual ***************************************\
**                                                                                     **
**    1st, create a BST and equal it to BST_default.                                   **
**    use just like the normal BST functions, but call it like this:                   **
**    test_tree.[function](test_tree.root, [n]);                                       **
**                                                                                     **
\***************************************************************************************/
/////////////////////////////////////////////////////////////////////////////////////////