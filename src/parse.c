#include "ecalc.h"

void parse(void) {
  Token t;
  for (;;) {
    parse_expression();
    t = get_token();
    t = get_token();
    if (t.type == T_EOF) {
      return;
    }
    unget_token();unget_token();
  }
}

void parse_expression(void) {
  Token t;
  parse_term();
  for(;;) {
    t = get_token();
    if (t.type != T_ADD &&
        t.type != T_SUB &&
        t.type != T_OR &&
        t.type != T_AND &&
        t.type != T_XOR) {
      unget_token();
      break;
    }
    parse_term();
    Opcode op = OP_ADD;
    if (t.type == T_ADD) {
      op = OP_ADD;
    } else if (t.type == T_SUB) {
      op = OP_SUB;
    } else if (t.type == T_OR) {
      op = OP_OR;
    } else if (t.type == T_AND) {
      op = T_AND;
    } else if (t.type == T_XOR) {
      op = T_XOR;
    }
    vm_add_opcode(op);
  }
}

void parse_term(void) {
  Token t;
  parse_primary_expression();
  for(;;) {
    t = get_token();
    if (t.type != T_MUL && t.type != T_DIV) {
      unget_token();
      break;
    }
    parse_primary_expression();
    Opcode op;
    if (t.type == T_MUL) {
      op = OP_MUL;
    } else if (t.type == T_DIV) {
      op = OP_DIV;
    }
    vm_add_opcode(op);
  }
}

void parse_primary_expression(void) {
  Token t = get_token();
  if (t.type == T_OPEN_BRACKET) {
    parse_expression();
    t = get_token();
    if (t.type != T_CLOSE_BRACKET) {
      fprintf(stderr, "Syntax Error: Mismatch Bracket\n");
      exit(-1);
    }
  } else {
    unget_token();
    parse_not();
  }
}

void parse_not(void) {
  Token t = get_token();
  if (t.type != T_NOT) {
    unget_token();
  }
  parse_pow();
  if (t.type == T_NOT) {
    vm_add_opcode(OP_NOT);
  }
}

void parse_pow (void) {
  Token t;
  parse_split_expression();
  for (;;) {
    t = get_token();
    if (t.type != T_POW) {
      unget_token();
      break;
    }
    parse_pow();
    if (t.type == T_POW) {
      vm_add_opcode(OP_POW);
    }
  }
}

void parse_split_expression(void) {
  Token t;
  t = get_token();
  if (t.type != T_SPLIT) {
    unget_token();
    parse_number();
    return;
  }
  vm_add_opcode(OP_PRINT);
  parse_primary_expression();
}

void parse_number(void) {
  numtypes_t t = NONE;
  PTR(value, mpz_t);
  int i = 1;
  MALLOC(value, mpz_t, 1);
  mpz_init2(*value, 2048);

  uint j = 0;
  Token token;
  token = get_token();
  if (token.type != T_NUM) {
    fprintf(stderr, "Syntax Error: Invalid Token\n");
    exit(-1);
  }
  for (j = 0; j < strlen(token.value); j++) {
    char ch = token.value[j];
    if (t == NONE && ch == '0') continue;
    if (t == NONE) {
      switch(ch) {
      case '-':
        i = -1;
        continue;
      case 'X':
        t = HEXADECIMAL;
        continue;
      case 'B':
        t = BINARY;
        continue;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        t = DECIMAL;
        break;
      }
    }
    switch(ch) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
      switch(t) {
      case BINARY:
        // '0' <= c <= '9' && 'A' <= c <= 'F' && 'a' <= c <= 'f'
        // -> c - 1 <= '0' -> c == '0' or '1'
        if (ch - 1 <= '0') {
          mpz_mul_ui(*value, *value, 2);
          mpz_add_ui(*value, *value, ch - '0');
        } else {
          fprintf(stderr, "Error: Invalid Binary String : %02x\n", ch);
          exit(-1);
        } 
        break;
      case DECIMAL:
        // similar BINARY
        // c - 9 <= '0' == 0x30 <= c <= '9'
        if (ch - 9 <= '0') {
          mpz_mul_ui(*value, *value, 10);
          mpz_add_ui(*value, *value, ch - '0');
        } else {
          fprintf(stderr, "Error: Invalid Decimal String : %02x\n", ch);
          exit(-1);
        }
        break;
      case HEXADECIMAL:
        mpz_mul_ui(*value, *value, 16);
        if (ch <= '9') {
          mpz_add_ui(*value, *value, ch - '0');
        } else if (ch <= 'f') {
          mpz_add_ui(*value, *value, 10 + ch - 'A');
        }
        break;
      case NONE:
        // should be not come here
        break;
      }
      break;
    }
  }
  mpz_mul_ui(*value, *value, i);
  vm_add_opcode(OP_NUM, value);
}
