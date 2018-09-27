CXX    = g++
CXXFLAGS = -Wall
CXXFLAGS += -g
DEP_FLAGS = -MMD -MP
SRC    = $(wildcard *.cpp)
OBJ    = $(SRC:.cpp=.o)
DEP    = $(SRC:.cpp=.d)
CXXFLAGS += $(DEP_FLAGS)
EXEC   = zestaw3
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(OBJ) $(CXXFLAGS) -o $@
	echo $(EXEC): zbudowane!

.PHONY: clean
clean:
	rm $(EXEC) $(OBJ) $(DEP)

.PHONY: dist
dist: clean
	tar cvzf ../$(EXEC).tar.gz *.cpp *.h makefile*

.PHONY: valgrind	
valgrind: $(EXEC)
	valgrind --leak-check=full ./$(EXEC)

.PHONY: gdb	
gdb: $(EXEC)
	gdb ./$(EXEC)

.PHONY: class
class: 
	read -p "Enter Class Name:" name; \
	touch $$name.cpp; \
	printf "#include \""$$name.h"\"\n\n#include <iostream>\n" > $$name.cpp; \
	touch $$name.h; \
	printf "#pragma once\n\nclass "$$name"{\npublic:\n\nprivate:\n\n};\n" > $$name.h
-include $(DEP)
