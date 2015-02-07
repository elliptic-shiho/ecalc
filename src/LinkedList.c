/* 
 * LinkedList.c
 * Copyright (c) 2015 Shiho Midorikawa
 */
#include "ecalc.h"
#include "LinkedList.h"

/* Node and LinkedList struct entity*/

typedef struct _node {
  LLData_t data;
  struct _node *next;
  struct _node *prev;
  uint pos;
} Node;

struct linkedlist_t {
  Node *current;
  Node *head;
  Node *tail;
};

/* private function */
Node* createNode(LLData_t);
void freeNode(Node*);
void freeNodeAll(Node*);

/* public function area */

LinkedList ll_get_instance(void) {
  PTR(l, struct linkedlist_t);
  MALLOC(l, struct linkedlist_t, 1);
  l->current = NULL;
  l->head = NULL;
  l->tail = NULL;
  return (LinkedList)l;
}

void ll_add(LinkedList l, LLData_t p) {
  PTR(n, Node);
  n = createNode(p);
  if (!IS_EFFECTIVE_PTR(l->head) && !IS_EFFECTIVE_PTR(l->tail)) {
    n->pos = 0;
    n->prev = NULL;
    l->head = l->tail = l->current = n;
  } else {
    n->pos = l->tail->pos + 1;
    l->tail->next = n;
    n->prev = l->tail;
    l->tail = n;
    l->current = n;
  }
}

uint ll_get_pos(LinkedList l) {
  PTR(current, Node);
  uint i = 0;
  current = l->current;
  ll_set_pos(l, 0);
  for(; i < ll_count(l); i++) {
    ll_set_pos(l, i);
    if(l->current == current) {
      break;
    }
  }
  return i;
}

void ll_set_pos_relative(LinkedList l, int relative_offset) {
  uint current_pos = ll_get_pos(l);
  int pos = relative_offset + current_pos;
  if (pos < 0) {
    pos = ll_count(l) - abs(pos);
  }
  ll_set_pos(l, pos);
}

void ll_set_pos(LinkedList l, uint pos) {
  if (pos < ll_count(l)) {
    uint fixedpos = 0;
    uint i = 0;
    Direction dir = D_FORWARD;
    if (l->current) {
      if (!IS_EFFECTIVE_PTR(l->head)) {
        return;
      } else {
        l->current = l->head;
      }
    }
    if (l->current->pos < pos) {
      fixedpos = pos - l->current->pos;
    } else {
      fixedpos = abs(pos - l->current->pos);
      dir = D_BACK;
    }
    for (i = 0; i < fixedpos; i++) {
      switch(dir) {
      case D_FORWARD:
        l->current = l->current->next;
        break;
      case D_BACK:
        l->current = l->current->prev;
      }
    }
  } else {
    fprintf(stderr, "Error: Invalid position (%d)\n", pos);
    exit(-1);
    return;
  }
}

LLData_t ll_get_data(LinkedList l, uint pos) {
  ll_set_pos(l, pos);
  return l->current->data;
}

LLData_t ll_get_current_data(LinkedList l) {
  return l->current->data;
}

LLData_t ll_remove_data(LinkedList l, uint pos) {
  LLData_t data;
  ll_set_pos(l, pos);
  data = l->current->data;
  freeNode(l->current);
  return data;
}

LinkedList ll_get_sublist(LinkedList l, uint start, uint end) {
  if (end < ll_count(l)) {
    LinkedList s = ll_get_instance();
    uint i;
    for (i = start; i < end; i++) {
      ll_add(s, ll_get_data(l, i));
    }
    return s;
  }
  fprintf(stderr, "Error: Invalid Range: (%d, %d)\n", start, end);
  exit(-1);
  return NULL;
}

LLData_t *ll_get_as_array(LinkedList l) {
  PTR(arr, LLData_t);
  MALLOC(arr, LLData_t, ll_count(l));
  uint i = 0;
  void f(LLData_t d) {
    arr[i++] = d;
  }

  ll_foreach(l, f, 0);

  return arr;
}

void ll_foreach(LinkedList l, fptr_lleach_t func, uint pos) {
  PTR(n, Node);
  if (!IS_EFFECTIVE_PTR(l->current)) {
    return;
  }
  ll_set_pos(l, pos);
  do {
    n = l->current;
    func(n->data);
  } while (l->current != l->tail && (l->current = n->next) != NULL);
}

uint ll_count(LinkedList l) {
  uint _size = 0;
  PTR(n, Node);
  n = l->head;
  while(n != NULL) {
    _size++;
    n = n->next;
  }
  return _size;
}

void ll_free(LinkedList l) {
  freeNodeAll(l->head);
}

#pragma GCC visibility push(hidden)

Node* createNode(LLData_t p) {
  PTR(n, Node);
  MALLOC(n, Node, 1);
  n->data = p;
  n->next = NULL;
  return n;
}

void freeNodeAll(Node *p) {
  if (IS_EFFECTIVE_PTR(p)){
    freeNodeAll(p->next);
    FREE(p);
  }
}

void freeNode(Node *p) {
  if (IS_EFFECTIVE_PTR(p->prev) && IS_EFFECTIVE_PTR(p->next)) {
    p->prev->next = p->next;
    p->next->prev = p->prev;
  }
  FREE(p);
}

#pragma GCC visibility pop
