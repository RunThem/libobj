#include "obj.h"

#include <assert.h>
#include <stdio.h>

typedef struct {
  int num;

  int (*get)(void);
  void (*set)(int);
  void (*sum)(int, int, int, int, int, int, int, int, int);
  void (*print)(void);
} st_t;

void st_set(int num) {
  obj_self(st_t*, self);

  self->num = num;
}

int st_get(void) {
  obj_self(st_t*, self);

  return self->num;
}

void st_sum(int a, int b, int c, int d, int e, int f, int g, int h, int k) {
  obj_self(st_t*, self);

  self->num = a + b + c + d + e + f + g + h + k;
}
void st_print(void) {
  obj_self(st_t*, self);

  printf("sum is %d\n", self->num);
}

st_t* st_new() {
  st_t* self = malloc(sizeof(st_t));

  obj_method(self, sum, st_sum);
  obj_method(self, get, st_get);
  obj_method(self, set, st_set);
  obj_method(self, print, st_print);

  return self;
}

void st_cleanup(st_t* self) {
  assert(self != NULL);

  obj_cleanup(self->get);
  obj_cleanup(self->set);
  obj_cleanup(self->sum);
  obj_cleanup(self->print);

  free(self);
}

int main() {
  st_t* st = st_new();
  st->print();

  st->set(10);
  st->print();

  st->set(9234);
  st->print();

  st->sum(1, 2, 3, 4, 5, 6, 7, 8, 9);
  st->print();

  st_cleanup(st);

  return 0;
}
