/*
 * Stack.c
 * Copyright (c) 2015 Shiho Midorikawa
 */

#include "ecalc.h"
#include "Stack.h"

typedef struct _stack_node {
  StackData data;
  struct _stack_node *prev;
} stack_node;

struct stack_t {
  stack_node *current_node;
};

stack_node *stack_create_node(void);
void stack_free_node(stack_node *);

Stack stack_get_instance(void) {
  PTR(d, struct stack_t);
  MALLOC(d, struct stack_t, 1);
  return d;
}

size_t stack_count(Stack s) {
  PTR(current, stack_node);
  current = s->current_node;
  uint size = 0;
  for(; IS_EFFECTIVE_PTR(s->current_node); size++) {
    s->current_node = s->current_node->prev;
  }
  s->current_node = current;
  return size;
}

void stack_push(Stack s, StackData data) {
  PTR(new, stack_node);
  new = stack_create_node();
  new->data = data;
  new->prev = s->current_node;
  s->current_node = new;
}

StackData stack_pop(Stack s) {
  PTR(node, stack_node);
  node = s->current_node;
  s->current_node = node->prev;
  StackData data = node->data;
  stack_free_node(node);
  return data;
}

void stack_free(Stack s) {
  PTR(node, stack_node);
  PTR(prev, stack_node);
  node = s->current_node;
  if (IS_EFFECTIVE_PTR(node)) {
    do {
      prev = node->prev;
      FREE(node);
      node = prev;
    } while(IS_EFFECTIVE_PTR(node));
  }
  FREE(s);
}

stack_node *stack_create_node(void) {
  PTR(node, stack_node);
  MALLOC(node, stack_node, 1);
  return node;
}

void stack_free_node(stack_node *node) {
  FREE(node);
}
