#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

static void die(void) {
  assert(0 && "out of memory"); // Runs on debug build only.
  fprintf(stderr, "out of memory\n");
  exit(1);
}

void *xmalloc(size_t size) {
  assert(size > 0);
  void *ptr = malloc(size);

  if (ptr == NULL && size) {
    die();
  }

  return ptr;
}

void *xcalloc(size_t nmemb, size_t size) {
  assert(nmemb > 0);
  assert(size > 0);

  void *ptr = calloc(nmemb, size);

  if (ptr == NULL && nmemb && size) {
    die();
  }

  return ptr;
}

/**
 *  ptr, !size --> free
 * !ptr,  size --> malloc
 *  ptr,  size --> realloc
 * !ptr, !size --> NULL
 */
void *xrealloc(void *ptr, size_t size) {
  assert(ptr || size);
  ptr = realloc(ptr, size);

  if (ptr == NULL && size) {
    die(); // malloc or realloc failed
  }

  return ptr;
}

void xfree(void *ptr) {
  if (ptr) {
    free(ptr);
  }
}

void *xclone(void const *ptr, size_t size) {
  assert(ptr);

  void *cloned_ptr = xmalloc(size);
  memcpy(cloned_ptr, ptr, size);

  return cloned_ptr;
}

char *xstrdup(char const *s) {
  char *s2 = strdup(s);

  if (s2 == NULL) {
    die();
  }

  return s2;
}

#if defined(_MSC_VER) || defined(__MINGW32__)
static char *strndup(char const *s, size_t max_len) {
  if (s == NULL) {
    return NULL;
  }

  size_t size = 0;

  for (size_t i = 0; i < max_len; i++) {
    if (s[i] == 0) {
      size = i;
      break;
    }
  }

  if (size == 0) {
    return NULL;
  }

  char *s2 = xmalloc(size);
  memcpy(s2, s, size);

  return s2;
}
#endif

char *xstrndup(char const *s, size_t max_len) {
  char *s2 = strndup(s, max_len);

  if (s2 == NULL) {
    die();
  }

  return s2;
}
