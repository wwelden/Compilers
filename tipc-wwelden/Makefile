CFLAGS= -g -O0 -std=c++17 -Wno-return-type -Wno-enum-compare-switch

BUILDDIR = build
EXECUTABLE = tipc 
CC = g++
INC = -Iinclude
LIBS = -lstdc++
DIRS=$(BUILDDIR)

SRCS = $(wildcard *.cpp)
HEADERS := $(wildcard *.h)
OBJS = $(SRCS:.cpp=.o)

%.o: %.cpp 
	$(CC) $(CFLAGS) -c $(INC) -o $@ $<

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o $(EXECUTABLE)

# Make the build directory
$(shell mkdir -p $(DIRS))

lines:
	wc -l *.h *.cpp | grep total
