CC = g++

CFLAGS = -Wall -O -g
EXECUTABLES = devices
resources = devices.cpp
LDFLAGS = -lrt -lrdmacm -libverbs

devices:	devices.cpp
	$(CC) devices.cpp -o  $@  $(LDFLAGS) $(CFLAGS)


clean:
	$(RM) *.o $(EXECUTABLES)