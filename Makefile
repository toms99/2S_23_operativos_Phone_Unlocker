all: build_Client build_Server

build_Server:
	gcc -o ./out/server ./server/server.c -ljansson

run_Server: build_Server
	./out/server

build_Client:
	gcc client/client.c -o out/client -ljansson -lc
	
run_Client: build_Client
	./out/client 123456789 2