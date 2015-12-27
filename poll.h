#include <poll.h>

#include <vector>
#include <unordered_map>

class MY_fd_set
{
	std::vector<pollfd> pollfds;
	std::unordered_map<int, int> fd_to_pollfd;
	
	
public:
	void set( int, short );
	bool isset( int, short );

	void clear();
	
	int size();
	pollfd *data();
};

#define MY_FD_ISSET(s,fdset) (fdset)->isset(s, POLLIN)
#define MY_FD_SET(s,fdset)   (fdset)->set(s, POLLIN)
#define MY_FD_ZERO(fdset)    (fdset)->clear()