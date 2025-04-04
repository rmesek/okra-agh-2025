#include <stdio.h>

static inline int max_inline(const int a, const int b) {
  return (a > b) ? a : b;
}

#define max_macro(a, b) ((a) > (b) ? (a) : (b))

int main(int argc, char** argv) {
  unsigned int a;
  int b, max_num;

  a = 1, b = 0;
  max_num = max_macro(a-3, b);
  printf("max_num_macro=%d\n", max_num);
  printf("a=%d b=%d\n", a, b);
  
  a = 1, b = 0;
  max_num = max_inline(a-3, b);
  printf("max_num_inline=%d\n", max_num);
  printf("a=%d b=%d\n", a, b);
  
  return 0;
}
