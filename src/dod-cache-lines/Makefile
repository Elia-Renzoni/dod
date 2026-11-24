CXX      := g++
CXXFLAGS := -O2 -std=c++20 -Wall -Wextra

TARGETS := bad good

.PHONY: all run clean

all: $(TARGETS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

run: all
	./bad
	@echo
	./good

clean:
	rm -f $(TARGETS)
