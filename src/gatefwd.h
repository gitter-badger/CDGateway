#ifndef __GATWFWD_H__
#define __GATWFWD_H__

#include <tr1/memory>


namespace gate{
	class Client;
	class Server;
	
	typedef std::tr1::shared_ptr<Client> Client_t;
	typedef std::tr1::shared_ptr<Server> Server_t;
	
}

#endif //__GATWFWD_H__
