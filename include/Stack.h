#ifndef __STACK_H__
# define __STACK_H__

typedef struct stack_t *Stack;

typedef void* StackData;

/* at Stack.c */
extern Stack stack_get_instance(void);
extern void stack_push(Stack, StackData);
extern size_t stack_count(Stack);
extern StackData stack_pop(Stack);
extern void stack_free(Stack);

#endif /* end of __STACK_H__ */
