#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <netinet/ip.h>

#include <set>
#include <vector>

#ifdef USE_SELECT
#include "select.h"
#else
#include "poll.h"
#endif

#define FAIL_IF_ERROR( expr ) \
	if( ( expr ) == -1 ) \
	{ \
		printf( "Failure at line %d, with errno %d\n", __LINE__, errno ); \
		abort(); \
	}

int required_socket_number, current_socket_number;
struct sockaddr_in server_addr = { AF_INET, 4321, 0 };

extern "C" void* worker_thread( void * )
{
	std::vector<int> connections;
	connections.reserve( required_socket_number );
	
	for ( current_socket_number = 0; current_socket_number < required_socket_number; current_socket_number++ )
	{
		int s;
		FAIL_IF_ERROR( s = socket( AF_INET, SOCK_STREAM, 0 ) );
		FAIL_IF_ERROR( connect( s, (sockaddr*) &server_addr, sizeof(server_addr) ) )
		
		connections.push_back( s );
	}
	
	while ( !connections.empty() )
	{
		int s = connections.back();
		connections.pop_back();

		FAIL_IF_ERROR( close( s ) );
	}

	return NULL;
}

int main( int argc, char* argv[] )
{
	int server_fd, ret;
	
	if( argc != 2 )
	{
		printf( "Usage: %s num-sockets\n", argv[0] );
		return 1;
	}
	
	required_socket_number = atoi( argv[1] );
	
	if( required_socket_number <= 0 || required_socket_number > 2048 )
	{
		printf( "The number of sockets must be between 1 and 2048\n" );
		return 1;
	}
	
	FAIL_IF_ERROR( server_fd = socket( AF_INET, SOCK_STREAM, 0 ) );
	FAIL_IF_ERROR( bind( server_fd, (sockaddr*) &server_addr, sizeof(server_addr) ) );
	FAIL_IF_ERROR( listen( server_fd, 10 ) );
	
	pthread_t t;
	pthread_create( &t, NULL, worker_thread, NULL );
	
	std::set<int> connections;

	while ( true )
	{
		int max_fd;
		
		MY_fd_set read_set;
		MY_FD_ZERO( &read_set );
		
		MY_FD_SET( server_fd, &read_set );
		max_fd = server_fd;
		for( std::set<int>::const_iterator it = connections.begin(); it != connections.end(); it++ )
		{
			int s = *it;
			MY_FD_SET( s, &read_set );
			max_fd = std::max( max_fd, s );
		}
	
#ifdef USE_SELECT
		FAIL_IF_ERROR( ret = select( max_fd + 1, &read_set, NULL, NULL, NULL ) );
#else
		FAIL_IF_ERROR( ret = poll( read_set.data(), read_set.size(), -1 ) );
#endif			
		
		if( ret == 0 )
			continue;
		
		if( MY_FD_ISSET( server_fd, &read_set ) )
		{
			FAIL_IF_ERROR( ret = accept( server_fd, NULL, NULL ) );
			connections.insert( ret );
		}

		for( std::set<int>::const_iterator it = connections.begin(); it != connections.end(); )
		{
			int s = *it;
			if ( MY_FD_ISSET( s, &read_set ) )
			{
				close( *it );
				connections.erase( it++ );
			}
			else
			{
				it++;
			}
		}
		
		if( connections.empty() )
			break;
	}
	
	close( server_fd );
	
	pthread_join( t, NULL );
	
	return 0;
}