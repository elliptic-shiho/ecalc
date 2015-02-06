#include "ecalc.h"

void lexical_analyze(void) {
  g_token = ll_get_instance();
  PTR(t, Token);
  void f(char c) {
    c = toupper(c);
    MALLOC(t, Token, 1);
    t->value = 0;
    switch(c) {
    case '+':
      t->type = T_ADD;
      break;
    case '-':
      if (((Token*)ll_get_data(g_token, (uint)(ll_get_pos(g_token) - 1)))->type != T_NUM) { // Minus Sign
        t->type = T_NUM;
      } else {
        t->type = T_SUB;
      }
      break;
    case '*':
      if (((Token*)ll_get_data(g_token, (uint)(ll_get_pos(g_token))))->type == T_MUL) {
        Token *mul_t = (Token*)ll_remove_data(g_token, (uint)(ll_get_pos(g_token)));
        FREE(mul_t);
        t->type = T_POW;
      } else {
        t->type = T_MUL;
      }
      break;
    case '/':
      t->type = T_DIV;
      break;
    case '(':
      t->type = T_OPEN_BRACKET;
      break;
    case ')':
      t->type = T_CLOSE_BRACKET;
      break;
    case '|':
      t->type = T_OR;
      break;
    case '&':
      t->type = T_AND;
      break;
    case '~':
      t->type = T_NOT;
      break;
    case '^':
      t->type = T_XOR;
      break;
    default:
      if ((c >= '0' && c <= '9') ||
          ((c == 'X' || (c <= 'F' && c >= 'A')) &&
           ((Token*)ll_get_data(g_token, (uint)(ll_get_pos(g_token))))->type == T_NUM)) {
        t->type = T_NUM;
        t->value = c;
        break;
      } else {
        if (isspace(c)) {
          FREE(t);
          t = NULL;
          return;
        } else {
          fprintf(stderr, "Invalid Token: %c\n", c);
          FREE(t);
          exit(-1);
        }
      }
    }
    ll_add(g_token, t);
    t = NULL;
  }

  EACH_ARRAY(f, g_expression, strlen(g_expression));

  MALLOC(t, Token, 1);
  t->type = T_EOF;
  t->value = 0;
  ll_add(g_token, t);

  ll_set_pos(g_token, 0);
}

