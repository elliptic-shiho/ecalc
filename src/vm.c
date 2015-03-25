#include "ecalc.h"

LinkedList g_bytecode;
typedef struct {
  Opcode code;
  mpz_t *param;
} OpcodeElem;

void vm_execute (void) {
  Stack stack = stack_get_instance();
  mpz_t *i, *j;
  mpz_t ONE, INV_XOR;
  mpz_init(ONE);
  mpz_init(INV_XOR);
  mpz_set_str(ONE, "1", 10);
  mpz_set_str(INV_XOR, "-1", 10);

  void f(LLData_t t) {
    OpcodeElem *elem = t;
    switch(elem->code) {
    case OP_NUM:
      stack_push(stack, (StackData)elem->param);
      break;
    case OP_ADD:;
      j = (mpz_t*)stack_pop(stack);
      i = (mpz_t*)stack_pop(stack);
      mpz_add(*i, *i, *j);
      stack_push(stack, (StackData)i);
      break;
    case OP_SUB:;
      j = (mpz_t*)stack_pop(stack);
      i = (mpz_t*)stack_pop(stack);
      mpz_sub(*i, *i, *j);
      stack_push(stack, (StackData)i);
      break;
    case OP_MUL:;
      j = (mpz_t*)stack_pop(stack);
      i = (mpz_t*)stack_pop(stack);
      mpz_mul(*i, *i, *j);
      stack_push(stack, (StackData)i);
      break;
    case OP_DIV:;
      j = (mpz_t*)stack_pop(stack);
      i = (mpz_t*)stack_pop(stack);
      mpz_div(*i, *i, *j);
      stack_push(stack, (StackData)i);
      break;
    case OP_POW:;
      j = (mpz_t*)stack_pop(stack);
      i = (mpz_t*)stack_pop(stack);
      {
        if (ll_get_pos(g_bytecode) + 1 < ll_count(g_bytecode)) {
          ll_set_pos_relative(g_bytecode, 1);
          OpcodeElem n = *(OpcodeElem*)ll_get_current_data(g_bytecode);
          if (n.code == OP_MOD) {
            mpz_t *k = (mpz_t*)stack_pop(stack);
            mpz_powm(*i, *i, *j, *k);
            goto POW_ST_PUSH;
          }
          ll_set_pos_relative(g_bytecode, -1);
        }
        mpz_pow_ui(*i, *i, mpz_get_ui(*j));
      }
POW_ST_PUSH:
      stack_push(stack, (StackData)i);
      break;
    case OP_MOD:;
      j = (mpz_t*)stack_pop(stack);
      i = (mpz_t*)stack_pop(stack);
      mpz_powm(*i, *i, ONE, *j);
      stack_push(stack, (StackData)i);
      break;
    case OP_OR:;
      j = (mpz_t*)stack_pop(stack);
      i = (mpz_t*)stack_pop(stack);
      mpz_ior(*i, *i, *j);
      stack_push(stack, (StackData)i);
      break;
    case OP_AND:;
      j = (mpz_t*)stack_pop(stack);
      i = (mpz_t*)stack_pop(stack);
      mpz_and(*i, *i, *j);
      stack_push(stack, (StackData)i);
      break;
    case OP_NOT:;
      j = (mpz_t*)stack_pop(stack);
      mpz_xor(*i, *j, INV_XOR);
      stack_push(stack, (StackData)i);
      break;
    case OP_XOR:;
      j = (mpz_t*)stack_pop(stack);
      i = (mpz_t*)stack_pop(stack);
      mpz_xor(*i, *i, *j);
      stack_push(stack, (StackData)i);
      break;
    case OP_PRINT:
      vm_print(stack);
      break;
    }
  }

  ll_foreach(g_bytecode, f, 0);
  vm_print(stack);
  stack_free(stack);
}

void vm_print(Stack stack) {
  mpz_t *data;
  for(; stack_count(stack) > 0; data = (mpz_t*)stack_pop(stack));

  gmp_printf("(signed decimal)   = %Zd\n", *data);
  gmp_printf("(unsigned decimal) = %Zu\n", *data);
  gmp_printf("(hex)              = 0x%Zx\n", *data);
  printf("(bin)              = 0b");
  mpz_out_str(stdout, 2, *data);
  printf("\n\n");
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
  PTR(param, mpz_t);
  va_list ap;
  va_start(ap, opcode);
  if (opcode == OP_NUM) {
    param = va_arg(ap, mpz_t*);
  }
  PTR(d, OpcodeElem);
  MALLOC(d, OpcodeElem, 1);
  d->code = opcode;
  d->param = param;
  ll_add(g_bytecode, d);
  va_end(ap);
}
