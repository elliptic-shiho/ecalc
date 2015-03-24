#include "ecalc.h"

#define GET_CURRENT_TOKEN() ((Token*)ll_get_data(g_token, (uint)(ll_get_pos(g_token))))

Token* gen_token(TokenKind kind, char* value) {
  PTR(t, Token);
  MALLOC(t, Token, 1);
  if (kind == T_NONE) {
    FREE(t);
    return NULL;
  }
  t->type = kind;
  t->value = value;
  DEBUG("%p", t);
  return t;
}

void lexical_analyze(void) {
  static char buf[2] = {0};
  Token t;
  PTR(ret, Token);

  g_token = ll_get_instance();

  void f(char c) {
    c = toupper(c);
    switch(c) {
    case '+':
      t.type = T_ADD;
      break;
    case '-':
      if (ll_count(g_token) < 1 || GET_CURRENT_TOKEN()->type != T_NUM) { // Minus Sign
        t.type = T_NUM;
        buf[0] = '-';
        t.value = strdup(buf);
      } else {
        t.type = T_SUB;
      }
      break;
    case '*':
      if (ll_count(g_token) > 0 && GET_CURRENT_TOKEN()->type == T_MUL) {
        GET_CURRENT_TOKEN()->type = T_POW;
      } else {
        t.type = T_MUL;
      }
      break;
    case '/':
      t.type = T_DIV;
      break;
    case '(':
    case '{':
      t.type = T_OPEN_BRACKET;
      break;
    case ')':
    case '}':
      t.type = T_CLOSE_BRACKET;
      break;
    case '|':
      t.type = T_OR;
      break;
    case '&':
      t.type = T_AND;
      break;
    case '~':
      t.type = T_NOT;
      break;
    case '^':
      t.type = T_XOR;
      break;
    case ',':
      t.type = T_SPLIT;
      break;
    default:
      if ((c >= '0' && c <= '9') ||
         ((c == 'X' || (c <= 'F' && c >= 'A')) &&
           GET_CURRENT_TOKEN()->type == T_NUM)) {
        buf[0] = c;
        if (ll_count(g_token) > 0 && GET_CURRENT_TOKEN()->type == T_NUM) {
          GET_CURRENT_TOKEN()->value = strcat(GET_CURRENT_TOKEN()->value, buf);
        } else {
          t.type = T_NUM;
          t.value = strdup(buf);
        }
        break;
      } else {
        if (isspace(c)) {
          return;
        } else {
          fprintf(stderr, "Invalid Token: %c\n", c);
          exit(-1);
        }
      }
    }
    ret = gen_token(t.type, t.value);
    if (ret != NULL) {
      ll_add(g_token, ret);
      DEBUG("%p", ret);
    }
    t.type = T_NONE;
    t.value = NULL;
    buf[0] = 0;
    buf[1] = 0;
    ret = NULL;
  }

  EACH_ARRAY(f, g_expression, strlen(g_expression));

  ret = gen_token(T_EOF, NULL);
  DEBUG("%p", ret);
  ll_add(g_token, ret);

  ll_set_pos(g_token, 0);
}

