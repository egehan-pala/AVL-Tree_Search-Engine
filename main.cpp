//
//  main.cpp
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
#include <fstream>

using namespace std;

// Function to convert a string to lowercase
string toLower(string& str) {
    string temp;
    for(int i=0; i<str.length(); i++){
        temp += tolower(str[i]);
    }
    return temp;
}

// Function to split a line of strings into single strings
vector<string> split(const string& str) {
    vector<string> queryWords;
    stringstream ss(str);
    string word;
    while (ss >> word) { // if there are empty spaces between strings split them and add to the vector
        queryWords.push_back(word);
    }
    return queryWords;
}

//Function for to check if given document name exists in the listoffiles vector
bool checkVector(vector<string> &listOfFiles, string &docName){
    for(int i=0; i<listOfFiles.size(); i++){
        if(docName == listOfFiles[i]){
            return true;
        }
    }
    return false;
}

// checks the given string whether contains a non alpha numerical char or not
bool isalpha(string &s){
    for(int i=0; i<s.length(); i++){
        if(!isalpha(i)){
            return false;
        }
    }
    return true;
}

int main() {
    AVLSearchTree<string, WordItem*> tree;

    int numFiles;
    cout << "Enter number of input files: ";
    cin >> numFiles;
    

    vector<string> listOfFiles; // The list of input files

    // Adding the files into the list
    for (int i = 1; i <= numFiles; i++) {
        string fileName;
        cout << "Enter " << i << ". file name: ";
        cin >> fileName; 
        listOfFiles.push_back(fileName);
    }

    for (int i = 0; i < numFiles; ++i) { //loops for the size of listOdFiles vector

        string filename = listOfFiles[i];
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file " << filename << endl; // if cannot open give error message and return 1
            return 1;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string word;
            while (ss >> word) {
                
                if(!isalpha(word)){ //if word contains non alphanum char pass if not continue

                    // Convert word to lowercase
                    word = toLower(word);

                    treeNode<string, WordItem*>* temp = tree.traversal(word);
                    WordItem* wordItem;
                    // Check if the word is already in the tree
                    if (temp == nullptr) {
                        // If not, create a new WordItem and insert it into the tree
                        wordItem = new WordItem(word);
                        //DocumentItem docItem(filename, 1);
                        //wordItem->documents.push_back(filename);
                        tree.insert(word, wordItem);
                    }

                    temp = tree.traversal(word);
                    wordItem = temp->value;
                    // Increment count for the document
                    bool documentFound = false;
                    for (int i=0; i<wordItem->documents.size(); i++){
                        if (wordItem->documents[i].documentName == filename) { //if documentName vector contains the file name that we took the word from, increment the count of it
                            wordItem->documents[i].count++;
                            documentFound = true; // change the bool for found
                            break;
                        }
                    }
                    if (!documentFound) { //if bool is false (meaning the document does not exists in the documentName vector of the tree node) 
                        DocumentItem docItem(filename, 1); // insert the document name to the vector and set the count for one (as it is newly added)
                        wordItem->documents.push_back(docItem);
                    }

                }
            }
        }

        file.close();
    }
    

    string query;

    cout << "Enter queried words in one line: "; 
    cin.ignore();
    getline(cin, query);

    do {
        vector<string> words_query = split(query); // split the line of strings in to a vector
        for(int i=0; i< words_query.size(); i++){
            
            words_query[i] = toLower(words_query[i]); // lower case all the words
        }
        if (words_query[0]!= "remove")
        {
            bool firstOutput = true;
            for (int i=0; i<words_query.size(); i++) {
                treeNode<string, WordItem*>* temp = tree.traversal(words_query[i]); // search for node
                WordItem* wordItem= nullptr;
                if(temp != nullptr) // if exists continue
                {
                    wordItem = temp->value;
                }
                if (wordItem != nullptr) {// if exists continue
                    for (int j=0; j<wordItem->documents.size(); j++){ 
                        DocumentItem doc = wordItem->documents[j]; 
                        if(checkVector(listOfFiles, doc.documentName)){ //check whether the given document name exists in the first given document list
                            cout << "in Document " << doc.documentName << ", "; // if so cout
                        }
                        cout << wordItem->word << " found " << doc.count << " times";
                        if(j != wordItem->documents.size()-1){
                            cout << ", ";
                        }
                        else{
                            cout<< ".";
                        }
                        firstOutput = false; 
                    }
                    cout << endl;
                }
            }
            if (firstOutput) { // If query word does not exist in the tree cout error message
                cout << "No document contains the given query" << endl;;
            }
        }
        else {
            tree.deletion(words_query[1]); //removing the given word from the tree
            cout << words_query[1] << " has been REMOVED" << endl;
        }

        cout << "Enter queried words in one line: ";
        getline(cin, query);
    }while (query != "ENDOFINPUT");

    // destructor

    return 0;
}

