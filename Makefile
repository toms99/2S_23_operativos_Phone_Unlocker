all: build_Client

build_Client:
	gcc client/client.c -o out/client
	
run_Client: build_Client
	./out/client 123456789 2