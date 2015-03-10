#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <string>
#include <map>

#include "gatefwd.h"

namespace gate
{

class Server
{
public:
	typedef struct server_info_s{
		std::string name;
		std::string ip;
		unsigned int port;
		unsigned int payload;
	}server_info_t;

public:
	Server(std::string ip, unsigned int port, std::string name){
		m_info.name = name;
		m_info.ip = ip;
		m_info.port = port;
		m_info.payload = 0;
	}
	
	~Server();
	void connect();
	void disconnect();
	
	const std::string key();
	const std::string name();
	const std::string ip();
	int port();


private:
	server_info_t m_info;
};

}

#endif // __SERVER_HPP__
