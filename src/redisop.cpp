#include "redisop.hpp"

#include <iostream>
#include <string>
#include <boost/regex.hpp>

#include <hiredis/async.h>
#include <hiredis/hiredis.h>
#include <hiredis/adapters/libevent.h>
#include <redis3m/redis3m.hpp>

#include "dispatcher.hpp"
#include "gatefwd.h"
#include "server.hpp"

extern "C"{
	#include "eventutil.h"
}

using namespace std;
using namespace gate;
using namespace redis3m;

static boost::regex regex_reg("^reg:__(\\w+)__(\\d+\\.\\d+\\.\\d+\\.\\d+):(\\d+)$");
static boost::regex regex_unreg("^unreg:__(\\w+)__(\\d+\\.\\d+\\.\\d+\\.\\d+):(\\d+)$");

static redisAsyncContext *async_conn = NULL;
static connection::ptr_t sync_conn = NULL;

static void subscribe_cb(struct redisAsyncContext* async_conn, void *r, void *privdata){
	char *msg = NULL;
	redisReply *reply = (redisReply *)r;  
    if (reply == NULL) return;  
    if ( reply->type == REDIS_REPLY_ARRAY && reply->elements == 3 ) {
		if ( strcmp( (char*)privdata, "*sub*" ) == 0 && reply->element[2]->str != NULL) {
			msg = reply->element[2]->str;
        }else{
			return;
		}
	}else
	{
		return;
	}
	
	boost::cmatch group;
	if(boost::regex_match(msg, group, regex_reg))
	{
		Server_t s = Server_t(new Server(group[2], atoi(((std::string)group[3]).c_str()), group[1]));
		reg_server(s);
		s->connect();
	}
	if(boost::regex_match(msg, group, regex_unreg))
	{
		Server_t s = Server_t(new Server(group[2], atoi(((std::string)group[3]).c_str()), group[1]));
		unreg_server(s->name(), s->key());
		s->disconnect();
	}
}

void init_spy(){
	struct event_base *base = eventutil_getevent();
	redisAsyncContext *async_conn = redisAsyncConnect("127.0.0.1", 6379);
	if (async_conn->err) {
		/* Let *async_conn leak for now... */
		printf("Error: %s\n", async_conn->errstr);
		return;
	}
	redisLibeventAttach(async_conn, base);
	redisAsyncCommand(async_conn, subscribe_cb, (void*)"*sub*", "SUBSCRIBE monitor");
	
	
	sync_conn = connection::create();
	
}

void free_spy()
{
	if(async_conn)
	{
		redisAsyncDisconnect(async_conn);
	}
}
