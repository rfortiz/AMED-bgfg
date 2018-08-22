CXX := g++

CXXFLAGS := -std=c++11 -pedantic -Wall -Wextra -Wconversion -Wsign-conversion `pkg-config --cflags opencv`
CXXFLAGS += -O2 -s -DNDEBUG
#~ CXXFLAGS += -O0 -g 
#~ CXXFLAGS += -O0 -ggdb

LDFLAGS :=  

LDLIBS := `pkg-config --libs opencv`
TESTLDLIBS := $(LDLIBS) -lgtest -lgtest_main -lpthread

INC := -I src

SRCDIR := src
BUILDDIR := build
EXEC := bin/bgfg_amed

SRCEXT := cpp
SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

TEST_OBJECTS := $(filter-out $(BUILDDIR)/main.o, $(OBJECTS))


all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)
  

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INC)

clean:
	rm -r -f $(BUILDDIR) $(EXEC)
	

test: $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) tests/test.cpp $(INC) $(LDFLAGS) $(TESTLDLIBS) -o bin/test $^
	bin/test 
	


run:
	$(EXEC)
	
valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose $(EXEC)


.PHONY: clean
