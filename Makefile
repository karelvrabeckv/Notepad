# VARIABLES
CC = g++
CFLAGS = -Wall -pedantic -Wno-long-long -O0 -ggdb -std=c++11
EXE = vrabekar

# THESE KEYWORDS ARE NOT FILES
.PHONY: all
.PHONY: compile
.PHONY: run
.PHONY: clean
.PHONY: doc

# CREATES ALL OUTPUTS
all: ./$(EXE) doc

# COMPILES THE PROGRAM
compile: ./$(EXE)

# RUNS THE PROGRAM
run: ./$(EXE)
	./$(EXE)

# DELETES CREATED FILES/DIRECTORIES
clean:
	rm -rf build
	rm -rf doc
	rm -f ./$(EXE)

# MAKES A DOCUMENTATION
doc: src/main.cpp src/constants.hpp src/database/database.hpp src/types/note.hpp src/types/plaintext/plaintext.hpp src/types/shoppingcart/shoppingcart.hpp src/types/tasklist/tasklist.hpp
	rm -rf doc
	doxygen Doxyfile

# PROGRAM BUILD
# $^ - all behind :
# $@ - before :
./$(EXE): build/main.o build/database.o build/plaintext.o build/shoppingcart.o build/tasklist.o
	$(CC) $^ -o $@

# .O BUILD
# $< - the first behind :
# $@ - before :
build/main.o: src/main.cpp | build
	$(CC) $(CFLAGS) -c $< -o $@
	
build/database.o: src/database/database.cpp | build
	$(CC) $(CFLAGS) -c $< -o $@
	
build/plaintext.o: src/types/plaintext/plaintext.cpp | build
	$(CC) $(CFLAGS) -c $< -o $@
	
build/shoppingcart.o: src/types/shoppingcart/shoppingcart.cpp | build
	$(CC) $(CFLAGS) -c $< -o $@
	
build/tasklist.o: src/types/tasklist/tasklist.cpp | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir build
	
# DEPENCIES OF .O FILES
build/main.o: src/main.cpp src/constants.hpp src/database/database.hpp src/types/note.hpp src/types/plaintext/plaintext.hpp src/types/shoppingcart/shoppingcart.hpp src/types/tasklist/tasklist.hpp
build/database.o: src/database/database.cpp src/database/database.hpp src/types/note.hpp src/types/plaintext/plaintext.hpp src/types/shoppingcart/shoppingcart.hpp src/types/tasklist/tasklist.hpp src/constants.hpp
build/plaintext.o: src/types/plaintext/plaintext.cpp src/types/plaintext/plaintext.hpp src/database/database.hpp src/constants.hpp src/types/note.hpp
build/shoppingcart.o: src/types/shoppingcart/shoppingcart.cpp src/types/shoppingcart/shoppingcart.hpp src/database/database.hpp src/constants.hpp src/types/note.hpp
build/tasklist.o: src/types/tasklist/tasklist.cpp src/types/tasklist/tasklist.hpp src/database/database.hpp src/constants.hpp src/types/note.hpp
