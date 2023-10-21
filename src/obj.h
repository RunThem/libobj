#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define __OBJ_MAXPHSIZE 1024
#define __OBJ_CLOFNNUM  0x58ffffbffdffffafULL

#ifndef obj_alloc
#  define obj_alloc(size) calloc(1, size)
#  define obj_free(mem)   free(mem)
#endif

#define obj_self(T, var)                                                                           \
  volatile uintptr_t __obj_##var = __OBJ_CLOFNNUM;                                                 \
  T var                          = (void*)(__obj_##var);

#define obj_cleanup(method)                                                                        \
  do {                                                                                             \
    obj_free((void*)method); /* NOLINT */                                                          \
  } while (0)

void* __obj_method(void* prototype, void* data);
#define obj_method(self, method, func)                                                             \
  do {                                                                                             \
    (self)->method = __obj_method(func, self);                                                     \
  } while (0)
