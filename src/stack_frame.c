#include "stack_frame.h"


Stack_frame_T* init_stack_frame()
{
  Stack_frame_T* stack_frame = calloc(1, sizeof(struct Stack_frame_T));
  stack_frame->stack = init_list(sizeof(char*));

  return stack_frame;
}