CXXFLAGS = -I . --std=c++17

SRCS = $(wildcard *.cpp)
HDRS = $(wildcard *.h *.hpp)
OBJS = $(SRCS:.cpp=.o)

TARGETS = main
.PHONY: $(TARGETS) clean

all: $(TARGETS)

$(TARGETS): $(OBJS) $(HDRS)
	$(CXX) $(CXXFLAGS) $^  -o $@ 

clean:
	rm -f $(OBJS) main
