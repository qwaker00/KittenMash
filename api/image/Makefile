CC=g++
CFLAGS=-c -Wall -O2 --std=c++11 -fPIC
LDFLAGS=${FCGI_LIBS} -shared
LDFLAGS+=-pthread -lmongoclient -lboost_thread -lboost_system -lboost_regex -lssl -lcrypto -lboost_filesystem
SOURCES=image.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=libimage.so

all: $(SOURCES) $(EXECUTABLE) ../orm
		
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) ../orm/orm.a $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS) $(EXECUTABLE)
