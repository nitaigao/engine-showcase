LIB=-L/usr/local/lib
INC=-I/usr/local/include -I/usr/include

SOURCES=consumer.cpp
HEADER=interface.h consumer.h
BIN=visitmock chainmock basicmock poormock cppunit

all: $(BIN)

clean:
  rm $(BIN)

cppunit: cppunit.cpp
	g++ -o $@ cppunit.cpp $(SOURCES) -lcppunit -ldl $(INC) $(LIB)

visitmock: visitmock.cpp $(HEADER) $(SOURCES)
	g++ -o $@ visitmock.cpp $(SOURCES) -lmockpp $(INC) $(LIB)

basicmock: basicmock.cpp $(HEADER) $(SOURCES)
	g++ -o $@ basicmock.cpp $(SOURCES) -lmockpp $(INC) $(LIB)

chainmock: chainmock.cpp $(HEADER) $(SOURCES)
	g++ -o $@ chainmock.cpp $(SOURCES) -lmockpp $(INC) $(LIB)

poormock: poormock.cpp $(HEADER) $(SOURCES)
	g++ -o $@ poormock.cpp $(SOURCES) -lmockpp $(INC) $(LIB)


