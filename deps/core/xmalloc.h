#ifndef __XMALLOC_H__
#define __XMALLOC_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void *xmalloc(size_t size);
extern void *xcalloc(size_t nmemb, size_t size);
extern void *xrealloc(void *ptr, size_t size);
extern void xfree(void *ptr);
extern void *xclone(void const *ptr, size_t size);
extern char *xstrdup(char const *s);
extern char *xstrndup(char const *s, size_t max_len);

#ifdef __cplusplus
}
#endif

#endif
