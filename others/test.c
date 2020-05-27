#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>


struct data_s
{
    int data;
};

struct node 
{
    struct data_s data;
    struct node *next;
    struct node *prev;
};  

struct list
{
    struct node *head;
    struct node *tail;
    uint32_t size;
};

void init_list(struct list new_list)
{   
    new_list.head = new_list.tail = (void *)0;
    new_list.size = 0;
}


void append(struct list *list_s , struct node *new_node)
{
    list_s->tail->next = new_node;
    new_node->prev = list_s->tail;
    list_s->tail = new_node;
    new_node->next = NULL;

    if(list_s->head==NULL)
    {
        list_s->head = new_node;
    }

    list_s->size +=1;
}   

void print_list(struct list *list_s)
{
    if(!list_s->head)
    {
        exit(0);
    }
    
    struct node *ptr;

    ptr = list_s->head;

    do
    {
        printf("%d \n" , ptr->data.data);

        ptr = ptr->next;

    }while(ptr!=list_s->tail->next);
}


int main()
{
    struct list new_list;

    struct node node1 , node2;   
    struct data_s data1 , data2;

    data1.data = 1;
    data2.data = 2;

    node1.data = data1;
    node2.data = data2;

    init_list(new_list);

    append(&new_list , &node1);

}