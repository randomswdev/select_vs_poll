#include "poll.h"

void MY_fd_set::set( int s, short status )
{
	std::unordered_map<int,int>::const_iterator it = fd_to_pollfd.find( s );
	if( it == fd_to_pollfd.end() )
	{
		fd_to_pollfd.insert( std::unordered_map<int,int>::value_type( s, pollfds.size() ) );
		
		struct pollfd pollfd;
		pollfd.fd = s;
		pollfd.events = status;
		pollfd.revents = 0;
		
		pollfds.push_back( pollfd );
	}
	else
	{
		pollfds[it->second].events |= status;
	}
	
}

bool MY_fd_set::isset( int s, short status )
{
	std::unordered_map<int,int>:: const_iterator it = fd_to_pollfd.find( s );
	if( it == fd_to_pollfd.end() )
	{
		return false;
	}
	
	return pollfds[it->second].revents & status;
}

void MY_fd_set::clear()
{
	pollfds.clear();
	fd_to_pollfd.clear();
}
	
int MY_fd_set::size()
{
	return pollfds.size();
}

pollfd * MY_fd_set::data()
{
	return pollfds.data();
}
