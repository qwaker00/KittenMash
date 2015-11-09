CFLAGS=-c -Wall -O2 --std=c++11 -fPIC
LDFLAGS=${FCGI_LIBS} -shared
SOURCES=image.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=libimage.so

all: $(SOURCES) $(EXECUTABLE)
		
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS) $(EXECUTABLE)
