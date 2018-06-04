#include "utf8.h"
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <syrup/buffer.h>
#include <unistd.h>

#define SY_STR_BLOCK_SIZE 64

struct sy_buffer_s {
  unsigned char *data;
  size_t len;
  size_t alloc;
  size_t block_size;
};

static bool alloc_atleast(sy_buffer_t *str, size_t len);

static bool alloc_atleast(sy_buffer_t *str, size_t len) {
  int i = ceil((double)len / (double)SY_STR_BLOCK_SIZE);
  int nsize = i * SY_STR_BLOCK_SIZE;
  unsigned char *data = realloc(str->data, sizeof(unsigned char) * nsize);

  if (!data) {
    return false;
  }
  str->data = data;
  str->alloc = nsize;

  return true;
}

sy_buffer_t *sy_buffer_alloc() {
  sy_buffer_t *str = (sy_buffer_t *)malloc(sizeof(sy_buffer_t));
  if (!str)
    return NULL;

  str->data = malloc(sizeof(char) * SY_STR_BLOCK_SIZE);
  str->alloc = SY_STR_BLOCK_SIZE;
  str->len = 0;
  str->block_size = SY_STR_BLOCK_SIZE;

  return str;
}
void sy_buffer_free(sy_buffer_t *str) {
  if (!str)
    return;

  free(str->data);
  str->data = NULL;
  free(str);
}
// Appending
void sy_buffer_append(sy_buffer_t *buffer, const unsigned char *buf,
                      size_t len) {

  size_t nlen = len + buffer->len;

  if (nlen >= buffer->alloc) {
    if (!alloc_atleast(buffer, nlen)) {
      return;
    }
  }

  memcpy(buffer->data + buffer->len, buf, len);
  buffer->len = nlen;
}
void sy_buffer_append_str(sy_buffer_t *buffer, const char *s) {
  size_t len = strlen(s);
  size_t nlen = len + buffer->len;

  if (nlen >= buffer->alloc) {
    if (!alloc_atleast(buffer, nlen)) {
      return;
    }
  }

  memcpy(buffer->data + buffer->len, s, len);
  buffer->len = nlen;
}
void sy_buffer_append_str_fmt(sy_buffer_t *buffer, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char *out;
  if (vasprintf(&out, fmt, args) == -1) {
    return;
  }
  va_end(args);
  sy_buffer_append_str(buffer, out);
  free(out);
}
void sy_buffer_append_char(sy_buffer_t *buffer, unsigned char c) {
  size_t nlen = 1 + buffer->len;

  if (nlen >= buffer->alloc) {
    if (!alloc_atleast(buffer, nlen)) {
      return;
    }
  }
  buffer->data[buffer->len] = c;
  buffer->len++;
}

void sy_buffer_insert(sy_buffer_t *buffer, size_t idx, const unsigned char *buf,
                      size_t len) {
  if (idx > buffer->len) {
    return;
  }
  if (idx == buffer->len) {
    sy_buffer_append(buffer, buf, len);
    return;
  }

  int nlen = buffer->len + len;

  if (nlen >= buffer->alloc) {
    if (!alloc_atleast(buffer, nlen)) {
      return;
    }
  }

  int l = buffer->len - idx;
  char tmp[l];
  memcpy(tmp, buffer->data + idx, l);
  memcpy(buffer->data + idx, buf, len);
  memcpy(buffer->data + idx + len, tmp, l);
  buffer->len = nlen;
}
void sy_buffer_insert_str(sy_buffer_t *buffer, size_t idx, const char *s) {
  if (idx > buffer->len) {
    return;
  }
  if (idx == buffer->len) {
    sy_buffer_append_str(buffer, s);
    return;
  }

  int len = strlen(s);
  int nlen = buffer->len + len;

  if (nlen >= buffer->alloc) {
    if (!alloc_atleast(buffer, nlen)) {
      return;
    }
  }

  int l = buffer->len - idx;
  char tmp[l];
  memcpy(tmp, buffer->data + idx, l);
  memcpy(buffer->data + idx, s, len);
  memcpy(buffer->data + idx + len, tmp, l);
  buffer->len = nlen;
}

void sy_buffer_insert_char(sy_buffer_t *buffer, size_t idx, unsigned char c) {
  int nlen = buffer->len + 1;
  if (nlen >= buffer->alloc) {
    if (!alloc_atleast(buffer, nlen)) {
      return;
    }
  }

  if (idx >= buffer->len) {
    sy_buffer_append_char(buffer, c);
  } else {
    int l = buffer->len - idx;
    char tmp[l];
    memcpy(tmp, buffer->data + idx, l);
    buffer->data[idx] = c;
    memcpy(buffer->data + idx + 1, tmp, l);
    buffer->len = nlen;
  }
}

size_t sy_buffer_indexof(sy_buffer_t *buffer, unsigned char c) {
  for (int i = 0; i < buffer->len; i++) {
    if (buffer->data[i] == c)
      return i;
  }
  return -1;
}

size_t sy_buffer_lastindexof(sy_buffer_t *buffer, unsigned char c) {
  int i = buffer->len;
  while (i >= 0) {
    if (buffer->data[--i] == c)
      return i;
  }
  return -1;
}

