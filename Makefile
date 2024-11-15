CC = g++
# CFLAGS = -Wall -pedantic -fsanitize=address -g
CFLAGS = -Wall -pedantic -g
main : main.o http_server.o hashmap.o parse_request.o
	$(CC) $(CFLAGS) $? -o $@

main.o : main.c 
	$(CC) $(CFLAGS) -c $? -o $@

http_server.o : http_server.c
	$(CC) $(CFLAGS) -c $? -o $@

hashmap.o : hashmap.c
	$(CC) $(CFLAGS) -c $? -o $@

parse_request.o : parse_request.c
	$(CC) $(CFLAGS) -c $? -o $@

clean :
	rm -rf *.o main
