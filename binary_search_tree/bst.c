#include "bst.h"

// creates a node for the bst
Node* bst_create_node(int data) {
    Node* ret = (Node*) malloc(sizeof(Node));

    ret->value = data;
    ret->left = NULL;
    ret->right = NULL;

    return ret;
}

// destroy the bst (frees all of its data)
void bst_destroy(Node* root) {
    if(root == NULL) return;

    bst_destroy(root->left);
    bst_destroy(root->right);

    free(root);
}

// search an element in the BST. Will return 1 for found, 0 for not found.
int bst_search(Node* root, int data) {
    // base cases
    if(root == NULL) return 0;
    else if (root->value == data) return 1;
    // recursive calls
    if(data < root->value) return bst_search(root->left, data);
    else return bst_search(root->right, data);
}

// insert a node into the bst
Node* bst_insert(Node* root, int data) {
    // time to create the node!
    if(root == NULL) return bst_create_node(data);

    // smaller or bigger than the current node.
    // if equal, we do nothing.
    if (data > root->value) root->right = bst_insert(root->right, data);
    else if (data < root->value) root->left = bst_insert(root->left, data);

    return root;
}

// function to find the minimum value in a node
// works for bst_delete
Node* bst_find_minimum(Node* root) {
  if (root == NULL) return NULL;

  // leftmost element will be minimum
  else if (root->left != NULL) return bst_find_minimum(root->left); 

  // if left is null
  return root;
}

// delete a node of the bst
Node* bst_delete(Node* root, int data) {
    if(root == NULL) return NULL;

    if (data > root->value) root->right = bst_delete(root->right, data);
    else if (data < root->value) root->left = bst_delete(root->left, data);

    // value equal to the node we want to erase
    else {
        //No Child node: delete node completely
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }

        //One Child node
        else if (root->left == NULL || root->right == NULL) {
            // get a temporary node pointing to the child and return it
            // to the parent.
            Node* temp;
            if (root->left == NULL)
                temp = root->right;
            else
                temp = root->left;

            free(root);
            return temp;
        }

        // two children nodes: replace with traversal successor and free it
        else {
            Node *temp = bst_find_minimum(root->right);
            root->value = temp->value;
            root->right = bst_delete(root->right, temp->value);
        }
    }
    
    return root;
}

// prints the values of the BST in order.
// must print \n after using it
void bst_traversal(Node* root) {
    // base cases
    if(root == NULL) return;

    // recursive calls
    bst_traversal(root->left);
    printf("%d ", root->value);
    bst_traversal(root->right);
}


int main() {
    BST test_tree = BST_DEFAULT;
    test_tree.root = test_tree.bst_create_node(20);
    test_tree.bst_insert(test_tree.root, 15);
    test_tree.bst_insert(test_tree.root, 9);
    test_tree.bst_insert(test_tree.root, 5);
    test_tree.bst_insert(test_tree.root, 1);
    test_tree.bst_insert(test_tree.root, 7);
    test_tree.bst_insert(test_tree.root, 12);
    test_tree.bst_insert(test_tree.root, 30);

    test_tree.bst_traversal(test_tree.root);
    printf("\n");

    test_tree.bst_delete(test_tree.root, 5);
    test_tree.bst_delete(test_tree.root, 40);

    test_tree.bst_traversal(test_tree.root);
    printf("\n");

    test_tree.bst_destroy(test_tree.root);
    return 0;
} 

/*
int main() {
    Node* root = bst_create_node(20);
    bst_insert(root, 15);
    bst_insert(root, 9);
    bst_insert(root, 5);
    bst_insert(root, 1);
    bst_insert(root, 7);
    bst_insert(root, 12);
    bst_insert(root, 30);
    bst_insert(root, 25);
    bst_insert(root, 40);
    bst_insert(root, 45);
    bst_insert(root, 42);

    bst_traversal(root);
    printf("\n");
    
    bst_delete(root, 1);
    bst_delete(root, 40);
    bst_delete(root, 45);
    bst_delete(root, 9);
    bst_delete(root, 20);

    bst_traversal(root);
    printf("\n");

    bst_destroy(root);
    return 0;
}
*/