.PHONE:all
all:dict_server dict_client
dict_server:dict_server.cc
	g++ $^ -o $@ -std=c++11
dict_client:dict_client.cc
	g++ $^ -o $@ -std=c++11
.PHONE:clean
clean:
	rm -f dict_server dict_client