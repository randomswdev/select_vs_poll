# select_vs_poll

This project shows how to modify code that uses select in order to avoid failures caused by socket's file descriptors larger than FD_MAXSIZE.

The orginal vrsion of the code, based on select, is modified to leverage the poll function, that has no limits in terms of value of file descriptors.

