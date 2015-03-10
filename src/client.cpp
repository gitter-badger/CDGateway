#include <iostream>
#include <map>
#include <boost/lexical_cast.hpp>

extern "C"{
	#include <errno.h>
	#include <assert.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <iostream>
	#include <sys/stat.h>
	#include <unistd.h>
	#include <stdlib.h>
	#include <errno.h>
	#include <string.h>
	#include <stdio.h>
	
	#include <event2/event.h>
	#include <event2/bufferevent.h>
	#include <event2/buffer.h>
	#include <event2/util.h>
	
	#include "eventutil.h"
}

#include "gatefwd.h"
#include "client.hpp"
#include "coder.hpp"

using namespace std; 

map<string, gate::Client_t> clients;

namespace gate
{
	

static void conn_eventcb(bufferevent* bev, short events, void* client)
{
	Client *cli = (Client*)client;	
	
	if (events & BEV_EVENT_EOF) {
		cli->logout();
		printf("Connection closed.\n");
	} else if (events & BEV_EVENT_ERROR) {
		cli->logout();
		printf("Got an error on the connection: %s\n",
		    strerror(errno));/*XXX win32*/
	}

}

extern void conn_readcb(bufferevent* bev, void* cli)
{
	Client *client = (Client*)cli;
	struct evbuffer *input = bufferevent_get_input(bev);
	size_t input_len = evbuffer_get_length(input);
	char data[100];
	int read_bytes = evbuffer_remove(input, data, sizeof(data));
	while(read_bytes > 0)
	{
		cout << "read " << read_bytes << " byte. ";
		cout << "data " << data << endl;
		client->m_coder.append(data, read_bytes);
		string result = client->m_coder.decode();
		cout << "result " << result << endl;
		
		bzero(data, sizeof(data));
		read_bytes = evbuffer_remove(input, data, sizeof(data));
		
	}
}

static void conn_writecb(bufferevent* bev, void* cli)
{
	cout << "writecb" << endl;
	
}

Client::Client(struct event_base* base, evutil_socket_t fd, struct sockaddr *socket, int socket_len)
{
	m_buffer = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	if (!m_buffer) 
	{
		fprintf(stderr, "Error constructing bufferevent!");
		event_base_loopbreak(base);
		throw;
	}
	
	bufferevent_setcb(m_buffer, conn_readcb, conn_writecb, conn_eventcb, this);
	bufferevent_enable(m_buffer, EV_READ|EV_WRITE);
//	bufferevent_setwatermark(m_buffer, EV_READ, 128, 0);
	
	struct timeval time1, time2;
//	time1.tv_sec = 10;
//	time2.tv_sec = 11;
//	bufferevent_set_timeouts(m_buffer, &time1, &time2);
	
	if(socket->sa_family == AF_INET)
	{
		struct sockaddr_in *socket4 = (struct sockaddr_in*)socket;
		m_ip = string(inet_ntoa(socket4->sin_addr));
		m_port = ntohs(socket4->sin_port);
	}
	
	bufferevent_write(m_buffer, "hello", strlen("hello"));//tmp
	
}

Client::~Client()
{
	bufferevent_free(m_buffer);
}

std::string Client::key()
{
	return m_ip + "." + boost::lexical_cast<string>(m_port);
}

void gate::Client::login()
{
	if(clients[key()]){
		std::cerr << "client exist" <<endl;
		return;
	}
	clients[key()] = Client_t(this);
}

void gate::Client::logout()
{
	clients.erase(key());
}

static void
listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sock, int socklen, void *base)
{
	gate::Client *client = new gate::Client((struct event_base*)base, fd, sock, socklen);
	
	client->login();
	
}
struct evconnlistener *listener;

void gate::Client::init()
{
	struct sockaddr_in sin;
	unsigned short port = 8888;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	
	struct event_base *base = eventutil_getevent();
	if (!base) {
		fprintf(stderr, "Couldn't create an event_base: exiting\n");
		return;
	}
	
	listener = evconnlistener_new_bind(base, listener_cb, (void *)base,
	    LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
	    (struct sockaddr*)&sin,
	    sizeof(sin));

	if (!listener) {
		fprintf(stderr, "Could not create a listener!\n");
		return;
	}
}

void gate::Client::free(){
	cout << "client listen free" << endl;
	evconnlistener_free(listener);
}

}// end namespace gate




