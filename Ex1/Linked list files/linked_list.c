#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

list_t list_create()
{
        list_t list = malloc(sizeof(list_t));
        list->head = NULL;
        list->tail = NULL;
        list->length = 0;
        return list;
}

void list_delete(list_t list)
{
        free(list);
}

void list_insert(list_t list, int index, int data)
{
        struct node* newNode = malloc(sizeof(struct node));
        int counter = 0;
        struct node* ptr = list->head;
        while (index != counter)
        {
                ptr = ptr->next;
                counter++;
        }
        ptr->prev->next = newNode;
        newNode->prev = ptr->prev;
        ptr->prev = newNode;
        newNode->next = ptr;
        newNode->data = data;
        (list->length)++;
}

void list_append(list_t list, int data)
{
        struct node* newNode = malloc(sizeof(struct node));

        if(list->length == 0)
        {
                list->tail = newNode;
                list->head = newNode;
        }
        else
        {
                list->tail->next = newNode;
                newNode->prev = list->tail;
                list->tail = newNode;
        }
        newNode->data = data;
        (list->length)++;
}

void list_print(list_t list)
{
        if (list->head != NULL && list->tail != NULL)
        {
                struct node* ptr = list->head;
                printf("%i ", ptr->data);
                while (ptr->next != NULL)
                {
                        ptr = ptr->next;
                        printf("%i ", ptr->data);
                }
        }
        
        printf("\n");
}

long list_sum(list_t list)
{
        int sum = 0;
        struct node* ptr = list->head;
        sum += ptr->data;
        while (ptr->next != NULL)
        {
                ptr = ptr->next;
                sum += ptr->data;
        }
        return sum;
}

int list_get(list_t list, int index)
{
        int counter = 0;
        struct node* ptr = list->head;
        while (index != counter)
        {
                ptr = ptr->next;
                counter++;
        }
        return ptr->data;
}

int list_extract(list_t list, int index)
{
        int counter = 0;
        int extracted = 0;
        struct node* ptr = list->head;
        while (index != counter)
        {
                ptr = ptr->next;
                counter++;
        }
        if (list->length == 1)
        {
                list->head = NULL;
                list->tail = NULL;
        }
        else if (index == 0)
        {
                list->head = ptr->next;
                ptr->next->prev = NULL;
        }
        else if (index == (list->length - 1))
        {
                list->tail = ptr->prev;
                ptr->prev->next = NULL;
        }
        else
        {
                ptr->prev->next = ptr->next;
                ptr->next->prev = ptr->prev;  
        }

        extracted = ptr->data;
        free(ptr);
        (list->length)--;
        return extracted;
}
