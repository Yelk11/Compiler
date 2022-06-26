#ifndef LIST_H
#define LIST_H
#include <stdlib.h>

typedef struct List_T
{
  void** items;
  size_t size;
  size_t item_size;
} List_T;

List_T* init_list(size_t item_size);

void list_push(List_T* list, void* item);

int list_indexof_str(List_T* list, char* item);
#endif