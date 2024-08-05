all:client server1 

client:client.cpp 
	g++ -g -o client client.cpp

server1:server1.cpp NetAddress.cpp Socket.cpp Epoll.cpp channel.cpp Acceptor.cpp Connection.cpp Eventloop.cpp Tcpserver.cpp ThreadPools.cpp Buffer.cpp EchoServer.cpp TimeStamp.cpp
	g++ -g -o server1 server1.cpp NetAddress.cpp Socket.cpp Epoll.cpp channel.cpp Acceptor.cpp Connection.cpp Eventloop.cpp Tcpserver.cpp ThreadPools.cpp Buffer.cpp EchoServer.cpp TimeStamp.cpp

clean:
	rm -f client echoserver client1 bankserver

