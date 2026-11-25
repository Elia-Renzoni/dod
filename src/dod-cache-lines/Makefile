CXX      := g++
CXXFLAGS := -O2 -std=c++20 -Wall -Wextra

TARGETS := bad good

.PHONY: all run run-linear run-binary clean

all: $(TARGETS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

run-linear: all
	@echo "=== LINEAR ==="
	./bad
	@echo
	./good

run-binary: all
	@echo "=== BINARY ==="
	./bad --binary
	@echo
	./good --binary

run: run-linear
	@echo
	@$(MAKE) --no-print-directory run-binary

clean:
	rm -f $(TARGETS)
