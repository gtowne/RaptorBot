all: RemoteInterface

RemoteInterface: *.cpp *.h VideoStreamer/* utils/*
	cd utils; make
	cd VideoStreamer; make
	g++ main.cpp utils/MySocketUtils.o RaptorRemoteServer.cpp RaptorRemoteSession.cpp RaptorRemoteSessionProtocol.cpp RaptorVideoStream.cpp RaptorVideoStreamProtocol.cpp VideoStreamer/VideoStreamer.o -o RemoteInterface -lpthread `pkg-config --cflags --libs opencv`

clean:
	rm -f *.o
