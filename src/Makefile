RaptorWireProtocolDir=arduino/libraries/RaptorWireProtocol

RaptorBot: RaptorBot.o ControlInterface.o RaptorSerialInterface.o RaptorWireProtocol.o
	g++ RaptorBot.o ControlInterface.o RaptorSerialInterface.o RaptorWireProtocol.o -o RaptorBot

RaptorBot.o: RaptorBot.h RaptorBot.cpp ControlInterface.o
	g++ -c RaptorBot.cpp -I$(RaptorWireProtocolDir) 

ControlInterface.o: ControlInterface.cpp ControlInterface.h RaptorSerialInterface.o RaptorWireProtocol.o
	g++ -c ControlInterface.cpp -I$(RaptorWireProtocolDir) 

RaptorSerialInterface.o: RaptorSerialInterface.cpp RaptorSerialInterface.h $(RaptorWireProtocolDir)/RaptorWireProtocol.h $(RaptorWireProtocolDir)/RaptorWireProtocol.cpp
	g++ -c RaptorSerialInterface.cpp -I$(RaptorWireProtocolDir) 

RaptorWireProtocol.o: $(RaptorWireProtocolDir)/RaptorWireProtocol.h $(RaptorWireProtocolDir)/RaptorWireProtocol.cpp
	g++ -c $(RaptorWireProtocolDir)/RaptorWireProtocol.cpp
	

clean:
	rm -rf *.o RaptorBot
