#include "eventutil.h"
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

struct event_base *base;

void eventutil_init(){
	base = event_base_new();
	if (!base) {
//		cerr << "Couldn't create an event_base: exiting" << endl;
		return;
	} 
}

struct event_base * eventutil_getevent(){
	return base;
}

void eventutil_free(){
	event_base_free(base);
}
