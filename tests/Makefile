# --- DEVELOPER AREA START ---

# We use nested namespaces for convenience
CXXFLAGS += -std=c++1z

# Include project-specific files
CPPFLAGS += -I ../include

# Where to find library code
LIB_DIR = ../include/algs

# Where to find test scenarios
SCENARIOS_DIR = scenarios

# Where to put intermediate files and test executables
BUILD_DIR = build

# --- DEVELOPER AREA END ---

# --- GOOGLE TEST AREA START; DO NOT TOUCH ---

GTEST_DIR = ../submodules/googletest/googletest

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

CPPFLAGS += -isystem $(GTEST_DIR)/include

CXXFLAGS += -g -Wall -Wextra -pthread

gtest.a :
	$(MAKE) -f Makefile_googletest gtest.a

gtest_main.a :
	$(MAKE) -f Makefile_googletest gtest_main.a

# --- GOOGLE TEST AREA END; DO NOT TOUCH ---

# --- DEVELOPER AREA START (add tests here) ---

sort.o : $(SCENARIOS_DIR)/sort.cpp \
	     $(LIB_DIR)/sort/selection.hpp \
	     $(LIB_DIR)/sort/insertion.hpp \
	     $(LIB_DIR)/sort/shell.hpp \
		 $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) \
		-c $(filter %.cpp,$^) \
		-o $(BUILD_DIR)/$@

# note: gtest_main.a defines main, gtest.a don't
test : sort.o gtest_main.a
	cd $(BUILD_DIR) && \
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread -o $@ $^

run : test
	./build/test

clean :
	rm -rf $(BUILD_DIR) && mkdir -p $(BUILD_DIR) && touch $(BUILD_DIR)/.gitignore

all : run

