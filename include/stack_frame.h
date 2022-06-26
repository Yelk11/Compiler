#ifndef STACK_FRAME_H
#define STACK_FRAME_H
#include "list.h"
typedef struct Stack_frame_T
{
  List_T* stack;
} Stack_frame_T;

Stack_frame_T* init_stack_frame();
#endif