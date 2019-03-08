#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SY_IS_UTF8_4C(k) (0xf0 == (0xf8 & (k)))
#define SY_IS_UTF8_3C(k) (0xe0 == (0xf0 & (k)))
#define SY_IS_UTF8_2C(k) (0xc0 == (0xe0 & (k)))
#define SY_IS_UTF(k) (SY_IS_UTF8_4C(k) || SY_IS_UTF8_3C(k) || SY_IS_UTF8_2C(k))

typedef struct sy_buffer_s sy_buffer_t;

sy_buffer_t *sy_buffer_alloc();
void sy_buffer_free(sy_buffer_t *);
// Appending
void sy_buffer_append(sy_buffer_t *, const unsigned char *, size_t size);
void sy_buffer_append_str(sy_buffer_t *, const char *);
void sy_buffer_append_str_fmt(sy_buffer_t *, const char *fmt, ...);
void sy_buffer_append_char(sy_buffer_t *, unsigned char);
void sy_buffer_append_buffer(sy_buffer_t *, sy_buffer_t *);
void sy_buffer_insert(sy_buffer_t *, size_t idx, const unsigned char *, size_t);
void sy_buffer_insert_str(sy_buffer_t *, size_t idx, const char *);
void sy_buffer_insert_char(sy_buffer_t *, size_t idx, unsigned char);
void sy_buffer_index_set(sy_buffer_t *str, size_t idx, unsigned char c);
// Get
unsigned sy_buffer_index_get(sy_buffer_t *str, size_t idx);
void sy_buffer_copy(sy_buffer_t *, unsigned char *buf);
char *sy_buffer_string(sy_buffer_t *);
size_t sy_buffer_indexof(sy_buffer_t *, unsigned char);
size_t sy_buffer_lastindexof(sy_buffer_t *buffer, unsigned char c);
// Remove
void sy_buffer_remove(sy_buffer_t *, size_t idx, size_t len);
void sy_buffer_clear(sy_buffer_t *);
void sy_buffer_compact(sy_buffer_t *);
// Stats
size_t sy_buffer_len(sy_buffer_t *);
size_t sy_buffer_allocs(sy_buffer_t *);

ssize_t sy_buffer_write(sy_buffer_t *buffer, int fd);

// UTF-8
void sy_buffer_utf8_append(sy_buffer_t *, const char *);
void sy_buffer_utf8_appendf(sy_buffer_t *, const char *fmt, ...);
int sy_buffer_utf8_insert(sy_buffer_t *, size_t idx, const char *);
int sy_buffer_utf8_insert_char(sy_buffer_t *, size_t idx, char);
int sy_buffer_utf8_remove(sy_buffer_t *, size_t idx, size_t len);
size_t sy_buffer_utf8_len(sy_buffer_t *);

#ifdef __cplusplus
}
#endif