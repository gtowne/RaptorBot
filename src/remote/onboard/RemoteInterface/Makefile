all: RemoteInterface

RemoteInterface: *.cpp *.h VideoStreamer/* utils/*
	cd utils; make
	cd VideoStreamer; make
	cd ControlInterface; make
	g++ main.cpp utils/MySocketUtils.o RaptorRemoteServer.cpp RaptorRemoteSession.cpp RaptorRemoteSessionProtocol.cpp RaptorVideoStream.cpp RaptorVideoStreamProtocol.cpp VideoStreamer/VideoStreamer.o ControlInterface/BehaviorQueue.o ControlInterface/ControlInterface.o -o RemoteInterface -lpthread `pkg-config --cflags --libs opencv`

clean:
	cd utils; make clean
	cd VideoStreamer; make clean
	rm -f *.o
	rm RemoteInterface
