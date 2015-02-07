#include "ecalc.h"

LinkedList g_bytecode;
typedef struct {
  Opcode code;
  int param;
} OpcodeElem;

void vm_execute (void) {
  int sp = 0;
  int stack[65536];
  int i, j;
  void f(LLData_t t) {
    OpcodeElem elem = *((OpcodeElem*) t);
    switch(elem.code) {
    case OP_NUM:
      stack[sp++] = elem.param;
      break;
    case OP_ADD:;
      i = stack[--sp];
      j = stack[--sp];
      stack[sp++] = i + j;
      break;
    case OP_SUB:;
      i = stack[--sp];
      j = stack[--sp];
      stack[sp++] = j - i;
      break;
    case OP_MUL:;
      i = stack[--sp];
      j = stack[--sp];
      stack[sp++] = i * j;
      break;
    case OP_DIV:;
      i = stack[--sp];
      j = stack[--sp];
      stack[sp++] = j / i;
      break;
    case OP_POW:;
      i = stack[--sp];
      j = stack[--sp];
      stack[sp++] = pow(j, i);
      break;
    case OP_OR:;
      i = stack[--sp];
      j = stack[--sp];
      stack[sp++] = j | i;
      break;
    case OP_AND:;
      i = stack[--sp];
      j = stack[--sp];
      stack[sp++] = j & i;
      break;
    case OP_NOT:;
      i = stack[--sp];
      stack[sp++] = ~i;
      break;
    case OP_XOR:;
      i = stack[--sp];
      j = stack[--sp];
      stack[sp++] = j ^ i;
      break;
    }
  }

  ll_foreach(g_bytecode, f, 0);

  printf("(signed decimal)   = %d\n", stack[0]);
  printf("(unsigned decimal) = %u\n", stack[0]);
  printf("(hex) = 0x%x\n", stack[0]);
  printf("(bin) = 0b");
  char bin[32] = {0};
  uint ui = stack[0];

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
