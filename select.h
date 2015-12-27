#define MY_fd_set               fd_set
#define MY_FD_ISSET(socket,set) FD_ISSET(socket,set)
#define MY_FD_SET(socket,set)	FD_SET(socket,set)
#define MY_FD_ZERO(set)			FD_ZERO(set)