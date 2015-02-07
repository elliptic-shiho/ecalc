#include "ecalc.h"

char *g_expression;
LinkedList g_token;

Token get_token(void) {
  Token t = *((Token*)ll_get_current_data(g_token));
  if (ll_get_pos(g_token) + 1 < ll_count(g_token)) {
    ll_set_pos_relative(g_token, 1);
  }
  return t;
}

void unget_token(void) {
  if (ll_get_pos(g_token) != 0) {
    ll_set_pos_relative(g_token, -1);
  }
}

void free_token(void) {
  void f(LLData_t t) {
    FREE(t);
  }
  ll_foreach(g_token, f, 0);
  ll_free(g_token);
}

int main (int ac, char **av) {
  if (ac < 2) {
    fprintf(stderr, "Usage: %s Expression\n", av[0]);
    return -1;
  }
  g_expression = av[1];
  vm_init();

  lexical_analyze();

  parse();

  vm_execute();

  free_token();
  vm_free();
  return 0;
}
