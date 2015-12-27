select.test: main.cc
	g++ -o select.test -g -lpthread main.cc

run.select: select.test
	ulimit -n 2048; ./select.test 1000

valgrind.select: select.test
	ulimit -n 2048;	valgrind ./select.test 1000
