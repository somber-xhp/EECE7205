#include <iostream>
#include <string>
using namespace std;
struct Node { int data; Node *left, *right; Node(int v):data(v),left(nullptr),right(nullptr){} };
Node* insert(Node* r,int k){if(!r)return new Node(k);if(k<r->data)r->left=insert(r->left,k);else if(k>r->data)r->right=insert(r->right,k);return r;}
Node* makeTree(char t){Node* r=nullptr;int T[]={40,20,60,10,30,50,70};int P[]={45,25,65,15,35,55,75,30,60};if(t=='T')for(int k:T)r=insert(r,k);else for(int k:P)r=insert(r,k);return r;}
void printInorder(Node* r){if(!r)return;printInorder(r->left);cout<<r->data<<' ';printInorder(r->right);}
void printPreorder(Node* r){if(!r)return;cout<<r->data<<' ';printPreorder(r->left);printPreorder(r->right);}
void freeTree(Node* r){if(!r)return;freeTree(r->left);freeTree(r->right);delete r;}
// B1-B6: deletion, returning possibly updated root. O(h) time.
Node* deleteNode(Node* root,int key){
 if(!root)return nullptr;
 if(key<root->data){root->left=deleteNode(root->left,key);return root;}
 if(key>root->data){root->right=deleteNode(root->right,key);return root;}
 if(!root->left){Node* child=root->right;delete root;return child;}
 if(!root->right){Node* child=root->left;delete root;return child;}
 Node* succ=root->right;
 while(succ->left)succ=succ->left;
 root->data=succ->data;
 root->right=deleteNode(root->right,succ->data);
 return root;
}
Node* deletePredecessor(Node* root,int key){
 if(!root)return nullptr;
 if(key<root->data){root->left=deletePredecessor(root->left,key);return root;}
 if(key>root->data){root->right=deletePredecessor(root->right,key);return root;}
 if(!root->left){Node* c=root->right;delete root;return c;}
 if(!root->right){Node* c=root->left;delete root;return c;}
 Node* pred=root->left;while(pred->right)pred=pred->right;
 root->data=pred->data;root->left=deletePredecessor(root->left,pred->data);return root;
}
void show(const string& name,Node* r){cout<<name<<": ";printInorder(r);cout<<" (root = ";if(r)cout<<r->data;else cout<<"NULL";cout<<")\n";}
int main(int argc,char**argv){
 if(argc==1){Node* r=makeTree('T');show("Before",r);for(int k:{40,10,60,25}){r=deleteNode(r,k);show("After deleting "+to_string(k),r);}freeTree(r);return 0;}
 string mode=(argc>2?argv[2]:argv[1]);
 if(mode=="empty"){Node* r=nullptr;cout<<"the tree is empty\n";r=deleteNode(r,25);freeTree(r);return 0;}
 if(argc<4){cerr<<"Usage: ./bst_all_cases T|P delete|pred|insert KEY\n";return 1;}
 char t=argv[1][0];if(t!='T'&&t!='P'){cerr<<"Choose T or P\n";return 1;}
 int key=stoi(argv[3]);Node* r=makeTree(t);show("Before",r);
 if(mode=="delete")r=deleteNode(r,key);
 else if(mode=="pred")r=deletePredecessor(r,key);
 else if(mode=="insert")r=insert(r,key);
 else{cerr<<"Unknown mode\n";freeTree(r);return 1;}
 show("After",r);cout<<"Preorder: ";printPreorder(r);cout<<'\n';freeTree(r);
}
