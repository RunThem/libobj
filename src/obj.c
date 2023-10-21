#include "obj.h"

#include <sys/mman.h>
#include <sys/user.h>

/*
 * -[clofn](https://github.com/yulon/clofn)
 * -[obj.h](https://github.com/small-c/obj.h)
 * */

#define check(expr)                                                                                \
  do {                                                                                             \
    if (expr) {                                                                                    \
      goto err;                                                                                    \
    }                                                                                              \
  } while (0)

#if defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
#  define __OBJ_X64 1
#elif defined(__i386) || defined(__i386__) || defined(_M_IX86)
#  define __OBJ_X86 1
#else
#  error This architecture is not supported!
#endif

void* __obj_method(void* prototype, void* data) {
  int ret       = 0;
  uint8_t* code = NULL;
  size_t offset = 0;
  size_t ihsize = 0;

  for (offset = 0; offset < __OBJ_MAXPHSIZE; offset++) {
    if (*(size_t*)((uintptr_t)prototype + offset) == (size_t)__OBJ_CLOFNNUM) {
      break;
    }
  }

  check(offset == 0 || offset == __OBJ_MAXPHSIZE);

#pragma pack(1)
#if __OBJ_X64

  /*
   *  push rax
   *  mov  rax, addr
   *  jmp  rax
   * */
  static struct {
    uintptr_t data;
    uint8_t push_rax;
    uint8_t mov_rax[2];
    uintptr_t addr;
    uint8_t jmp_rax[2];
  } asm_code = {
      .push_rax = 0x50,
      .mov_rax  = {0x48, 0xB8},
      .jmp_rax  = {0xFF, 0xE0}
  };
  /* ihsize = offset + sizeof(any_t) * 2 + 5; */

#elif __OBJ_X86
  /*
   * jmp addr
   * */
  static struct {
    uintptr_t data;
    uint8_t jmp;
    uintptr_t addr;
  } asm_code    = {.jmp = 0xE9};
  /* ihsize = offset + sizeof(any_t) * 2 + 1; */

#endif
#pragma pack()

  ihsize = offset + sizeof(asm_code);
  code   = obj_alloc(ihsize * sizeof(int8_t));
  check(code == NULL);

  ret = mprotect((void*)(((uintptr_t)code >> PAGE_SHIFT) << PAGE_SHIFT),
                 ihsize,
                 PROT_READ | PROT_EXEC | PROT_WRITE);
  check(ret == -1);

#if __OBJ_X64
  asm_code.addr = (uintptr_t)prototype + offset + sizeof(uintptr_t) - 1;
#elif __OBJ_X86
  asm_code.addr = ((uintptr_t)prototype + offset + sizeof(uintptr_t)) - ((uintptr_t)code + ihsize);
#endif

  asm_code.data = (uintptr_t)data;

  memcpy(code, prototype, offset);
  memcpy(code + offset, &asm_code, sizeof(asm_code));

  return (void*)(code);

err:
  if (code != NULL) {
    obj_free(code);
  }

  return NULL;
}
