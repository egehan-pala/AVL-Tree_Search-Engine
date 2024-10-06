//
//  AVLTree.h
//  Cs300_hw2
//
//  Created by Mehmet Egehan Pala on 29.03.2024.
//

#ifndef AVLTree
#define AVLTree

#include <string>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

struct DocumentItem {
    string documentName;
    int count;
    DocumentItem(){}
    DocumentItem(string documentName, int count=0): documentName(documentName), count(count)
    {}
};


struct WordItem {
    string word;
    vector<DocumentItem> documents;
    WordItem() {}
    WordItem(string wrd): word(wrd)
    {}
};

template <class Key, class Value>
struct treeNode
{
    Key key;
    Value value;
    int height;
    treeNode<Key, Value>* left;
    treeNode<Key, Value>* right;

    treeNode(Key key, Value value): key(key), value(value), height(0), left(nullptr), right(nullptr)
    {}
};


template <class Key, class Value>
class AVLSearchTree{
    private:
        treeNode<Key, Value>* root;
    
    public:
        AVLSearchTree();
        ~AVLSearchTree();
        treeNode<Key, Value>* getRoot();
        void clear(treeNode<Key, Value> * & root) const;
        int maxDepth(treeNode<Key, Value> * &root);
        int minDepth(treeNode<Key, Value> * &root);
        int findHeight(treeNode<Key, Value> * &root);
        bool isBalanced(treeNode<Key, Value> * &root);
        treeNode<Key, Value>* findMin(treeNode<Key, Value> * &root);
        treeNode<Key, Value>* findMax(treeNode<Key, Value> * &root);
        treeNode<Key, Value>* traversal(const Key& k);
        void insert(const Key &key, const Value &value);
        void deletion(const Key &key);
    protected:
        treeNode<Key, Value>* insert(treeNode<Key, Value>* &root, const Key &key, const Value &value);
        treeNode<Key, Value>* deletion(treeNode<Key, Value>* &root, const Key &key);
        void rotateLeft(treeNode<Key, Value>* &root) const;
        void rotateRight(treeNode<Key, Value>* &root) const;
        int height(treeNode<Key, Value>* &root) const;
        treeNode<Key, Value>* traversal(const Key& k, treeNode<Key, Value>* &root) const;
        int balanceFactor(treeNode<Key, Value>* &node);
        void updateHeight(treeNode<Key, Value>* &node);
        

};

#include "AVLTree.cpp"
#endif