
#include <string.h>

#include <teavpn2/global/common.h>

static void   *__t_arena     = NULL;
static size_t  __t_arena_len = 0;
static size_t  __t_arena_pos = 0;

#if defined(__x86_64__)
#define t_ar_memcpy(DST, SRC, N)    \
  __asm__(                          \
    "mov %0, %%rdi;"                \
    "mov %1, %%rsi;"                \
    "mov %2, %%rcx;"                \
    "rep movsb"                     \
    :                               \
    : "r"(DST), "r"(SRC), "r"(N)    \
    : "rdi", "rsi", "rcx"           \
  )
#else
#define t_ar_memcpy memcpy
#endif


void t_ar_init(register void *ptr, register size_t len)
{
  __t_arena     = ptr;
  __t_arena_len = len;
}


void *t_ar_alloc(register size_t len)
{
  register char *ret  = &(((char *)__t_arena)[__t_arena_pos]);
  __t_arena_pos      += len;

  return (void *)ret;
}


char *t_ar_strdup(register const char *str)
{
  register size_t len = strlen(str);
  register char *ret  = &(((char *)__t_arena)[__t_arena_pos]);
  __t_arena_pos      += len + 1;

  t_ar_memcpy(ret, str, len);
  ret[len] = '\0';

  return ret;
}


char *t_ar_strndup(register const char *str, register size_t tlen)
{
  register size_t len = strlen(str);
  register char *ret  = &(((char *)__t_arena)[__t_arena_pos]);

  if (len < tlen) {
    __t_arena_pos += len + 1;
    t_ar_memcpy(ret, str, len);
    ret[len] = '\0';
  } else {
    __t_arena_pos += tlen;
    t_ar_memcpy(ret, str, tlen);
  }

  return ret;
}
