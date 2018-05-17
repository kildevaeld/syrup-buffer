#include <stdlib.h>
#include <syrup/buffer.h>
#include <unity.h>

void test_buffer_utf8_size() {

  sy_buffer_t *str = sy_buffer_alloc();

  for (int i = 0; i < 1000; i++) {
    sy_buffer_utf8_append(str, "T");
  }
  printf("%zu\n", sy_buffer_len(str));
}

void test_buffer_utf8() {
  sy_buffer_t *str = sy_buffer_alloc();

  sy_buffer_utf8_append(str, "Kød");
  TEST_ASSERT_EQUAL_STRING("Kød", sy_buffer_string(str));
  TEST_ASSERT_EQUAL(3, sy_buffer_utf8_len(str));
  TEST_ASSERT_EQUAL(4, sy_buffer_len(str));

  sy_buffer_utf8_insert(str, 1, "å");
  TEST_ASSERT_EQUAL_STRING("Kåød", sy_buffer_string(str));

  sy_buffer_utf8_insert(str, 4, "piæ");
  TEST_ASSERT_EQUAL_STRING("Kåødpiæ", sy_buffer_string(str));

  sy_buffer_utf8_remove(str, 1, 2);
  TEST_ASSERT_EQUAL_STRING("Kdpiæ", sy_buffer_string(str));
}

void test_buffer() {
  sy_buffer_t *str = sy_buffer_alloc();
  TEST_ASSERT_NOT_NULL(str);

  sy_buffer_append_str(str, "Hello, World");
  TEST_ASSERT_EQUAL(12, sy_buffer_len(str));
  unsigned char buf[13];
  sy_buffer_copy(str, buf);
  buf[12] = '\0';
  TEST_ASSERT_EQUAL_STRING("Hello, World", buf);

  sy_buffer_append_str(str, "Hello to you too.");
  TEST_ASSERT_EQUAL(12 + 17, sy_buffer_len(str));

  unsigned char buf2[12 + 17 + 1];
  sy_buffer_copy(str, buf2);
  buf2[12 + 17] = '\0';
  TEST_ASSERT_EQUAL_STRING("Hello, WorldHello to you too.", buf2);

  int len = sy_buffer_len(str);
  sy_buffer_remove(str, 7, 5);
  TEST_ASSERT_EQUAL(len - 5, sy_buffer_len(str));
  sy_buffer_copy(str, buf2);
  buf2[sy_buffer_len(str)] = '\0';
  TEST_ASSERT_EQUAL_STRING("Hello, Hello to you too.", buf2);

  sy_buffer_insert_str(str, 7, "World. ");

  unsigned char buf4[200];
  sy_buffer_copy(str, buf4);
  buf4[sy_buffer_len(str)] = '\0';
  TEST_ASSERT_EQUAL_STRING("Hello, World. Hello to you too.", buf4);

  sy_buffer_clear(str);
  sy_buffer_compact(str);

  sy_buffer_append_str(str, "Hello");
  TEST_ASSERT_EQUAL(5, sy_buffer_len(str));

  sy_buffer_remove(str, 4, 1);
  TEST_ASSERT_EQUAL_STRING("Hell", sy_buffer_string(str));

  sy_buffer_insert_char(str, 2, 'o');
  printf("%s\n", sy_buffer_string(str));
}