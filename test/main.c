#include <unity.h>

extern void test_buffer();
extern void test_buffer_utf8();
extern void test_buffer_utf8_size();

int main() {
  UNITY_BEGIN();

  RUN_TEST(test_buffer);
  RUN_TEST(test_buffer_utf8);
  RUN_TEST(test_buffer_utf8_size);

  return UNITY_END();
}