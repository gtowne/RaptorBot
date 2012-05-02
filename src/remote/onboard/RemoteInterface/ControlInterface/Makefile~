all: BehaviorQueue.o ControlInterface.o BehaviorTest

BehaviorTest: BehaviorTest.cpp BehaviorQueue.cpp BehaviorQueue.h ControlInterface.cpp ControlInterface.h
	g++ BehaviorTest.cpp BehaviorQueue.o ControlInterface.o -o BehaviorTest -lpthread

BehaviorQueue.o: BehaviorQueue.cpp BehaviorQueue.h ControlInterface.cpp ControlInterface.h
	g++ BehaviorQueue.cpp -c -o BehaviorQueue.o

ControlInterface.o: ControlInterface.cpp ControlInterface.h
	g++ ControlInterface.cpp -c -o ControlInterface.o

clean:
	rm -f *.o
