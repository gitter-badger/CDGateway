#ifndef __DISPATCHER_HPP__
#define __DISPATCHER_HPP__

#include "gatefwd.h"
#include <string>

namespace gate
{
	
	void reg_server(Server_t server);
	void unreg_server(std::string server_name, std::string key);

} //namespace gate

#endif //__DISPATCHER_HPP__
