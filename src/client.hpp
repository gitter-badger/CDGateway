#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <string>
#include <map>

extern "C"{
	#include <time.h>
	
	#include <event2/bufferevent.h>
	#include <event2/buffer.h>
	#include <event2/listener.h>
	#include <event2/util.h>
	#include <event2/event.h>
}


#include "gatefwd.h"
#include "coder.hpp"

namespace gate 
{

class Client
{
public:
	Client(struct event_base* base, evutil_socket_t fd, struct sockaddr *socket, int socket_len);
	~Client();

	std::string key();
	void login();
	void logout();
	
public:	
	static void init();
	static void free();
	
private:
	friend void conn_readcb(bufferevent*, void*);

private:
	struct bufferevent *m_buffer;
	std::string m_ip;
	unsigned int m_port;
	time_t m_latest_activity_time;
	unsigned int m_isactive;
	Coder m_coder;
	

};

}// end namespace gate

#endif // CLIENT_HPP
