gcc -o server $(mysql_config --cflags) server.c $(mysql_config --libs) -pthread
./server
