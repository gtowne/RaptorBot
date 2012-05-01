all: VideoStreamer.o

VideoStreamer.o: VideoStreamer.cpp VideoStreamer.h 
	g++ VideoStreamer.cpp -o VideoStreamer.o -c `pkg-config --cflags --libs opencv` -lpthread

clean:
	rm -f *.o
