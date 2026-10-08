#include <stdio.h>

typedef struct Node
{
    int          data;
    struct Node *next;
} Node;

Node *reverse_iter(Node *head);

int main()
{
    return 0;
}

Node *reverse_iter(Node *head)
{
    Node *prev=NULL;
    Node *cur=head;
    Node *next;

    while(cur!=NULL)
    {
        next=cur->next;
        cur->next=prev;
        prev=cur;
        cur=next;
    }
    return prev;
}