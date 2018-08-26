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
	$(CXX) $(CXXFLAGS) tests/test.cpp -o bin/test $^ $(INC) $(LDFLAGS) $(TESTLDLIBS)
	gnome-terminal --window-with-profile=run -- bin/test
	
benchmark: $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) sandbox/main-benchmark.cpp -o bin/benchmark $^ $(INC) $(LDFLAGS) $(TESTLDLIBS)
	gnome-terminal --window-with-profile=run -- bin/benchmark

run:
	$(EXEC)
	
valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose $(EXEC)


.PHONY: clean
