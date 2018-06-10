#include <iostream>
#include <cstdio>
using namespace std;

struct Tnode {
  string word;
  int count;
  Tnode* left;
  Tnode* right;
};

Tnode* regist_word(Tnode* &root, const string& theWord);
void print_tree(Tnode* root, unsigned int indent);
void print_tree_byorder(Tnode* root);
void del_tree(Tnode* root);

int main() {
  Tnode* root = new Tnode;
  root->left = NULL; root->right = NULL; root->count = 1;
  string w;
  cin >> w;
  root -> word = w;

  // cout << root->word <<" "<< root->count <<endl;
  while (cin>>w) {
    // cout << w <<endl;
    regist_word(root, w);
    // cout << "endl" <<endl;
  }
  cout << endl;
  print_tree(root, 1);
  cout << endl;
  print_tree_byorder(root);
  del_tree(root);
  
  return 0;
}


Tnode* regist_word(Tnode* &root, const string& theWord) {
  Tnode* node = new Tnode;
  node->left = NULL; node->right = NULL; node->count = 1;
  node->word = theWord;

  Tnode* T = root;
  Tnode* Q = root;
  while (Q) {
    if (T->word == node->word) {
      T->count += 1;
      delete node;
      return root;
    } 
    if (T->word < node->word) {
      Q = Q->right;
      if (T->right) T = T->right;
    }
    if (T->word > node->word) {
      Q = Q->left;
      if (T->left) T = T->left;
    }
  }
  if (node) {
    if (T->word < node->word) T->right = node;
    if (T->word > node->word) T->left = node;
  }
  return root;
}

void print_tree(Tnode* root, unsigned int indent) {
  Tnode* T = root;
  if (T) {
    cout << T->word <<": "<< T->count <<endl;
    print_tree(T->left, 1);
    print_tree(T->right, 1);
  }
}

void print_tree_byorder(Tnode* root) {
  Tnode* T = root;
  if (T) {
    print_tree_byorder(T->left);
    cout << T->word <<": "<< T->count <<endl;
    print_tree_byorder(T->right);
  }
}

void del_tree(Tnode* root) {
  Tnode* T = root;
  if (T) {
    del_tree(T->left);
    del_tree(T->right);
    delete T;
  }
}









