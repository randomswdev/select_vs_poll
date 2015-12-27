select.test: main.cc select.h
	g++ -DUSE_SELECT -o select.test -g -lpthread main.cc

poll.test: main.cc poll.h poll.cc
	g++ -std=c++0x -o poll.test -g -lpthread main.cc poll.cc

run.select: select.test
	ulimit -n 2048; ./select.test 1000

valgrind.select: select.test
	ulimit -n 2048;	valgrind ./select.test 1000

run.poll: poll.test
	ulimit -n 2048; ./poll.test 1000

valgrind.poll: poll.test
	ulimit -n 2048;	valgrind ./poll.test 1000
