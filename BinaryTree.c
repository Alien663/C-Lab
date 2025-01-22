# include <stdio.h>
# include <stdlib.h>

struct Node
{
    Node* parent;
    Node* left;
    Node* right;
    int data;
}

void traversal_preorder(Node* p){
    if(!p) return;
    cout << p->data;
    traversal_preorder(p->left);
    traversal_preorder(p->right)
}

void traversal_inorder(Node* p){
    if(!p) return;
    traversal_inorder(p->left);
    cout << p->data;
    traversal_inorder(p->right);
}

void traversal_postorder(Node* p){
    if(!p) return;
    traversal_postorder(p->left);
    traversal_postorder(p->right);
    cout << p->data;
}

void traversal_levelorder(Node* root){
    queue<Node*> q;
    q.push(root);
    while(!q.empty()){
        Node* p = q.front();
        q.pop();
        cout << p->data;
        if(p->left) q.push(p->left);
        if(p->right) q.push(p->right);
    }
}

// TODO: Create Binary Tree