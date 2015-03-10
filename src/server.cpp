#include "server.hpp"
#include "gatefwd.h"

#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>

extern "C"{
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
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <signal.h>
	
	#include <event2/bufferevent.h>
	#include <event2/buffer.h>
	#include <event2/util.h>
	#include <event2/event.h>
	
	#include "eventutil.h"
}

using namespace std;

namespace gate
{

void read_callback(struct bufferevent *bev, void *ser)
{
	cout << "read"<< endl;
	bufferevent_free(bev);
	
	Server *server = (Server *)ser;
    char buf[1024];
    // 从bufferevent中，读取接收到的消息并打印
    size_t sz = bufferevent_read(bev, buf, 1024);
    printf("received bytes: %d\n", sz);
    if ( sz > 0 ) {
        printf("received: %s\n", buf);
    }
}

void write_callback(struct bufferevent *bev, void *server)
{
	cout << "write callback" << endl;
}

void error_callback(struct bufferevent *bev, short events, void *server)
{
	cout << "eve" << endl;
	if (events & BEV_EVENT_CONNECTED) {
        printf("***BEV_EVENT_CONNECTED\n");
    } else if (events & BEV_EVENT_ERROR) {
        printf("***BEV_EVENT_ERROR\n");
    }
}

Server::~Server()
{
	cout << key() << "deconstruster" << endl;
}


void gate::Server::disconnect()
{
	
}


const std::string gate::Server::key()
{
	return m_info.ip + "." + boost::lexical_cast<string>(m_info.port);
}

void gate::Server::connect()
{
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(m_info.port);
	inet_pton(AF_INET, m_info.ip.c_str(), &sin.sin_addr);
	
	struct event_base *base = eventutil_getevent();
	assert(base);
	
	
	struct bufferevent* bufevt = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
	if(!bufevt)
	{
		cerr << "error on bufferevent_socket_new" << endl;
		return;
	}
	bufferevent_setcb(bufevt, read_callback, write_callback, error_callback, this);
	bufferevent_enable(bufevt, EV_READ|EV_WRITE);
	if(bufferevent_socket_connect(bufevt, (sockaddr*)&sin, sizeof(sin)) < 0)
	{
		cerr << "error on bufferevent_socket_connect" << endl;
		return;
	}
	cout << "server " << name() << " " << key() << " connected" << endl;
}

const std::string Server::name()
{
	return m_info.name;
}


const std::string Server::ip()
{
	return m_info.ip;
}

int Server::port()
{
	return m_info.port;
}

} //namespace gate


