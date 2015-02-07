#ifndef __LINKEDLIST_H__
# define __LINKEDLIST_H__
# include "ecalc.h"

typedef void* LLData_t;
typedef void (*fptr_lleach_t)(LLData_t);
typedef struct linkedlist_t *LinkedList;

/* at LinkedList.c */
extern LinkedList ll_get_instance(void);
extern void ll_add(LinkedList, LLData_t);
extern void ll_set_pos(LinkedList, uint);
extern void ll_set_pos_relative(LinkedList, int);
extern uint ll_get_pos(LinkedList);
extern LLData_t ll_get_data(LinkedList, uint);
extern LLData_t ll_get_current_data(LinkedList);
extern LLData_t ll_remove_data(LinkedList, uint);
extern void ll_foreach(LinkedList, fptr_lleach_t, uint);
extern size_t ll_count(LinkedList);
extern void ll_free(LinkedList);


#endif /* end of __LINKEDLIST_H__ */
