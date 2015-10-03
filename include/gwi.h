#ifndef MRUBY_GWI_H
#define MRUBY_GWI_H 1

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gwi_Event gwi_Event;

typedef void (*gwi_loop_callback)(void *);
typedef void (*gwi_event_callback)(void *, gwi_Event *);

struct gwi_Event
{
  const char *name;
  
};

void 
gwi_init(const char *title, size_t width, size_t height);

void 
gwi_end(void);

void
gwi_main_loop(void *context, gwi_loop_callback callback);

void
gwi_message(const char *title, const char *msg);

void 
gwi_error(const char *title, const char *msg);

void 
gwi_info(const char *title, const char *msg);

void 
gwi_warning(const char *title, const char *msg);

int
gwi_ask(const char *title, const char *msg);

int
gwi_accept(const char *title, const char *msg);

void
gwi_on(const char *name, void *context, gwi_event_callback event);

void
gwi_off(const char *name);

void
gwi_fire(const char *name, gwi_Event *event);

#ifdef __cplusplus
}
#endif

#endif /* MRUBY_GWI_H */