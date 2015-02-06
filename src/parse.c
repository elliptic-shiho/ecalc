#include "ecalc.h"

void parse(void) {
  Token t;
  for (;;) {
    parse_expression();
    t = get_token();t = get_token();
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
    if (t.type != T_ADD && t.type != T_SUB) {
      unget_token();
      break;
    }
    parse_term();
    if (t.type == T_ADD) {
      vm_add_opcode(OP_ADD);
    } else if (t.type == T_SUB) {
      vm_add_opcode(OP_SUB);
    }
  }
}

void parse_term(void) {
  Token t;
  parse_primary_expression();
  for(;;) {
    t = get_token();
    if (t.type != T_MUL && t.type != T_DIV && t.type != T_POW) {
      unget_token();
      break;
    }
    parse_primary_expression();
    Opcode op;
    if (t.type == T_MUL) {
      op = OP_MUL;
    } else if (t.type == T_DIV) {
      op = OP_DIV;
    } else if (t.type == T_POW) {
      op = OP_POW;
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
    parse_number();
  }
}

void parse_number(void) {
  numtypes_t t = NONE;
  int value = 0;
  int i = 1;
  Token token;
  do {
      token = get_token();
      if (token.type != T_NUM) {
        unget_token();
        break;
      }
      if (t == NONE && token.value == '0') continue;
      if (t == NONE) {
        switch(token.value) {
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
      switch(token.value) {
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
          if (token.value - 1 <= '0') {
            value *= 2;
            value += token.value - '0';
          } else {
            fprintf(stderr, "Error: Invalid Binary String : %02x\n", token.value);
            exit(-1);
          } 
          break;
        case DECIMAL:
          // similar BINARY
          // c - 9 <= '0' == 0x30 <= c <= '9'
          if (token.value - 9 <= '0') {
            value *= 10;
            value += token.value - '0';
          } else {
            fprintf(stderr, "Error: Invalid Decimal String : %02x\n", token.value);
            exit(-1);
          }
          break;
        case HEXADECIMAL:
          value *= 16;
          if (token.value <= '9') {
            value += token.value - '0';
          } else if (token.value <= 'f') {
            value += 10 + token.value - 'A';
          }
          break;
        case NONE:
          // should be not come here
          break;
        }
        break;
      }
    } while(1);
  vm_add_opcode(OP_NUM, value * i);
}
