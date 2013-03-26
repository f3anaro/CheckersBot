CXXFLAGS = -g -Wall -std=c++0x -Os -pthread
#-m32
#-Wold-style-cast
#-std=c++98
#-std=c++0x

TARGETS = mcp example_player my_player

.PHONY: all help demo run clean dist

a all : $(TARGETS)

h help : mcp
	@echo "The following make commands are available:"
	@echo "  $(MAKE) all   - builds everything"
	@echo "  $(MAKE) demo  - runs two of the example players against each other"
	@echo "  $(MAKE) run   - runs your own player against the example player"
	@echo "  $(MAKE) fight - runs two of your own player with resource limits"
	@echo "  $(MAKE) clean - removes all created files"
	@echo ""
	@echo "The following mcp options are available:"
	@./mcp -?

d demo : mcp example_player
	./mcp -d example_player example_player

r run : mcp example_player my_player
	./mcp -d -r0 -R0 -s10 -S11 example_player my_player

f fight : mcp my_player
	./mcp -t 60 -T 61 -m 1024 -M 1024 my_player my_player

my_player: my_player.o src/CBitBoard.o
	$(CXX) $(CXXFLAGS) -o $@ $^

cl clean :
	rm -f $(TARGETS) *.o
	rm -f src/CBitBoard.o

mcp : mcp.o logic.o
	$(CXX) $(CXXFLAGS) -o $@ $^

ifeq ($(wildcard logic.cc), "logic.cc")
logic.o : logic.cc
else
logic.o : logic.S
endif
