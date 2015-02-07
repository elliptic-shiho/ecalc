#include "ecalc.h"

LinkedList g_bytecode;
typedef struct {
  Opcode code;
  int param;
} OpcodeElem;

void vm_execute (void) {
  Stack stack = stack_get_instance();
  int i, j;
  void f(LLData_t t) {
    OpcodeElem *elem = t;
    switch(elem->code) {
    case OP_NUM:
      stack_push(stack, (StackData)elem->param);
      break;
    case OP_ADD:;
      j = (int)stack_pop(stack);
      i = (int)stack_pop(stack);
      stack_push(stack, (StackData)(i + j));
      break;
    case OP_SUB:;
      j = (int)stack_pop(stack);
      i = (int)stack_pop(stack);
      stack_push(stack, (StackData)(i - j));
      break;
    case OP_MUL:;
      j = (int)stack_pop(stack);
      i = (int)stack_pop(stack);
      stack_push(stack, (StackData)(i * j));
      break;
    case OP_DIV:;
      j = (int)stack_pop(stack);
      i = (int)stack_pop(stack);
      stack_push(stack, (StackData)(i / j));
      break;
    case OP_POW:;
      j = (int)stack_pop(stack);
      i = (int)stack_pop(stack);
      i = pow(i, j);
      stack_push(stack, (StackData)i);
      break;
    case OP_OR:;
      j = (int)stack_pop(stack);
      i = (int)stack_pop(stack);
      stack_push(stack, (StackData)(i | j));
      break;
    case OP_AND:;
      j = (int)stack_pop(stack);
      i = (int)stack_pop(stack);
      stack_push(stack, (StackData)(i & j));
      break;
    case OP_NOT:;
      j = (int)stack_pop(stack);
      stack_push(stack, (StackData)~i);
      break;
    case OP_XOR:;
      j = (int)stack_pop(stack);
      i = (int)stack_pop(stack);
      stack_push(stack, (StackData)(i ^ j));
      break;
    }
  }

  ll_foreach(g_bytecode, f, 0);
  int data = 0;
  for(; stack_count(stack) > 0; data = (int)stack_pop(stack));
  printf("(signed decimal)   = %d\n", data);
  printf("(unsigned decimal) = %u\n", data);
  printf("(hex) = 0x%x\n", data);
  printf("(bin) = 0b");
  char bin[32] = {0};
  uint ui = data;

  for(j = 0; ui != 0;ui /= 2, j++) {
    bin[j] = ui % 2;
  }
  DIRECTION_FOR(j, 31, bin[j] == 0 && j >= 0, D_BACK);
  if ( j == -1 ){
    printf("0");
  } else {
    DIRECTION_FOR(i, j, i >= 0, D_BACK) {
      printf("%d", bin[i]);
    }
  }
  printf("\n");
  stack_free(stack);
}

void vm_init(void) {
  g_bytecode = ll_get_instance();
}

void vm_free(void) {
  void f(LLData_t t) {
    FREE(t);
  }
  ll_foreach(g_bytecode, f, 0);
  ll_free(g_bytecode);
}

void vm_add_opcode(Opcode opcode, ...) {
  int param = 0;
  va_list ap;
  va_start(ap, opcode);
  if (opcode == OP_NUM) {
    param = va_arg(ap, int);
  }
  PTR(d, OpcodeElem);
  MALLOC(d, OpcodeElem, 1);
  d->code = opcode;
  d->param = param;
  ll_add(g_bytecode, d);
  va_end(ap);
}
