#include <stdio.h>
#include <stdlib.h>

struct node{
    int data;
    struct node *next;
};
typedef struct node Node;

Node *current, *first, *previous;

void PrintList(Node* first){
    Node* node=first;
    if(first==NULL){
        printf("List is empty\n");
    }
    else{
        while(node!=NULL){
            printf("%d ", node->data);
            node=node->next;
        }
        printf("\n");
    }
}

Node* SearchNode(Node* first, int item){
    Node* node=first;
    while(node->data!=NULL){
        if (node->data==item){
            return node;
        }
        else{
            node=node->next;
        }
    }
    return NULL;
}

void FreeList(Node* first){
    Node *current, *temp;
    current=first;
    while(current!=NULL){
        temp=current;
        current=current->next;
        free(temp);
    }
}

void InsertNode(Node* node, int item){
    Node* newnode=(Node*)malloc(sizeof(Node));
    newnode->data=item;
    newnode->next=node->next;
    node->next=newnode;
}

void PushFront(int a){
    Node* newnode=(Node*)malloc(sizeof(Node));
    newnode->data=a;
    newnode->next=first;
    first=newnode;
}

void PushBack(int a){
    Node* newnode=(Node*)malloc(sizeof(Node));
    newnode->data=a;
    newnode->next=NULL;
    if(first==NULL){
        first=newnode;
    }
    else{
        Node* current=first;
        while(current->next!=NULL){
            current=current->next;
        }
        current->next=newnode;
    }
}

Node* DeleteNode(Node* first, Node* node){
    Node* ptr=first;
    if(first==NULL){
        printf("Nothing to delete!\n");
        return NULL;
    }
    if(node==first){
        first=first->next;
        free(node);
        return first;
    }
    else{
        while(ptr->next!=node){
            ptr=ptr->next;
        }
        ptr->next=node->next;
        free(node);
        return first;
    }
}

void Reverse(){
    if(first==0 || first->next==0){
        return;
    }
    Node *previous, *current, *preceding;
    previous=NULL;
    current=first;
    preceding=first->next;
    while(preceding!=NULL){
        current->next=previous;
        previous=current;
        current=preceding;
        preceding=preceding->next;
    }
    current->next=previous;
    first=current;
}

int main(){
    int num[5] = {9, 54, 61, 5, 27};
    for(int i=0; i<5; i++){
        current=(Node*)malloc(sizeof(Node));
        if(i==0){
            first=current;
        }
        else{
            previous->next=current;
        }
        current->data=num[i];
        current->next=NULL;
        previous=current;
    }
    current=first;
    PrintList(first);
    
    PushFront(38);
    PushBack(65);
    printf("After PushFront() and PushBack():\n");
    PrintList(first);

    Node* node1 = SearchNode(first, 61);
    InsertNode(node1, 46);
    printf("After SearchNode() and InsertNode():\n");
    PrintList(first);

    Reverse();
    printf("After Reverse():\n");
    PrintList(first);

    Node* node2 = SearchNode(first, 54);
    DeleteNode(first, node2);
    printf("After SearchNode() and DeleteNode():\n");
    PrintList(first);

    FreeList(first);
    return 0;
}