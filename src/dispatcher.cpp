#include "dispatcher.hpp"

#include <iostream>
#include <map>

#include "gatefwd.h"
#include "server.hpp"

using namespace std;
using namespace gate;

typedef std::multimap<std::string, Server_t> Servers_map;
typedef std::pair<std::string, Server_t> Server_pair;
Servers_map server_map;

namespace gate
{
void reg_server(Server_t server)
{
	cout << "register server: " << server->name() << " " << server->ip() << ":"<< server->port() << endl;
	server_map.insert(Server_pair(server->name(), server));
}

void unreg_server(std::string server_name, std::string key)
{
	for(Servers_map::iterator it = server_map.begin(); it != server_map.end(); ++it)
	{
		if(it->second->key() == key)
		{
			server_map.erase(it);
			return;
		}
	}
	
}

} //namespace gate

