#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/user.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <vector>

#include <map>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include "client.hpp"
#include "server.hpp"
#include "dispatcher.hpp"
#include "redisop.hpp"

extern "C"{
	#include "eventutil.h"
}

static void signal_cb(evutil_socket_t sig, short events, void *user_data);

using namespace std;
using namespace gate;

int main(){
	
	eventutil_init();
	struct event_base *base = eventutil_getevent();
	
	init_spy();
	Client::init();
	
	struct event *signal_event;

	signal_event = evsignal_new(base, SIGINT, signal_cb, (void *)base);

	if (!signal_event || event_add(signal_event, NULL)<0) {
		fprintf(stderr, "Could not create/add a signal event!\n");
		return 1;
	}
	
	event_base_dispatch(base);
	Client::free();

	event_free(signal_event);
	eventutil_free();
}



static void
signal_cb(evutil_socket_t sig, short events, void *user_data)
{
	struct event_base *base = (struct event_base *)user_data;
	struct timeval delay = { 0, 0 };

	printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");

	event_base_loopexit(base, &delay);
}