void sy_buffer_index_set(sy_buffer_t *str, size_t idx, unsigned char c) {
  if (idx >= str->len)
    return;
  str->data[idx] = c;
}
// Get
unsigned sy_buffer_index_get(sy_buffer_t *str, size_t idx) {
  if (idx >= str->len)
    return -1;
  return str->data[idx];
}
void sy_buffer_copy(sy_buffer_t *buffer, unsigned char *buf) {
  memcpy(buf, buffer->data, buffer->len);
}
char *sy_buffer_string(sy_buffer_t *buffer) {
  char *out = malloc(sizeof(char) * buffer->len + 1);
  sy_buffer_copy(buffer, (unsigned char *)out);
  out[buffer->len] = '\0';
  return out;
}
// Remove
void sy_buffer_remove(sy_buffer_t *buffer, size_t idx, size_t len) {

  if (idx + len > buffer->len) {
    return;
  }

  unsigned char *ptr =
      buffer->data + idx; // idx > 0 ? buffer->data + idx : buffer->data;
  memcpy(ptr, buffer->data + (idx) + len, buffer->len - (idx + len));
  buffer->len -= len;
}
void sy_buffer_clear(sy_buffer_t *buffer) {
  buffer->len = 0;
  memset(buffer->data, 0, buffer->alloc);
}

void sy_buffer_compact(sy_buffer_t *buffer) {
  int len = buffer->len;
  if (0 == len) {
    len = SY_STR_BLOCK_SIZE;
  }
  alloc_atleast(buffer, len);
}

// Stats
size_t sy_buffer_len(sy_buffer_t *buffer) { return buffer->len; }
size_t sy_buffer_allocs(sy_buffer_t *buffer) { return buffer->alloc; }

ssize_t sy_buffer_write(sy_buffer_t *buffer, int fd) {
  return write(fd, buffer->data, buffer->len);
}
// UTF-8

static int utf_width(char c) {
  if (SY_IS_UTF8_2C(c))
    return 2;
  else if (SY_IS_UTF8_3C(c))
    return 3;
  else if (SY_IS_UTF8_4C(c))
    return 4;
  return 1;
}

void sy_buffer_utf8_append(sy_buffer_t *str, const char *buf) {
  if (utf8valid(buf)) {
    return;
  }
  sy_buffer_append_str(str, buf);
}
void sy_buffer_utf8_appendf(sy_buffer_t *str, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char *out;
  if (vasprintf(&out, fmt, args) == -1) {
    return;
  }
  va_end(args);
  sy_buffer_utf8_append(str, out);
  free(out);
}

static size_t find_index(sy_buffer_t *str, size_t idx) {
  size_t i = 0;
  while (i < str->len) {
    // if (i == idx)
    // return i;

    if (SY_IS_UTF(str->data[i])) {
      i += utf_width(str->data[i]);
      // idx += utf_width(str->data[i]);
    } else {
      i++;
    }

    if (i >= idx)
      return i;
  }
  return -1;
}

static size_t find_rindex(sy_buffer_t *str, size_t idx) {

  size_t i = 0;
  while (i < str->len) {

    if (SY_IS_UTF(str->data[i])) {
      i += 1; // utf_width(str->data[i]);
      idx += utf_width(str->data[i]);
    } else {
      i++;
    }

    if (i >= idx)
      return i;
  }
  return -1;
}

int sy_buffer_utf8_insert(sy_buffer_t *str, size_t idx, const char *buf) {
  if (utf8valid(buf)) {
    return 0;
  }

  size_t u8len = sy_buffer_utf8_len(str);

  if (idx > u8len) {
    return 0;
  } else if (idx == u8len) {
    sy_buffer_append_str(str, buf);
    return u8len;
  } else if (idx == 0) {
    sy_buffer_insert_str(str, 0, buf);
    return u8len;
  }

  size_t i = find_index(str, idx);
  sy_buffer_insert_str(str, i, buf);

  return u8len;
}

int sy_buffer_utf8_insert_char(sy_buffer_t *str, size_t idx, char c) {

  size_t len = sy_buffer_utf8_len(str);

  if (idx > len) {
    return 0;
  } else if (idx == len) {
    sy_buffer_append_char(str, c);
    return 1;
  } else if (idx == 0) {
    sy_buffer_insert_char(str, 0, c);
    return 1;
  }

  size_t i = find_index(str, idx);
  sy_buffer_insert_char(str, i, c);

  return 1;
}

int sy_buffer_utf8_remove(sy_buffer_t *str, size_t idx, size_t len) {
  if (idx >= str->len)
    return 0;

  if (idx != 0) {
    idx = find_rindex(str, idx);
  }
  if (len == 1)
    len = 0;

  size_t eidx = find_index(str, idx + len + 1);
  size_t nlen = eidx - idx;

  sy_buffer_remove(str, idx, nlen);

  return 1;
}

size_t sy_buffer_utf8_len(sy_buffer_t *str) {
  size_t length = 0;
  size_t i = 0;
  while (i < str->len) {
    if (SY_IS_UTF(str->data[i])) {
      i += utf_width(str->data[i]);
    } else {
      i++;
    }
    length++;
  }

  return length;
}
