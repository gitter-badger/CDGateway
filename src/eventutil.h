#ifndef __EVENTUTIL_H__
#define __EVENTUTIL_H__

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/event.h>

void eventutil_init();
struct event_base * eventutil_getevent();
void eventutil_free();
#endif //__EVENTUTIL_H__
