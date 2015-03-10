#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/user.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <assert.h>
#include <stdio.h>
#include <fcntl.h>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

int setnonblock(int fd)
{
        int flags;

        flags = fcntl(fd, F_GETFL);
        if (flags < 0)
                return flags;
        flags |= O_NONBLOCK;
        if (fcntl(fd, F_SETFL, flags) < 0)
                return -1;

        return 0;
}

static void on_write(evutil_socket_t, short, void *);

int main(){
	
	struct event_base *base;
	struct evhttp_connection *conn;
	
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(1956);
	sin.sin_addr.s_addr= inet_addr("192.168.24.101");
	
	base = event_base_new();
	if (!base) {
		fprintf(stderr, "Couldn't create an event_base: exiting\n");
		return 1;
	}
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	assert(fd > 0);
	setnonblock(fd);
	perror("setnonblock:");
	connect(fd, (struct sockaddr*)(&sin), sizeof(struct sockaddr));
	perror("connect:");
	
	struct event *ev = event_new(base, fd, EV_WRITE|EV_PERSIST, on_write, base);
	event_add(ev, NULL);
	event_base_dispatch(base);

	event_base_free(base);
}

static void on_write(evutil_socket_t fd , short what, void *base) {
	
	struct bufferevent *bev;	
	bev = bufferevent_socket_new((struct event_base*)base, fd, BEV_OPT_CLOSE_ON_FREE);
	if (!bev) {
		fprintf(stderr, "Error constructing bufferevent!");
		event_base_loopbreak(base);
		return;
	}

	bufferevent_write(bev, "hello", strlen("hello"));
	sleep(1);
}

