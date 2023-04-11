.PHONY:all
all:server client

server:tcp_server.cc
	g++ -o $@ $^ -std=c++11

client:tcp_client.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f client server
