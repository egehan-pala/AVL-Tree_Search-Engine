//
//  AVLTree.cpp
//  Cs300_hw2
//
//  Created by Mehmet Egehan Pala on 29.03.2024.
//
#include "AVLTree.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <math.h>


using namespace std;


template <class Key, class Value>
AVLSearchTree<Key, Value>::AVLSearchTree(): root(nullptr) //constructor
{}

template <class Key, class Value>
AVLSearchTree<Key, Value>::~AVLSearchTree() // destructor
{
    clear(root);
}

// get the root value
template <class Key, class Value>
treeNode<Key, Value>* AVLSearchTree<Key, Value>::getRoot(){
    return this->root;
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::clear(treeNode<Key, Value> * & root) const //delete all the data in the avl tree
{   
    if( root != NULL )
    {
        clear(root->left); //recursively go to the end nodes end delete as return 
        clear(root->right);
        delete root;
    }
    root = NULL;
}


//from lecture slides
template <class Key, class Value>
int AVLSearchTree<Key, Value>::height(treeNode<Key, Value> * &root) const //returns the height
{
    if (root == NULL)
        return -1;

    return root->height;
}


//from lecture slides
template <class Key, class Value>
treeNode<Key, Value>* AVLSearchTree<Key, Value>::findMin(treeNode<Key, Value> * &root) //returns the node contains min value
{
    if(root->left == nullptr){
        return root;
    }
    return findMin(root->left);
}

//from lecture slides
template <class Key, class Value>
treeNode<Key, Value>* AVLSearchTree<Key, Value>::findMax(treeNode<Key, Value> * &root) //returns the node contains max value
{
    if(root->right == nullptr){
        return root;
    }
    return findMin(root->right);
}

template <class Key, class Value>
int AVLSearchTree<Key, Value>::balanceFactor(treeNode<Key, Value>* &root) { //returns the difference between the left subtree and right subtree (+ if left bigger, - if right bigger)

    if (root == nullptr)
        return 0;
    return height(root->left) - height(root->right); // to check whether the tree is balanced or not
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::updateHeight(treeNode<Key, Value>* &root) { // updates the height of the root

    root->height = max(height(root->left), height(root->right)) + 1;
}

//from lecture slides(recitation solutions)
template <class Key, class Value>
int AVLSearchTree<Key, Value>::maxDepth(treeNode<Key, Value> * &root)
{
     if (root == nullptr) {
          return 0;
    }
    return 1 + max(maxDepth(root->left), maxDepth(root->right));
}

//from lecture slides(recitation solutions)
template <class Key, class Value>
int AVLSearchTree<Key, Value>::minDepth(treeNode<Key, Value>* &root)
{
    if (root == nullptr) {
        return 0;
    }
    return 1 + min(minDepth(root->left), minDepth(root->right));
}

//from lecture slides(recitation solutions)
template <class Key, class Value>
bool AVLSearchTree<Key, Value>::isBalanced(treeNode<Key, Value>* &root)
{
    return ((maxDepth(root) - minDepth(root)) <=1);
}

//////////////////////////////////////////

//from lecture slides
template <class Key, class Value>
void AVLSearchTree<Key, Value>::rotateRight(treeNode<Key, Value> * &root) const // rotate with left child
{
    treeNode<Key, Value> *temp = root->left;
    root->left = temp->right;
    temp->right = root;
    root->height = max(height( root->left ), height( root->right ) ) + 1;
    temp->height = max( height( temp->left ), root->height ) + 1;
    root = temp;
}


//from lecture slides
template <class Key, class Value>
void AVLSearchTree<Key, Value>::rotateLeft(treeNode<Key, Value> * &root) const //rotate with right child
{
    treeNode<Key, Value> *temp = root->right;
    root->right = temp->left;
    temp->left = root;
    root->height = max(height( root->left ), height( root->right ) ) + 1;
    temp->height = max( height( temp->right ), root->height ) + 1;
    root = temp;
}


//from lecture slides
template <class Key, class Value> //taken from tree slides (from lecture)
treeNode<Key, Value>* AVLSearchTree<Key, Value>::insert(treeNode<Key, Value>* &root, const Key &key, const Value &value){

    if (root == nullptr){
        root = new treeNode<Key, Value>(key, value);
        return root;
    }
    else if (key < root->key){
        root->left = insert(root->left, key, value);

        if ((height(root->left) - height(root->right)) == 2){
            if (key < root->left->key ){ // Value was inserted to the left-left subtree!
                rotateRight(root); // rotate with left child
            }      
            else{    // Value was inserted to the left-right subtree!
                rotateLeft(root->left); //first left than right
                rotateRight(root); // rotate with left child
            }
        }
    }
    else if (key > root->key){
        root->right = insert(root->right, key, value);

        if((height(root->right) - height(root->left)) == 2){

            if(key > root->right->key){
                rotateLeft(root); //rotate with right child
            }
            else{
                rotateRight(root->right);// rotate with left child
                rotateLeft(root); //first right than left
            }
        }
    }
    updateHeight(root);
    return root;
}


template <class Key, class Value>  // inspired from stackoverflow
treeNode<Key, Value>* AVLSearchTree<Key, Value>::deletion(treeNode<Key, Value>* &root, const Key &key)
{
    if (root == nullptr){ //if could not found return
        return root;
    }
    if (key < root->key){ //search for word
        root->left = deletion(root->left, key);
    }
    else if (key > root->key){
        root->right = deletion(root->right, key);
    }
    else {
        // Node with only one child or no child
        if (root->left == nullptr || root->right == nullptr) {
            treeNode<Key, Value>* temp;

            if (root->left != nullptr) {
                temp = root->left;
            } 
            else {
                temp = root->right;
            }
            // No child case
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } 
            else {// One child case
                *root = *temp; // Copy the contents of the non-empty child
            }
            delete temp;
        } 
        else {
            // tree node with two children 
            
            treeNode<Key, Value>* temp = findMin(root->right); // get the smallest in the right subtree
            root->key = temp->key; // coppy the data of smallest one to the node

            root->right = deletion(root->right, temp->key); // delete the temp tree Node (the one we took the info from (smallest))
        }
    }

    // If the tree had only one node then return
    if (root == nullptr)
        return root;

    // Update height of the current node
    updateHeight(root);

    // Get the balance factor of this node (to check whether this node became unbalanced)
    int balance = balanceFactor(root);

    // Rotate with right child 
    if (balance > 1 && balanceFactor(root->left) >= 0)
        rotateRight(root);

    // Double rotate to right 
    else if (balance > 1 && balanceFactor(root->left) < 0) {
        rotateLeft(root->left);
        rotateRight(root);
    }

    // Rotate with left child 
    else if (balance < -1 && balanceFactor(root->right) <= 0)
        rotateLeft(root);

    // Double rotate to left 
    else if (balance < -1 && balanceFactor(root->right) > 0) {
        rotateRight(root->right);
        rotateLeft(root);
    }

    return root;

}

template <class Key, class Value>
treeNode<Key, Value>* AVLSearchTree<Key, Value>::traversal(const Key& k, treeNode<Key, Value>* &root) const //search functions
{
    if (root == nullptr) // if not found return nullptr
        return nullptr;
    else if (k < root->key) // search recursively until it is found
        return traversal(k, root->left);
    else if (root->key < k)
        return traversal(k, root->right);
    else
        return root;    // Found return the tree node
}


// Helper functions

template <class Key, class Value> 
treeNode<Key, Value>* AVLSearchTree<Key, Value>::traversal(const Key &k){
    return traversal(k, root);
}

template <class Key, class Value> 
void AVLSearchTree<Key, Value>::insert(const Key &k, const Value &v){
    insert(root, k, v);
}

template <class Key, class Value> 
void AVLSearchTree<Key, Value>::deletion(const Key &k){
    deletion(root, k);
}