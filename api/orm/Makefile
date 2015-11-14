CC=g++
CFLAGS=-c -Wall -O2 -std=c++11 -fPIC
LDFLAGS=-pthread -lmongoclient -lboost_thread -lboost_system -lboost_regex -lssl -lcrypto -lboost_filesystem
SOURCES=db.cpp db_impl.cpp image.cpp
OBJECTS=$(SOURCES:.cpp=.o)
LIB=orm.a

all: $(SOURCES) $(LIB)

$(LIB): $(OBJECTS)
	ar rvs $@ $(OBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS) $(LIB)
