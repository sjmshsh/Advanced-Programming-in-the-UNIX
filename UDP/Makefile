.PHONY:all
all:server client

server:udp_server.cc
	g++ -o $@ $^ -std=c++11

client:udp_client.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f client server
