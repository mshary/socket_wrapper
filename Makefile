#
# Makefile
#

all:	clean libs

libs:	libsocket_wrapper.so

libsocket_wrapper.so:	socket_wrapper.c mptcp_wrapper.c
	gcc socket_wrapper.c mptcp_wrapper.c -D DEBUG -fPIC -shared -ldl -o libsocket_wrapper.so

with_redis:
	gcc socket_wrapper.c mptcp_wrapper.c -g -D DEBUG -D WITH_REDIS -fPIC -shared -ldl -lhiredis -o libsocket_wrapper.so

without_mptcp:	socket_wrapper.c
	gcc socket_wrapper.c -D DEBUG -fPIC -shared -ldl -o libsocket_wrapper.so

clean:
	rm -f libsocket_wrapper.so* test_socket
