.PHONY:svr cli
all:cli svr
cli:cli.cpp
	g++ $^ -o $@ -g -lpthread
svr:svrprocess.cpp
	g++ $^ -o $@ -g -lpthread

